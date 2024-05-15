#!/bin/bash

rm dgc
gcc -o dgc *.c -std=c99 -Wall -Wextra -Werror -pedantic
./dgc

valgrind --leak-check=full \
         --show-leak-kinds=all \
         --track-origins=yes \
         --verbose \
         --error-exitcode=1 \
         ./dgc

