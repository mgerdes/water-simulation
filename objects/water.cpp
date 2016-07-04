#include "objects/water.h"

using namespace object3d;

water::water() {
    for (int i = 0; i < this->width; i++) {
        for (int j = 0; j < this->height; j++) {
            float x = -3 + 6 * (1 - (i / (float) this->width));
            float y = -3 + 6 * (1 - (j / (float) this->height));

            this->u[i][j] = 0.2 * (3 - fabs(x)) * (3 - fabs(y)) + 1.0;
            //this->u[i][j] = sin(x) * sin(x) * cos(y) + 1.0;
            //this->u[i][j] += 0.2 * (x + y);
            this->v[i][j] = 0;
        }
    }
}

/*
 * From here: http://matthias-mueller-fischer.ch/talks/GDC2008.pdf
 */
void water::update(float dt) {
    for (int i = 0; i < this->width; i++) {
        for (int j = 0; j < this->height; j++) {
            float v1, v2, v3, v4;

            if (i == 0) {
                v1 =  this->u[i][j];
            }
            else {
                v1 = this->u[i - 1][j];
            }

            if (i == this->width - 1) {
                v2 = this->u[i][j];
            }
            else {
                v2 = this->u[i + 1][j];
            }

            if (j == 0) {
                v3 = this->u[i][j];
            }
            else {
                v3 = this->u[i][j - 1];
            }

            if (j == this->height - 1) {
                v4 = this->u[i][j];
            }
            else {
                v4 = this->u[i][j + 1];
            }

            float f = c * c * ((v1 + v2 + v3 + v4) - 4 * this->u[i][j]); 
            this->v[i][j] += f * dt;
            //this->v[i][j] *= 0.995;
            this->u_new[i][j] = u[i][j] + v[i][j] * dt;
        }
    }

    for (int i = 0; i < this->width; i++) {
        for (int j = 0; j < this->height; j++) {
            this->u[i][j] = this->u_new[i][j];
        }
    }
}
