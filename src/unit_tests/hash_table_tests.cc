#include <gtest/gtest.h>

#include "../controller/controller.h"

using namespace s21;

Controller GetHashTableController() {
  std::shared_ptr<MainStore> model =
      std::shared_ptr<HashTable>(new HashTable());
  return {model};
}

TEST(test_Hash_Table_Set, test_1) {
  Controller controller = GetHashTableController();
  controller.SetModel("HashTable");
  ResultMessage resultMessage =
      controller.Set("1", "LastName", "FirstName", "1998", "Kazan", "300");
  ASSERT_FALSE(resultMessage.IsError());
}

TEST(test_Hash_Table_Set, test_2) {
  Controller controller = GetHashTableController();
  controller.SetModel("HashTable");
  controller.Set("1", "LastName", "FirstName", "1998", "Kazan", "300");
  ResultMessage resultMessage =
      controller.Set("1", "LastName", "FirstName", "1998", "Kazan", "300");
  ASSERT_TRUE(resultMessage.IsError());
}

TEST(test_Hash_Table_Set, test_3) {
  Controller controller = GetHashTableController();
  controller.SetModel("HashTable");
  ResultMessage resultMessage =
      controller.Set("1", "LastName", "FirstName", "ERROR", "Kazan", "300");
  ASSERT_TRUE(resultMessage.IsError());
}

TEST(test_Hash_Table_Set, test_4) {
  Controller controller = GetHashTableController();
  controller.SetModel("HashTable");
  ResultMessage resultMessage =
      controller.Set("1", "LastName", "FirstName", "1998", "Kazan", "ERROR");
  ASSERT_TRUE(resultMessage.IsError());
}

TEST(test_Hash_Table_Set, test_5) {
  Controller controller = GetHashTableController();
  controller.SetModel("HashTable");
  ResultMessage resultMessage =
      controller.Set("1", "LastName", "FirstName", "ERROR", "Kazan", "ERROR");
  ASSERT_TRUE(resultMessage.IsError());
}

TEST(test_Hash_Table_Set, test_6) {
  Controller controller = GetHashTableController();
  controller.SetModel("HashTable");
  ResultMessage resultMessage = controller.Set(
      "1", "LastName", "FirstName", "1998", "Kazan", "300", "EX", "10");
  ASSERT_FALSE(resultMessage.IsError());
}

TEST(test_Hash_Table_Set, test_7) {
  Controller controller = GetHashTableController();
  controller.SetModel("HashTable");
  ResultMessage resultMessage = controller.Set("1", "LastName", "FirstName",
                                               "1998", "Kazan", "300", "ERROR");
  ASSERT_TRUE(resultMessage.IsError());
}

TEST(test_Hash_Table_Get, test_1) {
  Controller controller = GetHashTableController();
  controller.SetModel("HashTable");
  controller.Set("1", "LastName", "FirstName", "1998", "Kazan", "300");
  std::shared_ptr<const Student> Student = controller.Get("1");
  ASSERT_TRUE(Student != nullptr);
}

TEST(test_Hash_Table_Get, test_2) {
  Controller controller = GetHashTableController();
  controller.SetModel("HashTable");
  controller.Set("1", "LastName1", "FirstName1", "1998", "Kazan1", "100");
  controller.Set("2", "LastName2", "FirstName2", "1998", "Kazan2", "200");
  controller.Set("3", "LastName3", "FirstName3", "1998", "Kazan3", "300");
  controller.Set("4", "LastName4", "FirstName4", "1998", "Kazan4", "400");
  controller.Set("5", "LastName5", "FirstName5", "1998", "Kazan5", "500");
  controller.Set("6", "LastName6", "FirstName6", "1998", "Kazan6", "600");
  std::shared_ptr<const Student> Student = controller.Get("1");
  ASSERT_TRUE(Student != nullptr);
  ASSERT_TRUE(Student->GetLastName() == "LastName1");
  ASSERT_TRUE(Student->GetFirstName() == "FirstName1");
  ASSERT_TRUE(Student->GetYearOfBirth() == 1998);
  ASSERT_TRUE(Student->GetCity() == "Kazan1");
  ASSERT_TRUE(Student->GetCoins() == 100);
}

