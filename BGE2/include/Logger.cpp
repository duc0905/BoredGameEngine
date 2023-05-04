#include "pch.h"
#include "Logger.h"

namespace Bored {


void Logger::OnSetup()
{
  source->OnSetup();
  formatter->OnSetup();
}

void Logger::OnStop()
{
  source->OnStop();
  formatter->OnStop();
}

Logger& InstantLogger::log(Severity s, const std::string& msg) noexcept
{
  std::string str = formatter->format(s, msg, std::time(nullptr));
  source->log(str);
  return *this;
}

FlushLogger::~FlushLogger()
{
  for (auto& l : logs)
    source->log(formatter->format(l.severity, l.message, l.ts));
}

Logger& FlushLogger::log(Severity s, const std::string& msg) noexcept
{
  logs.push_back({ s, msg, std::time(nullptr) });
  return *this;
}

Logger::FileSource::~FileSource()
{
  f.close();
}

const std::string& Logger::FileSource::GetDefaultFilename() const noexcept
{
  auto now = std::chrono::system_clock::now();
  std::time_t noww = std::chrono::system_clock::to_time_t(now);
  char str[32];
  ctime_s(str, sizeof str, &noww);
  return str;
}

Logger::Source& Logger::FileSource::log(const std::string& msg)
{
  f << msg << std::endl;
  return *this;
}

Logger::Source& Logger::ConsoleSource::log(const std::string& msg)
{
  std::cout << msg << std::endl;
  return *this;
}

std::string Logger::StandardFormatter::format(Severity s, const std::string& msg, std::time_t ts)
{
  std::stringstream ss;
  unsigned int seconds = ts % (24 * 60 * 60);
  unsigned int minutes = seconds / 60;
  unsigned int hours = minutes / 60;

  ss << '[' << hours << ':' << minutes % 60 << ':' << seconds % 60 << ']';
  ss << '(';
  switch (s)
  {
  case Severity::WARNING: ss << "Warning"; break;
  case Severity::ERROR: ss << "ERROR"; break;
  default: ss << "Info"; break;
  }
  ss << "): " << msg;
  return ss.str();
}

};
