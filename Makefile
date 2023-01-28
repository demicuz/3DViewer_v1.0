NAME		:= 3DViewer
PREFIX		:= /.local

SRC_DIR		:= src
LIB_DIR		:= lib
TEST_DIR	:= test
INCLUDE_DIR	:= include

CIMGUI_DIR	:= cimgui
LIBCIMGUI	:= $(LIB_DIR)/libcimgui.a

NFD_DIR		:= nativefiledialog
LIBNFD		:= $(LIB_DIR)/libnfd.a

SRC			:= $(wildcard $(SRC_DIR)/*.c $(SRC_DIR)/*/*.c)
OBJ			:= $(SRC:.c=.o)

TEST_SRC	:= $(wildcard $(TEST_DIR)/*.c $(SRC_DIR)/*/*.c)
TEST_OBJ	:= $(TEST_SRC:.c=.o)
TEST_EXEC	:= run_tests
TEST_LIBS   := -lcheck

# TODO set correct flags
CC			:= gcc -fdiagnostics-color=always
CPPFLAGS	:= -I $(INCLUDE_DIR) -I $(NFD_DIR)/src/include -MMD -MP -DCIMGUI_USE_OPENGL3 -DCIMGUI_USE_GLFW
CFLAGS		:= -std=c11 -pedantic -Og -g -Wall -Wextra -Werror
LDFLAGS		:= -L $(LIB_DIR)
LDLIBS		:= -lcimgui -lnfd -lstdc++

UNAME_S		:= $(shell uname -s)
ifeq ($(UNAME_S), Linux) #LINUX
	LDLIBS		+= -lGL `pkg-config --static --libs glfw3` `pkg-config --libs gtk+-3.0`
	NFD_MAKE	:= $(NFD_DIR)/build/gmake_linux
	TEST_LIBS   += -lm -lpthread -lrt -lsubunit
	OPEN		:= xdg-open
endif

ifeq ($(UNAME_S), Darwin) #APPLE
# 	CPPFLAGS	+= -I/usr/local/include -I/opt/local/include -I/opt/homebrew/include
	CPPFLAGS	+= -I/usr/local/include -I/opt/local/include -I/Users/$(USER)/goinfre/homebrew/opt/glfw/include
# 	LDFLAGS		+= -L/usr/local/lib -L/opt/local/lib -L/opt/homebrew/lib
	LDFLAGS		+= -L/usr/local/lib -L/opt/local/lib -L/Users/$(USER)/goinfre/homebrew/opt/glfw/lib
	LDLIBS		+= -lglfw -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo
	NFD_MAKE	:= $(NFD_DIR)/build/gmake_macosx
	OPEN		:= open
endif

.PHONY: all bonus clean_obj clean fclean re tests html dist install uninstall style

all: $(NAME)

bonus: $(NAME)

$(LIBCIMGUI): $(LIB_DIR)
	$(MAKE) --directory=$(CIMGUI_DIR) static
	cp $(CIMGUI_DIR)/libcimgui.a $(LIB_DIR)

$(LIBNFD): $(LIB_DIR)
	$(MAKE) --directory=$(NFD_MAKE)
	cp $(NFD_DIR)/build/lib/Release/x64/libnfd.a $(LIB_DIR)

$(NAME): $(OBJ) $(LIBCIMGUI) $(LIBNFD)
	$(CC) $(LDFLAGS) $(OBJ) $(LDLIBS) -o $@

# Turns out you don't have to have a rule for object files. 'Make' uses CFLAGS
# and CPPFLAGS variables to compile .o files.
# %.o: %.c
# 	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(LIB_DIR):
	mkdir -p $@

