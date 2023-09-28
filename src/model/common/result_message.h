#ifndef A6_TRANSACTIONS_1_SRC_RESULTMESSAGE_H_
#define A6_TRANSACTIONS_1_SRC_RESULTMESSAGE_H_

#include <string>
namespace s21 {

class ResultMessage {
 public:
  ResultMessage(bool error, std::string result_msg);
  bool IsError() const;
  const std::string& GetResultMsg() const;

 private:
  bool error_;
  std::string result_msg_;
};

}  // namespace s21

#endif  // A6_TRANSACTIONS_1_SRC_RESULTMESSAGE_H_
