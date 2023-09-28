#include "strings_functions.h"

namespace s21 {
bool Strings::isNumeric(const std::string& str) {
  return !str.empty() &&
         str.find_first_not_of("-0123456789") == std::string::npos;
}
}  // namespace s21