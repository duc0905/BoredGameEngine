#include "windowAPI.h"

namespace Bored {
namespace Editor {
namespace OSAdapter {
namespace Window {

std::unique_ptr<WindowAPI> WindowAPI::_window = nullptr;

WindowAPI& WindowAPI::GetInstance() {
  if (_window == nullptr) {
    _window = std::unique_ptr<WindowAPI>(new WindowAPI());
  }

  return *_window;
}

WindowAPI::WindowAPI(){};

std::string WindowAPI::OpenDirPath() {
  HRESULT hr =
      CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
  if (FAILED(hr)) {
    std::cerr << "Failed to initialize COM library" << std::endl;
    return "";
  }

  // Initialize a BROWSEINFO structure
  BROWSEINFO bi = {0};
  bi.lpszTitle = "Select a folder";
  LPITEMIDLIST pidl = SHBrowseForFolder(&bi);

  if (pidl != NULL) {
    // Get the selected folder's path
    TCHAR folderPath[MAX_PATH];
    if (SHGetPathFromIDList(pidl, folderPath)) {
      std::cout << "Selected folder: " << folderPath << std::endl;
      //   std::wstring returnStr(folderPath);
      std::string folderPathStr = folderPath;

      //   int bufferSize = WideCharToMultiByte(CP_UTF8, 0, folderPath, -1,
      //   nullptr,
      //    0, nullptr, nullptr);
      //   if (bufferSize > 0) {
      // std::string folderPathStr(
      // bufferSize, '\0');  // Create a
      // string of the required size
      // WideCharToMultiByte(CP_UTF8, 0,
      // folderPath, -1, &folderPathStr[0],
      // bufferSize, nullptr, nullptr);

      // Now, folderPathStr is a std::string containing
      // the path in UTF-8 encoding
      std::cout << "Folder Path: " << folderPathStr << std::endl;
      return folderPathStr;
      //   } else {
      // std::cerr << "Error converting TCHAR to
      // std::string" << std::endl; return "";
      //   }

    } else {
      std::cerr << "Failed to retrieve folder path" << std::endl;
      return "";
    }

    // Free the PIDL
    IMalloc* imalloc = NULL;
    if (SUCCEEDED(SHGetMalloc(&imalloc))) {
      imalloc->Free(pidl);
      imalloc->Release();
    }
  } else {
    std::cerr << "No folder selected" << std::endl;
    return "";
  }

  // Uninitialized the COM library
  CoUninitialize();
};
}  // namespace Window
}  // namespace OSAdapter
}  // namespace Editor
}  // namespace Bored
