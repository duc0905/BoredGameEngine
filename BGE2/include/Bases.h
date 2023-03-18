#pragma once

namespace Bored
{
/**
 * @brief Has the OnTick function that called every frame.
*/
class Tickable {
 public:
  /**
   * @brief This will be called every frame
   * @param dt time since last frame 
   * @return true if the game should stop, true otherwise
  */
  virtual bool OnTick(double dt) { return false; };
};

/**
 * @brief Highest class in the tree
*/
class Module : public Tickable {
 public:
  virtual void OnSetup() = 0;
  virtual void OnStop(){};
  virtual void OnImGui(){};
};

}