TEST(test_Hash_Table_Get, test_3) {
  Controller controller = GetHashTableController();
  controller.SetModel("HashTable");
  controller.Set("1", "LastName1", "FirstName1", "1998", "Kazan1", "100");
  controller.Set("2", "LastName2", "FirstName2", "1998", "Kazan2", "200");
  controller.Set("3", "LastName3", "FirstName3", "1998", "Kazan3", "300");
  controller.Set("4", "LastName4", "FirstName4", "1998", "Kazan4", "400");
  controller.Set("5", "LastName5", "FirstName5", "1998", "Kazan5", "500");
  controller.Set("6", "LastName6", "FirstName6", "1998", "Kazan6", "600");
  std::shared_ptr<const Student> Student = controller.Get("5");
  ASSERT_TRUE(Student != nullptr);
  ASSERT_TRUE(Student->GetLastName() == "LastName5");
  ASSERT_TRUE(Student->GetFirstName() == "FirstName5");
  ASSERT_TRUE(Student->GetYearOfBirth() == 1998);
  ASSERT_TRUE(Student->GetCity() == "Kazan5");
  ASSERT_TRUE(Student->GetCoins() == 500);
}

TEST(test_Hash_Table_Get, test_4) {
  Controller controller = GetHashTableController();
  controller.SetModel("HashTable");
  controller.Set("1", "LastName", "FirstName", "1998", "Kazan", "300");
  std::shared_ptr<const Student> Student = controller.Get("2");
  ASSERT_TRUE(Student == nullptr);
}

TEST(test_Hash_Table_Exists, test_1) {
  Controller controller = GetHashTableController();
  controller.SetModel("HashTable");
  controller.Set("1", "LastName", "FirstName", "1998", "Kazan", "300");
  ASSERT_TRUE(controller.Exists("1"));
}

TEST(test_Hash_Table_Exists, test_2) {
  Controller controller = GetHashTableController();
  controller.SetModel("HashTable");
  controller.Set("1", "LastName", "FirstName", "1998", "Kazan", "300");
  ASSERT_FALSE(controller.Exists("2"));
}

TEST(test_Hash_Table_Del, test_1) {
  Controller controller = GetHashTableController();
  controller.SetModel("HashTable");
  controller.Set("1", "LastName1", "FirstName1", "1998", "Kazan1", "100");
  controller.Set("2", "LastName2", "FirstName2", "1998", "Kazan2", "200");
  controller.Set("3", "LastName3", "FirstName3", "1998", "Kazan3", "300");
  ASSERT_TRUE(controller.Del("1"));
}

TEST(test_Hash_Table_Del, test_2) {
  Controller controller = GetHashTableController();
  controller.SetModel("HashTable");
  controller.Set("1", "LastName1", "FirstName1", "1998", "Kazan1", "100");
  controller.Set("2", "LastName2", "FirstName2", "1998", "Kazan2", "200");
  controller.Set("3", "LastName3", "FirstName3", "1998", "Kazan3", "300");
  ASSERT_FALSE(controller.Del("5"));
}

TEST(test_Hash_Table_Set_Del_Get_Exists, test_1) {
  Controller controller = GetHashTableController();
  controller.SetModel("HashTable");
  controller.Set("1", "LastName1", "FirstName1", "1998", "Kazan1", "100");
  controller.Set("2", "LastName2", "FirstName2", "1998", "Kazan2", "200");
  controller.Set("3", "LastName3", "FirstName3", "1998", "Kazan3", "300");
  ASSERT_TRUE(controller.Del("2"));
  ASSERT_TRUE(controller.Get("2") == nullptr);
  ASSERT_FALSE(controller.Exists("2"));
  ASSERT_TRUE(controller.Exists("1"));
  ASSERT_TRUE(controller.Exists("3"));
  ASSERT_FALSE(controller.Del("2"));
  ASSERT_TRUE(controller.Del("1"));
  ASSERT_TRUE(controller.Get("1") == nullptr);
  ASSERT_FALSE(controller.Exists("1"));
  ASSERT_TRUE(controller.Exists("3"));
}

TEST(test_Hash_Table_Update, test_1) {
  Controller controller = GetHashTableController();
  controller.SetModel("HashTable");
  controller.Set("1", "LastName1", "FirstName1", "1998", "Kazan1", "100");
  ResultMessage message = controller.Update("1", "LastName2", "FirstName2",
                                            "1998", "Kazan2", "200");
  ASSERT_FALSE(message.IsError());
}

