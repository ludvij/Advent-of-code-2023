CC := g++
FLAGS := -Wextra -Wall -Wshadow -Wnon-virtual-dtor -std=c++20

OUT := day2

OBJDIR := obj
SRCDIR := src

OBJS := day2_part2.o

$(OUT): $(OBJDIR)/$(OBJS)
	$(CC) $(FLAGS) -o $@ $<

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(OBJDIR)
	$(CC) $(FLAGS) -c -o $@ $< 


clean:
	rm -r obj
	rm $(OUT)