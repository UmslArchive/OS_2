CC = gcc
CFLAGS = -I. -g
TARGET = logParse
OBJS = main.o Options.o
.SUFFIXES: .c .o

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

.c.o:
	$(CC) $(CFLAGS) -c $<

.PHONY: clean
clean:
	rm -f *.o $(TARGET)
