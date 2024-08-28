#pragma once
#include <memory>
#include <string>
#include "SubWindow.hpp"

class FileContentWindow : public SubWindow
{
  public:
    FileContentWindow(Manifest& game, int width, int height, std::shared_ptr<Bored::FileSystem::File> file);

    virtual void OnSetup() override;
    virtual bool OnUpdate(double dt) override;
    virtual void OnShutdown() override;
    virtual void NewFrame() override;

    void SetFileToDisplay(std::shared_ptr<Bored::FileSystem::File> file);

  private:
    std::shared_ptr<Bored::FileSystem::File> current_file;
    std::string ProcessFileNameToTitle(std::shared_ptr<Bored::FileSystem::File> file);

};
