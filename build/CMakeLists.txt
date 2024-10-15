project(igel_argen)
set(CMAKE_C_FLAGS "-Wall -Wextra -pedantic -g")
add_executable(../igel_argen ../src/main.c ../src/plateau.c ../src/plateau.h ../src/stack.c ../src/stack.h ../src/types.h)