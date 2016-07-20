#include "objects/water.h"

using namespace object3d;

water::water() {
    // Set up the VAO and VBOS
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &elements_vbo);
    glGenBuffers(1, &normals_vbo);
    glGenBuffers(1, &points_vbo);

    // Initialize water heights
    for (int i = 0; i < this->width; i++) {
        for (int j = 0; j < this->height; j++) {
            float x = -3 + 6 * (1 - (i / (float) this->width));
            float y = -3 + 6 * (1 - (j / (float) this->height));


            if (i > 42 && i < 48 && j > 42 && j < 48) {
                this->u[i][j] = 0.7;
            }
            else {
                this->u[i][j] = 0;
            }

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

    // Calculate points
    int point_index = 0;
    for (float x = -3; x < 3; x += 0.1) {
        for (float y = -3; y < 3; y += 0.1) {
            int i = ((x + 3.0) / 6.0) * this->width;        
            int j = ((y + 3.0) / 6.0) * this->height;        

            // Take the average height of the blocks around us, weighted by their distances
            float sum_of_weights = 0.0;
            float avg_height = 0.0;
            for (int k = i-1; k <= i+1; k++) {
                if (k < 0 || k >= this->width) {
                    continue;
                }

                for (int l = j-1; l <= j+1; l++) {
                    if (l < 0 || l >= this->height) {
                        continue;
                    }

                    float u = -3 + 6 * (k / (float) this->width);
                    float v = -3 + 6 * (l / (float) this->height);

                    // Make sure the weight is larger for smaller distances
                    float weight = 100 - (u - x) * (u - x) + (y - v) * (y - v); 
                    avg_height += this->u[k][l] * weight;
                    sum_of_weights += weight;
                }
            }
            avg_height /= sum_of_weights;

            points[point_index++].set(x, y, avg_height);
        }
    }

    // Calculate normals
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            // Average the normals for each triangle around us
            int p1_i = i + N * j;
            int p2_i = i + 1 + N * j;
            int p3_i = i - 1 + N * j;
            int p4_i = i + N * (j - 1);
            int p5_i = i + N * (j + 1);

            vec3 normal;

            if (i == 0 && j == 0) {
                vec3 p1 = points[p1_i];
                vec3 p2 = points[p2_i];
                vec3 p5 = points[p5_i];

                normal = vec3::cross(p5 - p1, p2 - p1);
            } else if (i == 0 && j == N - 1) {
                vec3 p1 = points[p1_i];
                vec3 p2 = points[p2_i];
                vec3 p4 = points[p4_i];

                normal = vec3::cross(p2 - p1, p4 - p1);
            } else if (i == N - 1 && j == 0) {
                vec3 p1 = points[p1_i];
                vec3 p3 = points[p3_i];
                vec3 p5 = points[p5_i];

                normal = vec3::cross(p3 - p1, p5 - p1);
            } else if (i == N - 1 && j == N - 1) {
                vec3 p1 = points[p1_i];
                vec3 p3 = points[p3_i];
                vec3 p4 = points[p4_i];

                normal = vec3::cross(p4 - p1, p3 - p1);
            } else if (i == 0) {
                vec3 p1 = points[p1_i];
                vec3 p2 = points[p2_i];
                vec3 p4 = points[p4_i];
                vec3 p5 = points[p5_i];

                vec3 n1 = vec3::cross(p2 - p1, p4 - p1);
                n1.make_unit_length();
                vec3 n2 = vec3::cross(p5 - p1, p2 - p1);
                n2.make_unit_length();

                normal = 0.5 * (n1 + n2);
            } else if (j == 0) {
                vec3 p1 = points[p1_i];
                vec3 p2 = points[p2_i];
                vec3 p3 = points[p3_i];
                vec3 p5 = points[p5_i];

                vec3 n1 = vec3::cross(p3 - p1, p5 - p1);
                n1.make_unit_length();
                vec3 n2 = vec3::cross(p5 - p1, p2 - p1);
                n2.make_unit_length();

                normal = 0.5 * (n1 + n2);
            } else if (i == N - 1) {
                vec3 p1 = points[p1_i];
                vec3 p3 = points[p3_i];
                vec3 p4 = points[p4_i];
                vec3 p5 = points[p5_i];

                vec3 n1 = vec3::cross(p4 - p1, p3 - p1);
                n1.make_unit_length();
                vec3 n2 = vec3::cross(p3 - p1, p5 - p1);
                n2.make_unit_length();

                normal = 0.5 * (n1 + n2);
            } else if (j == N - 1) {
                vec3 p1 = points[p1_i];
                vec3 p2 = points[p2_i];
                vec3 p3 = points[p3_i];
                vec3 p4 = points[p4_i];

                vec3 n1 = vec3::cross(p2 - p1, p4 - p1);
                n1.make_unit_length();
                vec3 n2 = vec3::cross(p4 - p1, p3 - p1);
                n2.make_unit_length();

                normal = 0.5 * (n1 + n2);
            } else {
                vec3 p1 = points[p1_i];
                vec3 p2 = points[p2_i];
                vec3 p3 = points[p3_i];
                vec3 p4 = points[p4_i];
                vec3 p5 = points[p5_i];

                vec3 n1 = vec3::cross(p4 - p1, p3 - p1);
                n1.make_unit_length();
                vec3 n2 = vec3::cross(p2 - p1, p4 - p1);
                n2.make_unit_length();
                vec3 n3 = vec3::cross(p5 - p1, p2 - p1);
                n3.make_unit_length();
                vec3 n4 = vec3::cross(p3 - p1, p5 - p1);
                n4.make_unit_length();

                normal = 0.25 * (n1 + n2 + n3 + n4); 
            }

            normal.make_unit_length();
            normals[p1_i] = normal; 
        }
    }

    // Calculate the elements for each triangle
    int e_i = 0;
    for (int i = 0; i < N - 1; i++) {
        for (int j = 0; j < N - 1; j++) {
            // First triangle
            int p1 = i + N * j;
            int p2 = i + 1 + N * j;
            int p3 = i + N * (j + 1);

            elements[e_i++] = vec3(p1, p2, p3);

            // Second triangle
            int p4 = i + 1 + N * j;
            int p5 = i + 1 + N * (j + 1);
            int p6 = i + N * (j + 1);

            elements[e_i++] = vec3(p4, p5, p6);
        }
    }

    glBindVertexArray(vao);

    // Set up the points vbo
    for (int i = 0; i < N * N; i++) {
        points_buffer[i*3] = points[i].x;
        points_buffer[i*3 + 1] = points[i].y;
        points_buffer[i*3 + 2] = points[i].z;
    }
    glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * N * N * 3, points_buffer, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(0);

    // Set up the normals vbo
    for (int i = 0; i < N * N; i++) {
        normals_buffer[i*3] = normals[i].x;
        normals_buffer[i*3 + 1] = normals[i].y;
        normals_buffer[i*3 + 2] = normals[i].z;
    }
    glBindBuffer(GL_ARRAY_BUFFER, normals_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * N * N * 3, normals_buffer, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(1);


    // Set up elements vbo
    for (int i = 0; i < (N - 1) * (N - 1) * 2; i++) {
        elements_buffer[i*3] = elements[i].x;
        elements_buffer[i*3 + 1] = elements[i].y;
        elements_buffer[i*3 + 2] = elements[i].z;
    }
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elements_vbo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * (N - 1) * (N - 1) * 2 * 3, elements_buffer, GL_STATIC_DRAW);

    glBindVertexArray(0);
}
