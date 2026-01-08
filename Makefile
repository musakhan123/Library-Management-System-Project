# ------------------------------
# Makefile for Library Management + ImGui (Windows)
# ------------------------------

CXX = g++
CXXFLAGS = -std=c++11 -Wall -Iinclude -Iimgui -IC:/libs/glfw/include

LIBS = -LC:/libs/glfw/lib-mingw-w64 -lglfw3 -lopengl32 -lgdi32

SOURCES = \
src/main.cpp \
include/Book.cpp \
include/BST.cpp \
include/LibraryManager.cpp \
include/LinkedList.cpp \
include/Queue.cpp \
include/Stack.cpp \
include/Student.cpp \
include/Transaction.cpp \
imgui/imgui.cpp \
imgui/imgui_draw.cpp \
imgui/imgui_widgets.cpp \
imgui/imgui_tables.cpp \
imgui/imgui_impl_glfw.cpp \
imgui/imgui_impl_opengl3.cpp

TARGET = library_system.exe

all:
	$(CXX) $(SOURCES) $(CXXFLAGS) $(LIBS) -o $(TARGET)

clean:
	del /Q $(TARGET) 2>NUL || true

run: all
	$(TARGET)
