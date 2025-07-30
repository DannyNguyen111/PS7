# Compiler and flags
CC = g++
CFLAGS = -std=c++11 -Wall -Werror -pedantic
BOOST_LIBS = -lboost_date_time -lboost_regex

# Target executable
TARGET = ps7
SRC = main.cpp
OBJ = $(SRC:.cpp=.o)

# Default target
all: $(TARGET)

# Link the executable
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(BOOST_LIBS)

# Generic compile rule
%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

# Lint target
lint:
	cpplint *.cpp

# Clean up build files
clean:
	rm -f *.o $(TARGET)

.PHONY: all clean lint

