#version 330

in vec3 frag_position;
in vec3 frag_normal;
in vec4 vert_color;

out vec4 frag_color;

void main () {
   vec3 light_pos_1 = vec3(1.0, 2.0, 3.0);
   vec3 light_pos_2 = vec3(-1.0, -2.0, -3.0);

   float kd = 0.4 * max(dot(normalize(light_pos_1), abs(frag_normal)), 0);
   kd += 0.4 * max(dot(normalize(light_pos_2), abs(frag_normal)), 0);

   frag_color = kd * vec4(0.527, 0.843, 0.898, 1.0) + 0.2 * vec4(1.0, 1.0, 1.0, 1.0);
   //frag_color = vec4(abs(frag_normal), 1.0);
   frag_color = vert_color;
}
