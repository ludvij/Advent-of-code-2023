CC := g++
FLAGS := -Wextra -Wall -Wshadow -Wnon-virtual-dtor -pedantic -std=c++20

OUT := day1

OBJDIR := obj
SRCDIR := src

OBJS := day1.o

$(OUT): $(OBJDIR)/$(OBJS)
	$(CC) $(FLAGS) -o $@ $<

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(OBJDIR)
	$(CC) $(FLAGS) -c -o $@ $< 


clean:
	rm -r obj
	rm $(OUT)