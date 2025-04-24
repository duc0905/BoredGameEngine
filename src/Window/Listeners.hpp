#pragma once

/**
 * Interface for listeners listenning for frame buffer size event.
 */
class FrameBufferSizeListener {
public:
  /**
   * Called when the frame buffer size is changed.
   *
   * @param width New framebuffer width.
   * @param height New framebuffer height.
   */
  virtual void OnFrameBufferSize(int width, int height) = 0;
};
