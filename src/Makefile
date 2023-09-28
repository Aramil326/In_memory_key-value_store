CC = gcc -std=c++17
WWW = -Wall -Werror -Wextra
OS := $(shell uname)

ifeq ($(OS),Linux)
	CHFLAGS = -lgtest -lpthread -lrt -lm -lsubunit -lstdc++
else
	CHFLAGS = -lgtest -lm -lstdc++
endif

VC_SOURCES = controller/controller.cc \
			 view/menu.cc

COMMON_SOURCES =  model/common/record.cc \
                  model/common/result_message.cc \
                  model/common/student.cc \
                  model/file_handlers/data_reader.cc \
                  model/file_handlers/data_writer.cc \
                  useful_classes/tokenizer.cc \
                  useful_classes/strings_functions.cc \

HASH_TABLE_SOURCES = model/hash_table/hash_table.cc

RB_TREE_SOURCES = model/rb_tree/self_balancing_binary_search_tree.cc

TEST_SOURCES = unit_tests/unit_test.cc \
			   unit_tests/rb_tree_tests.cc \
		       unit_tests/hash_table_tests.cc

all: clean hash_table.a self_balancing_binary_search_tree.a run gcov_report

run: clean
	$(CC) $(WWW) main.cc $(HASH_TABLE_SOURCES) $(RB_TREE_SOURCES) $(COMMON_SOURCES) $(VC_SOURCES) $(CHFLAGS)
	./a.out

hash_table.a: clean
	$(CC) $(WWW) -c $(HASH_TABLE_SOURCES) $(COMMON_SOURCES) controller/controller.cc $(CHFLAGS)
	ar -crs hash_table.a *.o
	rm -rf *.o

self_balancing_binary_search_tree.a: clean
	$(CC) $(WWW) -c $(RB_TREE_SOURCES) $(COMMON_SOURCES) controller/controller.cc $(CHFLAGS)
	ar -crs self_balancing_binary_search_tree.a *.o
	rm -rf *.o

clean:
	rm -rf *.o a.out *.a unit_tests/unit_test unit_tests/*.txt
	rm -rf unit_tests/*.gcno unit_tests/*gcda unit_tests/*.gcov gcov program
	rm -rf report report.info
	rm -rf *.gcno *gcda *.gcov gcov

tests: clean
	$(CC) $(WWW) $(TEST_SOURCES) $(HASH_TABLE_SOURCES) $(RB_TREE_SOURCES) $(COMMON_SOURCES) controller/controller.cc -o unit_tests/unit_test $(CHFLAGS)
	leaks -atExit -- ./unit_tests/unit_test
	rm -rf unit_tests/unit_test

gcov_report: clean
	$(CC) --coverage $(TEST_SOURCES) $(HASH_TABLE_SOURCES) $(RB_TREE_SOURCES) $(COMMON_SOURCES) controller/controller.cc $(CHFLAGS) -o unit_tests/unit_test
	./unit_tests/unit_test
	lcov -t "unit_tests/unit_test" -o report.info --no-external --ignore-errors mismatch -c -d .
	genhtml -o report report.info
	open ./report/index.html
	rm -rf *.gcno *gcda *.gcov gcov

style:
	clang-format -style=google -n main.cc $(HASH_TABLE_SOURCES) $(RB_TREE_SOURCES) $(COMMON_SOURCES) $(VC_SOURCES) $(TEST_SOURCES)

