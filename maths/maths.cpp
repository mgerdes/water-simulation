#include <math.h>

#include "maths.h"

/*
 * Most formulas from 'Mathematics for 3D Game Programming and Computer Graphics' by Lengyel
 */

/*
 * 3-D Vectors
 */
vec3::vec3() 
    : x(0), y(0), z(0) { };

vec3::vec3(float x, float y, float z)
    : x(x), y(y), z(z) { }

const vec3 vec3::x_axis = vec3(1, 0, 0);

const vec3 vec3::y_axis = vec3(0, 1, 0);

const vec3 vec3::z_axis = vec3(0, 0, 1);

void vec3::make_unit_length() {
    float l = length();

    if (l == 0) {
        return;
    }

    x /= l;
    y /= l;
    z /= l;
}

void vec3::set(float x, float y, float z) {
    this->x = x;
    this->y = y;
    this->z = z;
}

float vec3::length() {
    return sqrt(x*x + y*y + z*z);
}

void vec3::print() {
    printf("<%f, %f, %f>\n", x, y, z);
}

float vec3::dot(const vec3 &u, const vec3 &v) {
    return u.x*v.x + u.y*v.y + u.z*v.z;
}

vec3 vec3::cross(const vec3 &u, const vec3 &v) {
    float x = u.y * v.z - u.z * v.y;
    float y = u.z * v.x - u.x * v.z;
    float z = u.x * v.y - u.y * v.x;
    return vec3(x, y, z);
}

vec3 operator-(const vec3 &u) {
    return vec3(-u.x, -u.y, -u.z);
}

vec3 operator-(const vec3 &u, const vec3 &v) {
    return vec3(u.x - v.x, u.y - v.y, u.z - v.z);
}

vec3 operator+(const vec3 &u, const vec3 &v) {
    return vec3(u.x + v.x, u.y + v.y, u.z + v.z);
}

vec3 operator+=(vec3 &u, const vec3 &v) {
    u.x += v.x;
    u.y += v.y;
    u.z += v.z;
    return u;
}

vec3 operator*(float c, const vec3 &u) {
    return vec3(c * u.x, c * u.y, c * u.z);
}

vec3 operator*(const mat4 &m, const vec3 &u) {
    float x = u.x*m.m[0] + u.y*m.m[1] + u.z*m.m[2] + m.m[3];
    float y = u.x*m.m[4] + u.y*m.m[5] + u.z*m.m[6] + m.m[7];
    float z = u.x*m.m[8] + u.y*m.m[9] + u.z*m.m[10] + m.m[11];
    return vec3(x, y, z);
}

/*
 * 4 x 4 matrices
 */
mat4::mat4() {
    // Default constructor creates an identity matrix
    make_zero();
    m[0] = 1;
    m[5] = 1;
    m[10] = 1;
    m[15] = 1;
}

mat4::mat4(float a, float b, float c, float d,
        float e, float f, float g, float h,
        float i, float j, float k, float l,
        float m, float n, float o, float p) {
    this->m[0] = a;
    this->m[1] = b;
    this->m[2] = c;
    this->m[3] = d;
    this->m[4] = e;
    this->m[5] = f;
    this->m[6] = g;
    this->m[7] = h;
    this->m[8] = i;
    this->m[9] = j;
    this->m[10] = k;
    this->m[11] = l;
    this->m[12] = m;
    this->m[13] = n;
    this->m[14] = o;
    this->m[15] = p;
}

