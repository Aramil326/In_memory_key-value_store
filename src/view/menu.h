#ifndef A6_TRANSACTIONS_1_MENU_H
#define A6_TRANSACTIONS_1_MENU_H

#include <iomanip>
#include <iostream>

#include "../controller/controller.h"
#include "../useful_classes/tokenizer.h"

namespace s21 {

class Menu {
 public:
  explicit Menu(std::shared_ptr<Controller> controller);
  void Start();

 private:
  std::shared_ptr<Controller> controller_;
  std::string GetStoreType();
  void PrintError(const std::string& message);
  void StartMainCycle();
  void ProceedInput(const std::string& command);
  void ProceedSet(const std::vector<std::string>& tokens);
  void ProceedGet(const std::vector<std::string>& tokens);
  void ProceedExists(const std::vector<std::string>& tokens);
  void ProceedDel(const std::vector<std::string>& tokens);
  void ProceedUpdate(const std::vector<std::string>& tokens);
  void ProceedKeys(const std::vector<std::string>& tokens);
  void ProceedRename(const std::vector<std::string>& tokens);
  void ProceedTTL(const std::vector<std::string>& tokens);
  void ProceedFind(const std::vector<std::string>& tokens);
  void ProceedShowAll(const std::vector<std::string>& tokens);
  void ProceedUpload(const std::vector<std::string>& tokens);
  void ProceedExport(const std::vector<std::string>& tokens);
  static void RewindBuffer();
};

}  // namespace s21

#endif  // A6_TRANSACTIONS_1_MENU_H
