#include "IWindow.h"

std::shared_ptr<IWindow> IWindow::defaultWindow_ = std::make_shared<NullWindow>();
