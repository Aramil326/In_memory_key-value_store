#include "tokenizer.h"

namespace s21 {
void Tokenizer::Tokenize(const std::string& str, const char delim,
                         std::vector<std::string>& out) {
  size_t start;
  size_t end = 0;
  while ((start = str.find_first_not_of(delim, end)) != std::string::npos) {
    end = str.find(delim, start);
    out.push_back(str.substr(start, end - start));
  }
}
}  // namespace s21