CXX = g++
FLAGS = -Wall -g -O3 -fopenmp
BINARYDIR = binary
EXENAME = aco

SRCS = $(filter-out main.cpp, $(wildcard *.cpp))
OBJS = $(SRCS:%.cpp=$(BINARYDIR)/%.o)

.PHONY: all clean

all: $(BINARYDIR) $(EXENAME)

$(EXENAME) : $(OBJS) $(BINARYDIR)/main.o
	$(CXX) $(FLAGS) $^ -o $@ -lm

$(BINARYDIR)/%.o: %.cpp %.hpp
	$(CXX) $(FLAGS) -c $< -o $@ -lm

$(BINARYDIR)/main.o: main.cpp
	$(CXX) $(FLAGS) -c $< -o $@ -lm

$(BINARYDIR):
	test ! -d $(BINARYDIR) && mkdir $(BINARYDIR)

clean:
	rm -rf $(BINARYDIR)/* $(EXENAME)