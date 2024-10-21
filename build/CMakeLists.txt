project(igel_argern)
set(CMAKE_C_FLAGS "-Wall -Wextra -Werror -pedantic -g")
add_executable(../igel_argern ../src/entry_point.c ../src/game_logic.c ../src/game_logic.h ../src/plateau.c ../src/plateau.h ../src/types.h ../src/logging.h ../src/logging.c ../src/build_options.h)
