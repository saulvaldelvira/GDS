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

#endif /* __ATTRS_H__ */
