CXXFLAGS = -g -Wall $$(pkg-config --cflags opencv)
LD_FLAGS = -g -Wall $$(pkg-config --cflags --libs opencv)

SRCS = \
	gaussian.cpp \
	gaussian_tierify.cpp \
	tier.cpp

HDRS = \
	gaussian.h \
	tier.h

OBJS = $(SRCS:.cpp=.o)

current: all

all: gaussian_tierify tier_t

gaussian_tierify: $(OBJS)
	$(CXX) $(LD_FLAGS) $^ -o $@ $(LIBS)

OBJS: $(HDRS)

clean:
	rm -f *.o *~ core

immaculate: clean
	rm -f gaussian tier_t

tier_t: tier.cpp gaussian.cpp
	$(CXX) $(LD_FLAGS) -DTEST -o $@ $^
