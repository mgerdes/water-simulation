#ifndef BEZIER_SURFACE_OBJECT_H
#define BEZIER_SURFACE_OBJECT_H

#include <GL/glew.h>
#include <math.h>
#include <vector>

#include "maths/maths.h"

namespace object3d {

    class bezier_surface {
        private:
            float eval_bernstien_polynomial(float u, int v);
        public:
            const int N = 100;
            GLuint vao, indices_vbo;
            int num_indices;

            bezier_surface(vec3 control_points[4][4]);
    };

};

#endif 
