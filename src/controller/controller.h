#ifndef A6_TRANSACTIONS_1_CONTROLLER_H
#define A6_TRANSACTIONS_1_CONTROLLER_H

#include "../model/common/main_store.h"
#include "../model/hash_table/hash_table.h"
#include "../model/rb_tree/self_balancing_binary_search_tree.h"

namespace s21 {

class Controller {
 public:
  Controller(std::shared_ptr<MainStore> model);

  void SetModel(const std::string& model);

  ResultMessage Set(const std::string& key, const std::string& last_name,
                    const std::string& first_name, const std::string& year,
                    const std::string& city, const std::string& coins,
                    const std::string& plug = "EX",
                    const std::string& EX = "-1");

  std::shared_ptr<const Student> Get(const std::string& key) const;

  bool Exists(const std::string& key);

  bool Del(const std::string& key);

  ResultMessage Update(const std::string& key, const std::string& last_name,
                       const std::string& first_name, const std::string& year,
                       const std::string& city, const std::string& coins);

  std::vector<std::shared_ptr<const std::string> > Keys();

  ResultMessage Rename(const std::string& key, const std::string& new_name);

  std::shared_ptr<const int> TTL(const std::string& key);

  std::vector<std::shared_ptr<const std::string> > Find(
      const std::string& last_name, const std::string& first_name,
      const std::string& year, const std::string& city,
      const std::string& coins);

  std::vector<std::shared_ptr<const Student> > ShowAll();

  ResultMessage Upload(const std::string& path);

  ResultMessage Export(const std::string& path);

 private:
  std::shared_ptr<MainStore> model_;
};

}  // namespace s21

#endif  // A6_TRANSACTIONS_1_CONTROLLER_H