mat4 mat4::inverse() {
    float det = 
        m[0]*m[5]*m[10]*m[15] +
        m[0]*m[6]*m[11]*m[13] +
        m[0]*m[7]*m[9]*m[14] +

        m[1]*m[4]*m[11]*m[14] +
        m[1]*m[6]*m[8]*m[15] +
        m[1]*m[7]*m[10]*m[12] +

        m[2]*m[4]*m[9]*m[15] +
        m[2]*m[5]*m[11]*m[12] +
        m[2]*m[7]*m[8]*m[13] +

        m[3]*m[4]*m[10]*m[13] +
        m[3]*m[5]*m[8]*m[14] +
        m[3]*m[6]*m[9]*m[12] -

        m[0]*m[5]*m[11]*m[14] -
        m[0]*m[6]*m[9]*m[15] -
        m[0]*m[7]*m[10]*m[13] -

        m[1]*m[4]*m[10]*m[15] -
        m[1]*m[6]*m[11]*m[12] -
        m[1]*m[7]*m[8]*m[14] -

        m[2]*m[4]*m[11]*m[13] -
        m[2]*m[5]*m[8]*m[15] -
        m[2]*m[7]*m[9]*m[12] -

        m[3]*m[4]*m[9]*m[14] -
        m[3]*m[5]*m[10]*m[12] -
        m[3]*m[6]*m[8]*m[13]; 

    float a0 = m[5]*m[10]*m[15] + m[6]*m[11]*m[13] + m[7]*m[9]*m[14]
        - m[5]*m[11]*m[14] - m[6]*m[9]*m[15] - m[7]*m[10]*m[13];

    float a1 = m[1]*m[11]*m[14] + m[2]*m[9]*m[15] + m[3]*m[10]*m[13]
        - m[1]*m[10]*m[15] - m[2]*m[11]*m[13] - m[3]*m[9]*m[14];

    float a2 = m[1]*m[6]*m[15] + m[2]*m[7]*m[13] + m[3]*m[5]*m[14]
        - m[1]*m[7]*m[14] - m[2]*m[5]*m[15] - m[3]*m[6]*m[13];

    float a3 = m[1]*m[7]*m[10] + m[2]*m[5]*m[11] + m[3]*m[6]*m[9]
        - m[1]*m[6]*m[11] - m[2]*m[7]*m[9] - m[3]*m[5]*m[10];

    float a4 = m[4]*m[11]*m[14] + m[6]*m[8]*m[15] + m[7]*m[10]*m[12]
        - m[4]*m[10]*m[15] - m[6]*m[11]*m[12] - m[7]*m[8]*m[14];

    float a5 = m[0]*m[10]*m[15] + m[2]*m[11]*m[12] + m[3]*m[8]*m[14]
        - m[0]*m[11]*m[14] - m[2]*m[8]*m[15] - m[3]*m[10]*m[12];

    float a6 = m[0]*m[7]*m[14] + m[2]*m[4]*m[15] + m[3]*m[6]*m[12]
        - m[0]*m[6]*m[15] - m[2]*m[7]*m[12] - m[3]*m[4]*m[14];

    float a7 = m[0]*m[6]*m[11] + m[2]*m[7]*m[8] + m[3]*m[4]*m[10]
        - m[0]*m[7]*m[10] - m[2]*m[4]*m[11] - m[3]*m[6]*m[8];

    float a8 = m[4]*m[9]*m[15] + m[5]*m[11]*m[12] + m[7]*m[8]*m[13]
        - m[4]*m[11]*m[13] - m[5]*m[8]*m[15] - m[7]*m[9]*m[12];

    float a9 = m[0]*m[11]*m[13] + m[1]*m[8]*m[15] + m[3]*m[9]*m[12]
        - m[0]*m[9]*m[15] - m[1]*m[11]*m[12] - m[3]*m[8]*m[13];

    float a10 = m[0]*m[5]*m[15] + m[1]*m[7]*m[12] + m[3]*m[4]*m[13]
        - m[0]*m[7]*m[13] - m[1]*m[4]*m[15] - m[3]*m[5]*m[12];

    float a11 = m[0]*m[7]*m[9] + m[1]*m[4]*m[11] + m[3]*m[5]*m[8]
        - m[0]*m[5]*m[11] - m[1]*m[7]*m[8] - m[3]*m[4]*m[9];

    float a12 = m[4]*m[10]*m[13] + m[5]*m[8]*m[14] + m[6]*m[9]*m[12]
        - m[4]*m[9]*m[14] - m[5]*m[10]*m[12] - m[6]*m[8]*m[13];

    float a13 = m[0]*m[9]*m[14] + m[1]*m[10]*m[12] + m[2]*m[8]*m[13]
        - m[0]*m[10]*m[13] - m[1]*m[8]*m[14] - m[2]*m[9]*m[12];

    float a14 = m[0]*m[6]*m[13] + m[1]*m[4]*m[14] + m[2]*m[5]*m[12]
        - m[0]*m[5]*m[14] - m[1]*m[6]*m[12] - m[2]*m[4]*m[13];

    float a15 = m[0]*m[5]*m[10] + m[1]*m[6]*m[8] + m[2]*m[4]*m[9]
        - m[0]*m[6]*m[9] - m[1]*m[4]*m[10] - m[2]*m[5]*m[8];

    return mat4(
            a0 / det, a1 / det, a2 / det, a3 / det,
            a4 / det, a5 / det, a6 / det, a7 / det,
            a8 / det, a9 / det, a10 / det, a11 / det, 
            a12 / det, a13 / det, a14 / det, a15 / det
            );
}


