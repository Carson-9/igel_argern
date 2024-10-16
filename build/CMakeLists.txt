project(igel_argens)
set(CMAKE_C_FLAGS "-Wall -Wextra -pedantic -g")
add_executable(../igel_argens ../src/main.c ../src/plateau.c ../src/plateau.h ../src/types.h ../src/logging.h ../src/logging.c ../src/build_options.h)