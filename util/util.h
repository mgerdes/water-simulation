#ifndef UTIL_H
#define UTIL_H

#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "stb_image.h"

GLFWwindow *util_init_gl_window(const char *title, int width, int height);

GLuint util_create_shader_program(const char* fragment_shader_string, const char* vertex_shader_string);

void util_check_for_shader_location_errors(GLuint location, const char *name);

void util_check_for_shader_compile_errors(GLuint shader, const char *name);

GLuint util_create_texture(const char *file_name);

#endif
