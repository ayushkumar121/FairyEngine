#shader vertex
#version 430

layout(location = 0) in vec3 position;

// Uniforms
layout(location = 0) uniform mat4 projection;
layout(location = 1) uniform mat4 view;

out vec3 TexCoords;

void main()
{
    TexCoords = position;
    vec4 pos = projection * view * vec4(position, 1.0);
    gl_Position = pos.xyww;
};

#shader fragment
#version 430

in vec3 TexCoords;

// Uniforms
layout(location = 3) uniform samplerCube skybox;

out vec4 FragColor;

void main()
{
    FragColor = texture(skybox, TexCoords);
};