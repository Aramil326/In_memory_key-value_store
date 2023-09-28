#include "hash_table.h"

namespace s21 {

HashTable::HashTable() {
  arr_.reserve(sizes_[sizes_index_]);
  arr_.resize(sizes_[sizes_index_], nullptr);
}

HashTable::~HashTable() {
  mx_.lock();
  for (int i = 0; i < sizes_[sizes_index_]; i++) {
    Node *temp = arr_[i];
    while (temp != nullptr) {
      temp->record.ResetEx();
      temp = temp->next_node;
    }
  }
  mx_.unlock();

  for (auto &thread : thread_pool_) {
    if (thread.joinable()) {
      thread.join();
    }
  }

  MemoryClean();
}

ResultMessage HashTable::Set(const std::string &key,
                             const std::string &last_name,
                             const std::string &first_name,
                             const std::string &year, const std::string &city,
                             const std::string &coins, const std::string &plug,
                             const std::string &EX) {
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
  if (!is_set) {
    delete node;
  }
  mx_.unlock();

  if (is_set) {
    if (i_EX >= 0) {
      thread_pool_.emplace_back(
          [this, node]() { StartRecordExpiration(node); });
    }
    return {false, "OK"};
  }
  return {true, "Key already exists"};
}

std::shared_ptr<const Student> HashTable::Get(const std::string &key) {
  int index = GetIndex(key);
  Node *temp = arr_[index];

  while (temp != nullptr) {
    if (temp->key == key) {
      return std::make_shared<Student>(temp->record.getStudent());
    }
    temp = temp->next_node;
  }

  return nullptr;
}

bool HashTable::Exists(const std::string &key) {
  std::lock_guard<std::mutex> lk(mx_);
  Node *node = FindNode(key);
  return node != nullptr;
}

bool HashTable::Del(const std::string &key) {
  std::lock_guard<std::mutex> lk(mx_);
  return DeleteNode(key);
}

ResultMessage HashTable::Update(const std::string &key,
                                const std::string &last_name,
                                const std::string &first_name,
                                const std::string &year,
                                const std::string &city,
                                const std::string &coins) {
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
      return {true, "ERROR: unable to cast value " + year + " to type int"};
    }
  } else {
    i_coins = -1;
  }

  std::lock_guard<std::mutex> lk(mx_);
  Node *node = FindNode(key);
  if (node != nullptr) {
    node->record.UpdateStudent(last_name, first_name, i_year, city, i_coins);
    return {false, "OK"};
  }
  return {true, "Key not found"};
}

std::vector<std::shared_ptr<const std::string>> HashTable::Keys() {
  std::vector<std::shared_ptr<const std::string>> result;
  std::lock_guard<std::mutex> lk(mx_);
  for (int i = 0; i < sizes_[sizes_index_]; i++) {
    Node *temp = arr_[i];
    while (temp != nullptr) {
      result.push_back(std::make_shared<const std::string>(temp->key));
      temp = temp->next_node;
    }
  }
  return result;
}

ResultMessage HashTable::Rename(const std::string &key,
                                const std::string &new_name) {
  if (key == new_name) {
    return {false, "OK"};
  }
  std::lock_guard<std::mutex> lk(mx_);
  if (FindNode(new_name) != nullptr) {
    return {true, "Key already exists"};
  }
  Node *node = FindNode(key);
  if (node != nullptr) {
    Node *new_node = new Node(new_name, node->record, nullptr);
    DeleteNode(node->key);
    bool is_set = Put(new_node);
    if (!is_set) {
      delete new_node;
    }

    return {false, "OK"};
  }
  return {true, "Key not found"};
}

std::shared_ptr<const int> HashTable::TTL(const std::string &key) {
  std::lock_guard<std::mutex> lk(mx_);
  Node *node = FindNode(key);
  if (node != nullptr) {
    int ex = node->record.getEx();
    return std::make_shared<const int>(ex);
  }
  return nullptr;
}

std::vector<std::shared_ptr<const std::string>> HashTable::Find(
    const std::string &last_name, const std::string &first_name,
    const std::string &year, const std::string &city,
    const std::string &coins) {
  std::vector<std::shared_ptr<const std::string>> result;
  std::lock_guard<std::mutex> lk(mx_);
  for (int i = 0; i < sizes_[sizes_index_]; i++) {
    Node *temp = arr_[i];
    while (temp != nullptr) {
      Student student = temp->record.getStudent();
      if (student.CompareTo(last_name, first_name, year, city, coins)) {
        result.push_back(std::make_shared<const std::string>(temp->key));
      }
      temp = temp->next_node;
    }
  }
  return result;
}

