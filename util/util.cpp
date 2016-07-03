#include "util.h"

GLFWwindow *util_init_gl_window(const char *title, int width, int height) {
    GLFWwindow *window;

    if (!glfwInit()) {
        printf("Could not start GLFW3\n");
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);

    window = glfwCreateWindow(width, height, title, NULL, NULL);

    glfwMakeContextCurrent(window);
    glewExperimental = GL_TRUE;
    glewInit();
    glEnable(GL_DEPTH_TEST); 
    glDepthFunc(GL_LESS);

    //glEnable(GL_BLEND);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    return window;
}

void util_check_for_shader_compile_errors(GLuint shader, const char *name) {
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

void util_check_for_shader_location_errors(GLuint location, const char *name) {
    if (location == -1) {
        printf("Bad location: %s\n", name);
    }
}

GLuint util_create_shader_program(const char *fragment_shader_string, const char *vertex_shader_string) {
    GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_string, NULL);
    glCompileShader(vertex_shader);
    util_check_for_shader_compile_errors(vertex_shader, "vertex_shader");

    GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_string, NULL);
    glCompileShader(fragment_shader);
    util_check_for_shader_compile_errors(fragment_shader, "fragment_shader");

    GLuint shader_program = glCreateProgram();
    glAttachShader(shader_program, fragment_shader);
    glAttachShader(shader_program, vertex_shader);
    glLinkProgram(shader_program);

    return shader_program;
}

GLuint util_create_texture(const char *file_name) {
    stbi_set_flip_vertically_on_load(1);
    int x, y, n;
    int force_channels = 3;
    unsigned char* image_data = stbi_load(file_name, &x, &y, &n, force_channels);

    if (!image_data) {
        printf("Could not load texture file %s\n", file_name);
        return 0;
    }

    GLuint location;
    glGenTextures(1, &location);
    glBindTexture(GL_TEXTURE_2D, location);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, x, y, 0, GL_RGB, GL_UNSIGNED_BYTE, image_data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    free(image_data);

    return location;
}
