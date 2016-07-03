#ifndef HILBERT_CURVE_OBJECT_H
#define HILBERT_CURVE_OBJECT_H

#include <GL/glew.h>
#include <vector>
#include <math.h>

#include "maths.h"
#include "util.h"

class hilbert_curve {
    private:
        class rectangle {
            public:
                GLuint vao;

                rectangle(const vec3 &p1, const vec3 &p2);

                void draw();
        };

        static constexpr const char *vertex_shader_string = 
            "#version 330\n"
            "in vec3 vertex_position;"
            "in vec3 vertex_normal;"
            "out vec3 frag_position;"
            "out vec3 frag_normal;"
            "uniform mat4 model_mat;"
            "uniform mat4 view_mat;"
            "uniform mat4 proj_mat;"
            "void main () {"
            "   frag_position = (model_mat * vec4(vertex_position, 1.0)).xyz;"
            "   frag_normal = vertex_normal;"
            "   gl_Position = proj_mat * view_mat * model_mat * vec4(vertex_position, 1.0);"
            "}";

        static constexpr const char *fragment_shader_string = 
            "#version 330\n"
            "in vec3 frag_position;"
            "in vec3 frag_normal;"
            "out vec4 frag_color;"
            "void main () {"
            "   vec3 pos = frag_position;"
            "   vec3 color;"
            "   if (pos.x > 0 && pos.y > 0 && pos.z > 0) {"
            "       color = vec3(1.0, 0.0, 0.0);"
            "   }"
            "   else if (pos.x > 0 && pos.y > 0 && pos.z < 0) {"
            "       color = vec3(0.0, 1.0, 0.0);"
            "   }"
            "   else if (pos.x > 0 && pos.y < 0 && pos.z > 0) {"
            "       color = vec3(0.0, 0.0, 1.0);"
            "   }"
            "   else if (pos.x > 0 && pos.y < 0 && pos.z < 0) {"
            "       color = vec3(1.0, 1.0, 0.0);"
            "   }"
            "   else if (pos.x < 0 && pos.y > 0 && pos.z > 0) {"
            "       color = vec3(1.0, 0.0, 1.0);"
            "   }"
            "   else if (pos.x < 0 && pos.y > 0 && pos.z < 0) {"
            "       color = vec3(0.0, 1.0, 1.0);"
            "   }"
            "   else if (pos.x < 0 && pos.y < 0 && pos.z > 0) {"
            "       color = vec3(0.5, 0.2, 0.0);"
            "   }"
            "   else if (pos.x < 0 && pos.y < 0 && pos.z < 0) {"
            "       color = vec3(1.0, 0.2, 0.5);"
            "   }"
            "   vec3 light_pos = vec3(1.0, 2.0, 3.0);"
            "   float kd = max(dot(normalize(light_pos), abs(frag_normal)), 0);"
            "   frag_color = kd * vec4(1.0, 0.0, 1.0, 1.0);"
            "}";

        void iterate_l_system(std::vector<char> &l_system, int n);

        std::vector<hilbert_curve::rectangle> rectangles;

        mat4 translation_mat;
    public:
        quaternion rotation;

        GLuint shader, 
               model_mat_location, 
               view_mat_location, 
               proj_mat_location;

        hilbert_curve();

        void draw();
};

#endif