TEST(test_Hash_Table_Update, test_2) {
  Controller controller = GetHashTableController();
  controller.SetModel("HashTable");
  controller.Set("1", "LastName1", "FirstName1", "1998", "Kazan1", "100");
  ResultMessage message = controller.Update("4", "LastName2", "FirstName2",
                                            "1998", "Kazan2", "200");
  ASSERT_TRUE(message.IsError());
}

TEST(test_Hash_Table_Update, test_3) {
  Controller controller = GetHashTableController();
  controller.SetModel("HashTable");
  controller.Set("1", "LastName1", "FirstName1", "1998", "Kazan1", "100");
  ResultMessage message =
      controller.Update("1", "-", "FirstName2", "1998", "Kazan2", "200");
  ASSERT_FALSE(message.IsError());
  std::shared_ptr<const Student> Student = controller.Get("1");
  ASSERT_TRUE(Student->GetLastName() == "LastName1");
  ASSERT_TRUE(Student->GetFirstName() == "FirstName2");
  ASSERT_TRUE(Student->GetYearOfBirth() == 1998);
  ASSERT_TRUE(Student->GetCity() == "Kazan2");
  ASSERT_TRUE(Student->GetCoins() == 200);
}

TEST(test_Hash_Table_Update, test_4) {
  Controller controller = GetHashTableController();
  controller.SetModel("HashTable");
  controller.Set("1", "LastName1", "FirstName1", "1998", "Kazan1", "100");
  ResultMessage message =
      controller.Update("1", "LastName2", "-", "1998", "Kazan2", "200");
  ASSERT_FALSE(message.IsError());
  std::shared_ptr<const Student> Student = controller.Get("1");
  ASSERT_TRUE(Student->GetLastName() == "LastName2");
  ASSERT_TRUE(Student->GetFirstName() == "FirstName1");
  ASSERT_TRUE(Student->GetYearOfBirth() == 1998);
  ASSERT_TRUE(Student->GetCity() == "Kazan2");
  ASSERT_TRUE(Student->GetCoins() == 200);
}

TEST(test_Hash_Table_Update, test_5) {
  Controller controller = GetHashTableController();
  controller.SetModel("HashTable");
  controller.Set("1", "LastName1", "FirstName1", "1998", "Kazan1", "100");
  ResultMessage message =
      controller.Update("1", "LastName2", "FirstName2", "-", "Kazan2", "200");
  ASSERT_FALSE(message.IsError());
  std::shared_ptr<const Student> Student = controller.Get("1");
  ASSERT_TRUE(Student->GetLastName() == "LastName2");
  ASSERT_TRUE(Student->GetFirstName() == "FirstName2");
  ASSERT_TRUE(Student->GetYearOfBirth() == 1998);
  ASSERT_TRUE(Student->GetCity() == "Kazan2");
  ASSERT_TRUE(Student->GetCoins() == 200);
}

TEST(test_Hash_Table_Update, test_6) {
  Controller controller = GetHashTableController();
  controller.SetModel("HashTable");
  controller.Set("1", "LastName1", "FirstName1", "1998", "Kazan1", "100");
  ResultMessage message =
      controller.Update("1", "LastName2", "FirstName2", "1998", "-", "200");
  ASSERT_FALSE(message.IsError());
  std::shared_ptr<const Student> Student = controller.Get("1");
  ASSERT_TRUE(Student->GetLastName() == "LastName2");
  ASSERT_TRUE(Student->GetFirstName() == "FirstName2");
  ASSERT_TRUE(Student->GetYearOfBirth() == 1998);
  ASSERT_TRUE(Student->GetCity() == "Kazan1");
  ASSERT_TRUE(Student->GetCoins() == 200);
}

TEST(test_Hash_Table_Update, test_7) {
  Controller controller = GetHashTableController();
  controller.SetModel("HashTable");
  controller.Set("1", "LastName1", "FirstName1", "1998", "Kazan1", "100");
  ResultMessage message =
      controller.Update("1", "LastName2", "FirstName2", "1998", "Kazan2", "-");
  ASSERT_FALSE(message.IsError());
  std::shared_ptr<const Student> Student = controller.Get("1");
  ASSERT_TRUE(Student->GetLastName() == "LastName2");
  ASSERT_TRUE(Student->GetFirstName() == "FirstName2");
  ASSERT_TRUE(Student->GetYearOfBirth() == 1998);
  ASSERT_TRUE(Student->GetCity() == "Kazan2");
  ASSERT_TRUE(Student->GetCoins() == 100);
}

