#pragma once
#include "../../oglpch.h"
#include "ShaderClass.h"
#include "Utils.h"

namespace Bored {
namespace OGL {
class Renderer : public Bored::Renderer {
 public:
  GLFWwindow* window;
  GLuint mVao, mPosVbo, mUVsVbo, mNormVbo, mIbo;
  GLuint mFbo, mFrameTex, mIDTex;
  GLuint mScreenVao, mScreenVbo, mScreenIbo;

  Shader screenShader;
  Shader testShader;

  Texture white, red, mixed, img;
  std::shared_ptr<Bored::Actor> light;
public:
  Renderer();

  // Inherited via Renderer
  virtual void OnSetup() override;
  virtual bool OnTick(double dt) override;
  virtual void OnImGui() override;
  virtual void OnStop() override;
  virtual std::shared_ptr<Bored::Actor> GetActorAt(unsigned int x, unsigned y) override;
private:
  void Render(const Bored::OGL::Model& m, Shader& shader);
  void Render(Bored::Render::Mesh&, Bored::OGL::Material&, Shader& shader);
  static void window_size_callback(GLFWwindow* window, int w, int h);
  static void framebuffer_size_callback(GLFWwindow* window, int w, int h);
};

class Input : public Bored::Input {
public:
	GLFWwindow* window;

	// Inherited via Input
	virtual void OnSetup() override;
	virtual bool OnTick(double dt) override;

private:
	static double mouse_X;
	static double mouse_Y;

	static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void MousePosCallback(GLFWwindow* window, double x, double y);
	static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	static void MouseEnterCallback(GLFWwindow* window, int entered);
	static void MouseScrollCallback(GLFWwindow* window, double x, double y);

	void SetupCallbacks();
public:
	virtual void SetCursorImage(unsigned char* image, unsigned int width, unsigned int height);
	virtual void EnableCursor();
	virtual void DisableCursor();
	virtual Input::Key GetKey(int keyCode) override;
	virtual int GetMods(int modBits) override;
	virtual Input::Action GetAction(int actionCode) override;

  // Inherited via Input
  virtual std::shared_ptr<Bored::Actor> GetCursorHoveringActor() override;
};
};  // namespace OGL
}  // namespace Bored
