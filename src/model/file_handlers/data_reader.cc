#include "data_reader.h"

namespace s21 {

DataReader::~DataReader() {
  if (in_ != nullptr && in_->is_open()) in_->close();
  delete in_;
}

bool DataReader::Open(const std::string& path) {
  if (in_ != nullptr && in_->is_open()) in_->close();
  delete in_;
  in_ = new std::ifstream(path);
  return in_->is_open();
}

DataReader& DataReader::GetInstance() {
  static DataReader instance;
  return instance;
}

bool DataReader::isAvailable() { return !in_->eof(); }

std::pair<std::string, std::shared_ptr<Student>> DataReader::GetNextData() {
  if (in_->is_open() && !in_->eof()) {
    std::string line;
    getline(*in_, line);
    std::vector<std::string> tokens;
    Tokenizer::Tokenize(line, ' ', tokens);
    if (tokens.size() != 6) return std::make_pair("", nullptr);
    int year;
    int coins;
    if (Strings::isNumeric(tokens[3]) && Strings::isNumeric(tokens[5])) {
      year = std::stoi(tokens[3]);
      coins = std::stoi(tokens[5]);
    } else {
      return std::make_pair("", nullptr);
    }
    std::shared_ptr<Student> ptr(
        new Student(tokens[1].substr(1, tokens[1].length() - 2),
                    tokens[2].substr(1, tokens[2].length() - 2), year,
                    tokens[4].substr(1, tokens[4].length() - 2), coins));
    return std::make_pair(tokens[0], ptr);
  }
  return std::make_pair("", nullptr);
}
}  // namespace s21