TEST(test_Hash_Table_Update, test_8) {
  Controller controller = GetHashTableController();
  controller.SetModel("HashTable");
  controller.Set("1", "LastName1", "FirstName1", "1998", "Kazan1", "100");
  ResultMessage message =
      controller.Update("1", "LastName2", "FirstName2", "-", "Kazan2", "-");
  ASSERT_FALSE(message.IsError());
  std::shared_ptr<const Student> Student = controller.Get("1");
  ASSERT_TRUE(Student->GetLastName() == "LastName2");
  ASSERT_TRUE(Student->GetFirstName() == "FirstName2");
  ASSERT_TRUE(Student->GetYearOfBirth() == 1998);
  ASSERT_TRUE(Student->GetCity() == "Kazan2");
  ASSERT_TRUE(Student->GetCoins() == 100);
}

TEST(test_Hash_Table_Update, test_9) {
  Controller controller = GetHashTableController();
  controller.SetModel("HashTable");
  controller.Set("1", "LastName1", "FirstName1", "1998", "Kazan1", "100");
  ResultMessage message =
      controller.Update("1", "-", "FirstName2", "1998", "Kazan2", "-");
  ASSERT_FALSE(message.IsError());
  std::shared_ptr<const Student> Student = controller.Get("1");
  ASSERT_TRUE(Student->GetLastName() == "LastName1");
  ASSERT_TRUE(Student->GetFirstName() == "FirstName2");
  ASSERT_TRUE(Student->GetYearOfBirth() == 1998);
  ASSERT_TRUE(Student->GetCity() == "Kazan2");
  ASSERT_TRUE(Student->GetCoins() == 100);
}

TEST(test_Hash_Table_Update, test_10) {
  Controller controller = GetHashTableController();
  controller.SetModel("HashTable");
  controller.Set("1", "LastName1", "FirstName1", "1998", "Kazan1", "100");
  ResultMessage message =
      controller.Update("1", "-", "FirstName2", "1998", "-", "-");
  ASSERT_FALSE(message.IsError());
  std::shared_ptr<const Student> Student = controller.Get("1");
  ASSERT_TRUE(Student->GetLastName() == "LastName1");
  ASSERT_TRUE(Student->GetFirstName() == "FirstName2");
  ASSERT_TRUE(Student->GetYearOfBirth() == 1998);
  ASSERT_TRUE(Student->GetCity() == "Kazan1");
  ASSERT_TRUE(Student->GetCoins() == 100);
}

TEST(test_Hash_Table_Update, test_11) {
  Controller controller = GetHashTableController();
  controller.SetModel("HashTable");
  controller.Set("1", "LastName1", "FirstName1", "1998", "Kazan1", "100");
  ResultMessage message =
      controller.Update("1", "LastName2", "-", "1998", "-", "-");
  ASSERT_FALSE(message.IsError());
  std::shared_ptr<const Student> Student = controller.Get("1");
  ASSERT_TRUE(Student->GetLastName() == "LastName2");
  ASSERT_TRUE(Student->GetFirstName() == "FirstName1");
  ASSERT_TRUE(Student->GetYearOfBirth() == 1998);
  ASSERT_TRUE(Student->GetCity() == "Kazan1");
  ASSERT_TRUE(Student->GetCoins() == 100);
}

TEST(test_Hash_Table_Update, test_12) {
  Controller controller = GetHashTableController();
  controller.SetModel("HashTable");
  controller.Set("1", "LastName1", "FirstName1", "1998", "Kazan1", "100");
  ResultMessage message = controller.Update("1", "-", "-", "-", "-", "-");
  ASSERT_FALSE(message.IsError());
  std::shared_ptr<const Student> Student = controller.Get("1");
  ASSERT_TRUE(Student->GetLastName() == "LastName1");
  ASSERT_TRUE(Student->GetFirstName() == "FirstName1");
  ASSERT_TRUE(Student->GetYearOfBirth() == 1998);
  ASSERT_TRUE(Student->GetCity() == "Kazan1");
  ASSERT_TRUE(Student->GetCoins() == 100);
}

