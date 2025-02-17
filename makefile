CC = gcc
CFLAGS = -Wall -Wextra -Werror -g3 -fsanitize=address
TARGET = my_ls
SRCS = my_ls.c

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCS)

clean:
	rm -f $(TARGET)

fclean: clean
	rm -f *.o

re: fclean all
