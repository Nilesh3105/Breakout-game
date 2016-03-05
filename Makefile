#
# Makefile
#

breakout: breakout.c Makefile
	gcc -ggdb3 -Ispl/include -O0 -std=c99 -Wall -Werror -o breakout breakout.c -Lspl/lib -lcs -lm

clean:
	rm -f *.o core breakout
