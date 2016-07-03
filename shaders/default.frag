#version 330

in vec3 frag_position;
in vec3 frag_normal;

out vec4 frag_color;

void main () {
   vec3 light_pos = vec3(1.0, 2.0, 3.0);
   float kd = max(dot(normalize(light_pos), abs(frag_normal)), 0);
   frag_color = kd * vec4(0.0, 0.0, 1.0, 1.0);
}