std::vector<std::shared_ptr<const Student>> HashTable::ShowAll() {
  std::vector<std::shared_ptr<const Student>> result;
  for (int i = 0; i < sizes_[sizes_index_]; i++) {
    Node *temp = arr_[i];
    while (temp != nullptr) {
      result.push_back(
          std::make_shared<const Student>(temp->record.getStudent()));
      temp = temp->next_node;
    }
  }
  return result;
}

ResultMessage HashTable::Upload(const std::string &path) {
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
      if (!is_set) {
        delete node;
      }
      mx_.unlock();
      if (is_set) {
        counter++;
      }
    }
  }
  return {false, "OK " + std::to_string(counter)};
}

ResultMessage HashTable::Export(const std::string &path) {
  DataWriter reader = DataWriter::GetInstance();
  bool isOpen = reader.Open(path);
  if (!isOpen) return {true, "Error: can't open file."};
  int counter = 0;

  for (int i = 0; i < sizes_[sizes_index_]; i++) {
    Node *temp = arr_[i];
    while (temp != nullptr) {
      if (!reader.WriteData({temp->key, temp->record.getStudent()}))
        return {true, "Error: access to the file was closed."};
      temp = temp->next_node;
      counter++;
    }
  }

  return {false, "OK " + std::to_string(counter)};
}

int HashTable::GetHash(const std::string &key) {
  int hash = 0;
  for (char i : key) {
    hash = (hash << 5) - hash + i;
  }
  return abs(hash);
}

int HashTable::GetIndex(const std::string &key) {
  arr_.reserve(sizes_[sizes_index_]);
  int hash = GetHash(key);
  return hash % static_cast<int>(arr_.capacity());
}

bool HashTable::CheckMemory() {
  return (double)count_ / sizes_[sizes_index_] >= factor_;
}

void HashTable::MemoryAdd() {
  std::vector<Node *> old_arr = arr_;
  count_ = 0;
  arr_.clear();

  arr_.reserve(sizes_[++sizes_index_]);
  arr_.resize(sizes_[sizes_index_], nullptr);

  for (int i = 0; i < sizes_[sizes_index_ - 1]; i++) {
    while (old_arr[i] != nullptr) {
      Node *temp = old_arr[i];
      old_arr[i] = old_arr[i]->next_node;
      temp->next_node = nullptr;
      bool is_set = Put(temp);
      if (!is_set) {
        delete temp;
      }
    }
  }
}

void HashTable::MemoryClean() {
  for (int i = 0; i < sizes_[sizes_index_]; i++) {
    while (arr_[i] != nullptr) {
      Node *to_clean = arr_[i];
      Node *next_node = arr_[i]->next_node;
      delete to_clean;
      arr_[i] = next_node;
    }
  }
}

bool HashTable::Put(HashTable::Node *node) {
  if (CheckMemory()) {
    MemoryAdd();
  }

  int index = GetIndex(node->key);
  Node *temp = arr_[index];

  while (temp != nullptr && temp->next_node != nullptr) {
    if (temp->key == node->key) {
      return false;
    }
    temp = temp->next_node;
  }

  if (temp == nullptr) {
    arr_[index] = node;
  } else {
    if (temp->key == node->key) {
      return false;
    }
    temp->next_node = node;
  }

  count_++;
  return true;
}

void HashTable::StartRecordExpiration(HashTable::Node *record) {
  while (record->record.getEx() > 0) {
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    std::lock_guard<std::mutex> lk(mx_);
    if (record->record.getEx() < 0) {
      return;
    }
    record->record.DecreaseEx();
  }
  mx_.lock();
  DeleteNode(record->key);
  mx_.unlock();
}

bool HashTable::DeleteNode(const std::string &key) {
  int index = GetIndex(key);
  Node *temp = arr_[index];

  if (temp == nullptr) {
    return false;
  }

  if (temp->key == key) {
    arr_[index] = temp->next_node;
    delete temp;
    return true;
  }

  Node *parent = temp;
  temp = temp->next_node;
  while (temp != nullptr) {
    if (temp->key == key) {
      parent->next_node = temp->next_node;
      delete temp;
      count_--;
      return true;
    }
    parent = temp;
    temp = temp->next_node;
  }
  return false;
}

HashTable::Node *HashTable::FindNode(const std::string &key) {
  int index = GetIndex(key);
  Node *temp = arr_[index];

  if (temp != nullptr) {
    while (temp != nullptr) {
      if (temp->key == key) {
        return temp;
      }
      temp = temp->next_node;
    }
  }
  return nullptr;
}

HashTable::Node::Node(std::string key, Record record,
                      HashTable::Node *next_node)
    : key(std::move(key)), record(std::move(record)), next_node(next_node) {}
}  // namespace s21
