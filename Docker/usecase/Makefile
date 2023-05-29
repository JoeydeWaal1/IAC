CXX =  clang++
LD  =  clang++

CCFLAGS  = -std=c++2a
# CCFLAGS += -o3
# CCFLAGS += -fPIC

LINKFLAGS  = -lcurl

CPPSRC = main.cpp
OBJS   = $(CPPSRC:.cpp=.o)

all: main

main: $(OBJS)
	$(LD) -o $@ $(LINKFLAGS) $@.o

%.o: %.cpp
	$(CXX) $(CCFLAGS) -c -o $@ $^

clean:
		rm -f *.o
		rm -f main
run:
	./main
