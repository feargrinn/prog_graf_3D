#version 420

layout(location=0) in  vec4 a_vertex_position;
layout(location=5) in  vec4 a_vertex_color;

layout(std140, binding = 1) uniform Transformations {
    mat4 PVM;
};

out  vec4 vertex_color;

void main() {
    vertex_color = a_vertex_color;
    gl_Position = PVM * a_vertex_position;
}