TEST(test_Hash_Table_Keys, test_1) {
  Controller controller = GetHashTableController();
  controller.SetModel("HashTable");
  controller.Set("5", "LastName5", "FirstName5", "1998", "Kazan5", "500");
  controller.Set("1", "LastName1", "FirstName1", "1998", "Kazan1", "100");
  controller.Set("6", "LastName6", "FirstName6", "1998", "Kazan6", "600");
  controller.Set("2", "LastName2", "FirstName2", "1998", "Kazan2", "200");
  controller.Set("9", "LastName9", "FirstName9", "1998", "Kazan9", "900");
  controller.Set("8", "LastName8", "FirstName8", "1998", "Kazan8", "800");
  controller.Set("7", "LastName7", "FirstName7", "1998", "Kazan7", "700");
  controller.Set("4", "LastName4", "FirstName4", "1998", "Kazan4", "400");
  controller.Set("3", "LastName3", "FirstName3", "1998", "Kazan3", "300");

  auto keys = controller.Keys();
  ASSERT_TRUE(keys.size() == 9);
}

TEST(test_Hash_Table_Keys, test_2) {
  Controller controller = GetHashTableController();
  controller.SetModel("HashTable");
  controller.Set("1", "LastName1", "FirstName1", "1998", "Kazan1", "100");
  controller.Del("1");
  auto keys = controller.Keys();
  ASSERT_TRUE(keys.empty());
}

TEST(test_Hash_Table_Rename, test_1) {
  Controller controller = GetHashTableController();
  controller.SetModel("HashTable");
  controller.Set("1", "LastName1", "FirstName1", "1998", "Kazan1", "100");
  ASSERT_TRUE(controller.Exists("1"));
  ASSERT_FALSE(controller.Exists("2"));
  ResultMessage message = controller.Rename("1", "2");
  ASSERT_FALSE(message.IsError());
  ASSERT_FALSE(controller.Exists("1"));
  ASSERT_TRUE(controller.Exists("2"));
}

TEST(test_Hash_Table_Rename, test_2) {
  Controller controller = GetHashTableController();
  controller.SetModel("HashTable");
  controller.Set("1", "LastName1", "FirstName1", "1998", "Kazan1", "100");
  ASSERT_TRUE(controller.Exists("1"));
  ASSERT_FALSE(controller.Exists("2"));
  ResultMessage message = controller.Rename("2", "1");
  ASSERT_TRUE(message.IsError());
  ASSERT_TRUE(controller.Exists("1"));
  ASSERT_FALSE(controller.Exists("2"));
}

TEST(test_Hash_Table_Rename, test_3) {
  Controller controller = GetHashTableController();
  controller.SetModel("HashTable");
  controller.Set("1", "LastName1", "FirstName1", "1991", "Kazan1", "100");
  controller.Set("2", "LastName2", "FirstName2", "1992", "Kazan2", "200");
  ASSERT_TRUE(controller.Exists("1"));
  ASSERT_TRUE(controller.Exists("2"));
  ResultMessage message = controller.Rename("1", "2");
  ASSERT_TRUE(message.IsError());
  ASSERT_TRUE(controller.Exists("1"));
  ASSERT_TRUE(controller.Exists("2"));
}

TEST(test_Hash_Table_Rename, test_4) {
  Controller controller = GetHashTableController();
  controller.SetModel("HashTable");
  controller.Set("1", "LastName1", "FirstName1", "1991", "Kazan1", "100");
  controller.Set("2", "LastName2", "FirstName2", "1992", "Kazan2", "200");
  ASSERT_TRUE(controller.Exists("1"));
  ASSERT_TRUE(controller.Exists("2"));
  ResultMessage message = controller.Rename("1", "1");
  ASSERT_FALSE(message.IsError());
  ASSERT_TRUE(controller.Exists("1"));
  ASSERT_TRUE(controller.Exists("2"));
}

TEST(test_Hash_Table_TTL, test_1) {
  Controller controller = GetHashTableController();
  controller.SetModel("HashTable");
  controller.Set("1", "LastName1", "FirstName1", "1991", "Kazan1", "100");
  std::shared_ptr<const int> TTL = controller.TTL("1");
  ASSERT_TRUE(*TTL == -1);
}

