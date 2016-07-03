#ifndef WATER_OBJECT_H
#define WATER_OBJECT_H

#include <math.h>

namespace object3d {

    class water {
        public:
            const int width = 50, height = 50;

            float c = 10.0;
            float u[50][50]; 
            float u_new[50][50]; 
            float v[50][50]; 

            water();

            void update(float dt);
    };

};

#endif
