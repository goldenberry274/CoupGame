CC = g++
CFLAGS = -Wall -g

TST_TARGET = Test
SRC = Player.cpp Governor.cpp Baron.cpp Game.cpp Main.cpp
OBJS = Player.o Governor.o Baron.o Game.o General.o Judge.o Merchant.o Main.o

TARGET = main

all: $(TARGET)
# Build the main executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

# Run the main program
run: $(TARGET)
	./$(TARGET)

# Run tests
test: $(TST_TARGET)
	./$(TST_TARGET)

# Use valgrind
valgrind: $(TARGET)
	valgrind --leak-check=full ./$(TARGET)

clean: 
	rm -f *.o $(TARGET)
.PHONY: Main test valgrind


