#include "data_writer.h"

namespace s21 {
DataWriter& DataWriter::GetInstance() {
  static DataWriter instance;
  return instance;
}

bool DataWriter::Open(const std::string& path) {
  if (out_ != nullptr && out_->is_open()) out_->close();
  delete out_;
  out_ = new std::ofstream(path);
  return out_->is_open();
}

DataWriter::~DataWriter() {
  if (out_ != nullptr && out_->is_open()) out_->close();
  delete out_;
}

bool DataWriter::WriteData(std::pair<std::string, Student> data) {
  if (out_->is_open()) {
    (*out_) << data.first << " " << data.second.toString() << "\n";
  }
  return out_->is_open();
}

}  // namespace s21