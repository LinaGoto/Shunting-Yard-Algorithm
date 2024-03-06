CXX	= g++
CFLAGS	= -O0 -g

all : shunting.exe
	./shunting.exe

shunting.exe : shunting.cpp
	$(CXX) $(CFLAGS) -o $@ $<

clean :
	rm -f *.exe *.o *~
