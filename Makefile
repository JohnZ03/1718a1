OS := $(shell uname)
OPTIONS:= 

ifeq ($(OS),Darwin)
	OPTIONS += -framework OpenCL
else
	OPTIONS += -l OpenCL
endif

main: cnn_mnist.cpp
	g++ -O2 -Wno-deprecated-declarations -g cnn_mnist.cpp -o a.out $(OPTIONS)

clean:
	rm -rf main
