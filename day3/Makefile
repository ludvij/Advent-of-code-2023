CC := g++
FLAGS := -Wextra -Wall -Wshadow -Wnon-virtual-dtor -std=c++20

OBJDIR := obj
SRCDIR := src

ALL: day3_part1 day3_part2


day3_part1: $(OBJDIR)/day3_part1.o
	$(CC) $(FLAGS) -o $@ $<

day3_part2: $(OBJDIR)/day3_part2.o
	$(CC) $(FLAGS) -o $@ $<

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(OBJDIR)
	$(CC) $(FLAGS) -c -o $@ $< 


clean:
	rm -r obj
	rm day3_part1