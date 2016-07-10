#define STB_IMAGE_IMPLEMENTATION

#include <stdlib.h>
#include <algorithm>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>

#include "util/util.h"
#include "maths/maths.h"
#include "objects/rectangle.h"
#include "objects/water.h"
#include "objects/bezier_surface.h"
#include "objects/plane.h"
#include "shaders/shader.h"

int main() {
    GLFWwindow *window = util_init_gl_window("opengl", 1000, 1000);

    vec3 cam_pos(5, 5, 5); 
    vec3 cam_targ(0, 0, -2);
    vec3 cam_up(0, 0, 1);

    mat4 view_mat = mat4::look_at(cam_pos, cam_targ, cam_up);
    mat4 inv_view_mat = view_mat.inverse();

    mat4 proj_mat = mat4::perspective_projection(60, 1, 0.1, 100);
    mat4 inv_proj_mat = proj_mat.inverse();

    graphics::shader shader("shaders/default.vert", "shaders/default.frag");
    graphics::shader floor_shader("shaders/floor.vert", "shaders/floor.frag");

    object3d::rectangle rectangle(vec3(0, 0, 0), vec3(0, 0, 1));
    object3d::water water;
    object3d::bezier_surface surface;
    object3d::plane plane;

    GLuint floor_texture = util_create_texture("floor.jpg");
    GLuint sky_texture = util_create_texture("sky.jpg");
    
    GLuint floor_texture_location = glGetUniformLocation(shader.program, "floor_texture"); 
    GLuint sky_texture_location = glGetUniformLocation(shader.program, "sky_texture"); 
    glUseProgram(shader.program);
    glUniform1i(floor_texture_location, 0);
    glUniform1i(sky_texture_location, 1);
    glUseProgram(0);

    float previous_time = glfwGetTime();

    while (!glfwWindowShouldClose(window)) {
        // Handle mouse clicks
        {
            static bool is_mouse_down = false;

            double xpos, ypos;
            glfwGetCursorPos(window, &xpos, &ypos);

            vec3 mouse_world = inv_view_mat * inv_proj_mat * vec3((2.0 * xpos) / 1000.0 - 1.0, 1.0 - (2.0 * ypos) / 1000.0, 1.0);
            mouse_world.make_unit_length();
            vec3 mouse_intersection = cam_pos + (-cam_pos.z / mouse_world.z) * mouse_world;

            int mouse_state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
            if (mouse_state == GLFW_PRESS && !is_mouse_down) {
                is_mouse_down = true;

                if (mouse_intersection.x > -3.0 &&
                        mouse_intersection.x < 3.0 &&
                        mouse_intersection.y > -3.0 &&
                        mouse_intersection.y < 3.0) {
                    int i = (-mouse_intersection.x + 3.0) / 6.0 * water.width;
                    int j = (-mouse_intersection.y + 3.0) / 6.0 * water.height;

                    water.u[i][j] = 1.0;
                    water.u[i-1][j-1] = 0.5;
                    water.u[i-1][j] = 0.5;
                    water.u[i-1][j+1] = 0.5;
                    water.u[i+1][j-1] = 0.5;
                    water.u[i+1][j] = 0.5;
                    water.u[i+1][j+1] = 0.5;
                    water.u[i][j+1] = 0.5;
                    water.u[i][j-1] = 0.5;
                }
            }
            else if (mouse_state == GLFW_RELEASE && is_mouse_down) {
                is_mouse_down = false;
            }
        }
        

        float current_time = glfwGetTime();
        float elapsed_time = current_time - previous_time;
        previous_time = current_time;
        //printf("%f\n", elapsed_time);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(floor_shader.program);

        // Set up uniforms for floor of water
        mat4 model_mat = mat4::translation(vec3(0, 0, -3)) * mat4::scale(vec3(3, 3, 1)) * mat4::rotation_z(M_PI / 2.0);
        glUniformMatrix4fv(floor_shader.view_mat_location, 1, GL_TRUE, view_mat.m);
        glUniformMatrix4fv(floor_shader.proj_mat_location, 1, GL_TRUE, proj_mat.m);
        glUniformMatrix4fv(floor_shader.model_mat_location, 1, GL_TRUE, model_mat.m);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, floor_texture);

        // Draw floor of water
        glBindVertexArray(plane.vao);
        glDrawArrays(GL_TRIANGLES, 0, 18);
        glBindVertexArray(0);

        // Set up uniforms for side 1 of water
        model_mat = mat4::translation(vec3(0, -3, -1)) * mat4::scale(vec3(3, 1, 2)) * mat4::rotation_x(M_PI / 2.0);
        glUniformMatrix4fv(floor_shader.model_mat_location, 1, GL_TRUE, model_mat.m);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, floor_texture);

        // Draw side 1 of water
        glBindVertexArray(plane.vao);
        glDrawArrays(GL_TRIANGLES, 0, 18);
        glBindVertexArray(0);

        // Set up uniforms for side 2 of water
        model_mat = mat4::translation(vec3(-3, 0, -1)) * mat4::scale(vec3(1, 3, 2)) * mat4::rotation_y(M_PI / 2.0);
        glUniformMatrix4fv(floor_shader.model_mat_location, 1, GL_TRUE, model_mat.m);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, floor_texture);

        // Draw side 2 of water
        glBindVertexArray(plane.vao);
        glDrawArrays(GL_TRIANGLES, 0, 18);
        glBindVertexArray(0);

        glUseProgram(0);


        // Draw the rectangles
        glUseProgram(shader.program);

        water.update(1/60.0);

        /*
         * Discrete Drawing
         */

        /*
           for (int i = 0; i < water.width; i++) {
           for (int j = 0 ; j < water.height; j++) {
           float x = -3 + 6 * (1 - (i / (float) water.width));
           float y = -3 + 6 * (1 - (j / (float) water.height));

           vec3 position(x, y, 0);
           vec3 scale(1.0, 1.0, water.u[i][j]);

           mat4 model_mat = mat4::translation(position) * mat4::scale(scale);

           glUniform3f(shader.color_location, 0.527, 0.843, 0.898);
           glUniformMatrix4fv(shader.model_mat_location, 1, GL_TRUE, model_mat.m);
           glUniformMatrix4fv(shader.view_mat_location, 1, GL_TRUE, view_mat.m);
           glUniformMatrix4fv(shader.proj_mat_location, 1, GL_TRUE, proj_mat.m);

           glBindVertexArray(rectangle.vao);
           glDrawArrays(GL_TRIANGLES, 0, 36);
           glBindVertexArray(0);
           }
           }
           */

        /*
         * Continuous Drawing
         */
        for (int i = 0; i < water.width / 3 - 1; i++) {
            for (int j = 0; j < water.height / 3 - 1; j++) {
                vec3 control_points[4][4];

                for (int k = 0; k < 4; k++) {
                    for (int l = 0; l < 4; l++) {
                        float x = -3 + 6 * (1 - ((3 * i + k) / (float) water.width));
                        float y = -3 + 6 * (1 - ((3 * j + l) / (float) water.height));

                        control_points[k][l].set(x, y, water.control_point_heights[3 * i + k][3 * j + l]);
                    }
                }

                surface.update_control_points(control_points);

                // Set up uniforms for water surface
                mat4 model_mat = mat4::identity();
                glUniformMatrix4fv(shader.model_mat_location, 1, GL_TRUE, model_mat.m);
                glUniformMatrix4fv(shader.view_mat_location, 1, GL_TRUE, view_mat.m);
                glUniformMatrix4fv(shader.proj_mat_location, 1, GL_TRUE, proj_mat.m);
                glUniform3f(shader.color_location, 0.527, 0.843, 0.898);
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, floor_texture);
                glActiveTexture(GL_TEXTURE1);
                glBindTexture(GL_TEXTURE_2D, sky_texture);

                // Draw the surface
                glBindVertexArray(surface.vao);
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, surface.elements_vbo);
                glDrawElements(GL_TRIANGLES, (surface.N - 1) * (surface.N - 1) * 2 * 3, GL_UNSIGNED_INT, 0);
                glBindVertexArray(0);
            }
        }

        glUseProgram(0);

        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
