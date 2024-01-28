#version 420

layout(location=0) out vec4 vFragColor;

layout(std140, binding=0) uniform Material {
    vec4 Ka;
    vec4 Kd;
    bool use_vertex_color;
    bool use_map_Kd;
};

uniform sampler2D map_Kd;

const int MAX_POINT_LIGHTS = 16;

struct PointLight {
    vec3 position;
    float radius;
    vec3 color;
    float intensity;
};

layout (std140, binding = 2) uniform Lights {
    vec3 ambient;
    int n_lights;
    PointLight lights[MAX_POINT_LIGHTS];
};

layout(location=6) in vec4 vertex_color;
layout(location=7) in vec2 vertex_texcoord_0;
layout(location=1) in vec3 vertex_normal_vs;
layout(location=0) in vec3 vertex_position_vs;

vec3 srgb_gamma_correction(vec3 color) {
    color = clamp(color, 0.0, 1.0);
    color = mix(color * 12.92, (1.055 * pow(color, vec3(1.0 / 2.4))) - 0.055, step(0.0031308, color));
    return color;
}

void main() {
    
    vec4 color = vec4(1.0f, 1.0f, 1.0f, 1.0f);
    vec3 normal = normalize(vertex_normal_vs);
    float INV_PI = 1.0/3.1415;

    if (use_vertex_color) {
        color = vertex_color * Kd;
    } else {
        color = Kd;
    }
    if (use_map_Kd) {
        vec4 texture_color = texture(map_Kd, vertex_texcoord_0);
        color *= texture_color;
    }

    // vFragColor = color;
    vFragColor.a = color.a;
    // vFragColor.rgb = srgb_gamma_correction(color.rgb);
    vFragColor.rgb = srgb_gamma_correction(normal.rgb);
    // ???
    // vFragColor.rgb = abs(vertex_position_vs);

    // vFragColor.rgb = abs(vertex_normal_vs);

    vFragColor.rgb = Ka.rgb * ambient;

    for (int i = 0; i < n_lights; i++) {
        float light_distance = distance(lights[i].position, vertex_position_vs);
        float r = max(lights[i].radius, light_distance);
        float attenuation = 1.0 / (r * r);
        // kolorki......
        // vFragColor.rgb += INV_PI * color.rgb * lights[i].color * lights[i].intensity * attenuation * normal;
        // vFragColor.rgb += INV_PI * color.rgb * lights[i].color * lights[i].intensity * attenuation;
        if (gl_FrontFacing) vFragColor.rgb += 1.0/3.1415 * color.rgb * lights[i].color * lights[i].intensity * attenuation;
    }
}
