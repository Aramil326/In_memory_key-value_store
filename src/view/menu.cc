#include "menu.h"

namespace s21 {

Menu::Menu(std::shared_ptr<Controller> controller) {
  controller_ = std::move(controller);
}

void Menu::Start() {
  std::string store_type = GetStoreType();
  if (store_type == "Error") {
    PrintError("No such option: " + store_type);
    return;
  }
  controller_->SetModel(store_type);
  StartMainCycle();
}

std::string Menu::GetStoreType() {
  std::cout << "Select and enter the number of store type:\n"
               "\t1. HashTable\n"
               "\t2. SelfBalancingBinarySearchTree\n";
  std::string input;
  getline(std::cin, input);
  if (input == "1") {
    return "HashTable";
  } else if (input == "2") {
    return "SelfBalancingBinarySearchTree";
  } else {
    return "Error";
  }
}

void Menu::PrintError(const std::string& message) {
  std::cout << message << "\n";
}

void Menu::StartMainCycle() {
  std::string input;
  while (input != "Q") {
    std::cout << "Enter command. Enter Q to exit\n";
    getline(std::cin, input);
    RewindBuffer();
    if (input != "Q" && !input.empty()) ProceedInput(input);
  }
  std::cout << "Exit\n";
}

void Menu::ProceedInput(const std::string& input) {
  std::vector<std::string> tokens;
  Tokenizer::Tokenize(input, ' ', tokens);
  if (tokens[0] == "SET") {
    ProceedSet(tokens);
  } else if (tokens[0] == "GET") {
    ProceedGet(tokens);
  } else if (tokens[0] == "EXISTS") {
    ProceedExists(tokens);
  } else if (tokens[0] == "DEL") {
    ProceedDel(tokens);
  } else if (tokens[0] == "UPDATE") {
    ProceedUpdate(tokens);
  } else if (tokens[0] == "KEYS") {
    ProceedKeys(tokens);
  } else if (tokens[0] == "RENAME") {
    ProceedRename(tokens);
  } else if (tokens[0] == "TTL") {
    ProceedTTL(tokens);
  } else if (tokens[0] == "FIND") {
    ProceedFind(tokens);
  } else if (tokens[0] == "SHOWALL") {
    ProceedShowAll(tokens);
  } else if (tokens[0] == "UPLOAD") {
    ProceedUpload(tokens);
  } else if (tokens[0] == "EXPORT") {
    ProceedExport(tokens);
  } else {
    PrintError("No such command");
  }
}

void Menu::ProceedSet(const std::vector<std::string>& tokens) {
  ResultMessage result(false, "Error: SET command was entered wrong.");
  if (tokens.size() == 7) {
    result = controller_->Set(tokens[1], tokens[2], tokens[3], tokens[4],
                              tokens[5], tokens[6]);
  }
  if (tokens.size() == 9) {
    result = controller_->Set(tokens[1], tokens[2], tokens[3], tokens[4],
                              tokens[5], tokens[6], tokens[7], tokens[8]);
  }
  std::cout << result.GetResultMsg() << "\n";
}

void Menu::ProceedGet(const std::vector<std::string>& tokens) {
  if (tokens.size() != 2) {
    std::cout << "Error: GET command was entered wrong.\n";
    return;
  }
  std::shared_ptr<const Student> student = controller_->Get(tokens[1]);
  std::cout << (student == nullptr ? "(null)" : student->toString()) << "\n";
}

void Menu::ProceedExists(const std::vector<std::string>& tokens) {
  if (tokens.size() != 2) {
    std::cout << "Error: EXISTS command was entered wrong.\n";
    return;
  }
  bool exists = controller_->Exists(tokens[1]);
  std::cout << (exists ? "true" : "false") << "\n";
}

void Menu::ProceedDel(const std::vector<std::string>& tokens) {
  if (tokens.size() != 2) {
    std::cout << "Error: DEL command was entered wrong.\n";
    return;
  }
  bool exists = controller_->Del(tokens[1]);
  std::cout << (exists ? "true" : "false") << "\n";
}

void Menu::ProceedUpdate(const std::vector<std::string>& tokens) {
  if (tokens.size() != 7) {
    std::cout << "Error: UPDATE command was entered wrong.\n";
    return;
  }
  ResultMessage result = controller_->Update(tokens[1], tokens[2], tokens[3],
                                             tokens[4], tokens[5], tokens[6]);
  std::cout << result.GetResultMsg() << "\n";
}

void Menu::ProceedKeys(const std::vector<std::string>& tokens) {
  if (tokens.size() != 1) {
    std::cout << "Error: KEYS command was entered wrong.\n";
    return;
  }
  std::vector<std::shared_ptr<const std::string>> keys = controller_->Keys();
  for (int i = 0; i < (int)keys.size(); ++i) {
    std::cout << (i + 1) << ") " << *keys[i] << "\n";
  }
}

void Menu::ProceedRename(const std::vector<std::string>& tokens) {
  if (tokens.size() != 3) {
    std::cout << "Error: RENAME command was entered wrong.\n";
    return;
  }
  ResultMessage result = controller_->Rename(tokens[1], tokens[2]);
  std::cout << result.GetResultMsg() << "\n";
}

void Menu::ProceedTTL(const std::vector<std::string>& tokens) {
  if (tokens.size() != 2) {
    std::cout << "Error: TTL command was entered wrong.\n";
    return;
  }
  std::shared_ptr<const int> ex = controller_->TTL(tokens[1]);
  std::cout << (ex == nullptr ? "(null)" : std::to_string(*ex)) << "\n";
}

void Menu::ProceedFind(const std::vector<std::string>& tokens) {
  if (tokens.size() != 6) {
    std::cout << "Error: FIND command was entered wrong.\n";
    return;
  }
  std::vector<std::shared_ptr<const std::string>> keys =
      controller_->Find(tokens[1], tokens[2], tokens[3], tokens[4], tokens[5]);
  for (int i = 0; i < (int)keys.size(); ++i) {
    std::cout << (i + 1) << ") " << *keys[i] << "\n";
  }
}

void Menu::ProceedShowAll(const std::vector<std::string>& tokens) {
  if (tokens.size() != 1) {
    std::cout << "Error: SHOWALL command was entered wrong.\n";
    return;
  }
  std::vector<std::shared_ptr<const Student>> students = controller_->ShowAll();

  printf("%1s |%10s%*s%13s%*s%6s%*s%6s%*s%16s%*s\n", "№", "Last name", 2, "|",
         "First name", 4, "|", "Year", 3, "|", "City", 3, "|",
         "Number of coins", 2, "|");

  for (int i = 0; i < (int)students.size(); ++i) {
    int last_name_size = (int)students[i]->GetLastName().length();
    int first_name_size = (int)students[i]->GetFirstName().length();
    int city_size = (int)students[i]->GetCity().length();
    int coins_size = (int)std::to_string(students[i]->GetCoins()).length();
    printf("%1d |%*s%*s%*s%*s%*d%*s%*s%*s%*s%*s\n", i + 1,

           last_name_size + ((11 - last_name_size) / 2),
           students[i]->GetLastName().c_str(),

           1 + (int)(ceil((11. - last_name_size) / 2)), "|",

           first_name_size + ((16 - first_name_size) / 2),
           students[i]->GetFirstName().c_str(),

           1 + (int)(ceil((16. - first_name_size) / 2)), "|", 6,

           students[i]->GetYearOfBirth(), 3, "|",

           city_size + ((8 - city_size) / 2), students[i]->GetCity().c_str(),
           1 + (int)ceil(((8. - city_size) / 2)), "|",

           coins_size + ((17 - coins_size) / 2),
           std::to_string(students[i]->GetCoins()).c_str(),
           1 + (int)ceil((17. - coins_size) / 2), "|");
  }
}

void Menu::ProceedUpload(const std::vector<std::string>& tokens) {
  if (tokens.size() != 2) {
    std::cout << "Error: UPLOAD command was entered wrong.\n";
    return;
  }
  ResultMessage result = controller_->Upload(tokens[1]);
  std::cout << result.GetResultMsg() << "\n";
}

void Menu::ProceedExport(const std::vector<std::string>& tokens) {
  if (tokens.size() != 2) {
    std::cout << "Error: UPLOAD command was entered wrong.\n";
    return;
  }
  ResultMessage result = controller_->Export(tokens[1]);
  std::cout << result.GetResultMsg() << "\n";
}

void Menu::RewindBuffer() {
  std::cin.clear();
  //  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

}  // namespace s21