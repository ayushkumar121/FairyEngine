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

float metallic = 0.4;
float roughness = 0.6;
float ao = 1.0;

const float PI = 3.14159265359;

out vec4 FragColor;


float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness * roughness;
    float a2 = a * a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH * NdotH;

    float num = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return num / denom;
}
float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r * r) / 8.0;

    float num = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return num / denom;
}
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}
vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

void main()
{
    if (texture(albedoMap, FragCoord).a - 0.1 < 0) discard;

    vec3 albedo = vec3(matColor);

    if(hasAlbedo > 0)
        albedo = pow(texture(albedoMap, FragCoord).rgb, vec3(2.2));

    vec3 N = normalize(FragNormal);
    vec3 V = normalize(u_cameraPos - FragPos);

    vec3 F0 = vec3(0.04);
    F0 = mix(F0, albedo, metallic);

    vec3 Lo = vec3(0.0);

    // reflectance equation
    for (int i = 0; i < 2; i++)
    {
        vec3 L = normalize(u_lightDirs[i]);
        vec3 H = normalize(V + L);

        vec3 radiance = u_lightColors[i] * u_lightIntensity[i] * 1.0;

        float NDF = DistributionGGX(N, H, roughness);
        float G = GeometrySmith(N, V, L, roughness);
        vec3 F = fresnelSchlick(max(dot(H, V), 0.0), F0);

        vec3 kS = F;
        vec3 kD = vec3(1.0) - kS;
        kD *= 1.0 - metallic;

        vec3 numerator = NDF * G * F;
        float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0);
        vec3 specular = numerator / max(denominator, 0.001);

        // add to outgoing radiance Lo
        float NdotL = max(dot(N, L), 0.0);
        Lo += (kD * albedo / PI + specular) * radiance * NdotL;
    }
    // reflectance equation

    vec3 ambient = vec3(0.03) * albedo * ao;
    vec3 color = ambient + Lo;

    color = color / (color + vec3(1.0));
    color = pow(color, vec3(1.0 / 2.2));

    if(hasAlbedo > 0)
        FragColor = vec4(color, texture(albedoMap, FragCoord).a);
    else
        FragColor = vec4(color, 1.0);
};