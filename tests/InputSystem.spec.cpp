// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// clang-format on
#include <gtest/gtest.h>

#include <Components/InputComponent.hpp>
#include <Scene/Scene.hpp>
#include <Systems/Input/InputSystem.hpp>
#include <memory>
#include <vector>

/**
 * InputHandler that records every received event so tests can inspect them.
 */
class RecordingHandler : public Bored::InputHandler {
 public:
  virtual void OnInput(double dt, Bored::InputEvent& event,
                       std::shared_ptr<Bored::Object> node) override {
    events.push_back(event);
    last_dt = dt;
    if (mark_handled) event.handled = true;
  }

  std::vector<Bored::InputEvent> events;
  double last_dt = 0.0;
  bool mark_handled = false;
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
    m_input = std::make_unique<Bored::Input>(*m_io);
  }

  void TearDown() override {
    m_input.reset();
    m_io.reset();
  }

  /**
   * Build a scene with a single input node wired to the given handler.
   */
  std::shared_ptr<Bored::Scene> MakeScene(
      std::shared_ptr<RecordingHandler> handler) {
    auto scene = std::make_shared<Bored::Scene>();
    auto root = scene->CreateNode();
    scene->SetRoot(root);
    auto node = scene->CreateNode();
    root->AddChild(node);
    node->AddComponent<Bored::InputComponent>().input_handler = handler;
    return scene;
  }

  static GLFWwindow* m_window;
  std::unique_ptr<Bored::IOService> m_io;
  std::unique_ptr<Bored::Input> m_input;
};
GLFWwindow* InputSystemTest::m_window = nullptr;

///////////// Key press detection //////////////

/**
 * A GLFW key press event must be delivered as a KEY_DOWN event carrying the
 * key code and modifier bits.
 */
TEST_F(InputSystemTest, KeyPress_DetectsPressDown) {
  auto handler = std::make_shared<RecordingHandler>();
  auto scene = MakeScene(handler);

  m_io->key_callback(GLFW_KEY_W, GLFW_PRESS, 0);
  m_input->OnUpdate(0.0, *scene);

  ASSERT_EQ(handler->events.size(), 1);
  EXPECT_EQ(handler->events[0].type, Bored::InputType::KEY_DOWN);
  EXPECT_EQ(handler->events[0].key.keyCode, GLFW_KEY_W);
  EXPECT_EQ(handler->events[0].key.mods, 0);
}

/**
 * GLFW_REPEAT and GLFW_RELEASE must map to KEY_REPEAT and KEY_UP.
 */
TEST_F(InputSystemTest, KeyPress_RepeatAndRelease) {
  auto handler = std::make_shared<RecordingHandler>();
  auto scene = MakeScene(handler);

  m_io->key_callback(GLFW_KEY_SPACE, GLFW_REPEAT, 0);
  m_io->key_callback(GLFW_KEY_SPACE, GLFW_RELEASE, 0);
  m_input->OnUpdate(0.0, *scene);

  ASSERT_EQ(handler->events.size(), 2);
  EXPECT_EQ(handler->events[0].type, Bored::InputType::KEY_REPEAT);
  EXPECT_EQ(handler->events[0].key.keyCode, GLFW_KEY_SPACE);
  EXPECT_EQ(handler->events[1].type, Bored::InputType::KEY_UP);
  EXPECT_EQ(handler->events[1].key.keyCode, GLFW_KEY_SPACE);
}

///////////// Mouse position //////////////

/**
 * Cursor movement must be delivered as MOUSE_MOVE events with the absolute
 * position and the delta relative to the previous position.
 */
TEST_F(InputSystemTest, MousePosition_TracksAbsoluteAndDelta) {
  auto handler = std::make_shared<RecordingHandler>();
  auto scene = MakeScene(handler);

  m_io->cursor_pos_callback(10, 20);
  m_io->cursor_pos_callback(30, 40);
  m_input->OnUpdate(0.0, *scene);

  ASSERT_EQ(handler->events.size(), 2);
  EXPECT_EQ(handler->events[0].type, Bored::InputType::MOUSE_MOVE);
  EXPECT_EQ(handler->events[0].mouseMove.x, 10);
  EXPECT_EQ(handler->events[0].mouseMove.y, 20);

  EXPECT_EQ(handler->events[1].type, Bored::InputType::MOUSE_MOVE);
  EXPECT_EQ(handler->events[1].mouseMove.x, 30);
  EXPECT_EQ(handler->events[1].mouseMove.y, 40);
  EXPECT_EQ(handler->events[1].mouseMove.dx, 20);
  EXPECT_EQ(handler->events[1].mouseMove.dy, 20);
}

///////////// Input mapping //////////////

/**
 * Events must only reach nodes that carry an InputComponent with a handler;
 * plain nodes and handler-less components must be ignored.
 */
TEST_F(InputSystemTest, InputMapping_OnlyHandlersReceiveEvents) {
  auto scene = std::make_shared<Bored::Scene>();
  auto root = scene->CreateNode();
  scene->SetRoot(root);

  auto plain_node = scene->CreateNode();
  root->AddChild(plain_node);

  auto no_handler_node = scene->CreateNode();
  root->AddChild(no_handler_node);
  no_handler_node->AddComponent<Bored::InputComponent>();

  auto handler = std::make_shared<RecordingHandler>();
  auto input_node = scene->CreateNode();
  root->AddChild(input_node);
  input_node->AddComponent<Bored::InputComponent>().input_handler = handler;

  m_io->key_callback(GLFW_KEY_A, GLFW_PRESS, 0);
  m_input->OnUpdate(0.0, *scene);

  ASSERT_EQ(handler->events.size(), 1);
  EXPECT_EQ(handler->events[0].type, Bored::InputType::KEY_DOWN);
  EXPECT_EQ(handler->events[0].key.keyCode, GLFW_KEY_A);
}

/**
 * Once a handler marks an event as handled, it must not be delivered to any
 * further node in the scene.
 */
TEST_F(InputSystemTest, InputMapping_HandledEventStopsPropagation) {
  auto scene = std::make_shared<Bored::Scene>();
  auto root = scene->CreateNode();
  scene->SetRoot(root);

  auto first_handler = std::make_shared<RecordingHandler>();
  first_handler->mark_handled = true;
  root->AddComponent<Bored::InputComponent>().input_handler = first_handler;

  auto second_handler = std::make_shared<RecordingHandler>();
  auto child = scene->CreateNode();
  root->AddChild(child);
  child->AddComponent<Bored::InputComponent>().input_handler = second_handler;

  m_io->key_callback(GLFW_KEY_B, GLFW_PRESS, 0);
  m_input->OnUpdate(0.0, *scene);

  ASSERT_EQ(first_handler->events.size(), 1);
  EXPECT_EQ(second_handler->events.size(), 0);
}
