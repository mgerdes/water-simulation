#ifndef WATER_OBJECT_H
#define WATER_OBJECT_H

#include <GL/glew.h>
#include <math.h>
#include <stdio.h>

#include "maths/maths.h"

namespace object3d {

    class water {
        private:
            GLuint points_vbo, normals_vbo;

        public:
            static constexpr int N = 61;
            static constexpr int width = 60, height = 60;

            float c = 16.0;
            float u[width][height]; 
            float u_new[width][height]; 
            float v[width][height]; 
            float control_point_heights[width][height];

            GLuint vao, elements_vbo;

            vec3 points[N * N];
            vec3 normals[N * N];
            vec3 elements[(N - 1) * (N - 1) * 2];

            float points_buffer[N * N * 3];
            float normals_buffer[N * N * 3];
            int elements_buffer[(N - 1) * (N - 1) * 2 * 3];

            water();

            void update(float dt);
    };

};

#endif
