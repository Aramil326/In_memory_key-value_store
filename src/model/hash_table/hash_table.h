#ifndef A6_TRANSACTIONS_1_HASH_TABLE_H
#define A6_TRANSACTIONS_1_HASH_TABLE_H

#include <iostream>
#include <mutex>
#include <thread>

#include "../../useful_classes/strings_functions.h"
#include "../common/main_store.h"
#include "../common/record.h"
#include "../file_handlers/data_reader.h"
#include "../file_handlers/data_writer.h"

namespace s21 {
class HashTable : public MainStore {
 public:
  HashTable();
  ~HashTable();

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

  class Node {
   public:
    Node(std::string key, Record record, Node* next_node = nullptr);
    ~Node() = default;
    std::string key;
    Record record;
    Node* next_node{};
  };

 private:
  const std::vector<int> sizes_{
      5,         11,        23,        47,         97,        193,
      389,       769,       1543,      3072,       12289,     24593,
      49157,     98317,     196613,    393241,     786433,    1572896,
      3145739,   6291469,   12582917,  25165843,   50331653,  100663319,
      201326611, 402653189, 805306457, 1610612736, 2147483629};
  const double factor_ = 0.75;
  int count_ = 0;
  int sizes_index_ = 0;
  std::vector<Node*> arr_;
  std::mutex mx_;
  std::vector<std::thread> thread_pool_;

  static int GetHash(const std::string& key);
  int GetIndex(const std::string& key);
  bool CheckMemory();
  void MemoryAdd();
  void MemoryClean();
  bool Put(Node* node);
  void StartRecordExpiration(Node* record);
  bool DeleteNode(const std::string& key);
  HashTable::Node* FindNode(const std::string& key);
};
}  // namespace s21

#endif  // A6_TRANSACTIONS_1_HASH_TABLE_H
