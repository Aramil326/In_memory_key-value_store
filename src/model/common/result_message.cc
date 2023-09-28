#include "result_message.h"

#include <utility>

namespace s21 {
ResultMessage::ResultMessage(bool error, std::string result_msg)
    : error_(error), result_msg_(std::move(result_msg)) {}

bool ResultMessage::IsError() const { return error_; }

const std::string& ResultMessage::GetResultMsg() const { return result_msg_; }
}  // namespace s21