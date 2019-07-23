all:
	g++ -std=c++17 -g *.cpp -lm  -o audioAnalyzer

clean:
	rm -rf audioAnalyzer

so:
	g++ -std=c++17 -g -fPIC -shared -o libx264.so *.c core/*.c  decoder/*.c encoder/*.c

