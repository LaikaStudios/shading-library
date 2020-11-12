
all: _pow.so and.so split.so

%.o: %.cpp
	g++ -fPIC -I$(RMANTREE)/include -c $< -o $@

%.so: %.o
	g++ -shared $< -o $@

clean:
	rm -f *.o *.so
