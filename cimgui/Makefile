#
# Cross Platform Makefile
# Compatible with Ubuntu 14.04.1 and macOS

OBJS = cimgui.o
OBJS += ./imgui/imgui.o
OBJS += ./imgui/imgui_draw.o
OBJS += ./imgui/imgui_demo.o
OBJS += ./imgui/imgui_tables.o
OBJS += ./imgui/imgui_widgets.o
OBJS += ./imgui/backends/imgui_impl_opengl3.o
OBJS += ./imgui/backends/imgui_impl_glfw.o

CXXFLAGS=-O2 -fno-exceptions -fno-rtti -std=c++11

UNAME_S := $(shell uname -s)

AR := ar -rc

ifeq ($(UNAME_S), Linux) #LINUX
	ECHO_MESSAGE = "Linux"

	OUTPUTNAME = cimgui.so
	CXXFLAGS += -I./imgui/
	CXXFLAGS += -Wall
	CXXFLAGS += -shared -fPIC
	CFLAGS = $(CXXFLAGS)
endif

ifeq ($(UNAME_S), Darwin) #APPLE
	ECHO_MESSAGE = "macOS"

	OUTPUTNAME = cimgui.dylib
	CXXFLAGS += -I/usr/local/include -I./imgui/
#	CXXFLAGS += -I/Users/$(USER)/.brew/Cellar/glfw/3.3.8/include -I./imgui/ #MacOS mmicheli
#	CXXFLAGS += -I/Users/$(USER)/goinfre/homebrew/opt/glfw/include -I./imgui/ #MacOS mmicheli
	CXXFLAGS += -I/Users/$(USER)/Documents/21school/homebrew/opt/glfw/include -I./imgui/#MacOS sber

	CXXFLAGS += -Wall
	LINKFLAGS = -dynamiclib
	CFLAGS = $(CXXFLAGS)
endif

ifeq ($(OS), Windows_NT)
	ECHO_MESSAGE = "Windows"

	OUTPUTNAME = cimgui.dll
	CXXFLAGS += -I./imgui/
	CXXFLAGS += -Wall
	CXXFLAGS += -shared
	LINKFLAGS = -limm32
	CFLAGS = $(CXXFLAGS)
endif

.cpp.o:
	$(CXX) -DIMGUI_IMPL_API="extern \"C\"" $(CXXFLAGS) -c -o $@ $<

all:$(OUTPUTNAME)
	@echo Build complete for $(ECHO_MESSAGE)

static:$(OBJS)
	$(AR) libcimgui.a $(OBJS)

$(OUTPUTNAME):$(OBJS)
	$(CXX) -o $(OUTPUTNAME) $(OBJS) $(CXXFLAGS) $(LINKFLAGS)

clean:
	rm -f $(OBJS)

fclean: clean
	@rm -vf $(OUTPUTNAME)
	@rm -vf libcimgui.a

re: fclean all

.PHONY: all clean fclean re static
