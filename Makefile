NAME		:= 3DViewer

SRC_DIR		:= src
LIB_DIR		:= lib

CIMGUI_DIR	:= cimgui
LIBCIMGUI	:= $(LIB_DIR)/libcimgui.a

SRC			:= $(wildcard $(SRC_DIR)/*.c $(SRC_DIR)/*/*.c)
OBJ			:= $(SRC:.c=.o)

# TODO set correct flags
# TODO also compile for macOS
CC			:= gcc -fdiagnostics-color=always
CPPFLAGS	:= -I include -MMD -MP
CFLAGS		:= -std=c11 -pedantic -Og -g -Wall#-Wextra#-Werror
LDFLAGS		:=
LDLIBS		:= -lglfw -lGL -lm

.PHONY: all bonus clean fclean re

all: $(NAME)

bonus: $(NAME)

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
