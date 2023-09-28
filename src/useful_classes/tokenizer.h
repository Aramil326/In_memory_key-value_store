#ifndef A6_TRANSACTIONS_1_SRC_MODEL_COMMON_TOKENIZER_H_
#define A6_TRANSACTIONS_1_SRC_MODEL_COMMON_TOKENIZER_H_

#include <string>
#include <vector>

namespace s21 {

class Tokenizer {
 public:
  Tokenizer() = delete;
  static void Tokenize(const std::string &str, const char delim,
                       std::vector<std::string> &out);
};

}  // namespace s21

#endif  // A6_TRANSACTIONS_1_SRC_MODEL_COMMON_TOKENIZER_H_
