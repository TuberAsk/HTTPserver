if [ ! -d "bin" ]; then
    mkdir bin
    pushd bin
    gcc -o server ../main.c -Wall -Wextra -Werror
    popd
else
    gcc -o bin/server main.c -Wall -Wextra -Werror
fi