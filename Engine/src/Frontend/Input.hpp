#pragma once
#include <memory>
#include <map>
#include <string>
#include <functional>
#include "../GameLoop.hpp"

namespace Bored
{
  class Actor;
namespace Input
{
enum Key
{
    KEY_1,
    KEY_2,
    KEY_3,
    KEY_4,
    KEY_5,
    KEY_6,
    KEY_7,
    KEY_8,
    KEY_9,
    KEY_0,
    KEY_Q,
    KEY_W,
    KEY_E,
    KEY_R,
    KEY_T,
    KEY_Y,
    KEY_U,
    KEY_I,
    KEY_O,
    KEY_P,
    KEY_A,
    KEY_S,
    KEY_D,
    KEY_F,
    KEY_G,
    KEY_H,
    KEY_J,
    KEY_K,
    KEY_L,
    KEY_Z,
    KEY_X,
    KEY_C,
    KEY_V,
    KEY_B,
    KEY_N,
    KEY_M,

    KEY_LEFT_SHIFT,
    KEY_LEFT_CONTROL,
    KEY_SPACE,
    KEY_TAB,

    // Mouse butts
    KEY_MB_1,
    KEY_MB_2,
    KEY_MB_3,
    KEY_MB_4,
    KEY_MB_5,
    KEY_MB_6,
    KEY_MB_7,
    KEY_MB_8,

    MOUSE_POS_X,
    MOUSE_POS_Y,
    MOUSE_SCROLL_X,
    MOUSE_SCROLL_Y,
    MOUSE_ENTER,
    MOUSE_LEAVE,

    // default
    KEY_UNKNOWN
};

enum Mod
{
    CTRL = 1 << 0,
    SHIFT = 1 << 1,
    ALT = 1 << 2,
    MOD_UNKNOWN = 1 << 8
};

enum Action
{
    PRESS,
    REPEAT,
    RELEASE,
    UNKNOWN
};

enum ActionEnum
{
    ACTION,
    RANGE
};

/**
 * @brief 
 */
class Manager : public Module
{
  public:
    /** Context
     * Define a set of actions/state/ranges those are available when this context is active
     * Multiple contexts can be active at once
     * Using Chain of responsibility pattern
     */
    class Context
    {
        friend class Manager;
      public:
        Context() = default;

        void AddActionMapping(Input::Key key, int mods, const std::string& name);
        void RemoveActionMapping(Input::Key key, int mods);

        void AddRangeMapping(Input::Key key, int mods, const std::string& name, const float& weight);
        void RemoveRangeMapping(Input::Key key, int mods);

        std::string MapKeyAction(Input::Key key, int mods);
        std::pair<std::string, float> MapKeyRange(Input::Key key, int mods);

      private:
        void ResetPriority(int priority);
        void Activate();
        void Deactivate();

      private:
        std::shared_ptr<Context> next_;
        int priority_ = 1;
        bool isActive_ = false;

        std::map<std::pair<Key, int>, std::string> actionMap_;
        std::map<std::pair<Key, int>, std::pair<std::string, float>> rangeMap_;
    };

  public:
    typedef std::function<void(Input::Action)> ActionCallback;
    typedef std::function<void(Input::Action, float)> RangeCallback;

  public:
    /* ============ Context management ============= */

    void EvaluateKey(Input::Key key, Input::Action action, int mods, double val);

    void BindAction(const std::string& name, ActionCallback func);
    void BindRange(const std::string& name, RangeCallback func, float val = 1.0f);

    void AddContext(Context* con);
    void AddContext(std::shared_ptr<Context> con);

    bool IsContextActivate(Context* con);
    bool IsContextActivate(std::shared_ptr<Context> con);

    void RemoveContext(Context* con);
    void RemoveContext(std::shared_ptr<Context> con);

    void ActivateContext(Context* con);
    void ActivateContext(std::shared_ptr<Context> con);

    void DeactivateContext(Context* con);
    void DeactivateContext(std::shared_ptr<Context> con);

    void ResetPriority(Context* con, int priorityLevel);
    void ResetPriority(std::shared_ptr<Context> con, int priorityLevel);

    /* ========== Cursor management =========== */

    inline std::pair<double, double> GetMousePosition() const
    {
        return {mouseInfo.posX, mouseInfo.posY};
    }

    virtual void SetCursorImage(unsigned char* image, unsigned int width, unsigned int height) = 0;
    virtual void EnableCursor() = 0;
    virtual void DisableCursor() = 0;

    std::shared_ptr<Actor> GetCursorHoveringActor();

  private:
    std::shared_ptr<Context> headContext;
    std::map<std::string, ActionCallback> actionMap;
    std::map<std::string, std::pair<RangeCallback, float>> rangeMap;

    struct MouseInfo
    {
        double posX;
        double posY;
        double scrollX;
        double scrollY;
        bool isHidden;
        bool isEntered;
        bool btn1;
        bool btn2;
        bool btn3;
        bool btn4;
        bool btn5;
        bool btn6;
        bool btn7;
        bool btn8;
    } mouseInfo;
  public:
    inline MouseInfo GetMouseInfo() const
    {
        return mouseInfo;
    }
};

} // namespace Input

} // namespace Bored
