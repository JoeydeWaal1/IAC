CXX =  clang++
LD  =  clang++

CCFLAGS  = -std=c++2a
CCFLAGS += -Iinclude

LINKFLAGS  = -lcurl

CPPSRC = main.cpp
OBJS   = $(CPPSRC:.cpp=.o)

all: main

main: $(OBJS)
	$(CXX) -o $@ $(LINKFLAGS) $@.o

%.o: %.cpp
	$(CXX) $(CCFLAGS) -c -o $@ $^

clean:
		rm -f $(BIN_DIR)/*
		rm -f $(SRC_DIR)/*.o
		rm -f lib/*/include/*.o
		rm -f include/*.o
run:
	./main
