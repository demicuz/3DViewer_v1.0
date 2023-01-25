NAME		:= 3DViewer

SRC_DIR		:= src
LIB_DIR		:= lib
TEST_DIR	:= test

CIMGUI_DIR	:= cimgui
LIBCIMGUI	:= $(LIB_DIR)/libcimgui.a

NFD_DIR		:= nativefiledialog
LIBNFD		:= $(LIB_DIR)/libnfd.a

SRC			:= $(wildcard $(SRC_DIR)/*.c $(SRC_DIR)/*/*.c)
OBJ			:= $(SRC:.c=.o)

TEST_SRC	:= $(wildcard $(TEST_DIR)/*.c $(SRC_DIR)/*/*.c)
TEST_OBJ	:= $(TEST_SRC:.c=.o)
TEST_EXEC	:= run_tests

# TODO set correct flags
CC			:= gcc -fdiagnostics-color=always
CPPFLAGS	:= -I include -I $(NFD_DIR)/src/include -MMD -MP -DCIMGUI_USE_OPENGL3 -DCIMGUI_USE_GLFW
CFLAGS		:= -std=c11 -pedantic -Og -g -Wall#-Wextra#-Werror
LDFLAGS		:= -L $(LIB_DIR)
LDLIBS		:= -lcimgui -lnfd -lstdc++

UNAME_S		:= $(shell uname -s)
ifeq ($(UNAME_S), Linux) #LINUX
	LDLIBS		+= -lGL `pkg-config --static --libs glfw3` `pkg-config --libs gtk+-3.0`
	NFD_MAKE	:= $(NFD_DIR)/build/gmake_linux
endif

ifeq ($(UNAME_S), Darwin) #APPLE
# 	CPPFLAGS	+= -I/usr/local/include -I/opt/local/include -I/opt/homebrew/include
	CPPFLAGS	+= -I/usr/local/include -I/opt/local/include -I/Users/$(USER)/goinfre/homebrew/opt/glfw/include
# 	LDFLAGS		+= -L/usr/local/lib -L/opt/local/lib -L/opt/homebrew/lib
	LDFLAGS		+= -L/usr/local/lib -L/opt/local/lib -L/Users/$(USER)/goinfre/homebrew/opt/glfw/lib
	LDLIBS		+= -lglfw -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo
	NFD_MAKE	:= $(NFD_DIR)/build/gmake_macosx
endif

.PHONY: all bonus clean fclean re

all: $(NAME)

bonus: $(NAME)

$(LIBCIMGUI): $(LIB_DIR)
	$(MAKE) --directory=$(CIMGUI_DIR) static
	cp $(CIMGUI_DIR)/libcimgui.a $(LIB_DIR)

$(LIBNFD): $(LIB_DIR)
	$(MAKE) --directory=$(NFD_MAKE)
	cp $(NFD_DIR)/build/lib/Release/x64/libnfd.a $(LIB_DIR)

$(NAME): $(OBJ) $(LIBCIMGUI) $(LIBNFD)
	$(CC) $(LDFLAGS) $(OBJ) $(LDLIBS) -o $(NAME)

# Turns out you don't have to have a rule for object files. 'Make' uses CFLAGS
# and CPPFLAGS variables to compile .o files.
# %.o: %.c
# 	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(LIB_DIR):
	mkdir -p $@

clean:
	@rm -vf $(OBJ)
	@rm -vf $(OBJ:.o=.d)
	@rm -vf $(wildcard $(TEST_DIR)/*.o $(TEST_DIR)/*.d)
	$(MAKE) clean --directory=$(CIMGUI_DIR)
	$(MAKE) clean --directory=$(NFD_MAKE)

fclean: clean
	@rm -vf $(NAME)
	@rm -vf $(TEST_EXEC)
	@rm -rvf $(LIB_DIR)
	$(MAKE) fclean --directory=$(CIMGUI_DIR)

re: fclean all

tests: $(TEST_EXEC)
	@echo "------------------"
	@./$(TEST_EXEC)
	@echo "------------------"

$(TEST_EXEC): $(TEST_OBJ) $(LIBCIMGUI) $(LIBNFD)
	$(CC) $(LDFLAGS) $(TEST_OBJ) $(LDLIBS) -lcheck -o $(TEST_EXEC)

-include $(OBJ:.o=.d)
