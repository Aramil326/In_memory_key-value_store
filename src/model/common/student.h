#ifndef A6_TRANSACTIONS_1_SRC_STUDENT_H_
#define A6_TRANSACTIONS_1_SRC_STUDENT_H_

#include <iostream>
#include <string>

namespace s21 {

class Student {
 public:
  Student(const std::string& last_name, const std::string& first_name,
          int year_of_birth, const std::string& city, int coins);

  const std::string& GetLastName() const;
  void SetLastName(const std::string& last_name);
  const std::string& GetFirstName() const;
  void SetFirstName(const std::string& first_name);
  int GetYearOfBirth() const;
  void SetYearOfBirth(int year_of_birth);
  const std::string& GetCity() const;
  void SetCity(const std::string& city);
  int GetCoins() const;
  void SetCoins(int coins);

  std::string toString() const;

  bool CompareTo(const std::string& last_name, const std::string& first_name,
                 const std::string& year, const std::string& city,
                 const std::string& coins);

  friend std::ostream& operator<<(std::ostream& os, const Student& student);

 private:
  std::string last_name_;
  std::string first_name_;
  int year_of_birth_;
  std::string city_;
  int coins_;
};

}  // namespace s21

#endif  // A6_TRANSACTIONS_1_SRC_STUDENT_H_
