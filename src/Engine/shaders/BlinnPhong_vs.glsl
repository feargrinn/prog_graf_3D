#version 420

layout(location=0) in vec4 a_vertex_position;
layout(location=5) in vec4 a_vertex_color;
layout(location=3) in vec2 a_vertex_texcoord_0;
layout(location=1) in vec3 a_vertex_normal;

layout(std140, binding = 1) uniform Transformations {
    mat4 PVM;
    mat4 VM;
    mat3 VM_Normal;
};

layout(location=6) out vec4 vertex_color;
layout(location=7) out vec2 vertex_texcoord_0;
layout(location=1) out vec3 vertex_normal_vs;
layout(location=0) out vec3 vertex_position_vs;

void main() {
    vertex_color = a_vertex_color;
    gl_Position = PVM * a_vertex_position;
    vertex_texcoord_0 = a_vertex_texcoord_0;
    vertex_normal_vs = a_vertex_normal;
    
    vec4 a_vertex_position_vs = VM * a_vertex_position;
    vertex_position_vs = a_vertex_position_vs.xyz/a_vertex_position_vs.w;
    // vertex_position_vs = (a_vertex_position * VM).xyz;

    vertex_normal_vs = normalize(VM_Normal * a_vertex_normal);
}
