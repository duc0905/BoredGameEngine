#pragma once

namespace Bored {
template <typename T> T &Behaviour::GetComponent() { return obj->GetComponent<T>(); }
template <typename T> bool Behaviour::HasComponent() { return obj->HasComponent<T>(); }
template <typename T, typename... Args> T &Behaviour::AddComponent(Args &&...args) {
  return obj->AddComponent<T>(std::forward<Args>(args)...);
}
} // namespace Bored
