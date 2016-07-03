#include "shaders/shader.h"

using namespace graphics;

shader::shader(const char *vertex_shader_file, const char *fragment_shader_file) {
    // Create the vertex shader
    FILE *f = fopen(vertex_shader_file, "r");
    fseek(f, 0, SEEK_END);
    int l = ftell(f); 
    fseek(f, 0, SEEK_SET);
    char *vertex_shader_string = (char*) malloc(sizeof(char) * (l + 1));
    fread(vertex_shader_string, sizeof(char), l, f);
    vertex_shader_string[l] = 0;
    fclose(f);

    GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_string, NULL);
    glCompileShader(vertex_shader);
    this->check_for_compile_errors(vertex_shader, "vertex_shader");

    // Create the fragment shader
    f = fopen(fragment_shader_file, "r");
    fseek(f, 0, SEEK_END);
    l = ftell(f); 
    fseek(f, 0, SEEK_SET);
    char *fragment_shader_string = (char*) malloc(sizeof(char) * (l + 1));
    fread(fragment_shader_string, sizeof(char), l, f);
    fragment_shader_string[l] = 0;
    fclose(f);

    GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_string, NULL);
    glCompileShader(fragment_shader);
    this->check_for_compile_errors(fragment_shader, "fragment_shader");

    // Create the shader program
    this->program = glCreateProgram();
    glAttachShader(this->program, fragment_shader);
    glAttachShader(this->program, vertex_shader);
    glLinkProgram(this->program);
    
    // Set up the uniform locations
    this->model_mat_location = glGetUniformLocation(this->program, "model_mat");
    this->view_mat_location = glGetUniformLocation(this->program, "view_mat");
    this->proj_mat_location = glGetUniformLocation(this->program, "proj_mat");
}

void shader::check_for_compile_errors(GLuint shader, const char *name) {
    int params = -1;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &params);
    if (params != GL_TRUE) {
        char *log = (char*) malloc(sizeof(char)*2048);
        int length;
        glGetShaderInfoLog(shader, 2048, &length, log);

        printf("Could not compile shader: %s\n", name);
        printf("Shader log:\n%s\n", log);
    }
}
