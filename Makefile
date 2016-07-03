all:
	em++ main.cpp maths/maths.cpp util/util.cpp objects/rectangle.cpp objects/water.cpp shaders/shader.cpp -I. -lGL -lGLEW -lglfw -lm
