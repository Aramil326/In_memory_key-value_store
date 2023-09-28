#ifndef A6_TRANSACTIONS_1_SRC_MODEL_FILE_HANDLERS_DATA_WRITER_H_
#define A6_TRANSACTIONS_1_SRC_MODEL_FILE_HANDLERS_DATA_WRITER_H_

#include <fstream>
#include <memory>
#include <string>

#include "../common/student.h"

namespace s21 {

class DataWriter {
 public:
  static DataWriter& GetInstance();
  bool Open(const std::string& path);
  bool WriteData(std::pair<std::string, Student> data);
  ~DataWriter();

 private:
  DataWriter() = default;
  std::ofstream* out_;
};

}  // namespace s21

#endif  // A6_TRANSACTIONS_1_SRC_MODEL_FILE_HANDLERS_DATA_WRITER_H_
