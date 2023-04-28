#version 330 core

/* ===== Structs ===== */
struct MVPMats
{
    mat4 Model;
    mat4 View;
    mat4 Proj;
};

struct Material
{
    float opacity;
    sampler2D diffuse;
    sampler2D specular;
};

struct Light
{
    vec3 pos;
    vec3 color;
    float ambient, diffuse, specular;
};

struct Camera
{
    vec3 pos;
    vec3 dir;
    vec3 up;
};

/* ===== Inputs ===== */
in vec3 vPos;
in vec2 vUV;
in vec3 vNorm;

in MVPMats vMVPMats;
uniform Material mat;
uniform Light light;
uniform Camera cam;
uniform int actorID;

/* ===== Outputs ===== */
layout(location = 0) out vec4 glColor;
layout(location = 1) out int ID;

/* ===== Functions ===== */
vec3 GetAmbient(Material m, Light l, vec2 uv)
{
  return l.color * l.ambient;
}

vec3 GetDiffuse(Material m, Light l, vec3 pos, vec2 uv, vec3 norm)
{
  if (l.diffuse <= 0.0)
    return vec3(0.0);
  vec3 lightDir = normalize(l.pos - pos);
  float diff = max(dot(norm, lightDir), 0.0);
  vec4 texDiff = texture(m.diffuse, uv);
  return l.diffuse * l.color * diff * texDiff.xyz;
}

vec3 GetSpecular(Material m, Camera c, Light l, vec3 pos, vec2 uv, vec3 norm)
{
  if (l.specular <= 0.0)
    return vec3(0.0);

  vec3 viewDir = normalize(c.pos - pos);
  vec3 lightDir = l.pos - pos;
  vec3 reflectDir = normalize(reflect(-lightDir, norm));
  int shiny = int(texture(m.specular, uv).r * 255);
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), shiny);
  return l.specular * spec * l.color;
}

vec4 GetPhongModel(Material m, Camera c, Light l, vec3 pos, vec2 uv, vec3 norm)
{
  vec3 lightDir = l.pos - pos;
  float len = length(lightDir);
  return vec4(GetAmbient(m, l, uv)
    + GetDiffuse(m, l, pos, uv, norm)
    + GetSpecular(m, c ,l, pos, uv, norm), 0.0)
    + vec4(0.0, 0.0, 0.0, m.opacity);
}

void main()
{
  vec4 ans = vec4(1.0);
  vec4 FragPos = vMVPMats.Model * vec4(vPos, 1.0);
  ans *= GetPhongModel(mat, cam, light, FragPos.xyz, vUV, vNorm);
  glColor = ans;
  ID = actorID;
}