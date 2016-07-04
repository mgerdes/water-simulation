#include "objects/bezier_surface.h"

using namespace object3d;

bezier_surface::bezier_surface(vec3 control_points[4][4]) {
    // Set up the VAO
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Calculate all the points on the surface
    std::vector<vec3> points;
    for (int k = 0; k < N; k++) {
        for (int l = 0; l < N; l++) {
            float u = k / (float) N;
            float v = l / (float) N;

            vec3 p(0, 0, 0);

            for (int i = 0; i < 4; i++) {
                for (int j = 0; j < 4; j++) {
                    float b1 = eval_bernstien_polynomial(u, i);
                    float b2 = eval_bernstien_polynomial(v, j);

                    p += b1 * b2 * control_points[i][j];
                }
            }

            points.push_back(p);
        }
    }

    // Calculate the points and normals for the triangles
    std::vector<vec3> triangle_points;
    std::vector<vec3> triangle_normals;
    for (int i = 0; i < (N - 1); i++) {
        for (int j = 0; j < (N - 1); j++) {
            // First triangle
            int p1 = i + N * j;
            int p2 = i + 1 + N * j;
            int p3 = i + N * (j + 1);
            triangle_points.push_back(points[p1]);
            triangle_points.push_back(points[p2]);
            triangle_points.push_back(points[p3]);

            vec3 v1 = points[p1] - points[p2];
            v1.make_unit_length();
            vec3 v2 = points[p1] - points[p3];
            v2.make_unit_length();

            vec3 n1 = vec3::cross(v1, v2);
            triangle_normals.push_back(n1);
            triangle_normals.push_back(n1);
            triangle_normals.push_back(n1);

            // Second triangle
            int p4 = i + 1 + N * j;
            int p5 = i + 1 + N * (j + 1);
            int p6 = i + N * (j + 1);
            triangle_points.push_back(points[p4]);
            triangle_points.push_back(points[p5]);
            triangle_points.push_back(points[p6]);

            vec3 v3 = points[p4] - points[p5];
            v3.make_unit_length();
            vec3 v4 = points[p4] - points[p6];
            v4.make_unit_length();

            vec3 n2 = vec3::cross(v3, v4);
            triangle_normals.push_back(n1);
            triangle_normals.push_back(n1);
            triangle_normals.push_back(n1);
        }
    }

    // Set up the points vbo
    float *points_buffer = new float[triangle_points.size() * 3];
    for (int i = 0; i < triangle_points.size(); i++) {
        points_buffer[i*3] = triangle_points[i].x;
        points_buffer[i*3 + 1] = triangle_points[i].y;
        points_buffer[i*3 + 2] = triangle_points[i].z;
    }
    GLuint points_vbo;
    glGenBuffers(1, &points_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * triangle_points.size() * 3, points_buffer, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(0);
    delete[] points_buffer;

    // Set up the normals vbo
    float *normals_buffer = new float[triangle_normals.size() * 3];
    for (int i = 0; i < triangle_normals.size(); i++) {
        normals_buffer[i*3] = triangle_normals[i].x;
        normals_buffer[i*3 + 1] = triangle_normals[i].y;
        normals_buffer[i*3 + 2] = triangle_normals[i].z;
    }
    GLuint normals_vbo;
    glGenBuffers(1, &normals_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, normals_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * triangle_normals.size() * 3, normals_buffer, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(1);
    delete[] normals_buffer;

    glBindVertexArray(0);
}

/*
 * https://en.wikipedia.org/wiki/Bernstein_polynomial
 * Assumes n to be 3
 * e.g: (3 choose v) * x^v * (1 - x)^(3 - v)
 */
float bezier_surface::eval_bernstien_polynomial(float x, int v) {
    static const float binomial_coeffs[4] = {1, 3, 3, 1}; 
    return binomial_coeffs[v] * pow(x, v) * pow(1 - x, 3 - v);
}
