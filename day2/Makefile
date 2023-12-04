CC := g++
FLAGS := -Wextra -Wall -Wshadow -Wnon-virtual-dtor -std=c++20

OBJDIR := obj
SRCDIR := src

ALL: day2_part1 day2_part2


day2_part1: $(OBJDIR)/day2_part1.o
	$(CC) $(FLAGS) -o $@ $<

day2_part2: $(OBJDIR)/day2_part2.o
	$(CC) $(FLAGS) -o $@ $<

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(OBJDIR)
	$(CC) $(FLAGS) -c -o $@ $< 


clean:
	rm -r obj
	rm day2_part1
	rm day2_part2
