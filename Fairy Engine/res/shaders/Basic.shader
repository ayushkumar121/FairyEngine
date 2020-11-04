#shader vertex
#version 430

layout(location = 0) in vec4 position;
//layout(location = 1) in vec2 texCoord;

out vec2 v_TexCoord;

layout(location = 0) uniform mat4 u_MVP;

void main()
{
    gl_Position = u_MVP * position;
  //  v_TexCoord = texCoord;
};

#shader fragment
#version 430

//layout(location = 1) uniform sampler2D u_Texture;

//in vec2 v_TexCoord;
out vec4 color;

void main()
{
  //  vec4 texColor = texture(u_Texture, v_TexCoord);
    color = vec4(0.3, 0.4, 1.0, 1.0);
};