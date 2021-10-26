CC = g++
CFLAGS = -O3
LFLAGS = -lepoxy -lGL -lglfw -lm
NAME = lab3

OBJS = $(shell ls *.cpp | sed 's/\.cpp/\.o/')
DEPS = $(shell ls *.hpp)

$(NAME) : $(OBJS)
	$(CC) $(CFLAGS) $^ -o $(NAME) $(LFLAGS)

debug : CFLAGS += -Og -g
debug : $(NAME)

%.o : %.cpp $(DEPS)
	$(CC) $(CFLAGS) $< -c -o $@ $(LFLAGS)

clean :
	@rm -f *.o

distclean : clean
	@rm -f $(NAME)

.PHONY : clean distclean debug
