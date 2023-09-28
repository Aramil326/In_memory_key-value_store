#include "student.h"

s21::Student::Student(const std::string& last_name,
                      const std::string& first_name, int year_of_birth,
                      const std::string& city, int coins)
    : last_name_(last_name),
      first_name_(first_name),
      year_of_birth_(year_of_birth),
      city_(city),
      coins_(coins) {}

const std::string& s21::Student::GetLastName() const { return last_name_; }

void s21::Student::SetLastName(const std::string& last_name) {
  last_name_ = last_name;
}

const std::string& s21::Student::GetFirstName() const { return first_name_; }

void s21::Student::SetFirstName(const std::string& first_name) {
  first_name_ = first_name;
}

int s21::Student::GetYearOfBirth() const { return year_of_birth_; }

void s21::Student::SetYearOfBirth(int year_of_birth) {
  year_of_birth_ = year_of_birth;
}

const std::string& s21::Student::GetCity() const { return city_; }

void s21::Student::SetCity(const std::string& city) { city_ = city; }

int s21::Student::GetCoins() const { return coins_; }

void s21::Student::SetCoins(int coins) { coins_ = coins; }

std::string s21::Student::toString() const {
  return last_name_ + " " + first_name_ + " " + std::to_string(year_of_birth_) +
         " " + city_ + " " + std::to_string(coins_);
}

bool s21::Student::CompareTo(const std::string& last_name,
                             const std::string& first_name,
                             const std::string& year, const std::string& city,
                             const std::string& coins) {
  if ((last_name != "-" && last_name_ != last_name) ||
      (first_name != "-" && first_name_ != first_name) ||
      (year != "-" && std::to_string(year_of_birth_) != year) ||
      (city != "-" && city_ != city) ||
      (coins != "-" && std::to_string(coins_) != coins)) {
    return false;
  }
  return true;
}

std::ostream& operator<<(std::ostream& os, const s21::Student& student) {
  os << student.toString();
  return os;
}
