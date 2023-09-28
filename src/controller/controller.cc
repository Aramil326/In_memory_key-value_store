#include "controller.h"

namespace s21 {

Controller::Controller(std::shared_ptr<MainStore> model) {
  model_ = std::move(model);
}

void Controller::SetModel(const std::string& model) {
  if (model == "SelfBalancingBinarySearchTree") {
    model_ = std::make_shared<SelfBalancingBinarySearchTree>();
  } else if (model == "HashTable") {
    model_ = std::make_shared<HashTable>();
  }
}

ResultMessage Controller::Set(const std::string& key,
                              const std::string& last_name,
                              const std::string& first_name,
                              const std::string& year, const std::string& city,
                              const std::string& coins, const std::string& plug,
                              const std::string& EX) {
  return model_->Set(key, last_name, first_name, year, city, coins, plug, EX);
}

std::shared_ptr<const Student> Controller::Get(const std::string& key) const {
  return model_->Get(key);
}

bool Controller::Exists(const std::string& key) { return model_->Exists(key); }

bool Controller::Del(const std::string& key) { return model_->Del(key); }

ResultMessage Controller::Update(const std::string& key,
                                 const std::string& last_name,
                                 const std::string& first_name,
                                 const std::string& year,
                                 const std::string& city,
                                 const std::string& coins) {
  return model_->Update(key, last_name, first_name, year, city, coins);
}

std::vector<std::shared_ptr<const std::string>> Controller::Keys() {
  return model_->Keys();
}

ResultMessage Controller::Rename(const std::string& key,
                                 const std::string& new_name) {
  return model_->Rename(key, new_name);
}

std::shared_ptr<const int> Controller::TTL(const std::string& key) {
  return model_->TTL(key);
}

std::vector<std::shared_ptr<const std::string>> Controller::Find(
    const std::string& last_name, const std::string& first_name,
    const std::string& year, const std::string& city,
    const std::string& coins) {
  return model_->Find(last_name, first_name, year, city, coins);
}

std::vector<std::shared_ptr<const Student>> Controller::ShowAll() {
  return model_->ShowAll();
}

ResultMessage Controller::Upload(const std::string& path) {
  return model_->Upload(path);
}
ResultMessage Controller::Export(const std::string& path) {
  return model_->Export(path);
}

}  // namespace s21