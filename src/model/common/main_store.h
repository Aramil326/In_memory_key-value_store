//
// Created by maslynem on 15.05.23.
//

#ifndef A6_TRANSACTIONS_1_SRC_MAINSTORE_H_
#define A6_TRANSACTIONS_1_SRC_MAINSTORE_H_

#include <memory>
#include <vector>

#include "result_message.h"
#include "student.h"

namespace s21 {

class MainStore {
 public:
  virtual ResultMessage Set(const std::string& key,
                            const std::string& last_name,
                            const std::string& first_name,
                            const std::string& year, const std::string& city,
                            const std::string& coins, const std::string& plug,
                            const std::string& EX) = 0;
  virtual std::shared_ptr<const Student> Get(const std::string& key) = 0;
  virtual bool Exists(const std::string& key) = 0;
  virtual bool Del(const std::string& key) = 0;
  virtual ResultMessage Update(const std::string& key,
                               const std::string& last_name,
                               const std::string& first_name,
                               const std::string& year, const std::string& city,
                               const std::string& coins) = 0;
  virtual std::vector<std::shared_ptr<const std::string>> Keys() = 0;
  virtual ResultMessage Rename(const std::string& key,
                               const std::string& new_name) = 0;
  virtual std::shared_ptr<const int> TTL(const std::string& key) = 0;
  virtual std::vector<std::shared_ptr<const std::string>> Find(
      const std::string& last_name, const std::string& first_name,
      const std::string& year, const std::string& city,
      const std::string& coins) = 0;
  virtual std::vector<std::shared_ptr<const Student>> ShowAll() = 0;
  virtual ResultMessage Upload(const std::string& path) = 0;
  virtual ResultMessage Export(const std::string& path) = 0;
};

}  // namespace s21

#endif  // A6_TRANSACTIONS_1_SRC_MAINSTORE_H_