clean_obj:
	@rm -vf $(OBJ)
	@rm -vf $(wildcard $(TEST_DIR)/*.o)

clean: clean_obj
	@rm -vf $(OBJ:.o=.d)
	@rm -vf $(wildcard $(TEST_DIR)/*.d)
	$(MAKE) clean --directory=$(CIMGUI_DIR)
	$(MAKE) clean --directory=$(NFD_MAKE)
	@rm -rvf $(NFD_DIR)/build/obj

fclean: clean
	@rm -vf $(NAME)
	@rm -vf $(TEST_EXEC)
	@rm -rvf $(LIB_DIR)
	@rm -rvf gcov_report
	@rm -vf gcov_report.info
	@rm -vf $(shell find $(SRC_DIR) -type f -name "*.gcda")
	@rm -vf $(shell find $(SRC_DIR) -type f -name "*.gcno")
	@rm -vf $(shell find $(TEST_DIR) -type f -name "*.gcno")
	@rm -vf $(shell find $(TEST_DIR) -type f -name "*.gcno")
	@rm -rvf docs_generated
	@rm -rf 3DViewer_v1.0.tar.gz
	$(MAKE) fclean --directory=$(CIMGUI_DIR)

re: fclean all

tests: $(TEST_EXEC)
	@echo "-------TESTS-------"
	@./$(TEST_EXEC) 2> /dev/null
	@echo "-------------------"

$(TEST_EXEC): $(TEST_OBJ) $(LIBCIMGUI) $(LIBNFD)
	$(CC) $(LDFLAGS) $(TEST_OBJ) $(LDLIBS) $(TEST_LIBS) -o $@

add_coverage_flag:
	$(eval CFLAGS += --coverage)
	$(eval LDFLAGS += --coverage)

# TODO wasteful `gcov_report`
# Currently deletes all `.o` files, generates new ones with `--coverage`,
# then deletes them again. Far from optimal!
gcov_report: add_coverage_flag clean_obj tests
	@lcov --directory . -t "3DViewer_tests" -o gcov_report.info -c --no-external
	@lcov -q --remove gcov_report.info "`pwd`/test/*" -o gcov_report.info
	@lcov -q --remove gcov_report.info "`pwd`/src/3rd-party/*" -o gcov_report.info
	@genhtml -o $@ gcov_report.info
	@$(OPEN) ./gcov_report/index.html
	@rm -vf $(OBJ) $(wildcard $(TEST_DIR)/*.o)

dvi: html

html:
	@doxygen doxygen.conf
	@$(OPEN) docs_generated/html/index.html

dist: fclean
	mkdir -p 3DViewer_v1.0_dist
	ln -s "`pwd`/Makefile" 3DViewer_v1.0_dist/Makefile
	ln -s "`pwd`/doxygen.conf" 3DViewer_v1.0_dist/doxygen.conf
	ln -s "`pwd`/imgui.ini" 3DViewer_v1.0_dist/imgui.ini
	ln -s "`pwd`/.clang-format" 3DViewer_v1.0_dist/.clang-format
	ln -s "`pwd`/src" 3DViewer_v1.0_dist/src
	ln -s "`pwd`/include" 3DViewer_v1.0_dist/include
	ln -s "`pwd`/docs" 3DViewer_v1.0_dist/docs
	ln -s "`pwd`/models" 3DViewer_v1.0_dist/models
	ln -s "`pwd`/cimgui" 3DViewer_v1.0_dist/cimgui
	ln -s "`pwd`/nativefiledialog" 3DViewer_v1.0_dist/nativefiledialog
	tar -h -cvzf 3DViewer_v1.0.tar.gz 3DViewer_v1.0_dist
	rm -rvf 3DViewer_v1.0_dist

# Example usage:
# make install DESTDIR=~
install: $(NAME)
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	install -m 755 $(NAME) $(DESTDIR)$(PREFIX)/bin
	cp imgui.ini $(DESTDIR)$(PREFIX)/bin

uninstall:
	rm -f $(DESTDIR)$(PREFIX)/bin/$(NAME)
	rm -f $(DESTDIR)$(PREFIX)/bin/imgui.ini

style:
	@clang-format -n $(shell find $(SRC_DIR) -type f -name "*.[ch]") \
                     $(shell find $(TEST_DIR) -type f -name "*.[ch]") \
                     $(wildcard $(INCLUDE_DIR)/*.h)

-include $(OBJ:.o=.d)
