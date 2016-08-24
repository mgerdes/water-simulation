#define STB_IMAGE_IMPLEMENTATION

#include <stdlib.h>
#include <algorithm>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>

#include "util/util.h"
#include "maths/maths.h"
#include "objects/rectangle.h"
#include "objects/water_surface.h"
#include "objects/plane.h"
#include "shaders/shader.h"

int main() {
    GLFWwindow *window = util_init_gl_window("opengl", 1000, 1000);

    /*
     * Set up vectors used for the camera
     */
    vec3 cam_pos(5, 5, 5); 
    vec3 cam_targ(0, 0, -2);
    vec3 cam_up(0, 0, 1);

    /*
     * Set up the view and projection matrices
     */
    mat4 view_mat = mat4::look_at(cam_pos, cam_targ, cam_up);
    mat4 inv_view_mat = view_mat.inverse();

    mat4 proj_mat = mat4::perspective_projection(60, 1, 0.1, 100);
    mat4 inv_proj_mat = proj_mat.inverse();

    /*
     * Set up the shaders
     */
    graphics::shader surface_shader("shaders/surface.vert", "shaders/surface.frag");
    graphics::shader floor_shader("shaders/floor.vert", "shaders/floor.frag");
    graphics::shader rectangle_shader("shaders/rectangle.vert", "shaders/rectangle.frag");

    /*
     * Set up all the objects we draw
     */
    object3d::rectangle rectangle(vec3(0, 0, 0), vec3(0, 0, 1));
    object3d::water_surface water_surface;
    object3d::plane plane;

    /*
     * Set up the textures
     */
    GLuint floor_texture = util_create_texture("floor.jpg");
    GLuint sky_texture = util_create_texture("sky.jpg");

    /*
     * Initialize the shaders
     */
    glUseProgram(surface_shader.program);
    GLuint floor_texture_location = glGetUniformLocation(surface_shader.program, "floor_texture"); 
    glUniform1i(floor_texture_location, 0);

    GLuint sky_texture_location = glGetUniformLocation(surface_shader.program, "sky_texture"); 
    glUniform1i(sky_texture_location, 1);
    glUseProgram(0);

    float previous_time = glfwGetTime();

    bool is_drawing_continous = true; 
    bool is_c_key_down = false;

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        /*
         * Check if c key was pressed and invert whether we are drawing continuously
         */
        int c_key_state = glfwGetKey(window, GLFW_KEY_C);
        if (c_key_state == GLFW_PRESS && !is_c_key_down) {
            is_c_key_down = true;
            is_drawing_continous = !is_drawing_continous;
        }
        if (c_key_state == GLFW_RELEASE) {
            is_c_key_down = false;
        }

        /*
         * Figure out how long the last frame was.
         */
        float current_time = glfwGetTime();
        float elapsed_time = current_time - previous_time;
        previous_time = current_time;

        /*
         * Update the viewport (Really should not happen every frame, only if the width/height changes)
         */
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0, 0, width, height);
        proj_mat = mat4::perspective_projection(60, width / (float) height, 0.1, 100);
        inv_proj_mat = proj_mat.inverse();

        /*
         * Handle mouse clicks
         */
        {
            static bool is_mouse_down = false;

            double xpos, ypos;
            glfwGetCursorPos(window, &xpos, &ypos);

            vec3 mouse_world = inv_view_mat * inv_proj_mat * vec3((2.0 * xpos) / width - 1.0, 1.0 - (2.0 * ypos) / height, 1.0);
            mouse_world.make_unit_length();
            vec3 mouse_intersection = cam_pos + (-cam_pos.z / mouse_world.z) * mouse_world;

            int mouse_state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
            if (mouse_state == GLFW_PRESS && !is_mouse_down) {
                is_mouse_down = true;

                if (mouse_intersection.x > -3.0 &&
                        mouse_intersection.x < 3.0 &&
                        mouse_intersection.y > -3.0 &&
                        mouse_intersection.y < 3.0) {
                    int i = (mouse_intersection.x + 3.0) / 6.0 * water_surface.width;
                    int j = (mouse_intersection.y + 3.0) / 6.0 * water_surface.height;

                    if (i > 0 && j > 0 && i < water_surface.width - 1 && j < water_surface.height - 1) { 
                        water_surface.u[i][j] = 1.2;
                        water_surface.u[i-1][j-1] = 0.7;
                        water_surface.u[i-1][j] = 0.7;
                        water_surface.u[i-1][j+1] = 0.7;
                        water_surface.u[i+1][j-1] = 0.7;
                        water_surface.u[i+1][j] = 0.7;
                        water_surface.u[i+1][j+1] = 0.7;
                        water_surface.u[i][j+1] = 0.7;
                        water_surface.u[i][j-1] = 0.5;
                    }
                }
            }
            else if (mouse_state == GLFW_RELEASE && is_mouse_down) {
                is_mouse_down = false;
            }
        }

        /*
         * Draw the box which contains the water
         */
        {
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
        }

        water_surface.update(elapsed_time);

        /*
         * Draw a discrete representation of the water surface
         */
        if (!is_drawing_continous) {
            glUseProgram(rectangle_shader.program);

            for (int i = 0; i < water_surface.width; i++) {
                for (int j = 0 ; j < water_surface.height; j++) {
                    float x = 3 - 6 * (1 - (i / (float) water_surface.width));
                    float y = 3 - 6 * (1 - (j / (float) water_surface.height));

                    vec3 position(x, y, 0);
                    vec3 scale(1.0, 1.0, water_surface.u[i][j]);

                    mat4 model_mat = mat4::translation(position) * mat4::scale(scale);

                    glUniform3f(rectangle_shader.color_location, 0.527, 0.843, 0.898);
                    glUniformMatrix4fv(rectangle_shader.model_mat_location, 1, GL_TRUE, model_mat.m);
                    glUniformMatrix4fv(rectangle_shader.view_mat_location, 1, GL_TRUE, view_mat.m);
                    glUniformMatrix4fv(rectangle_shader.proj_mat_location, 1, GL_TRUE, proj_mat.m);

                    glBindVertexArray(rectangle.vao);
                    glDrawArrays(GL_TRIANGLES, 0, 36);
                    glBindVertexArray(0);
                }
            }

            glUseProgram(0);
        }


        /*
         * Draw a continous representation of the water surface
         */
        if (is_drawing_continous) {   
            glUseProgram(surface_shader.program);

            // Set up uniforms for water surface
            mat4 model_mat = mat4::identity();
            glUniformMatrix4fv(surface_shader.model_mat_location, 1, GL_TRUE, model_mat.m);
            glUniformMatrix4fv(surface_shader.view_mat_location, 1, GL_TRUE, view_mat.m);
            glUniformMatrix4fv(surface_shader.proj_mat_location, 1, GL_TRUE, proj_mat.m);
            glUniform3f(surface_shader.color_location, 0.527, 0.843, 0.898);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, floor_texture);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, sky_texture);

            // Draw the water surface
            glBindVertexArray(water_surface.vao);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, water_surface.elements_vbo);
            glDrawElements(GL_TRIANGLES, (water_surface.N - 1) * (water_surface.N - 1) * 2 * 3, GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);

            glUseProgram(0);
        }

        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
