#ifndef A6_TRANSACTIONS_1_SRC_MODEL_COMMON_DATA_READER_H_
#define A6_TRANSACTIONS_1_SRC_MODEL_COMMON_DATA_READER_H_

#include <fstream>
#include <memory>
#include <utility>
#include <vector>

#include "../../useful_classes/strings_functions.h"
#include "../../useful_classes/tokenizer.h"
#include "../common/student.h"

namespace s21 {

class DataReader {
 public:
  static DataReader& GetInstance();
  bool Open(const std::string& path);
  bool isAvailable();
  std::pair<std::string, std::shared_ptr<Student>> GetNextData();

  ~DataReader();

 private:
  DataReader() = default;
  std::ifstream* in_;
};

}  // namespace s21

#endif  // A6_TRANSACTIONS_1_SRC_MODEL_COMMON_DATA_READER_H_
