gcc -Wall -Wextra -c GraphicLib.c
gcc -Wall -Wextra -c bmp_lib.c
gcc -Wall -Wextra -c graphic_filters.c
gcc -Wall -Wextra -o test test.c GraphicLib.o bmp_lib.o graphic_filters.o