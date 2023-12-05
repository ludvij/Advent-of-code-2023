CC := g++
FLAGS := -Wextra -Wall -Wshadow -Wnon-virtual-dtor -std=c++20

OBJDIR := obj
SRCDIR := src
BINDIR := bin

ALL: day3_part1 day3_part2



day3_part1: $(OBJDIR)/day3_part1.o
	@mkdir -p $(BINDIR)
	$(CC) $(FLAGS) -o $(BINDIR)/$@ $<

day3_part2: $(OBJDIR)/day3_part2.o
	@mkdir -p $(BINDIR)
	$(CC) $(FLAGS) -o $(BINDIR)/$@ $<


$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(OBJDIR)
	$(CC) $(FLAGS) -c -o $@ $< 


clean:
	rm -r bin
	rm -r obj