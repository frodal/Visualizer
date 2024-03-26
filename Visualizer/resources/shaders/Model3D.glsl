#shader vertex
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec3 tangent;
layout(location = 3) in vec2 texCoord;

out vec3 v_Normal;
out vec2 v_TexCoord;
out mat3 v_TBN;

uniform mat4 u_MVP;
uniform mat4 u_NormalMatrix;
uniform mat4 u_ModelMatrix;

void main()
{
	gl_Position = u_MVP * vec4(position, 1.0);
    v_Normal = mat3(u_NormalMatrix) * normal;
	v_TexCoord = texCoord;

    // Calculate TBN matrix for normal mapping
    vec3 T = normalize(vec3(u_ModelMatrix * vec4(tangent, 0.0)));
    vec3 N = normalize(vec3(u_ModelMatrix * vec4(normal, 0.0)));
    T = normalize(T - dot(T, N) * N); // re-orthogonalize T with respect to N
    vec3 B = cross(N, T);
    v_TBN = mat3(T, B, N);
};

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec3 v_Normal;
in vec2 v_TexCoord;
in mat3 v_TBN;

uniform vec4 u_Color;
uniform sampler2D u_texture_diffuse1;
uniform int u_flag_texture_diffuse1 = 0;
uniform sampler2D u_texture_specular1;
uniform int u_flag_texture_specular1 = 0;
uniform sampler2D u_texture_normal1;
uniform int u_flag_texture_normal1 = 0;

void main()
{
    // Set up lighting and material parameters
    const float ambientStrength = 0.1;
    const float diffuseStrength = 0.8;
    const float specularStrength = 0.5;
    const float shininess = 32.0;
    const float exposure = 1.5;
    const float gamma = 2.2;
    const vec3 lightColor = vec3(1.0);
    const vec3 lightDir = normalize(vec3(-1.0, 1.0, 1.0));
    const vec3 viewDir = normalize(vec3(0.0, 0.0, 1.0));

    // Set up default colors and normal from model
    vec4 diffuseColor = vec4(1.0);
    vec4 specularColor = vec4(1.0);
    vec3 norm = normalize(v_Normal);
    if (u_flag_texture_diffuse1 == 1)
        diffuseColor = texture(u_texture_diffuse1, v_TexCoord);
    if (u_flag_texture_specular1 == 1)
        specularColor = texture(u_texture_specular1, v_TexCoord);
    if (u_flag_texture_normal1 == 1)
    {
        norm = texture(u_texture_normal1, v_TexCoord).rgb;
        norm = norm * 2.0 - 1.0;
        norm = normalize(v_TBN * norm);
    }

    // Diffuse shading (Lambert)
    float diff = max(dot(norm, lightDir), 0.0);

    // Specular shading (Blinn-Phong)
    vec3 halfWayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(norm, halfWayDir), 0.0), shininess);
    
    // Combine results
    vec4 ambient = vec4(ambientStrength * lightColor, 1.0) * diffuseColor;
	vec4 diffuse = vec4(diffuseStrength * diff * diffuseColor.rgb, diffuseColor.a);
    vec4 specular = vec4(specularStrength * spec * specularColor.rgb, specularColor.a);

    color = (ambient + diffuse + specular) * u_Color;

    // tone (HDR) mapping
    vec3 hdrColor = color.rgb;
    hdrColor = vec3(1.0) - exp(-hdrColor * exposure);

    // gamma correction
    hdrColor = pow(hdrColor, vec3(1.0 / gamma));

    color.rgb = hdrColor;
};