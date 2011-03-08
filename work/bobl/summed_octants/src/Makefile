CXXFLAGS = -g -Wall $$(pkg-config --cflags opencv)
LD_FLAGS = -g -Wall $$(pkg-config --cflags --libs opencv)

SRCS = \
	gaussian.cpp \
	tier.cpp

OBJS = $(SRCS:.cpp=.o)

main: gaussian

gaussian: $(OBJS)
	$(CXX) $(LD_FLAGS) $^ -o $@ $(LIBS)

clean:
	rm -f *.o *~ core

immaculate: clean
	rm -f gaussian
