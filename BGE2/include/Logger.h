#pragma once
#include "../pch.h"
#include "Bases.h"

namespace Bored
{
class Logger : public Module
{
public:
  enum class Severity : int
  {
    INFO,
    WARNING,
    ERROR
  };
  struct Log
  {
    Severity severity;
    std::string message;
    std::time_t ts;
  };
public:
  void log(Severity s, const std::string& msg) noexcept { doLog(s, msg); };
  void info(const std::string& msg) noexcept { log(Severity::INFO, msg); }
  void warn(const std::string& msg) noexcept { log(Severity::WARNING, msg); }
  void error(const std::string& msg) noexcept { log(Severity::ERROR, msg); }

  // Inherited via Module
  virtual void OnSetup() override;
public:
  class Source {
    friend class Logger;
  public:
    Source() = default;
    virtual ~Source() {}

    template <typename T>
    Source& operator <<(T val) { return *this; }
  };
  class ConsoleSource : Source {
    ConsoleSource() = default;

    template <typename T>
    ConsoleSource& operator <<(T val) { std::cout << val; return *this; }
  };
  class FileSource : Source {
  public:
    FileSource()
      : FileSource("./logs/" + GetDefaultFilename()) {}
    FileSource(const std::string& filename)
      : f(filename) {}
    virtual ~FileSource() override; // TODO close file

    template <typename T>
    FileSource& operator <<(T val) { }
  private:
    std::ofstream f;
    const std::string& GetDefaultFilename() const noexcept;
  };

  class Formatter {
    friend class Logger;
    virtual const std::string& Format(Severity s, const std::string& msg, std::time_t ts) = 0;
  };
  class StandardFormatter : Formatter {
    virtual const std::string& Format(Severity s, const std::string& msg, std::time_t ts) override;
  };
  class JSONFormatter : Formatter {
    virtual const std::string& Format(Severity s, const std::string& msg, std::time_t ts) override;
  };
  class YAMLFormatter : Formatter {
    virtual const std::string& Format(Severity s, const std::string& msg, std::time_t ts) override;
  };

  class Behaviour {
    friend class Logger;
    virtual void OnTick(double dt) = 0;
    virtual void OnClose() = 0;
  };
  class InstantBehaviour : Behaviour {
    virtual void OnTick(double dt); // TODO
    virtual void OnClose() {};
  };
  class FlushBehaviour : Behaviour {
    virtual void OnTick(double dt) {};
    virtual void OnClose(); // TODO
  };
private:
  // TODO
  // Source
  std::unique_ptr<Source> s;
  // Formatter
  std::unique_ptr<Formatter> f;
  // Behaviour: Flush / instant
  std::unique_ptr<Behaviour> b;
  std::vector<Log> logs;
private:
  void doLog(Severity s, const std::string& msg);
};

}
