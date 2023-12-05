CC := g++
FLAGS := -Wextra -Wall -Wshadow -Wnon-virtual-dtor -std=c++20

OBJDIR := obj
SRCDIR := src
BINDIR := bin

ALL: day1



day1: $(OBJDIR)/day1.o
	@mkdir -p $(BINDIR)
	$(CC) $(FLAGS) -o $(BINDIR)/$@ $<

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(OBJDIR)
	$(CC) $(FLAGS) -c -o $@ $< 


clean:
	rm -r bin
	rm -r obj