TEST(test_Hash_Table_TTL, test_2) {
  Controller controller = GetHashTableController();
  controller.SetModel("HashTable");
  controller.Set("1", "LastName1", "FirstName1", "1991", "Kazan1", "100", "EX",
                 "10");
  std::shared_ptr<const int> TTL = controller.TTL("1");
  ASSERT_TRUE(*TTL == 10);
}

TEST(test_Hash_Table_TTL, test_3) {
  Controller controller = GetHashTableController();
  controller.SetModel("HashTable");
  ResultMessage resultMessage = controller.Set(
      "1", "LastName1", "FirstName1", "1991", "Kazan1", "100", "ERROR");
  ASSERT_TRUE(resultMessage.IsError());
  std::shared_ptr<const int> TTL = controller.TTL("1");
  ASSERT_TRUE(TTL == nullptr);
}

TEST(test_Hash_Table_TTL, test_4) {
  Controller controller = GetHashTableController();
  controller.SetModel("HashTable");
  controller.Set("1", "LastName1", "FirstName1", "1991", "Kazan1", "100", "EX",
                 "1");
  ASSERT_TRUE(controller.Exists("1"));
  std::shared_ptr<const int> TTL = controller.TTL("1");
  ASSERT_TRUE(*TTL == 1);
  std::this_thread::sleep_for(std::chrono::milliseconds(1100));
  ASSERT_FALSE(controller.Exists("1"));
  TTL = controller.TTL("1");
  ASSERT_TRUE(TTL == nullptr);
}

TEST(test_Hash_Table_TTL, test_5) {
  Controller controller = GetHashTableController();
  controller.SetModel("HashTable");
  controller.Set("1", "LastName1", "FirstName1", "1991", "Kazan1", "100", "EX",
                 "10");
  controller.Set("2", "LastName2", "FirstName2", "1992", "Kazan2", "200", "EX",
                 "5");
  controller.Set("3", "LastName3", "FirstName3", "1993", "Kazan3", "300", "EX",
                 "1");
  ASSERT_TRUE(controller.Exists("1"));
  ASSERT_TRUE(controller.Exists("2"));
  ASSERT_TRUE(controller.Exists("3"));
  std::shared_ptr<const int> TTL_1 = controller.TTL("1");
  std::shared_ptr<const int> TTL_2 = controller.TTL("2");
  std::shared_ptr<const int> TTL_3 = controller.TTL("3");
  ASSERT_TRUE(*TTL_1 == 10);
  ASSERT_TRUE(*TTL_2 == 5);
  ASSERT_TRUE(*TTL_3 == 1);
  std::this_thread::sleep_for(std::chrono::milliseconds(1100));
  TTL_1 = controller.TTL("1");
  TTL_2 = controller.TTL("2");
  TTL_3 = controller.TTL("3");
  ASSERT_TRUE(*TTL_1 == 9);
  ASSERT_TRUE(*TTL_2 == 4);
  ASSERT_TRUE(TTL_3 == nullptr);
  ASSERT_TRUE(controller.Exists("1"));
  ASSERT_TRUE(controller.Exists("2"));
  ASSERT_FALSE(controller.Exists("3"));
}

TEST(test_Hash_Table_Find, test_1) {
  Controller controller = GetHashTableController();
  controller.SetModel("HashTable");
  controller.Set("5", "LastName5", "FirstName5", "1998", "Kazan5", "500");
  controller.Set("1", "LastName1", "FirstName1", "1998", "Kazan1", "100");
  controller.Set("6", "LastName6", "FirstName6", "1998", "Kazan6", "600");
  controller.Set("2", "LastName2", "FirstName2", "1998", "Kazan2", "200");
  controller.Set("9", "LastName9", "FirstName9", "1998", "Kazan9", "900");
  controller.Set("8", "LastName8", "FirstName8", "1998", "Kazan8", "800");
  controller.Set("7", "LastName7", "FirstName7", "1998", "Kazan7", "700");
  controller.Set("4", "LastName4", "FirstName4", "1998", "Kazan4", "400");
  controller.Set("3", "LastName3", "FirstName3", "1998", "Kazan3", "300");

  auto keys =
      controller.Find("LastName1", "FirstName1", "1998", "Kazan1", "100");
  ASSERT_TRUE(keys.size() == 1);
  ASSERT_TRUE(*keys[0] == "1");
}

