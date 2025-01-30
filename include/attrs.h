#ifndef __ATTRS_H__
#define __ATTRS_H__

#ifndef __has_attribute
#       define __has_attribute(x) 0
#endif

#if __has_attribute(__nonnull__)
#       define NONNULL(...) __attribute__((__nonnull__ (__VA_ARGS__)))
#else
#       define NONNULL(...)
#endif

#if __has_attribute(always_inline)
#       define __inline inline __attribute__((always_inline))
#else
#       define __inline inline
#endif

#if __has_attribute(const)
#       define _const_fn __attribute__((const))
#else
#       define _const_fn
#endif

#if __has_attribute(pure)
#       define _pure_fn __attribute__((pure))
#else
#       define _pure_fn
#endif

#endif /* __ATTRS_H__ */
