all:
	g++ main.cpp maths/maths.cpp util/util.cpp objects/rectangle.cpp objects/water.cpp objects/plane.cpp shaders/shader.cpp -I. -lGL -lGLEW -lglfw -lm
