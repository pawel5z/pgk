CC = g++
CFLAGS = -O3
LFLAGS = -lepoxy -lGL -lglfw -lm
NAME = lab2

OBJS = AGL3-example.o AGL3Window.o CirclePolygon.o
DEPS = AGL3Drawable.hpp AGL3Window.hpp CirclePolygon.hpp

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
