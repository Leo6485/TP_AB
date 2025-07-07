COMPILER = g++
FLAGS = -Wall -Wextra -g -O3
BINARYDIR = binary
EXENAME = ag

SRCS = $(filter-out main.cpp, $(wildcard *.cpp))
OBJS = $(SRCS:%.cpp=$(BINARYDIR)/%.o)

.PHONY: all clean

all: $(BINARYDIR) $(EXENAME)

$(EXENAME): $(OBJS) $(BINARYDIR)/main.o
		$(COMPILER) $(FLAGS) $^ -o $@ -lm

$(BINARYDIR)/%.o: %.cpp %.hpp
		$(COMPILER) $(FLAGS) -c $< -o $@ -lm

$(BINARYDIR)/main.o: main.cpp
		$(COMPILER) $(FLAGS) -c $< -o $@ -lm

$(BINARYDIR):
	test ! -d $(BINARYDIR) && mkdir $(BINARYDIR)

clean:
	rm -rf $(BINARYDIR)/* $(EXENAME)
