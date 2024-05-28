COMPILE_FLAGS = -Wall -Werror -Wextra
EXECUTABLE = tests
TAR_FILE = calc.tar


ifeq ($(shell uname), Linux)
	BUILD_FLAGS = -lcheck_pic -lsubunit -lm
endif
ifeq ($(shell uname), Darwin)
	BUILD_FLAGS = -lcheck -lm
endif

SOURCES = front/parser.c
CUSTOM_DATA_TYPE = $(wildcard backend/user_data_types/*.c)

OBJ_SOURCES = $(patsubst %.c, %.o, SOURCES)
OBJ_HELPERS = $(patsubst %.c, %.o)
OBJ_CUSTOM_DATA_TYPE = $(patsubst %.c, %.o)


all:  install 

test:
	@gcc $(COMPILE_FLAGS) $(SOURCES)   calc_tests/backend_tests.c -o tests $(BUILD_FLAGS)
	@./$(EXECUTABLE)
	@rm -f $(EXECUTABLE)

valgrind:
	@gcc $(SOURCES)   calc_tests/backend_tests.c -o tests $(BUILD_FLAGS)
	@./$(EXECUTABLE)
	valgrind --leak-check=full \
			 --show-leak-kinds=all \
			 --track-origins=yes \
			 --verbose  ./$(EXECUTABLE)



install: clean
	mkdir -p build
	cd build && cmake ../front && $(MAKE) all

uninstall:
	rm -rf build

dist:
	tar cvf $(TAR_FILE) .
	tar tvf $(TAR_FILE)

gcov_report: clean
	@gcc $(COMPILE_FLAGS) $(SOURCES) calc_tests/backend_tests.c $(BUILD_FLAGS) -o $(EXECUTABLE)  --coverage
	@./$(EXECUTABLE)
	@lcov -t "tests" -o tests.info -c -d .
	@genhtml -o report tests.info
	open report/index.html

dvi:
	doxygen Doxyfile
	open docs/html/index.html

clang:
	clang-format --style=google -n  $(SOURCES)   calc_tests/backend_tests.c


clean:
	@rm -rf test-test* test-eq* test-is* equal_size* is_* tests*
	@rm -rf *.gcda
	@rm -rf *.gcno
	@rm -rf *.info
	@rm -f tests
	@rm -rf report
	@rm -rf docs
	@rm -rf build
	@rm -rf calc.tar
