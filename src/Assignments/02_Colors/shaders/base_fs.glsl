#version 410

layout(location=0) out vec4 vFragColor;

in vec4 vertex_color;

// out vec4 vFragColor;

void main() {
    // vFragColor = vec4(1.0, 0.0, 0.0, 1.0);
    vFragColor = vertex_color;
}
