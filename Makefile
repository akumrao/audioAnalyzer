.PHONY:	clean all

all: 
	g++ -std=c++17 -g *.cpp  plot/*.cpp audio/*.cpp audio/encoder/*.cpp -lm -lSDL2  -o audioAnalyzer

clean:
	rm -rf audioAnalyzer

exe:
	g++ -std=c++17 -g  *.cpp  plot/*.cpp audio/*.cpp audio/encoder/*.cpp -lm -lSDL2 -o audioAnalyzer

wasm:
	. /root/emsdk_set_env.sh && emcc *.cpp  plot/*.cpp audio/*.cpp audio/encoder/*.cpp  -std=c++17 -s WASM=1 -s ALLOW_MEMORY_GROWTH=1 -s USE_SDL=2 -s 'EXTRA_EXPORTED_RUNTIME_METHODS=["writeArrayToMemory", "cwrap"]' -o index.js                                                                               
run:
	echo "http://localhost:8080/index.html"	
	python -m SimpleHTTPServer 8080
	


