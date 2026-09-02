#version 460 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec3 vNormal;
out vec3 vFragPos;
out vec2 vTexCoords;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;

void main() {
    gl_PointSize = 2;

    vec4 position = uModel * vec4(aPosition, 1.0f);
    vTexCoords = aTexCoords;
    vFragPos   = vec3(position);
    vNormal    = mat3(transpose(inverse(uModel))) * aNormal;
    gl_Position = uProjection * uView * position;
}
