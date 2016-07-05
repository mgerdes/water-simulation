#ifndef BEZIER_SURFACE_OBJECT_H
#define BEZIER_SURFACE_OBJECT_H

#include <GL/glew.h>
#include <math.h>
#include <vector>

#include "maths/maths.h"

namespace object3d {

    class bezier_surface {
        private:
            float eval_bernstien_polynomial(int u, int v);

            GLuint points_vbo, normals_vbo;

        public:
            static const int N = 5;

            GLuint vao;
            int num_indices;

            vec3 points[N * N];
            vec3 triangle_points[(N-1) * (N-1) * 3 * 2];
            vec3 triangle_normals[(N-1) * (N-1) * 3 * 2];

            float normals_buffer[(N-1) * (N-1) * 3 * 2 * 3];
            float points_buffer[(N-1) * (N-1) * 3 * 2 * 3];

            float bernstein_results[N][4];

            bezier_surface();

            void update_control_points(vec3 control_points[4][4]);
    };

};

#endif 
