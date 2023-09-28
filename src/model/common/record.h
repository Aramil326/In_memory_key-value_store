#ifndef A6_TRANSACTIONS_1_RECORD_H
#define A6_TRANSACTIONS_1_RECORD_H

#include <utility>

#include "student.h"

namespace s21 {

class Record {
 public:
  Record(const std::string& last_name, const std::string& first_name,
         int year_of_birth, const std::string& city, int coins, int EX)
      : student_(Student(last_name, first_name, year_of_birth, city, coins)) {
    EX_ = EX;
  }

  Record(Student student, int EX) : student_(std::move(student)), EX_(EX) {}

  const Student& getStudent() const;
  void UpdateStudent(const std::string& last_name,
                     const std::string& first_name, int year,
                     const std::string& city, int coins);
  int getEx() const;
  void DecreaseEx();
  void ResetEx();

 private:
  Student student_;
  volatile int EX_;
};

}  // namespace s21

#endif  // A6_TRANSACTIONS_1_RECORD_H