mat4 mat4::transpose() {
    return mat4(
            m[0], m[4], m[8], m[12],
            m[1], m[5], m[9], m[13],
            m[2], m[6], m[10], m[14],
            m[3], m[7], m[11], m[15]
            );
}

mat4 mat4::normal_transform() {
    return inverse().transpose();
}

void mat4::make_zero() {
    for (int i = 0; i < 16; i++) {
        m[i] = 0;
    }
}

mat4 mat4::identity() {
    return mat4(1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1);
}

mat4 mat4::translation(const vec3 &v) {
    return mat4(1, 0, 0, v.x,
            0, 1, 0, v.y,
            0, 0, 1, v.z,
            0, 0, 0, 1);
}

mat4 mat4::scale(const vec3 &v) {
    return mat4(v.x, 0, 0, 0,
            0, v.y, 0, 0,
            0, 0, v.z, 0,
            0, 0, 0, 1);
}

mat4 mat4::rotation_x(float theta) {
    float c = cos(theta);
    float s = sin(theta);
    return mat4(
            1, 0, 0, 0,
            0, c, -s, 0,
            0, s, c, 0,
            0, 0, 0, 1
            );
}

mat4 mat4::rotation_y(float theta) {
    float c = cos(theta);
    float s = sin(theta);
    return mat4(
            c, 0, s, 0,
            0, 1, 0, 0,
            -s, 0, c, 0,
            0, 0, 0, 1
            );
}

mat4 mat4::rotation_z(float theta) {
    float c = cos(theta);
    float s = sin(theta);
    return mat4(
            c, -s, 0, 0,
            s, c, 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1
            );
}

/*
 * http://www.cs.virginia.edu/~gfx/Courses/2000/intro.spring00.html/lectures/lecture09/sld017.htm
 */
mat4 mat4::perspective_projection(float fov, float aspect, float near, float far) {
    float f = 1.0 / tan(fov * DEGREES_PER_RADIAN / 2.0);
    float denominator = near - far;

    float a = f / aspect;
    float b = f;
    float c = (far + near) / denominator;
    float d = (2.0 * far * near) / denominator;

    return mat4(
            a, 0, 0, 0,
            0, b, 0, 0,
            0, 0, c, d,
            0, 0, -1, 0
            );
}

/*
 * http://www.cs.virginia.edu/~gfx/Courses/1999/intro.fall99.html/lookat.html
 */
mat4 mat4::look_at(const vec3 &position, const vec3 &target, const vec3 &up) {
    vec3 up_p = vec3(up);
    up_p.make_unit_length();

    vec3 f = target - position;
    f.make_unit_length();

    vec3 s = vec3::cross(f, up_p);
    s.make_unit_length();

    vec3 u = vec3::cross(s, f);
    u.make_unit_length();

    mat4 M = mat4(
            s.x, s.y, s.z, 0,
            u.x, u.y, u.z, 0,
            -f.x, -f.y, -f.z, 0,
            0, 0, 0, 1
            );
    mat4 T = mat4::translation(-position);
    return M * T;
}

