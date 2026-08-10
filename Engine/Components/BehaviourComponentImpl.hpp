#pragma once

namespace Bored {
template <typename T> T &Behaviour::GetComponent() { return node->GetComponent<T>(); }
template <typename T> bool Behaviour::HasComponent() { return node->HasComponent<T>(); }
template <typename T, typename... Args> T &Behaviour::AddComponent(Args &&...args) {
  return node->AddComponent<T>(std::forward<Args>(args)...);
}
} // namespace Bored
