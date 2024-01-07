#version 420

layout(location=0) in  vec4 a_vertex_position;
layout(location=5) in  vec4 a_vertex_color;
layout(location=3) in  vec2 a_vertex_tex0;

layout(std140, binding = 1) uniform Transformations {
    mat4 PVM;
};

layout(location=6) out vec4 vertex_color;
layout(location=7) out vec2 vertex_tex0;

void main() {
    vertex_color = a_vertex_color;
    gl_Position = PVM * a_vertex_position;
    vertex_tex0 = a_vertex_tex0;
}