mat4 operator*(const mat4 &A, const mat4 &B) {
    const float *a = A.m;
    const float *b = B.m;

    float c0 = a[0]*b[0] + a[1]*b[4] + a[2]*b[8] + a[3]*b[12];
    float c1 = a[0]*b[1] + a[1]*b[5] + a[2]*b[9] + a[3]*b[13];
    float c2 = a[0]*b[2] + a[1]*b[6] + a[2]*b[10] + a[3]*b[14];
    float c3 = a[0]*b[3] + a[1]*b[7] + a[2]*b[11] + a[3]*b[15];

    float c4 = a[4]*b[0] + a[5]*b[4] + a[6]*b[8] + a[7]*b[12];
    float c5 = a[4]*b[1] + a[5]*b[5] + a[6]*b[9] + a[7]*b[13];
    float c6 = a[4]*b[2] + a[5]*b[6] + a[6]*b[10] + a[7]*b[14];
    float c7 = a[4]*b[3] + a[5]*b[7] + a[6]*b[11] + a[7]*b[15];

    float c8 = a[8]*b[0] + a[9]*b[4] + a[10]*b[8] + a[11]*b[12];
    float c9 = a[8]*b[1] + a[9]*b[5] + a[10]*b[9] + a[11]*b[13];
    float c10 = a[8]*b[2] + a[9]*b[6] + a[10]*b[10] + a[11]*b[14];
    float c11 = a[8]*b[3] + a[9]*b[7] + a[10]*b[11] + a[11]*b[15];

    float c12 = a[12]*b[0] + a[13]*b[4] + a[14]*b[8] + a[15]*b[12];
    float c13 = a[12]*b[1] + a[13]*b[5] + a[14]*b[9] + a[15]*b[13];
    float c14 = a[12]*b[2] + a[13]*b[6] + a[14]*b[10] + a[15]*b[14];
    float c15 = a[12]*b[3] + a[13]*b[7] + a[14]*b[11] + a[15]*b[15];

    return mat4(
            c0, c1, c2, c3,
            c4, c5, c6, c7,
            c8, c9, c10, c11,
            c12, c13, c14, c15
            );
}

void mat4::print() {
    printf("[%f, %f, %f, %f]\n", m[0], m[1], m[2], m[3]);
    printf("[%f, %f, %f, %f]\n", m[4], m[5], m[6], m[7]);
    printf("[%f, %f, %f, %f]\n", m[8], m[9], m[10], m[11]);
    printf("[%f, %f, %f, %f]\n", m[12], m[13], m[14], m[15]);
}

/*
 * Quaternions
 */
quaternion::quaternion() {
    w = 1;
    x = y = z = 0;
    make_unit_length();
}

quaternion::quaternion(float theta, const vec3 &rotation_axis) {
    w = cos(theta / 2.0);

    float s = sin(theta / 2.0);
    x = s * rotation_axis.x;
    y = s * rotation_axis.y;
    z = s * rotation_axis.z;

    make_unit_length();
}

mat4 quaternion::matrix() {
    float a = 1 - 2*y*y -2*z*z;
    float b = 2*x*y - 2*w*z;
    float c = 2*x*z + 2*w*y;

    float d = 2*x*y + 2*w*z;
    float e = 1 - 2*x*x - 2*z*z;
    float f = 2*y*z - 2*w*x;

    float g = 2*x*z - 2*w*y;
    float h = 2*y*z + 2*w*x;
    float i = 1 - 2*x*x - 2*y*y;

    return mat4(a, b, c, 0,
            d, e, f, 0,
            g, h, i, 0,
            0, 0, 0, 1);
}

float quaternion::length() {
    return sqrt(w*w + x*x + y*y + z*z);
}

void quaternion::make_unit_length() {
    float l = length();

    if (l == 0) {
        return;
    }

    w /= l;
    y /= l;
    x /= l;
    z /= l;
}

quaternion operator*(const quaternion &u, const quaternion &v) {
    quaternion q;
    q.w = u.w*v.w - u.x*v.x - u.y*v.y - u.z*v.z;
    q.x = u.w*v.x + u.x*v.w + u.y*v.z - u.z*v.y;
    q.y = u.w*v.y - u.x*v.z + u.y*v.w + u.z*v.x;
    q.z = u.w*v.z + u.x*v.y - u.y*v.x + u.z*v.w;
    q.make_unit_length();
    return q;
}