TEST(test_Hash_Table_Find, test_2) {
  Controller controller = GetHashTableController();
  controller.SetModel("HashTable");
  controller.Set("5", "LastName5", "FirstName5", "1998", "Kazan5", "500");
  controller.Set("1", "LastName1", "FirstName1", "1998", "Kazan1", "100");
  controller.Set("6", "LastName6", "FirstName6", "1998", "Kazan6", "600");
  controller.Set("2", "LastName2", "FirstName2", "1998", "Kazan2", "200");
  controller.Set("9", "LastName9", "FirstName9", "1998", "Kazan9", "900");
  controller.Set("8", "LastName1", "FirstName8", "1998", "Kazan8", "800");
  controller.Set("7", "LastName7", "FirstName7", "1998", "Kazan7", "700");
  controller.Set("4", "LastName4", "FirstName4", "1998", "Kazan4", "400");
  controller.Set("3", "LastName3", "FirstName3", "1998", "Kazan3", "300");

  auto keys = controller.Find("LastName1", "-", "-", "-", "-");
  ASSERT_TRUE(keys.size() == 2);
  ASSERT_TRUE((*keys[0] == "1" && *keys[1] == "8") ||
              (*keys[0] == "8" && *keys[1] == "1"));
}

TEST(test_Hash_Table_Find, test_3) {
  Controller controller = GetHashTableController();
  controller.SetModel("HashTable");
  controller.Set("5", "LastName5", "FirstName5", "1998", "Kazan5", "500");
  controller.Set("1", "LastName1", "FirstName1", "1998", "Kazan1", "100");
  controller.Set("6", "LastName6", "FirstName6", "1998", "Kazan6", "600");
  controller.Set("2", "LastName2", "FirstName2", "1998", "Kazan2", "200");
  controller.Set("9", "LastName9", "FirstName9", "1998", "Kazan9", "900");
  controller.Set("8", "LastName1", "FirstName8", "1998", "Kazan8", "800");
  controller.Set("7", "LastName7", "FirstName7", "1998", "Kazan7", "700");
  controller.Set("4", "LastName4", "FirstName4", "1998", "Kazan4", "400");
  controller.Set("3", "LastName3", "FirstName3", "1998", "Kazan3", "300");

  auto keys = controller.Find("LastName1", "FirstName8", "-", "-", "-");
  ASSERT_TRUE(keys.size() == 1);
  ASSERT_TRUE(*keys[0] == "8");
}

TEST(test_Hash_Table_Find, test_4) {
  Controller controller = GetHashTableController();
  controller.SetModel("HashTable");
  controller.Set("5", "LastName5", "FirstName5", "1998", "Kazan5", "500");
  controller.Set("1", "LastName1", "FirstName1", "1998", "Kazan1", "100");
  controller.Set("6", "LastName6", "FirstName6", "1998", "Kazan6", "600");
  controller.Set("2", "LastName2", "FirstName2", "1998", "Kazan2", "200");
  controller.Set("9", "LastName9", "FirstName9", "1998", "Kazan9", "900");
  controller.Set("8", "LastName1", "FirstName8", "1998", "Kazan8", "800");
  controller.Set("7", "LastName7", "FirstName7", "1998", "Kazan7", "700");
  controller.Set("4", "LastName4", "FirstName4", "1998", "Kazan4", "400");
  controller.Set("3", "LastName3", "FirstName3", "1998", "Kazan3", "300");

  auto keys = controller.Find("-", "-", "1998", "-", "-");
  ASSERT_TRUE(keys.size() == 9);
}

TEST(test_Hash_Table_Find, test_5) {
  Controller controller = GetHashTableController();
  controller.SetModel("HashTable");
  controller.Set("1", "LastName1", "FirstName8", "1998", "Kazan1", "100");
  controller.Set("3", "LastName1", "FirstName8", "1998", "Kazan8", "800");
  controller.Set("8", "LastName1", "FirstName3", "1998", "Kazan3", "300");
  controller.Set("5", "LastName5", "FirstName5", "1998", "Kazan5", "500");
  controller.Set("6", "LastName6", "FirstName6", "1998", "Kazan6", "600");
  controller.Set("2", "LastName2", "FirstName2", "1998", "Kazan2", "200");
  controller.Set("9", "LastName9", "FirstName9", "1998", "Kazan9", "900");
  controller.Set("7", "LastName7", "FirstName7", "1998", "Kazan7", "700");
  controller.Set("4", "LastName4", "FirstName4", "1998", "Kazan4", "400");

  auto keys = controller.Find("LastName1", "FirstName8", "1998", "-", "-");
  ASSERT_TRUE(keys.size() == 2);
  ASSERT_TRUE((*keys[0] == "1" && *keys[1] == "3") ||
              (*keys[0] == "3" && *keys[1] == "1"));
}

