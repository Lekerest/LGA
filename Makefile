CC = gcc
CFLAGS = -O2 -Wall -Wextra -std=c11
LDFLAGS = -lm

SRC = main.c lattice.c collision.c boundary.c grid_io.c render.c
OBJ = $(SRC:.c=.o)

TEST_SRC = tests.c lattice.c collision.c boundary.c grid_io.c render.c

TARGET = lga
TEST_TARGET = tests

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

test: $(TEST_TARGET)
	./$(TEST_TARGET)

$(TEST_TARGET): $(TEST_SRC)
	$(CC) $(CFLAGS) -o $(TEST_TARGET) $(TEST_SRC) $(LDFLAGS)

run-small: $(TARGET)
	./$(TARGET) -w 60 -h 40 -n 500 -d 10 --rho-in 1.6 -v 10

run-big: $(TARGET)
	./$(TARGET) -w 256 -h 160 -n 1000 -d 30 --rho-in 1.8 -v 100

resume: $(TARGET)
	./$(TARGET) -w 60 -h 40 -n 200 -d 10 --rho-in 1.6 -v 10 --resume

help: $(TARGET)
	./$(TARGET) --help

clean:
	rm -f *.o $(TARGET) $(TEST_TARGET) grid.bin test_grid.bin
	rm -rf frames test_frames

.PHONY: all test run-small run-big resume help clean