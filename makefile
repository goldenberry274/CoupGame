CC = g++
CFLAGS = -Wall -g

# GUI and audio libraries
GUIFLAGS = -lsfml-audio -lsfml-system `wx-config --cxxflags --libs`

# Source files
CMN_SRC = Player.cpp General.cpp Spy.cpp Merchant.cpp Judge.cpp Governor.cpp Baron.cpp Game.cpp
GUI_SRC = Main.cpp 
DEMO_SRC = Demo.cpp
TEST_SRC = Test.cpp

# Object files
CMN_OBJ = $(CMN_SRC:.cpp=.o)
GUI_OBJ = $(GUI_SRC:.cpp=.o)
DEMO_OBJ = $(DEMO_SRC:.cpp=.o)
TEST_OBJ = $(TEST_SRC:.cpp=.o)

# Final targets
TARGET = Main
DEMO_TARGET = Demo
TST_TARGET = Test

# Default target
all: $(TARGET) $(TST_TARGET) $(DEMO_TARGET)

# Build rules
$(TARGET): $(CMN_OBJ) $(GUI_OBJ)
	$(CC) $(CFLAGS) $^ -o $@ $(GUIFLAGS)

$(DEMO_TARGET): $(CMN_OBJ) $(DEMO_OBJ)
	$(CC) $(CFLAGS) $^ -o $@ $(GUIFLAGS)

$(TST_TARGET): $(CMN_OBJ) $(TEST_OBJ)
	$(CC) $(CFLAGS) $^ -o $@ 

$(TEST_OBJ): $(TEST_SRC)
	$(CC) $(CFLAGS) $^ -c $@

$(CMN_OBJ): $(CMN_SRC)
	$(CC) $(CFLAGS) $^ -c $@ $(GUIFLAGS)

$(DEMO_OBJ): $(DEMO_SRC)
	$(CC) $(CFLAGS) $^ -c $@ $(GUIFLAGS)


$(GUI_OBJ): $(GUI_SRC)
	$(CC) $(CFLAGS) $^ -c $@ $(GUIFLAGS)
# Run targets
main: $(TARGET)
	./$(TARGET)

demo: $(DEMO_TARGET)
	./$(DEMO_TARGET)

test: $(TST_TARGET)
	./$(TST_TARGET)

valgrind: $(TARGET)
	valgrind --leak-check=full ./$(TARGET)

valgrind-demo: $(DEMO_TARGET)
	valgrind --leak-check=full ./$(DEMO_TARGET)

valgrind-test: $(TST_TARGET)
	valgrind --leak-check=full ./$(TST_TARGET)

clean:
	rm -f *.o $(TARGET) $(DEMO_TARGET) $(TST_TARGET)

.PHONY: all run demo test valgrind valgrind-demo clean
