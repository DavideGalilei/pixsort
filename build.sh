set -xe
gcc -o pixsort -Wall -Wextra -Werror -pedantic -O3 -lm pixsort.c
./pixsort $@
