TARGET=code
SRCS = main.cpp

OBJS = $(SRCS:.c=.o)

$(TARGET):$(OBJS)
	g++ main.cpp -o code -std=c++11

clean:
	rm -rf code
