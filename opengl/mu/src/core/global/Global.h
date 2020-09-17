/**
  * @file Global.h
  */

#ifndef MU_GLOBAL_H
#define MU_GLOBAL_H

// platform-linux ----------
#if !defined(__linux) || !defined(__linux__) || !defined(__gnu_linux__)
 #error "Only support linux for now!"
#else
 #define MU_PLATFORM_LINUX
#endif
// platform-linux ----------

// c/c++-compatibility ----------
#ifndef __cplusplus
 #error "Only support C++, C is not compatible!"
#endif
// c/c++-compatibility ----------

// compiler-gcc ----------
#ifdef MU_PLATFORM_LINUX
 #define MU_EXPORT __attribute__((visibility("default")))
#else
 #define MU_EXPORT
#endif
// compiler-gcc ----------

// c++-feature ---------
#define MU_BEGIN_NAMESPACE namespace mu {
#define MU_END_NAMESPACE   }
#define MU_USE_NAMESPACE   using namespace ::mu;

#define MU_DISABLE_COPY(CLASS) \
    CLASS(const CLASS&); \
    CLASS& operator=(const CLASS&);

#define MU_DECLARE_PRIVATE(CLASS) \
    inline CLASS##Private* dFunc() { return reinterpret_cast<CLASS##Private*>(dPtr); } \
    inline const CLASS##Private* dFunc() const { return reinterpret_cast<const CLASS##Private*>(dPtr); } \
    friend class CLASS##Private;

#define MU_DECLARE_PUBLIC(CLASS) \
    inline CLASS* mFunc() { return static_cast<CLASS*>(mPtr); } \
    inline const CLASS* mFunc() const { return static_cast<const CLASS*>(mPtr); } \
    friend class CLASS;

#define MU_D(CLASS) CLASS##Private * const d = dFunc()
#define MU_M(CLASS) CLASS * const m = mFunc()
// c++-feature ---------

// assert ----------
void _mu_assert_fail(const char *_expr, const char *_file, int _line, const char *_func, const char *_msg);

#define MU_ASSERT(EXPR, MSG) \
    ((EXPR) \
     ? static_cast<void>(0) \
     : _mu_assert_fail(#EXPR, __FILE__, __LINE__, __PRETTY_FUNCTION__, #MSG))

#define MU_ASSERT2(EXPR) MU_ASSERT(EXPR, "")
// assert ----------

// types ----------
#include <cstdint>
// types ----------

#endif // MU_GLOBAL_H
