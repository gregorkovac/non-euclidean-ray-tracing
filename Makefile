# OBJS specifies which files to compile as part of the project
OBJS = src/*.cpp 

# CC specifies which compiler we're using
CC = gcc

# INCLUDE_PATHS specifies the additional include paths we'll need
INCLUDE_PATHS = -I./include

# LIBRARY_PATHS specifies the additional library paths we'll need
LIBRARY_PATHS = -L./lib

# COMPILER_FLAGS specifies the additional compilation options we're using
# -w suppresses all warnings
COMPILER_FLAGS = -w

# LINKER_FLAGS specifies the libraries we're linking against
# Cocoa, IOKit, and CoreVideo are needed for static GLFW3.
LINKER_FLAGS = -lglfw3 -framework Cocoa -framework OpenGL -framework IOKit -framework CoreVideo

# OBJ_NAME specifies the name of our exectuable
OBJ_NAME = main

#This is the target that compiles our executable
all : $(OBJS)
	$(CC) $(OBJS) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o src/Raytracing.out