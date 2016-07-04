#include "objects/rectangle.h"

using namespace object3d;

rectangle::rectangle(const vec3 &p1, const vec3 &p2) {
    float width = 0.05;

    vec3 corner_points[8];
    vec3 face_normals[6];

    if (fabs(p1.x - p2.x) > EPSILON) {
        vec3 right_point, left_point; 
        if (p1.x > p2.x) {
            right_point = p1;
            left_point = p2;
        }
        else {
            right_point = p2;
            left_point = p1;
        }

        /* Vectors for Right Face */
        corner_points[0] = vec3(right_point.x + width, right_point.y - width, right_point.z + width);
        corner_points[1] = vec3(right_point.x + width, right_point.y - width, right_point.z - width);
        corner_points[2] = vec3(right_point.x + width, right_point.y + width, right_point.z + width);
        corner_points[3] = vec3(right_point.x + width, right_point.y + width, right_point.z - width);

        /* Vectors for Left Face */
        corner_points[4] = vec3(left_point.x - width, left_point.y - width, left_point.z + width);
        corner_points[5] = vec3(left_point.x - width, left_point.y - width, left_point.z - width);
        corner_points[6] = vec3(left_point.x - width, left_point.y + width, left_point.z + width);
        corner_points[7] = vec3(left_point.x - width, left_point.y + width, left_point.z - width);
        
        /* Set up the face normals */
        face_normals[0] = vec3(1, 0, 0);
        face_normals[1] = vec3(-1, 0, 0);
        face_normals[2] = vec3(0, 0, 1);
        face_normals[3] = vec3(0, 1, 0);
        face_normals[4] = vec3(0, 0, -1);
        face_normals[5] = vec3(0, -1, 0);
    }
    else if (fabs(p1.y - p2.y) > EPSILON) {
        vec3 top_point, bot_point;
        if (p1.y > p2.y) {
            top_point = p1;
            bot_point = p2;
        }
        else {
            top_point = p2;
            bot_point = p1;
        }

        /* Vectors for Top Face */
        corner_points[0] = vec3(top_point.x + width, top_point.y + width, top_point.z + width);
        corner_points[1] = vec3(top_point.x + width, top_point.y + width, top_point.z - width);
        corner_points[2] = vec3(top_point.x - width, top_point.y + width, top_point.z + width);
        corner_points[3] = vec3(top_point.x - width, top_point.y + width, top_point.z - width);

        /* Vectors for Bottom Face */
        corner_points[4] = vec3(bot_point.x + width, bot_point.y - width, bot_point.z + width);
        corner_points[5] = vec3(bot_point.x + width, bot_point.y - width, bot_point.z - width);
        corner_points[6] = vec3(bot_point.x - width, bot_point.y - width, bot_point.z + width);
        corner_points[7] = vec3(bot_point.x - width, bot_point.y - width, bot_point.z - width);

        /* Set up the face normals */
        face_normals[0] = vec3(0, 1, 0);
        face_normals[1] = vec3(0, -1, 0);
        face_normals[2] = vec3(0, 0, 1);
        face_normals[3] = vec3(-1, 0, 0);
        face_normals[4] = vec3(0, 0, -1);
        face_normals[5] = vec3(1, 0, 0);
    }
    else if (fabs(p1.z - p2.z) > EPSILON) {
        vec3 forward_point, backward_point; 
        if (p1.z > p2.z) {
            forward_point = p1;
            backward_point = p2;
        }
        else {
            forward_point = p2;
            backward_point = p1;
        }

        /* Vectors for Forward Face */
        corner_points[0] = vec3(forward_point.x + width, forward_point.y + width, forward_point.z + width);
        corner_points[1] = vec3(forward_point.x - width, forward_point.y + width, forward_point.z + width);
        corner_points[2] = vec3(forward_point.x + width, forward_point.y - width, forward_point.z + width);
        corner_points[3] = vec3(forward_point.x - width, forward_point.y - width, forward_point.z + width);

        /* Vectors for Backward Face */
        corner_points[4] = vec3(backward_point.x + width, backward_point.y + width, backward_point.z - width);
        corner_points[5] = vec3(backward_point.x - width, backward_point.y + width, backward_point.z - width);
        corner_points[6] = vec3(backward_point.x + width, backward_point.y - width, backward_point.z - width);
        corner_points[7] = vec3(backward_point.x - width, backward_point.y - width, backward_point.z - width);

        /* Set up the face normals */
        face_normals[0] = vec3(0, 0, 1);
        face_normals[1] = vec3(0, 0, -1);
        face_normals[2] = vec3(1, 0, 0);
        face_normals[3] = vec3(0, -1, 0);
        face_normals[4] = vec3(-1, 0, 0);
        face_normals[5] = vec3(0, 1, 0);
    }


    /* Set up positions for the triangles vertices */
    vec3 positions[36];
    /* Top Face Triangle */
    positions[0] = corner_points[0];
    positions[1] = corner_points[1];
    positions[2] = corner_points[2];
    positions[3] = corner_points[1];
    positions[4] = corner_points[2];
    positions[5] = corner_points[3];
    /* Bottom Face Triangles */
    positions[6] = corner_points[4];
    positions[7] = corner_points[5];
    positions[8] = corner_points[6];
    positions[9] = corner_points[5];
    positions[10] = corner_points[6];
    positions[11] = corner_points[7];
    /* Side 1 Face Triangles */
    positions[12] = corner_points[0];
    positions[13] = corner_points[4];
    positions[14] = corner_points[6];
    positions[15] = corner_points[0];
    positions[16] = corner_points[2];
    positions[17] = corner_points[6];
    /* Side 2 Face Triangles */
    positions[18] = corner_points[2];
    positions[19] = corner_points[6];
    positions[20] = corner_points[7];
    positions[21] = corner_points[2];
    positions[22] = corner_points[3];
    positions[23] = corner_points[7];
    /* Side 3 Face Triangles */
    positions[24] = corner_points[3];
    positions[25] = corner_points[5];
    positions[26] = corner_points[7];
    positions[27] = corner_points[1];
    positions[28] = corner_points[3];
    positions[29] = corner_points[5];
    /* Side 4 Face Triangles */
    positions[30] = corner_points[1];
    positions[31] = corner_points[4];
    positions[32] = corner_points[5];
    positions[33] = corner_points[0];
    positions[34] = corner_points[1];
    positions[35] = corner_points[4];

    /* Set up the normals for the triangles */
    vec3 normals[36];
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 6; j++) {
            normals[i*6 + j] = face_normals[i]; 
            normals[i*6 + j] = face_normals[i]; 
            normals[i*6 + j] = face_normals[i]; 
        }
    }

    /* Set up the position and normal floats for OpenGL */
    GLfloat position_floats[36*3];
    GLfloat normal_floats[36*3];
    for (int i = 0; i < 36; i++) {
        position_floats[i*3] = positions[i].x;
        position_floats[i*3+1] = positions[i].y;
        position_floats[i*3+2] = positions[i].z;
        normal_floats[i*3] = normals[i].x;
        normal_floats[i*3+1] = normals[i].y;
        normal_floats[i*3+2] = normals[i].z;
    }

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLuint positions_vbo;
    glGenBuffers(1, &positions_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, positions_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(position_floats), position_floats, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(0);

    GLuint normals_vbo;
    glGenBuffers(1, &normals_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, normals_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(normal_floats), normal_floats, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}
