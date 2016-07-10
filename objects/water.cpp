#include "objects/water.h"

using namespace object3d;

water::water() {
    for (int i = 0; i < this->width; i++) {
        for (int j = 0; j < this->height; j++) {
            float x = -3 + 6 * (1 - (i / (float) this->width));
            float y = -3 + 6 * (1 - (j / (float) this->height));

            if (x < 0.1 && x > -0.1 && y < 0.1 && y > -0.1) {
                this->u[i][j] = 1.0;
            }
            else {
                this->u[i][j] = 0;
            }

            //this->u[i][j] = 1.0 * sin(x) * sin(x) * cos(y);
            //this->u[i][j] += 0.2 * (x + y);
            //this->u[i][j] = abs(x) + 1.0;
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
            this->v[i][j] *= 0.995;
            this->u_new[i][j] = u[i][j] + v[i][j] * dt;
        }
    }

    for (int i = 0; i < this->width; i++) {
        for (int j = 0; j < this->height; j++) {
            this->u[i][j] = this->u_new[i][j];
            this->control_point_heights[i][j] = this->u[i][j];
        }
    }

    static int x_delta[9] = {0, -1, -1, 0, 1, 1, 1, 0, -1};
    static int y_delta[9] = {0, 0, -1, -1, -1, 0, 1, 1, 1};

    for (int i = 3; i < this->width; i+=3) {
        for (int j = 3; j < this->height; j+=3) {
            vec3 points[9];

            for (int k = 0; k < 9; k++) {
                int x_index = i + x_delta[k];
                int y_index = j + y_delta[k];

                points[k].x = -3 + 6 * (1 - (x_index / (float) this->width));
                points[k].y = -3 + 6 * (1 - (y_index / (float) this->height));
                points[k].z = this->control_point_heights[x_index][y_index];
            }

            float sum_xx = 0.0;
            float sum_yy = 0.0;
            float sum_xy = 0.0;
            float sum_yz = 0.0;
            float sum_xz = 0.0;

            for (int k = 0; k < 9; k++) {
                sum_xx += points[k].x * points[k].x;
                sum_yy += points[k].y * points[k].y;
                sum_xy += points[k].x * points[k].y;
                sum_yz += points[k].y * points[k].z;
                sum_xz += points[k].x * points[k].z;
            }

            float D = sum_xx * sum_yy - sum_xy * sum_xy;
            float a = (sum_yz * sum_xy - sum_xz * sum_yy) / D;
            float b = (sum_xy * sum_xz - sum_xx * sum_yz) / D;

            vec3 n(a, b, 1);
            vec3 p = points[0];

            for (int k = 1; k < 9; k++) {
                vec3 p0 = points[k];

                float z = (n.x * (p.x - p0.x) + n.y * (p.y - p0.y)) / n.z + p.z;

                int x_index = i + x_delta[k];
                int y_index = j + y_delta[k];
                this->control_point_heights[x_index][y_index] = z;
            }
        }
    }
}
