// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// clang-format on
#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <Components/InputComponent.hpp>
#include <Scene/Scene.hpp>
#include <Systems/Input/InputSystem.hpp>
#include <functional>
#include <memory>
#include <stdexcept>

#include "gmock/gmock.h"

class MockHandler {
 public:
  MOCK_METHOD(void, OnMouseMove, (Bored::InputEvent&), ());
  MOCK_METHOD(void, OnKeyUp, (Bored::InputEvent&), ());
  MOCK_METHOD(void, OnKeyDown, (Bored::InputEvent&), ());
  MOCK_METHOD(void, OnKeyRepeat, (Bored::InputEvent&), ());
};

class InputSystemTest : public testing::Test {
 protected:
  static void SetUpTestSuite() {
    glfwInit();
    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
    m_window = glfwCreateWindow(1, 1, "", nullptr, nullptr);
    ASSERT_NE(m_window, nullptr);
    glfwMakeContextCurrent(m_window);
    ASSERT_TRUE(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress));
  }

  static void TearDownTestSuite() {
    glfwDestroyWindow(m_window);
    glfwTerminate();
  }

  void SetUp() override {
    m_io = std::make_unique<Bored::IOService>();
    m_input = std::make_unique<Bored::InputSystem>(*m_io);

    // Create 2 input contexts
    auto& ctx1 = m_input->CreateContext().second;

    ctx1.RegisterInput(Bored::InputType::MOUSE_MOVE, "Mouse move");
    ctx1.RegisterInput(Bored::InputType::KEY_DOWN, "Key down");
    ctx1.RegisterInput(Bored::InputType::KEY_UP, "Key up");
    ctx1.RegisterInput(Bored::InputType::KEY_REPEAT, "Key repeat");

    ctx1.RegisterHandler("Mouse move", [this](Bored::InputEvent& event) {
      mock.OnMouseMove(event);
    });

    ctx1.RegisterHandler(
        "Key up", [this](Bored::InputEvent& event) { mock.OnKeyUp(event); });
    ctx1.RegisterHandler("Key down", [this](Bored::InputEvent& event) {
      mock.OnKeyDown(event);
    });
    ctx1.RegisterHandler("Key repeat", [this](Bored::InputEvent& event) {
      mock.OnKeyRepeat(event);
    });

    auto& ctx2 = m_input->CreateContext().second;
  }

  void TearDown() override {
    m_input.reset();
    m_io.reset();
  }

  std::shared_ptr<Bored::Scene> MakeScene() {
    auto scene = std::make_shared<Bored::Scene>();
    auto root = scene->CreateNode();
    scene->SetRoot(root);
    auto node = scene->CreateNode();
    root->AddChild(node);

    return scene;
  }

  static GLFWwindow* m_window;
  std::unique_ptr<Bored::IOService> m_io;
  std::unique_ptr<Bored::InputSystem> m_input;
  MockHandler mock;
};
GLFWwindow* InputSystemTest::m_window = nullptr;

///////////// Key press detection //////////////

/**
 * A GLFW key press event must be delivered as a KEY_DOWN event carrying the
 * key code and modifier bits. Same for RELEASE and REPEAT.
 */
TEST_F(InputSystemTest, KeyPress_DetectsPressDown) {
  auto handler = std::make_shared<MockHandler>();
  auto scene = MakeScene();
  m_input->SwitchContext(0);

  EXPECT_CALL(mock, OnKeyDown).Times(1);
  EXPECT_CALL(mock, OnKeyUp).Times(1);
  EXPECT_CALL(mock, OnKeyRepeat).Times(1);

  m_io->key_callback(GLFW_KEY_W, GLFW_PRESS, 0);
  m_io->key_callback(GLFW_KEY_U, GLFW_RELEASE, 0);
  m_io->key_callback(GLFW_KEY_U, GLFW_REPEAT, 0);
  m_input->OnUpdate(0.0, *scene);
}

///////////// Mouse position //////////////

/**
 * Cursor movement must be delivered as MOUSE_MOVE events with the absolute
 * position and the delta relative to the previous position.
 */
TEST_F(InputSystemTest, MousePosition_MousePosCallback) {
  auto handler = std::make_shared<MockHandler>();
  auto scene = MakeScene();

  EXPECT_CALL(mock, OnMouseMove).Times(3);

  m_io->cursor_pos_callback(10, 20);
  m_io->cursor_pos_callback(20, 20);
  m_io->cursor_pos_callback(30, 20);
  m_input->OnUpdate(0.0, *scene);
}

///////////// Multi-context //////////////

/**
 * Second context is empty, so it being active should not fire any handler
 * functions.
 */
TEST_F(InputSystemTest, MultiContext_CorrectActiveContext) {
  auto handler = std::make_shared<MockHandler>();
  auto scene = MakeScene();
  EXPECT_NO_THROW(m_input->SwitchContext(1));

  EXPECT_CALL(mock, OnKeyDown).Times(0);
  EXPECT_CALL(mock, OnKeyUp).Times(0);
  EXPECT_CALL(mock, OnKeyRepeat).Times(0);
  EXPECT_CALL(mock, OnMouseMove).Times(0);

  m_io->key_callback(GLFW_KEY_W, GLFW_PRESS, 0);
  m_io->key_callback(GLFW_KEY_U, GLFW_RELEASE, 0);
  m_io->key_callback(GLFW_KEY_U, GLFW_REPEAT, 0);
  m_io->cursor_pos_callback(30, 40);
  m_input->OnUpdate(0.0, *scene);
}

/**
 * Using out of bound context.
 */
TEST_F(InputSystemTest, MultiContext_ContextOutOfBound) {
  auto handler = std::make_shared<MockHandler>();
  auto scene = MakeScene();
  EXPECT_THROW(m_input->SwitchContext(2), std::runtime_error);
}
