#version 330 core

layout(location = 0) in vec3 aPos;   // 버텍스 위치
layout(location = 1) in vec3 aColor; // 버텍스 색상

out vec3 vertexColor; // 프래그먼트 셰이더로 전달할 색상

uniform mat4 transform;

void main()
{
    gl_Position = transform * vec4(aPos, 1.0);
    vertexColor = aColor; // 입력된 색상 정보를 그대로 전달
}