#pragma once

#include "../../../config.h"
#include "log_sink.h"

#include <functional>

namespace objects {
namespace detail {

class HookLogSink : public LogSink {
 public:
  typedef std::function<void(Verbosity, const utils::Timestamp&, int,
                             const char*, int, const std::string&, const char*)>
      HookFn;

  HookLogSink(Verbosity verbosity, HookFn fn);

  virtual void WriteEntry(Verbosity severity, const utils::Timestamp& timestamp,
                          int tid, const char* file, int line,
                          const std::string& component,
                          const char* msg) override;

 private:
  const HookFn hook_fn_;
};

typedef std::unique_ptr<HookLogSink> HookLogSinkPtr;

}  // namespace detail
}  // namespace objects