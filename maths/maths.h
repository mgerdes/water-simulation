#ifndef MATHS_H
#define MATHS_H

#include <stdio.h>

#define DEGREES_PER_RADIAN 0.01745329251
#define EPSILON 0.0001

class mat4;

/*
 * 3-D vectors
 */
class vec3 
{
    public:
        float x, y, z;

        static const vec3 x_axis, y_axis, z_axis;

        vec3();

        vec3(float x, float y, float z);

        void set(float x, float y, float z);

        float length();

        void make_unit_length();

        static float dot(const vec3 &u, const vec3 &v);

        static vec3 cross(const vec3 &u, const vec3 &v);

        void print();
};

vec3 operator-(const vec3 &u);

vec3 operator-(const vec3 &u, const vec3 &v);

vec3 operator+(const vec3 &u, const vec3 &v);

vec3 operator+=(vec3 &u, const vec3 &v);

vec3 operator*(float c, const vec3 &u);

vec3 operator*(const mat4 &m, const vec3 &u);

/*
 * 4 x 4 matrices
 * Stored in row-order form
 */
class mat4 {
    public:
        float m[16];

        mat4();

        mat4(float a, float b, float c, float d,
                float e, float f, float g, float h,
                float i, float j, float k, float l,
                float m, float n, float o, float p);

        mat4 inverse();

        mat4 transpose();

        mat4 normal_transform();

        void make_zero();

        static mat4 identity();

        static mat4 translation(const vec3 &v);

        static mat4 scale(const vec3 &v);

        static mat4 rotation_x(float theta);

        static mat4 rotation_y(float theta);

        static mat4 rotation_z(float theta);

        static mat4 perspective_projection(float fov, float aspect, float near, float far);

        static mat4 look_at(const vec3 &position, const vec3 &target, const vec3 &up);

        void print();
};

mat4 operator*(const mat4 &A, const mat4 &B);

/*
 * Quaternions
 */
class quaternion {
    public:
        float w, x, y, z;

        quaternion();

        quaternion(float theta, const vec3 &rotation_axis);

        mat4 matrix();

        float length();

        void make_unit_length();
};

quaternion operator*(const quaternion &q1, const quaternion &q2);

#endif
