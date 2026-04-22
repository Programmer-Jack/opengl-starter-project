#version 460 core
out vec4 FragColor;

// input from vertex shader
in vec3 color;
in vec2 texCoord;

uniform sampler2D tex0;
uniform float time;
uniform float deltaTime;

void main()
{
    vec3 colorTest = 0.6 + 0.5 * cos(time / 1000.0 + texCoord.xyx + vec3(0, 2, 4));
    FragColor = texture(tex0, texCoord) * vec4(colorTest, 1.0);
}