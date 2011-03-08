CXX_FLAGS = -Wall $$(pkg-config --cflags --libs opencv)

main: gaussian

gaussian: gaussian.cpp
	$(CXX) $(CXX_FLAGS) $< -o $@ $(LIBS)

clean:
	rm -f *.o *~

immaculate: clean
	rm -f gaussian
