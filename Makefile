NAME		:= 3DViewer

SRC_DIR		:= src
LIB_DIR		:= lib

CIMGUI_DIR	:= cimgui
LIBCIMGUI	:= $(LIB_DIR)/libcimgui.a

SRC			:= $(wildcard $(SRC_DIR)/*.c $(SRC_DIR)/*/*.c)
OBJ			:= $(SRC:.c=.o)

# TODO set correct flags
CC			:= gcc -fdiagnostics-color=always
CPPFLAGS	:= -I include -MMD -MP -DCIMGUI_USE_OPENGL3 -DCIMGUI_USE_GLFW
CFLAGS		:= -std=c11 -pedantic -Og -g -Wall#-Wextra#-Werror
LDFLAGS		:= -L $(LIB_DIR)
LDLIBS		:= -lcimgui

UNAME_S		:= $(shell uname -s)
ifeq ($(UNAME_S), Linux) #LINUX
	LDLIBS	+= -lGL -lstdc++ `pkg-config --static --libs glfw3`
endif

# TODO test macOS compilation
ifeq ($(UNAME_S), Darwin) #APPLE
	CPPFLAGS	+= -I/usr/local/include -I/opt/local/include -I/opt/homebrew/include
	LDFLAGS		+= -L/usr/local/lib -L/opt/local/lib -L/opt/homebrew/lib
	LDLIBS		+= -lglfw -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo
endif

.PHONY: all bonus clean fclean re

all: $(NAME)

bonus: $(NAME)

# TODO maybe a dynamic lib is also fine?
$(LIBCIMGUI): $(LIB_DIR)
	$(MAKE) --directory=$(CIMGUI_DIR) static
	cp $(CIMGUI_DIR)/libcimgui.a $(LIB_DIR)

$(NAME): $(OBJ) $(LIBCIMGUI)
	$(CC) $(LDFLAGS) $(OBJ) $(LDLIBS) -o $(NAME)

# Turns out you don't have to have a rule for object files. 'Make' uses CFLAGS
# and CPPFLAGS variables to compile .o files.
# %.o: %.c
# 	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(LIB_DIR):
	mkdir -p $@

clean:
	rm -vf $(OBJ)
	rm -vf $(OBJ:.o=.d)
	$(MAKE) clean --directory=$(CIMGUI_DIR)

fclean: clean
	rm -vf $(NAME)
	rm -rvf $(LIB_DIR)
	$(MAKE) fclean --directory=$(CIMGUI_DIR)

re: fclean all

-include $(OBJ:.o=.d)
