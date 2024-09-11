#version 330 core

in vec3 vertexColor; // 버텍스 셰이더에서 전달된 색상

out vec4 FragColor;

void main()
{
    FragColor = vec4(vertexColor, 1.0); // 그리드 색상 적용
}