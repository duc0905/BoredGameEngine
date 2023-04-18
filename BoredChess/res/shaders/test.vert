#version 330 core

/* ===== Structs ===== */
struct MVPMats
{
    mat4 Model;
    mat4 View;
    mat4 Proj;
};

/* ===== Inputs ===== */
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aUV;
layout(location = 2) in vec3 aNorm;
uniform MVPMats mvp = MVPMats(mat4(1.0), mat4(1.0), mat4(1.0));

/* ===== Outputs ===== */
out vec3 vPos;
out vec2 vUV;
out vec3 vNorm;
out mat4 vMVP;
out MVPMats vMVPMats;

/* ===== Functions ===== */

void main()
{

  vPos = aPos;
  vUV = aUV;
  vNorm = aNorm;
  vMVP = mvp.Proj * mvp.View * mvp.Model;
  vMVPMats = mvp;
	gl_Position = vMVP * vec4(aPos, 1.0);
}
