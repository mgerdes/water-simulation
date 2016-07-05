#include "objects/bezier_surface.h"

using namespace object3d;

bezier_surface::bezier_surface() {
    // Set up the VAO and VBOS
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &normals_vbo);
    glGenBuffers(1, &points_vbo);

    float binomial_coeffs[4] = {1, 3, 3, 1}; 

    // Set up the bernstein_results cached array
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < 4; j++) {
            float x = i / (float) (N - 1);
            bernstein_results[i][j] = binomial_coeffs[j] * pow(x, j) * pow(1 - x, 3 - j);
        }
    }
}

void bezier_surface::update_control_points(vec3 control_points[4][4]) {
    glBindVertexArray(vao);

    // Calculate all the points on the surface
    for (int k = 0; k < N; k++) {
        for (int l = 0; l < N; l++) {
            float u = k / (float) (N - 1);
            float v = l / (float) (N - 1);

            vec3 p(0, 0, 0);

            for (int i = 0; i < 4; i++) {
                for (int j = 0; j < 4; j++) {
                    float b1 = bernstein_results[k][i];
                    float b2 = bernstein_results[l][j];

                    p += b1 * b2 * control_points[i][j];
                }
            }

            points[k * N + l] = p;
        }
    }

    // Calculate the points and normals for the triangles
    int tpi = 0;
    int tpn = 0;
    for (int i = 0; i < (N - 1); i++) {
        for (int j = 0; j < (N-1); j++) {
            // First triangle
            int p1 = i + N * j;
            int p2 = i + 1 + N * j;
            int p3 = i + N * (j+1);
            triangle_points[tpi++] = points[p1];
            triangle_points[tpi++] = points[p2];
            triangle_points[tpi++] = points[p3];

            vec3 v1 = points[p1] - points[p2];
            v1.make_unit_length();
            vec3 v2 = points[p1] - points[p3];
            v2.make_unit_length();

            vec3 n1 = vec3::cross(v1, v2);
            triangle_normals[tpn++] = n1;
            triangle_normals[tpn++] = n1;
            triangle_normals[tpn++] = n1;

            // Second triangle
            int p4 = i + 1 + N * j;
            int p5 = i + 1 + N * (j+1);
            int p6 = i + N * (j + 1);
            triangle_points[tpi++] = points[p4];
            triangle_points[tpi++] = points[p5];
            triangle_points[tpi++] = points[p6];

            vec3 v3 = points[p5] - points[p6];
            v3.make_unit_length();
            vec3 v4 = points[p5] - points[p4];
            v4.make_unit_length();

            vec3 n2 = vec3::cross(v3, v4);
            triangle_normals[tpn++] = n2;
            triangle_normals[tpn++] = n2;
            triangle_normals[tpn++] = n2;
        }
    }

    // Set up the points vbo
    for (int i = 0; i < (N-1) * (N-1) * 3 * 2; i++) {
        points_buffer[i*3] = triangle_points[i].x;
        points_buffer[i*3 + 1] = triangle_points[i].y;
        points_buffer[i*3 + 2] = triangle_points[i].z;
    }
    glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * (N-1) * (N-1) * 3 * 2 * 3, points_buffer, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(0);

    // Set up the normals vbo
    for (int i = 0; i < (N-1) * (N-1) * 3 * 2; i++) {
        normals_buffer[i*3] = triangle_normals[i].x;
        normals_buffer[i*3 + 1] = triangle_normals[i].y;
        normals_buffer[i*3 + 2] = triangle_normals[i].z;
    }
    glBindBuffer(GL_ARRAY_BUFFER, normals_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * (N-1) * (N-1) * 3 * 2 * 3, normals_buffer, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

/*
 * https://en.wikipedia.org/wiki/Bernstein_polynomial
 * Assumes n to be 3
 * e.g: (3 choose v) * x^v * (1 - x)^(3 - v)
 */
float bezier_surface::eval_bernstien_polynomial(int u, int v) {
    return bernstein_results[u][v];
}
