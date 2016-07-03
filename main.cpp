#define STB_IMAGE_IMPLEMENTATION

#include <stdlib.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>

#include "util/util.h"
#include "maths/maths.h"
#include "objects/hilbert_curve.h"

void normalize_screen_positions(double &x, double &y) {
    x = -500 + x;
    y = -500 + y;
}

vec3 get_point_on_sphere(double x, double y) {
    vec3 o = vec3(x, -y, 2000);
    vec3 v = vec3(0, 0, -1);
    float r = 1000;

    float det = 4 * vec3::dot(o, v) * vec3::dot(o, v) - 4 * vec3::dot(v, v) * (vec3::dot(o, o) - r * r);
    if (det < 0) {
        return vec3(0, 0, 0);
    }

    float t = (-2 * vec3::dot(o, v) + sqrt(det)) / (2 * vec3::dot(v, v));

    return o + t * v;
}

int main() {
    GLFWwindow *window = util_init_gl_window("opengl", 1000, 1000);

    float azimuth = 0.0;
    float inclination = 0.0;

    vec3 cam_pos(0, 0, 10); 
    vec3 cam_targ(0, 0, 0);
    vec3 cam_up(0, 1, 0);
    mat4 view_mat = mat4::look_at(cam_pos, cam_targ, cam_up);
    mat4 proj_mat = mat4::perspective_projection(60, 1, 0.001, 20);

    hilbert_curve curve;
    glUseProgram(curve.shader);
    glUniformMatrix4fv(curve.view_mat_location, 1, GL_TRUE, view_mat.m);
    glUniformMatrix4fv(curve.proj_mat_location, 1, GL_TRUE, proj_mat.m);
    glUseProgram(0);

    double prev_xpos, prev_ypos;
    glfwGetCursorPos(window, &prev_xpos, &prev_ypos);
    normalize_screen_positions(prev_xpos, prev_ypos);

    while (!glfwWindowShouldClose(window)) {
        /* Frame Update */

        int right_key = glfwGetKey(window, GLFW_KEY_RIGHT);
        int left_key = glfwGetKey(window, GLFW_KEY_LEFT);
        int up_key = glfwGetKey(window, GLFW_KEY_UP);
        int down_key = glfwGetKey(window, GLFW_KEY_DOWN);

        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        normalize_screen_positions(xpos, ypos);

        int mouse_state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
        if (mouse_state == GLFW_PRESS) {
            double xpos_delta = xpos - prev_xpos;
            double ypos_delta = ypos - prev_ypos;
            double delta = fabs(sqrt(xpos_delta*xpos_delta + ypos_delta*ypos_delta));

            vec3 point = get_point_on_sphere(xpos, ypos);
            vec3 prev_point = get_point_on_sphere(prev_xpos, prev_ypos);

            vec3 rotation_axis = vec3::cross(point, prev_point);
            rotation_axis.make_unit_length();

            curve.rotation = quaternion(delta * 0.005, rotation_axis) * curve.rotation;
        }

        prev_xpos = xpos;
        prev_ypos = ypos;

        glUseProgram(curve.shader);
        glUniformMatrix4fv(curve.view_mat_location, 1, GL_TRUE, view_mat.m);
        glUseProgram(0);

        /* Drawing */

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        curve.draw();

        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
