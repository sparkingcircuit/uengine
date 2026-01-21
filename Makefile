LIBS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
WARNINGS= -Wall -Wextra

all:
	cc *.c -o a.out $(LIBS) $(WARNINGS)

