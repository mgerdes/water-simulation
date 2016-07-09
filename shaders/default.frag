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

    vec3 refraction = refract(normalize(vec3(5, 5, 5) - frag_position), normalize(frag_normal), 0.6);

    vec3 floor_hit_point = frag_position + refraction * ((-3 - frag_position.z) / refraction.z);
    vec2 texture_coord = ((floor_hit_point + vec3(3.0, 3.0, 0.0)) / 6.0).xy;

    if (texture_coord.x < 0.0 && texture_coord.y < 0.0) {
        if (abs(texture_coord.x) < abs(texture_coord.y)) {
            kd -= 0.1;
            floor_hit_point = frag_position + refraction * ((-3 - frag_position.y) / refraction.y);
            texture_coord = ((floor_hit_point + vec3(3.0, 0.0, 3.0)) / 6.0).xz;
        } else {
            kd -= 0.18;
            floor_hit_point = frag_position + refraction * ((-3 - frag_position.x) / refraction.x);
            texture_coord = ((floor_hit_point + vec3(0.0, 3.0, 3.0)) / 6.0).yz;
        }
    } else if (texture_coord.x < 0.0) {
        kd -= 0.18;
        floor_hit_point = frag_position + refraction * ((-3 - frag_position.x) / refraction.x);
        texture_coord = ((floor_hit_point + vec3(0.0, 3.0, 3.0)) / 6.0).yz;
    }
    else if (texture_coord.y < 0.0) {
        kd -= 0.1;
        floor_hit_point = frag_position + refraction * ((-3 - frag_position.y) / refraction.y);
        texture_coord = ((floor_hit_point + vec3(3.0, 0.0, 3.0)) / 6.0).xz;
    }

    vec3 texel = texture(floor_texture, texture_coord).xyz;
    frag_color = vec4(0.3 * vec3(0.527, 0.843, 0.898) + kd * texel, 1.0);
    //frag_color = vec4(texture_coord, 0.0, 1.0);
}
