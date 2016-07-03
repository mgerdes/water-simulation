all:
	gcc main.cpp maths/maths.cpp util/util.cpp objects/hilbert_curve.cpp -Imaths -Iutil -Iobjects -I. -lGL -lGLEW -lglfw -lm -lstdc++ -std=c++11
