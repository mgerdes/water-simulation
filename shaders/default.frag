#version 330

in vec3 frag_position;
in vec3 frag_normal;
in vec4 vert_color;

out vec4 frag_color;

uniform sampler2D floor_texture;
uniform sampler2D sky_texture;

void main () {
    vec3 light_pos_1 = vec3(1.0, 2.0, 3.0);
    vec3 light_pos_2 = vec3(-1.0, -2.0, -3.0);

    vec3 normal = frag_normal;

    float kd = 0.3 * max(dot(normalize(light_pos_1), abs(normal)), 0);
    kd += 0.3 * max(dot(normalize(light_pos_2), abs(normal)), 0);

    vec3 incident = normalize(vec3(5, 5, 5) - frag_position);
    vec3 refraction = refract(incident, normal, 0.6);

    vec3 floor_hit_point = frag_position + refraction * ((-3 - frag_position.z) / refraction.z);
    vec2 floor_texture_coord = ((floor_hit_point + vec3(3.0, 3.0, 0.0)) / 6.0).xy;

    float floor_kd = kd;

    if (floor_texture_coord.x < 0.0 && floor_texture_coord.y < 0.0) {
        if (abs(floor_texture_coord.x) < abs(floor_texture_coord.y)) {
            floor_kd -= 0.10;
            floor_hit_point = frag_position + refraction * ((-3 - frag_position.y) / refraction.y);
            floor_texture_coord = ((floor_hit_point + vec3(3.0, 0.0, 3.0)) / 6.0).xz;
        } else {
            floor_kd -= 0.15;
            floor_hit_point = frag_position + refraction * ((-3 - frag_position.x) / refraction.x);
            floor_texture_coord = ((floor_hit_point + vec3(0.0, 3.0, 3.0)) / 6.0).yz;
        }
    } else if (floor_texture_coord.x < 0.0) {
        floor_kd -= 0.15;
        floor_hit_point = frag_position + refraction * ((-3 - frag_position.x) / refraction.x);
        floor_texture_coord = ((floor_hit_point + vec3(0.0, 3.0, 3.0)) / 6.0).yz;
    }
    else if (floor_texture_coord.y < 0.0) {
        floor_kd -= 0.10;
        floor_hit_point = frag_position + refraction * ((-3 - frag_position.y) / refraction.y);
        floor_texture_coord = ((floor_hit_point + vec3(3.0, 0.0, 3.0)) / 6.0).xz;
    }

    vec3 reflection = reflect(incident, normal);

    float sky_kd = kd;

    vec3 sky_hit_point = frag_position + reflection * ((3 - frag_position.z) / reflection.z);
    vec2 sky_texture_coord = ((sky_hit_point + vec3(3.0, 3.0, 0.0)) / 6.0).xy;

    vec3 floor_texel = texture(floor_texture, floor_texture_coord).xyz;
    vec3 sky_texel = texture(sky_texture, sky_texture_coord).xyz;
    frag_color = vec4(kd * vec3(0.327, 0.543, 0.998) + floor_kd * 0.5 * floor_texel + sky_kd * 0.5 * sky_texel, 1.0);
}
