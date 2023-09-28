#ifndef A6_TRANSACTIONS_1_SRC_SELFBALANCINGBINARYSEARCHTREE_H_
#define A6_TRANSACTIONS_1_SRC_SELFBALANCINGBINARYSEARCHTREE_H_

#include <cmath>
#include <fstream>
#include <mutex>
#include <thread>
#include <utility>

#include "../../useful_classes/strings_functions.h"
#include "../../useful_classes/tokenizer.h"
#include "../common/main_store.h"
#include "../common/record.h"
#include "../file_handlers/data_reader.h"
#include "../file_handlers/data_writer.h"

namespace s21 {
#define RED false
#define BLACK true

class SelfBalancingBinarySearchTree : public MainStore {
 public:
  SelfBalancingBinarySearchTree();
  ~SelfBalancingBinarySearchTree();

  ResultMessage Set(const std::string& key, const std::string& last_name,
                    const std::string& first_name, const std::string& year,
                    const std::string& city, const std::string& coins,
                    const std::string& plug, const std::string& EX) override;

  std::shared_ptr<const Student> Get(const std::string& key) override;

  bool Exists(const std::string& key) override;

  bool Del(const std::string& key) override;

  ResultMessage Update(const std::string& key, const std::string& last_name,
                       const std::string& first_name, const std::string& year,
                       const std::string& city,
                       const std::string& coins) override;

  std::vector<std::shared_ptr<const std::string>> Keys() override;

  ResultMessage Rename(const std::string& key,
                       const std::string& new_name) override;

  std::shared_ptr<const int> TTL(const std::string& key) override;

  std::vector<std::shared_ptr<const std::string>> Find(
      const std::string& last_name, const std::string& first_name,
      const std::string& year, const std::string& city,
      const std::string& coins) override;

  std::vector<std::shared_ptr<const Student>> ShowAll() override;

  ResultMessage Upload(const std::string& path) override;

  ResultMessage Export(const std::string& path) override;

 private:
  class Node;
  class Iterator;
  using node_ptr = Node*;

  node_ptr root_;
  size_t size_;
  std::mutex mx_;
  std::vector<std::thread> thread_pool_;

  class Node {
   public:
    std::string key;
    Record value;
    node_ptr left;
    node_ptr right;
    node_ptr parent;
    bool color;

    Node(std::string key, Record value, node_ptr parent);
  };

  class Iterator {
   public:
    explicit Iterator(Node* node);
    [[nodiscard]] bool HasNext() const;
    node_ptr Next();
    std::string GetKey();
    std::shared_ptr<Record> GetValue();
    void ResetValue();

   private:
    node_ptr next_;
    node_ptr last_returned_;
  };

  void RotateLeft(const node_ptr& node);
  void RotateRight(const node_ptr& node);

  bool Put(Node* node);

  void FixAfterInsertion(node_ptr node);

  void DeleteNode(node_ptr node);
  void FixAfterDeletion(node_ptr node);

  node_ptr FindMinNode();
  node_ptr FindNode(const std::string& key);

  void StartRecordExpiration(Node* record);

  static bool ColorOf(const node_ptr& node);
  static void SetColor(const node_ptr& node, bool color);
  static node_ptr ParentOf(const node_ptr& node);
  static node_ptr LeftOf(const node_ptr& node);
  static node_ptr RightOf(const node_ptr& node);
  static node_ptr Increment(const node_ptr& node);

  void DeleteTree_(node_ptr node);
};

}  // namespace s21

#endif  // A6_TRANSACTIONS_1_SRC_SELFBALANCINGBINARYSEARCHTREE_H_
