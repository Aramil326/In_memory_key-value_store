#include "view/menu.h"

using namespace s21;
int main() {
  std::shared_ptr<MainStore> model =
      std::shared_ptr<SelfBalancingBinarySearchTree>(
          new SelfBalancingBinarySearchTree());
  std::shared_ptr<Controller> controller =
      std::shared_ptr<Controller>(new Controller(model));
  Menu menu(controller);
  menu.Start();
}
