CC = gcc
CFLAGS = -I../include -Wall -Werror -DDEBUG -lcunit
SRC = alu_decoder_tests.c macro_tests.c selector_tests.c multiply_decoding_tests.c memory_decoding_tests.c test_runner.c ../src/decoder.c ../src/constants.c ../src/shift_ops.c ../src/instruction_builder.c
HEADERS = ../include/constants.h ../include/debug.h ../include/decoder.h ../include/constants.h ../include/shift_ops.h ../include/core.h ../include/instruction_builder.h
TARGET = run_tests

# Build the main project executable

$(TARGET): $(SRC) $(HEADERS)
	$(CC) $(SRC) -o $(TARGET) $(CFLAGS)

clean:
	rm -f $(TARGET)
	$(MAKE) -C tests clean  # Calls the clean target in the tests directory

# To run tests from the root directory
