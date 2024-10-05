CC = gcc
CFLAGS = -Iinclude -Wall -Werror
SRC = src/arm_decoder.c src/constants.c src/shift_ops.c src/instruction_builder.c src/main.c src/thumb_decoder.c
HEADERS = include/constants.h include/debug.h include/arm_decoder.h include/constants.h include/shift_ops.h include/core.h include/instruction_builder.h include/thumb_decoder.h
TARGET = main

# Build the main project executable

$(TARGET): $(SRC) $(HEADERS)
	$(CC) $(SRC) -o $(TARGET) $(CFLAGS)

clean:
	rm -f $(TARGET)
	$(MAKE) -C tests clean  # Calls the clean target in the tests directory

# To run tests from the root directory
