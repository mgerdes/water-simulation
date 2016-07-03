#version 330

in vec3 vertex_position;
in vec3 vertex_normal;

out vec3 frag_position;
out vec3 frag_normal;

uniform mat4 model_mat;
uniform mat4 view_mat;
uniform mat4 proj_mat;

void main () {
   frag_position = (model_mat * vec4(vertex_position, 1.0)).xyz;
   frag_normal = vertex_normal;
   gl_Position = proj_mat * view_mat * model_mat * vec4(vertex_position, 1.0);
}
