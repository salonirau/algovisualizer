# Makefile for Sorting Visualizer

# Compiler
CC = g++

# Paths
SDL2_INCLUDE_PATH = C:\DEVELOPMENT\SDLNORMAL\SDLFULL\include
SDL2_LIB_PATH = C:\DEVELOPMENT\SDLNORMAL\SDLFULL\lib
SDL2_TTF_INCLUDE_PATH = C:\DEVELOPMENT\SDLNORMAL\SDLFULL\include
SDL2_TTF_LIB_PATH = C:\DEVELOPMENT\SDLNORMAL\SDLFULL\lib
INCLUDE_PATH = ./include

# Compiler Flags
CFLAGS = -I$(SDL2_INCLUDE_PATH) -I$(SDL2_TTF_INCLUDE_PATH) -I$(INCLUDE_PATH) -std=c++11

# Linker Flags
LDFLAGS = -L$(SDL2_LIB_PATH) -L$(SDL2_TTF_LIB_PATH) -lSDL2 -lSDL2_ttf -static-libgcc -static-libstdc++

# Source and object files
SRCS = src/rendering.cpp src/sorting.cpp src/utilities.cpp src/events.cpp main.cpp
OBJS = src/rendering.o src/sorting.o src/utilities.o src/events.o main.o

# Output binary
TARGET = sorting_visualizer.exe

# Build target
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) $(LDFLAGS)

# Compile source files to object files
src/%.o: src/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@

main.o: main.cpp
	$(CC) $(CFLAGS) -c $< -o $@

# Clean target
clean:
	del /Q .\src\*.o .\main.o sorting_visualizer.exe 2>nul || exit 0

