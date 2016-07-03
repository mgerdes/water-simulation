#ifndef RECTANGLE_OBJECT_H
#define RECTANGLE_OBJECT_H

#include <GL/glew.h>
#include <math.h>

#include "maths/maths.h"

namespace object3d {

    class rectangle {
        public:
            GLuint vao;

            rectangle(const vec3 &p1, const vec3 &p2);
    };

};

#endif
