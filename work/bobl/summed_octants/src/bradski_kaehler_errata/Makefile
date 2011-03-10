CXXFLAGS = -g -Wall

main: tilt_sum_t

tilt_sum_t: tilt_sum_t.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@ $$(pkg-config --cflags --libs opencv)

clean:
	rm -f *~ core

immaculate:
	rm -f tilt_sum_t
