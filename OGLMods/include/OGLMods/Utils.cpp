#include "oglpch.h"
#include "Utils.h"

Bored::OGL::Texture::Texture(std::shared_ptr<Bored::Render::Texture> tex)
  : Bored::Render::Texture(*tex)
{
  glGenTextures(1, &id);
  glBindTexture(GL_TEXTURE_2D, id);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  GLenum format = GL_RGBA;
  GLenum inFormat = GL_RGBA;
  
  if (bpp != 4)
  {
    logger->warn("Bored has yet support bpp other than 4. Texture '"
      + name + "' might not display correctly!");
  }
 
  glTexImage2D(GL_TEXTURE_2D, 0, inFormat, width, height, 0, 
    format, GL_UNSIGNED_BYTE, data);

  glBindTexture(GL_TEXTURE_2D, 0);
  isInit = true;
}

void Bored::OGL::Texture::Bind(Shader& shader, const std::string& uniName, unsigned int texSlot) const
{
  shader.Activate();
  glActiveTexture(GL_TEXTURE0 + texSlot);
  glBindTexture(GL_TEXTURE_2D, id);
  shader.SetUniform1i(uniName, texSlot);
}

void Bored::OGL::Texture::Delete()
{
  glDeleteTextures(1, &id);
  isInit = false;
}

Bored::OGL::Material::Material(std::shared_ptr<Bored::Render::Material> mat)
  : Bored::Render::Material{mat->name, mat->opacity}, isInit(true) {
  // Create textures in opengl
  diffuse = std::make_shared<Bored::OGL::Texture>(mat->diffuse);
  specular = std::make_shared<Bored::OGL::Texture>(mat->specular);
}

Bored::OGL::Model::Model(Bored::Render::Model& mod)
{
  renderables.assign(mod.renderables.size(), {nullptr, nullptr});
  for (int i = 0; i < mod.renderables.size(); i++)
  {
    renderables[i].first = mod.renderables[i].first;
    renderables[i].second = std::make_shared<Bored::OGL::Material>(mod.renderables[i].second);
  }
}
