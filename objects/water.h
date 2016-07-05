#ifndef WATER_OBJECT_H
#define WATER_OBJECT_H

#include <math.h>

namespace object3d {

    class water {
        public:
            static constexpr int width = 60, height = 60;

            float c = 16.0;
            float u[width][height]; 
            float u_new[width][height]; 
            float v[width][height]; 

            water();

            void update(float dt);
    };

};

#endif
