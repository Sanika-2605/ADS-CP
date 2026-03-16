CC      = gcc
CFLAGS  = -Wall -Wextra -Iinclude
SRC     = src/*.c
TARGET  = finance_manager

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

clean:
	rm -f $(TARGET) $(TARGET).exe

.PHONY: all clean
