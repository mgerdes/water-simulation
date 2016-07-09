#version 330

in vec3 frag_position;
in vec3 frag_normal;
in vec4 vert_color;

out vec4 frag_color;

uniform sampler2D floor_texture;

void main () {
    vec3 light_pos_1 = vec3(1.0, 2.0, 3.0);
    vec3 light_pos_2 = vec3(-1.0, -2.0, -3.0);

    float kd = 0.4 * max(dot(normalize(light_pos_1), abs(frag_normal)), 0);
    kd += 0.4 * max(dot(normalize(light_pos_2), abs(frag_normal)), 0);

    vec3 refraction = refract(normalize(-vec3(5, 5, 5)), normalize(frag_normal), 1.0 / 1.333);

    vec3 floor_hit_point = frag_position + refraction * ((-3 - frag_position.z) / refraction.z);
    floor_hit_point = (floor_hit_point + vec3(3.0, 3.0, 0.0)) / 6.0;

    vec3 texel = texture(floor_texture, floor_hit_point.xy).xyz;

    frag_color = kd * vec4(0.527, 0.843, 0.898, 1.0) + 0.2 * vec4(1.0, 1.0, 1.0, 1.0);
    frag_color = vec4(abs(frag_normal), 1.0);
    frag_color = vert_color;

    if (abs(refraction.x) < 0.1 && abs(refraction.y) < 0.1) {
        frag_color = vec4(0.0, 0.0, 1.0, 1.0);
    } else if (floor_hit_point.x > 3.0 || floor_hit_point.x < -3.0 || floor_hit_point.y > 3.0 || floor_hit_point.y < -3.0) {
        frag_color = vec4(1.0, 0.0, 0.0, 1.0);
    }
    else {
        frag_color = vec4(0.0, 1.0, 0.0, 1.0);
    }

    frag_color = vec4(0.3 * vec3(0.527, 0.843, 0.898) + kd * texel, 1.0);
}
