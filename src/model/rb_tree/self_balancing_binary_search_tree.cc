#include "self_balancing_binary_search_tree.h"

namespace s21 {
ResultMessage SelfBalancingBinarySearchTree::Set(
    const std::string& key, const std::string& last_name,
    const std::string& first_name, const std::string& year,
    const std::string& city, const std::string& coins, const std::string& plug,
    const std::string& EX) {
  if (plug != "EX") {
    return {true, "ERROR: unknown option: " + plug};
  }

  int i_year;
  if (Strings::isNumeric(year)) {
    i_year = std::stoi(year);
  } else {
    return {true, "ERROR: unable to cast value " + year + " to type int"};
  }

  int i_coins;
  if (Strings::isNumeric(coins)) {
    i_coins = std::stoi(coins);
  } else {
    return {true, "ERROR: unable to cast value " + coins + " to type int"};
  }

  int i_EX;
  if (Strings::isNumeric(EX)) {
    i_EX = std::stoi(EX);
  } else {
    return {true, "ERROR: unable to cast value " + EX + " to type int"};
  }

  Record record(last_name, first_name, i_year, city, i_coins, i_EX);
  auto node = new Node(key, record, nullptr);

  mx_.lock();
  bool is_set = Put(node);
  mx_.unlock();

  if (is_set) {
    if (i_EX >= 0) {
      thread_pool_.emplace_back(
          [this, node]() { StartRecordExpiration(node); });
    }
    return {false, "OK"};
  }
  delete node;
  return {true, "Key already exists"};
}

std::shared_ptr<const Student> SelfBalancingBinarySearchTree::Get(
    const std::string& key) {
  std::lock_guard<std::mutex> lk(mx_);
  node_ptr node = FindNode(key);
  if (node != nullptr) {
    return std::make_shared<Student>(node->value.getStudent());
  }
  return nullptr;
}

bool SelfBalancingBinarySearchTree::Exists(const std::string& key) {
  std::lock_guard<std::mutex> lk(mx_);
  node_ptr node = FindNode(key);
  return node != nullptr;
}

bool SelfBalancingBinarySearchTree::Del(const std::string& key) {
  std::lock_guard<std::mutex> lk(mx_);
  node_ptr node = FindNode(key);
  if (node != nullptr) DeleteNode(node);
  return node != nullptr;
}

ResultMessage SelfBalancingBinarySearchTree::Update(
    const std::string& key, const std::string& last_name,
    const std::string& first_name, const std::string& year,
    const std::string& city, const std::string& coins) {
  int i_year;
  if (year != "-") {
    if (Strings::isNumeric(year)) {
      i_year = std::stoi(year);
    } else {
      return {true, "ERROR: unable to cast value " + year + " to type int"};
    }
  } else {
    i_year = -1;
  }

  int i_coins;
  if (coins != "-") {
    if (Strings::isNumeric(coins)) {
      i_coins = std::stoi(coins);
    } else {
      return {true, "ERROR: unable to cast value " + coins + " to type int"};
    }
  } else {
    i_coins = -1;
  }

  std::lock_guard<std::mutex> lk(mx_);
  node_ptr node = FindNode(key);
  if (node != nullptr) {
    node->value.UpdateStudent(last_name, first_name, i_year, city, i_coins);
    return {false, "OK"};
  }
  return {true, "Key not found"};
}

std::vector<std::shared_ptr<const std::string>>
SelfBalancingBinarySearchTree::Keys() {
  std::vector<std::shared_ptr<const std::string>> result;
  std::lock_guard<std::mutex> lk(mx_);
  Iterator iterator(FindMinNode());
  while (iterator.HasNext()) {
    result.push_back(std::make_shared<std::string>(iterator.GetKey()));
    iterator.Next();
  }
  return result;
}

ResultMessage SelfBalancingBinarySearchTree::Rename(
    const std::string& key, const std::string& new_name) {
  if (key == new_name) {
    return {false, "OK"};
  }
  std::lock_guard<std::mutex> lk(mx_);
  if (FindNode(new_name) != nullptr) {
    return {true, "Key already exists"};
  }
  node_ptr node = FindNode(key);
  if (node != nullptr) {
    auto new_node = new Node(new_name, node->value, nullptr);
    DeleteNode(node);
    Put(new_node);
    return {false, "OK"};
  }
  return {true, "Key not found"};
}

std::shared_ptr<const int> SelfBalancingBinarySearchTree::TTL(
    const std::string& key) {
  std::lock_guard<std::mutex> lk(mx_);
  node_ptr node = FindNode(key);
  if (node != nullptr) {
    int ex = node->value.getEx();
    return std::make_shared<const int>(ex);
  }
  return nullptr;
}

std::vector<std::shared_ptr<const std::string>>
SelfBalancingBinarySearchTree::Find(const std::string& last_name,
                                    const std::string& first_name,
                                    const std::string& year,
                                    const std::string& city,
                                    const std::string& coins) {
  std::vector<std::shared_ptr<const std::string>> result;
  std::lock_guard<std::mutex> lk(mx_);
  Iterator iterator(FindMinNode());
  while (iterator.HasNext()) {
    Student student = iterator.GetValue()->getStudent();
    if (student.CompareTo(last_name, first_name, year, city, coins)) {
      result.push_back(std::make_shared<const std::string>(iterator.GetKey()));
    }
    iterator.Next();
  }
  return result;
}

std::vector<std::shared_ptr<const Student>>
SelfBalancingBinarySearchTree::ShowAll() {
  std::lock_guard<std::mutex> lk(mx_);
  std::vector<std::shared_ptr<const Student>> result;
  Iterator iterator(FindMinNode());
  while (iterator.HasNext()) {
    result.push_back(
        std::make_shared<const Student>(iterator.GetValue()->getStudent()));
    iterator.Next();
  }
  return result;
}

ResultMessage SelfBalancingBinarySearchTree::Upload(const std::string& path) {
  DataReader reader = DataReader::GetInstance();
  bool isOpen = reader.Open(path);
  if (!isOpen) return {true, "Error: can't open file."};
  int counter = 0;
  while (reader.isAvailable()) {
    std::pair<std::string, std::shared_ptr<Student>> data =
        reader.GetNextData();
    if (data.second != nullptr) {
      auto node = new Node(data.first, Record(*data.second, -1), nullptr);
      mx_.lock();
      bool is_set = Put(node);
      mx_.unlock();
      if (is_set) {
        counter++;
      } else {
        delete node;
      }
    }
  }
  return {false, "OK " + std::to_string(counter)};
}

ResultMessage SelfBalancingBinarySearchTree::Export(const std::string& path) {
  DataWriter reader = DataWriter::GetInstance();
  bool isOpen = reader.Open(path);
  if (!isOpen) return {true, "Error: can't open file."};
  int counter = 0;

  std::lock_guard<std::mutex> lk(mx_);
  Iterator iterator(FindMinNode());
  while (iterator.HasNext()) {
    if (!reader.WriteData(
            {iterator.GetKey(), iterator.GetValue()->getStudent()}))
      return {true, "Error: access to the file was closed."};
    counter++;
    iterator.Next();
  }

  return {false, "OK " + std::to_string(counter)};
}

void SelfBalancingBinarySearchTree::StartRecordExpiration(Node* record) {
  while (record->value.getEx() > 0) {
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    std::lock_guard<std::mutex> lk(mx_);
    if (record->value.getEx() < 0) {
      return;
    }
    record->value.DecreaseEx();
  }
  mx_.lock();
  DeleteNode(record);
  mx_.unlock();
}

SelfBalancingBinarySearchTree::node_ptr SelfBalancingBinarySearchTree::FindNode(
    const std::string& key) {
  node_ptr node = root_;
  while (node != nullptr) {
    if (node->key == key) return node;
    if (key > node->key)
      node = node->right;
    else
      node = node->left;
  }
  return nullptr;
}

bool SelfBalancingBinarySearchTree::ColorOf(const node_ptr& node) {
  return (node == nullptr ? BLACK : node->color);
}

void SelfBalancingBinarySearchTree::SetColor(const node_ptr& node, bool color) {
  if (node != nullptr) {
    node->color = color;
  }
}

SelfBalancingBinarySearchTree::node_ptr SelfBalancingBinarySearchTree::ParentOf(
    const node_ptr& node) {
  return node == nullptr ? nullptr : node->parent;
}

SelfBalancingBinarySearchTree::node_ptr SelfBalancingBinarySearchTree::LeftOf(
    const node_ptr& node) {
  return node == nullptr ? nullptr : node->left;
}

SelfBalancingBinarySearchTree::node_ptr SelfBalancingBinarySearchTree::RightOf(
    const node_ptr& node) {
  return node == nullptr ? nullptr : node->right;
}

void SelfBalancingBinarySearchTree::RotateLeft(
    const SelfBalancingBinarySearchTree::node_ptr& node) {
  if (node != nullptr) {
    node_ptr right_son = node->right;
    node->right = right_son->left;
    if (right_son->left != nullptr) {
      right_son->left->parent = node;
    }
    right_son->parent = node->parent;
    if (node->parent == nullptr) {
      root_ = right_son;
    } else if (node->parent->left == node) {
      node->parent->left = right_son;
    } else {
      node->parent->right = right_son;
    }
    right_son->left = node;
    node->parent = right_son;
  }
}

void SelfBalancingBinarySearchTree::RotateRight(
    const SelfBalancingBinarySearchTree::node_ptr& node) {
  if (node != nullptr) {
    node_ptr left_son = node->left;
    node->left = left_son->right;
    if (left_son->right != nullptr) {
      left_son->right->parent = node;
    }
    left_son->parent = node->parent;
    if (node->parent == nullptr) {
      root_ = left_son;
    } else if (node->parent->right == node) {
      node->parent->right = left_son;
    } else {
      node->parent->left = left_son;
    }
    left_son->right = node;
    node->parent = left_son;
  }
}

bool SelfBalancingBinarySearchTree::Put(Node* node) {
  node_ptr temp = root_;
  if (root_ == nullptr) {
    root_ = node;
    size_ = 1;
  } else {
    node_ptr parent;
    while (temp != nullptr) {
      parent = temp;
      if (node->key < temp->key) {
        temp = temp->left;
      } else if (node->key > temp->key) {
        temp = temp->right;
      } else {
        return false;
      }
    }
    node->parent = parent;
    if (node->key < parent->key) {
      parent->left = node;
    } else {
      parent->right = node;
    }
    FixAfterInsertion(node);
    size_++;
  }
  return true;
}

void SelfBalancingBinarySearchTree::FixAfterInsertion(node_ptr node) {
  node->color = RED;

  while (node != nullptr && node != root_ && node->parent->color == RED) {
    if (ParentOf(node) == LeftOf(ParentOf(ParentOf(node)))) {
      node_ptr uncle = RightOf(ParentOf(ParentOf(node)));
      if (ColorOf(uncle) == RED) {
        SetColor(ParentOf(node), BLACK);
        SetColor(uncle, BLACK);
        SetColor(ParentOf(ParentOf(node)), RED);
        node = ParentOf(ParentOf(node));
      } else {
        if (node == RightOf(ParentOf(node))) {
          node = ParentOf(node);
          RotateLeft(node);
        }
        SetColor(ParentOf(node), BLACK);
        SetColor(ParentOf(ParentOf(node)), RED);
        RotateRight(ParentOf(ParentOf(node)));
      }
    } else {
      node_ptr uncle = LeftOf(ParentOf(ParentOf(node)));
      if (ColorOf(uncle) == RED) {
        SetColor(ParentOf(node), BLACK);
        SetColor(uncle, BLACK);
        SetColor(ParentOf(ParentOf(node)), RED);
        node = ParentOf(ParentOf(node));
      } else {
        if (node == LeftOf(ParentOf(node))) {
          node = ParentOf(node);
          RotateRight(node);
        }
        SetColor(ParentOf(node), BLACK);
        SetColor(ParentOf(ParentOf(node)), RED);
        RotateLeft(ParentOf(ParentOf(node)));
      }
    }
  }
  SetColor(root_, BLACK);
}

void SelfBalancingBinarySearchTree::DeleteNode(
    SelfBalancingBinarySearchTree::node_ptr node) {
  size_--;

  // If node has 2 children swap with next node until node has 1 or 0 children
  if (node->left != nullptr && node->right != nullptr) {
    node_ptr next = Increment(node);
    node->key = next->key;
    node->value = next->value;
    node = next;
  }

  node_ptr replacement = node->left == nullptr ? node->right : node->left;

  if (replacement != nullptr) {  // node with one RED child
    replacement->parent = node->parent;

    // hang to parent
    if (node->parent == nullptr) {
      root_ = replacement;
    } else if (node == node->parent->left) {
      node->parent->left = replacement;
    } else {
      node->parent->right = replacement;
    }

    // if node is BLACK so replacement must be BLACK
    if (node->color == BLACK) {
      SetColor(replacement, BLACK);
    }
  } else if (node->parent == nullptr) {
    root_ = nullptr;
  } else {  // if node has no children
    if (node->color == BLACK) {
      FixAfterDeletion(node);
    }
    if (node->parent != nullptr) {
      if (node == node->parent->left) {
        node->parent->left = nullptr;
      } else if (node == node->parent->right) {
        node->parent->right = nullptr;
      }
      node->parent = nullptr;
    }
  }
  delete node;
}

void SelfBalancingBinarySearchTree::FixAfterDeletion(node_ptr node) {
  while (node != root_ && ColorOf(node) == BLACK) {
    if (node == LeftOf(ParentOf(node))) {  // left case
      node_ptr brother = RightOf(ParentOf(node));

      if (ColorOf(brother) == RED) {  // if brother is RED
        SetColor(brother, BLACK);     //  we will make case where he is BLACK
        SetColor(ParentOf(node), RED);
        RotateLeft(ParentOf(node));
        brother = RightOf(ParentOf(node));
      }

      // if brother's children is BLACK
      if (ColorOf(LeftOf(brother)) == BLACK &&
          ColorOf(RightOf(brother)) == BLACK) {
        SetColor(brother, RED);  // make brother RED
        node = ParentOf(node);   // If parent is RED, make him BLACK and exit,
                                 // else fix from parent
      } else {
        // if brother's right child is BLACK (left is right)
        // make case where brother's right child is RED
        if (ColorOf(RightOf(brother)) == BLACK) {
          SetColor(LeftOf(brother), BLACK);
          SetColor(brother, RED);
          RotateRight(brother);
          brother = RightOf(ParentOf(node));
        }

        // if brother's right child is RED
        SetColor(brother, ColorOf(ParentOf(node)));
        SetColor(ParentOf(node), BLACK);
        SetColor(RightOf(brother), BLACK);
        RotateLeft(ParentOf(node));
        break;
      }
    } else {  // right case (symmetric to left)
      node_ptr brother = LeftOf(ParentOf(node));
      if (ColorOf(brother) == RED) {
        SetColor(brother, BLACK);
        SetColor(ParentOf(node), RED);
        RotateRight(ParentOf(node));
        brother = LeftOf(ParentOf(node));
      }

      if (ColorOf(LeftOf(brother)) == BLACK &&
          ColorOf(RightOf(brother)) == BLACK) {
        SetColor(brother, RED);
        node = ParentOf(node);
      } else {
        if (ColorOf(LeftOf(brother)) == BLACK) {
          SetColor(RightOf(brother), BLACK);
          SetColor(brother, RED);
          RotateLeft(brother);
          brother = LeftOf(ParentOf(node));
        }
        SetColor(brother, ColorOf(ParentOf(node)));
        SetColor(ParentOf(brother), BLACK);
        SetColor(LeftOf(brother), BLACK);
        RotateRight(ParentOf(node));
        break;
      }
    }
  }
  SetColor(node, BLACK);
}

SelfBalancingBinarySearchTree::node_ptr
SelfBalancingBinarySearchTree::Increment(const node_ptr& node) {
  if (node == nullptr) {
    return nullptr;
  } else if (node->right != nullptr) {
    node_ptr next = node->right;
    while (next->left != nullptr) {
      next = next->left;
    }
    return next;
  } else {
    node_ptr parent = node->parent;
    node_ptr temp = node;
    while (parent != nullptr && temp == parent->right) {
      temp = parent;
      parent = parent->parent;
    }
    return parent;
  }
}

SelfBalancingBinarySearchTree::node_ptr
SelfBalancingBinarySearchTree::FindMinNode() {
  node_ptr min = root_;
  if (min != nullptr) {
    while (min->left != nullptr) {
      min = min->left;
    }
  }
  return min;
}

SelfBalancingBinarySearchTree::SelfBalancingBinarySearchTree() {
  root_ = nullptr;
  size_ = 0;
}

SelfBalancingBinarySearchTree::~SelfBalancingBinarySearchTree() {
  mx_.lock();
  Iterator iterator(FindMinNode());
  while (iterator.HasNext()) {
    iterator.ResetValue();
    iterator.Next();
  }
  mx_.unlock();
  for (auto& thread : thread_pool_) {
    if (thread.joinable()) {
      thread.join();
    }
  }
  if (root_ != nullptr) {
    DeleteTree_(root_);
    delete root_;
  }
}

void SelfBalancingBinarySearchTree::DeleteTree_(node_ptr node) {
  if (node->left != nullptr) DeleteTree_(node->left);
  if (node->right != nullptr) DeleteTree_(node->right);
  node_ptr parent = node->parent;
  if (node != root_) {
    if (parent->left == node)
      parent->left = nullptr;
    else
      parent->right = nullptr;
    delete node;
  }
}

SelfBalancingBinarySearchTree::Node::Node(std::string key, Record value,
                                          Node* parent)
    : value(std::move(value)) {
  this->key = std::move(key);
  this->parent = parent;
  this->left = nullptr;
  this->right = nullptr;
  this->color = BLACK;
}

SelfBalancingBinarySearchTree::Iterator::Iterator(Node* node) {
  next_ = node;
  last_returned_ = nullptr;
}

bool SelfBalancingBinarySearchTree::Iterator::HasNext() const {
  return next_ != nullptr;
}

SelfBalancingBinarySearchTree::node_ptr
SelfBalancingBinarySearchTree::Iterator::Next() {
  if (next_ != nullptr) {
    last_returned_ = next_;
    next_ = Increment(next_);
  }
  return last_returned_;
}

std::string SelfBalancingBinarySearchTree::Iterator::GetKey() {
  return next_->key;
}

std::shared_ptr<Record> SelfBalancingBinarySearchTree::Iterator::GetValue() {
  return std::make_shared<Record>(next_->value);
}

void SelfBalancingBinarySearchTree::Iterator::ResetValue() {
  next_->value.ResetEx();
}

}  // namespace s21