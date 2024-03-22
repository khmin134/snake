mkdir build
cd build
g++ ../src/main.cpp ../src/console.cpp  -lncurses -Werror -o snake
