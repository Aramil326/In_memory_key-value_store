#include "record.h"

namespace s21 {
void Record::DecreaseEx() { EX_ -= 1; }

const Student& Record::getStudent() const { return student_; }

int Record::getEx() const { return EX_; }

void Record::ResetEx() { EX_ = -1; }

void Record::UpdateStudent(const std::string& last_name,
                           const std::string& first_name, int year,
                           const std::string& city, int coins) {
  if (last_name != "-") {
    student_.SetLastName(last_name);
  }
  if (first_name != "-") {
    student_.SetFirstName(first_name);
  }
  if (year != -1) {
    student_.SetYearOfBirth(year);
  }
  if (city != "-") {
    student_.SetCity(city);
  }
  if (coins != -1) {
    student_.SetCoins(coins);
  }
}
}  // namespace s21