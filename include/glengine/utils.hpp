#ifndef GLENGINE_UTILS_HPP
#define GLENGINE_UTILS_HPP

#define GLN_DELETE_COPY_CTORS(TypeName)                                        \
  TypeName(TypeName const &) = delete;                                         \
  TypeName &operator=(TypeName const &) = delete

#define GLN_DEFINE_NOEXCEPT_MOVE_CTORS(TypeName)                               \
  TypeName(TypeName &&) noexcept = default;                                    \
  TypeName &operator=(TypeName &&) noexcept = default

#endif