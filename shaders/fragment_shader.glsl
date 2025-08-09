#version 330 core

out vec4 FragColor;
in vec3 vertexColor;
//uniform vec4 triangleColor;

void main()
{
    //FragColor = triangleColor;
    FragColor = vec4(vertexColor, 1.0);
} 