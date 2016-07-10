#include "objects/bezier_surface.h"

using namespace object3d;

bezier_surface::bezier_surface() {
    // Set up the VAO and VBOS
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &elements_vbo);
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

    // Calculate all the normals for the surface
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
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

/*
 * https://en.wikipedia.org/wiki/Bernstein_polynomial
 * Assumes n to be 3
 * e.g: (3 choose v) * x^v * (1 - x)^(3 - v)
 */
float bezier_surface::eval_bernstien_polynomial(int u, int v) {
    return bernstein_results[u][v];
}
