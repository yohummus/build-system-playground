#pragma once

#include "io.h"
#include "internal/string_conversion.h"
#include "internal/library.h"
#include "internal/error_code_helpers.h"

#include <chrono>
#include <cassert>
#include <string>
#include <memory>
#include <type_traits>
#include <initializer_list>

namespace yogi {
namespace internal {

template <typename Fn, typename... Args>
inline void* CallApiCreate(Fn fn, Args&&... args) {
  void* handle;
  int res = fn(&handle, std::forward<Args>(args)...);
  CheckErrorCode(res);
  return handle;
}

template <typename Fn, typename... Args>
inline void* CallApiCreateWithDescriptiveErrorCode(Fn fn, Args&&... args) {
  void* handle;
  CheckDescriptiveErrorCode([&](auto err, auto size) {
    return fn(&handle, std::forward<Args>(args)..., err, size);
  });

  return handle;
}

}  // namespace internal

YOGI_DEFINE_API_FN(int, YOGI_FormatObject,
                   (void* object, char* str, int strsize, const char* objfmt,
                    const char* nullstr))
YOGI_DEFINE_API_FN(int, YOGI_Destroy, (void* object))

class Object;
using ObjectPtr = std::shared_ptr<Object>;

/// Base class for all "creatable" objects.
///
/// "Creatable" Yogi objects are objects that get instantiated and live until
/// they are destroyed by the user.
class Object : public std::enable_shared_from_this<Object> {
 public:
  virtual ~Object() {
    if (handle_ == nullptr) return;
    int res = internal::YOGI_Destroy(handle_);
    assert(res == 0);
  }

  /// Creates a string describing the object.
  ///
  /// The \p objfmt parameter describes the format of the string. The following
  /// placeholders are supported:
  ///  - *$T*: Type of the object (e.g. Branch)
  ///  - *$x*: Handle of the object in lower-case hex notation
  ///  - *$X*: Handle of the object in upper-case hex notation
  ///
  /// By default, the object will be formatted in the format "Branch [44fdde]"
  /// with the hex value in brackets being the object's handle.
  ///
  /// If, for any reason, the object's handle is NULL, this function returns
  /// the nullstr parameter value ("INVALID HANDLE" by default).
  ///
  /// \tparam FmtString Type of the \p fmt string.
  /// \tparam NullString Type of the \p nullstr string.
  ///
  /// \param fmt     Format of the string
  /// \param nullstr String to use if the object's handle is NULL
  ///
  /// \returns Formatted string.
  template <typename FmtString = char*, typename NullString = char*>
  std::string Format(FmtString&& fmt = nullptr,
                     NullString&& nullstr = nullptr) const {
    char str[128];
    int res = internal::YOGI_FormatObject(handle_, str, sizeof(str),
                                          internal::ToCoreString(fmt),
                                          internal::ToCoreString(nullstr));
    internal::CheckErrorCode(res);
    return str;
  }

  /// Returns a human-readable string identifying the object.
  ///
  /// \returns Human-readable string identifying the object.
  virtual std::string ToString() const { return Format(); }

 protected:
  Object(void* handle, std::initializer_list<ObjectPtr> dependencies)
      : handle_(handle), dependencies_(dependencies) {}

  void* GetHandle() const { return handle_; }

  static void* GetForeignHandle(const ObjectPtr& other) {
    return other->GetHandle();
  }

 private:
  Object(const Object&) = delete;
  Object& operator=(const Object&) = delete;

  void* handle_;
  const std::initializer_list<ObjectPtr> dependencies_;
};

/// Templated base class for all "creatable" objects.
///
/// "Creatable" Yogi objects are objects that get instantiated and live until
/// they are destroyed by the user.
///
/// \tparam T Class that derives from ObjectT.
template <typename T>
class ObjectT : public Object {
 public:
  using Object::Object;
};

}  // namespace yogi
