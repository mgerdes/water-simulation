#ifndef SHADER_HEADER
#define SHADER_HEADER

#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>

namespace graphics {

    class shader {
        private:
            void check_for_compile_errors(GLuint shader, const char *name);

        public:
            GLuint program, 
                   model_mat_location, 
                   view_mat_location, 
                   proj_mat_location,
                   color_location
                   ;

            shader(const char *vertex_shader_file, const char *fragment_shader_file);
    };

};

#endif
