#pragma once

#include "../../../config.h"
#include "log_sink.h"

#include <string>

namespace objects {
namespace detail {

class TextBasedLogSink : public LogSink {
 public:
  TextBasedLogSink(Verbosity verbosity, std::string time_fmt, std::string fmt,
                   bool ignore_colour);

 protected:
  virtual void WriteEntry(Verbosity severity, const utils::Timestamp& timestamp,
                          int tid, const char* file, int line,
                          const std::string& component,
                          const char* msg) override;

  virtual void WritePartialOutput(const std::string& str) = 0;
  virtual void SetOutputColours(Verbosity) {}
  virtual void ResetOutputColours() {}
  virtual void Flush() {}

 private:
  const std::string time_fmt_;
  const std::string fmt_;
  const bool ignore_colour_;
};

}  // namespace detail
}  // namespace objects