TEST(test_Hash_Table_Show_All, test_1) {
  Controller controller = GetHashTableController();
  controller.SetModel("HashTable");
  controller.Set("5", "LastName5", "FirstName5", "1998", "Kazan5", "500");
  controller.Set("1", "LastName1", "FirstName1", "1998", "Kazan1", "100");
  controller.Set("6", "LastName6", "FirstName6", "1998", "Kazan6", "600");
  controller.Set("2", "LastName2", "FirstName2", "1998", "Kazan2", "200");
  controller.Set("9", "LastName9", "FirstName9", "1998", "Kazan9", "900");
  controller.Set("8", "LastName8", "FirstName8", "1998", "Kazan8", "800");
  controller.Set("7", "LastName7", "FirstName7", "1998", "Kazan7", "700");
  controller.Set("4", "LastName4", "FirstName4", "1998", "Kazan4", "400");
  controller.Set("3", "LastName3", "FirstName3", "1998", "Kazan3", "300");
  auto all = controller.ShowAll();
  ASSERT_TRUE(all.size() == 9);
}

TEST(test_Hash_Table_Show_All, test_2) {
  Controller controller = GetHashTableController();
  controller.SetModel("HashTable");
  auto all = controller.ShowAll();
  ASSERT_TRUE(all.empty());
}

TEST(test_Hash_Table_UPLOAD, test_1) {
  Controller controller = GetHashTableController();
  controller.SetModel("HashTable");
  ResultMessage result = controller.Upload("../src/data.txt");
  ASSERT_FALSE(result.IsError());
  ASSERT_TRUE(result.GetResultMsg() == "OK 9");
}

TEST(test_Hash_Table_UPLOAD_GET, test_1) {
  Controller controller = GetHashTableController();
  controller.SetModel("HashTable");
  ResultMessage result = controller.Upload("../src/data.txt");
  ASSERT_FALSE(result.IsError());
  ASSERT_TRUE(result.GetResultMsg() == "OK 9");
  std::shared_ptr<const Student> student = controller.Get("key1");
  ASSERT_TRUE(student != nullptr);
  ASSERT_TRUE(student->GetLastName() == "Vasiliev");
  ASSERT_TRUE(student->GetFirstName() == "Ivan");
  ASSERT_TRUE(student->GetYearOfBirth() == 2001);
  ASSERT_TRUE(student->GetCity() == "Rostov");
  ASSERT_TRUE(student->GetCoins() == 55);
}

TEST(test_Hash_Table_UPLOAD_GET, test_2) {
  Controller controller = GetHashTableController();
  controller.SetModel("HashTable");
  controller.Set("key10", "LastName5", "FirstName5", "1998", "Kazan5", "500");
  ResultMessage result = controller.Upload("../src/data.txt");
  ASSERT_FALSE(result.IsError());
  ASSERT_TRUE(result.GetResultMsg() == "OK 9");
  std::shared_ptr<const Student> student = controller.Get("key10");
  ASSERT_TRUE(student != nullptr);
  ASSERT_TRUE(student->GetLastName() == "LastName5");
  ASSERT_TRUE(student->GetFirstName() == "FirstName5");
  ASSERT_TRUE(student->GetYearOfBirth() == 1998);
  ASSERT_TRUE(student->GetCity() == "Kazan5");
  ASSERT_TRUE(student->GetCoins() == 500);
}

TEST(test_Hash_Table_EXPORT, test_1) {
  Controller controller = GetHashTableController();
  controller.SetModel("HashTable");
  controller.Upload("../src/data.txt");
  ResultMessage result =
      controller.Export("unit_tests/export_hash_table_data.txt");
  ASSERT_FALSE(result.IsError());
  ASSERT_TRUE(result.GetResultMsg() == "OK 9");
}
