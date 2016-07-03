#ifndef RECTANGLE_OBJECT_H
#define RECTANGLE_OBJECT_H

class rectangle {
    private:
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
    public:
        GLuint vao;

        rectangle(const vec3 &p1, const vec3 &p2);

        static GLuint shader, 
                      model_mat_location, 
                      view_mat_location, 
                      proj_mat_location;

        void draw();
};

#endif
