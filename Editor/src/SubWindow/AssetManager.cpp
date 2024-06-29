#include "AssetManager.hpp"

AssetManager::AssetManager(Manifest& game): SubWindow(game) {}

void AssetManager::SetType(const AssetType& type) {
    _type = type;
}

void AssetManager::OnSetup() {

}

bool AssetManager::OnUpdate(double dt) {
    return true;
}

void AssetManager::OnShutdown() {

}

void AssetManager::DrawContent() {

}
