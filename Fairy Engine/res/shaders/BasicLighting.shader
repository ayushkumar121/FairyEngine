#shader vertex
#version 430

layout(location = 0) in vec4 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texcoord;

// Uniforms
layout(location = 0) uniform mat4 projection;
layout(location = 1) uniform mat4 view;
layout(location = 2) uniform mat4 model;

out vec3 FragNormal;
out vec3 FragPos;
out vec2 FragCoord;


void main()
{
    FragNormal = mat3(transpose(inverse(model))) * normal;
    FragPos = vec3(model * position);
    FragCoord = texcoord;

    gl_Position = projection * view * model * position;
};

#shader fragment
#version 430

in vec3 FragNormal;
in vec3 FragPos;
in vec2 FragCoord;

// Uniforms
layout(location = 7) uniform vec3 u_cameraPos;
layout(location = 8) uniform vec3 u_lightDirs[2];
layout(location = 10) uniform vec3 u_lightColors[2];
layout(location = 12) uniform float u_lightIntensity[2];

// Material Properties
layout(location = 4) uniform vec3 matColor;
layout(location = 5) uniform int hasAlbedo;
layout(location = 6) uniform sampler2D albedoMap;

float shininess = 32;
float specularStrength = 0.5;
float ambientStrength = 0.1;

out vec4 FragColor;

void main()
{
    if (texture(albedoMap, FragCoord).a - 0.1 < 0) discard;

    vec3 albedo = vec3(matColor);

    if(hasAlbedo > 0)
        albedo = pow(texture(albedoMap, FragCoord).rgb, vec3(2.2));

    vec3 N = normalize(FragNormal);
    vec3 V = normalize(u_cameraPos - FragPos);

    vec3 diffuse = vec3(0.0);
    vec3 ambient = vec3(0.0);
    vec3 specular = vec3(0.0);

    for (int i = 0; i < 2; i++)
    {
        vec3 L = normalize(u_lightDirs[i]);
        vec3 H = normalize(V + L);

        ambient += ambientStrength * u_lightColors[i];

        float diff = max(dot(N, L), 0.0);
        diffuse += diff * u_lightColors[i] * u_lightIntensity[i];

        float spec = pow(max(dot(N, H), 0.0), shininess);
        specular = u_lightColors[i] * spec * specularStrength;
    }

    vec3 color = (ambient + diffuse + specular) * albedo;

    if(hasAlbedo > 0)
        FragColor = vec4(color, texture(albedoMap, FragCoord).a);
    else
        FragColor = vec4(color, 1.0);
};