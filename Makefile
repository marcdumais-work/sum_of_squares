# Variables
CC = gcc
CFLAGS = -Wall
OPTIMIZATON1 = -o1
OPTIMIZATON2 = -o2

TARGET = sum_of_squares_openmp
SRC = sum_of_squares_openmp.c
TARGET1 = sum_of_squares
SRC1 = sum_of_squares.c

# Default target
all: $(TARGET) $(TARGET1)

# Build the target
$(TARGET): $(SRC)
	$(CC) -fopenmp  $(CFLAGS) $(OPTIMIZATON1) -o $(TARGET)$(OPTIMIZATON1) $(SRC)
	$(CC) -fopenmp  $(CFLAGS) $(OPTIMIZATON2) -o $(TARGET)$(OPTIMIZATON2) $(SRC)

$(TARGET1): $(SRC1)
	$(CC) -pthread  $(CFLAGS) $(OPTIMIZATON1) -o $(TARGET1)$(OPTIMIZATON1)  $(SRC1)
	$(CC) -pthread  $(CFLAGS) $(OPTIMIZATON2) -o $(TARGET1)$(OPTIMIZATON2)  $(SRC1)


# Clean target
clean:
	rm -f $(TARGET)$(OPTIMIZATON1) $(TARGET)$(OPTIMIZATON2) $(TARGET1)$(OPTIMIZATON1) $(TARGET1)$(OPTIMIZATON2)

# Phony targets
.PHONY: all clean