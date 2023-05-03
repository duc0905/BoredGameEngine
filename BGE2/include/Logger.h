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
  class Source {
    friend class Logger;
  public:
    Source() = default;
    virtual ~Source() {}
    virtual Source& log(const std::string const& msg) = 0;
    virtual void OnSetup() = 0;
    virtual void OnStop() = 0;
  };
  class Formatter {
    friend class Logger;
  public:
    [[nodiscard]] virtual const std::string& format(Severity s, const std::string& msg, std::time_t ts) = 0;
    virtual void OnSetup() = 0;
    virtual void OnStop() = 0;
  };
public:
  Logger(std::unique_ptr<Source> src, std::unique_ptr<Formatter> fmt)
    : source{std::move(src)}, formatter{std::move(fmt)} {}

  virtual Logger& log(Severity s, const std::string& msg) noexcept = 0;
  void info(const std::string& msg) noexcept { log(Severity::INFO, msg); }
  void warn(const std::string& msg) noexcept { log(Severity::WARNING, msg); }
  void error(const std::string& msg) noexcept { log(Severity::ERROR, msg); }

  // Inherited via Module
  virtual void OnSetup() override;
  virtual void OnStop() override;
public:
  class ConsoleSource : Source {
    ConsoleSource() = default;

    // Inherited via Source
    virtual Source& log(const std::string const& msg) override;
    virtual void OnSetup() {};
    virtual void OnStop() {};
  };
  class FileSource : Source {
  public:
    FileSource()
      : FileSource("./logs/" + GetDefaultFilename()) {}
    FileSource(const std::string& filename)
      : f(filename) {}
    virtual ~FileSource() override;

    virtual void OnSetup() {};
    virtual void OnStop() {};
  private:
    std::ofstream f;
    [[nodiscard]] const std::string& GetDefaultFilename() const noexcept;

    // Inherited via Source
    virtual Source& log(const std::string const& msg) override;
  };

  class StandardFormatter : Formatter {
    virtual const std::string& format(Severity s, const std::string& msg, std::time_t ts) override;
    virtual void OnSetup() {};
    virtual void OnStop() {};
  };
  class JSONFormatter : Formatter {
    virtual const std::string& format(Severity s, const std::string& msg, std::time_t ts) = 0;
    virtual void OnSetup() = 0;
    virtual void OnStop() = 0;
  };
  class YAMLFormatter : Formatter {
    virtual const std::string& format(Severity s, const std::string& msg, std::time_t ts) = 0;
    virtual void OnSetup()  = 0;
    virtual void OnStop() = 0;
  };
protected:
  // Source
  std::unique_ptr<Source> source;
  // Formatter
  std::unique_ptr<Formatter> formatter;

};

class InstantLogger : Logger
{
  InstantLogger(std::unique_ptr<Source> src, std::unique_ptr<Formatter> fmt)
    : Logger(std::move(src), std::move(fmt)) {}

  // Inherited via Logger
  virtual Logger& log(Severity s, const std::string& msg) noexcept override;
};

class FlushLogger : Logger
{
public:
  FlushLogger(std::unique_ptr<Source> src, std::unique_ptr<Formatter> fmt)
    : Logger(std::move(src), std::move(fmt)) {}
  virtual ~FlushLogger();

  // Inherited via Logger
  virtual Logger& log(Severity s, const std::string& msg) noexcept override;
private:
  std::vector<Log> logs;

};

}
