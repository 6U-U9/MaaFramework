#pragma once

#if !defined(MAA_USE_RANGES_STL) && !defined(MAA_USE_RANGES_RANGE_V3) && !defined(MAA_USE_RANGES_BOOST)
#if defined(__clang__) && __clang_major__ < 15
#define MAA_USE_RANGES_RANGE_V3
#else
#define MAA_USE_RANGES_STL
#endif
#endif

#ifdef _MSC_VER
#define MAA_DO_PRAGMA(x) __pragma(x)
#elif defined(__GNUC__)
#define MAA_DO_PRAGMA(x) _Pragma(#x)
#else
#define MAA_DO_PRAGMA(x)
#endif

#ifdef _MSC_VER
#define MAA_SUPPRESS_CV_WARNINGS_START \
    MAA_DO_PRAGMA(warning(push))       \
    MAA_DO_PRAGMA(warning(disable : 5054 4251 4305 4275 4100 4244 4127))
#define MAA_SUPPRESS_CV_WARNINGS_END MAA_DO_PRAGMA(warning(pop))
#elif defined(__clang__)
#define MAA_SUPPRESS_CV_WARNINGS_START                                               \
    MAA_DO_PRAGMA(clang diagnostic push)                                             \
    MAA_DO_PRAGMA(clang diagnostic ignored "-Wdeprecated-enum-enum-conversion")      \
    MAA_DO_PRAGMA(clang diagnostic ignored "-Wdeprecated-anon-enum-enum-conversion") \
    MAA_DO_PRAGMA(clang diagnostic ignored "-Wc11-extensions")                       \
    MAA_DO_PRAGMA(clang diagnostic ignored "-Wunused-but-set-variable")
#define MAA_SUPPRESS_CV_WARNINGS_END MAA_DO_PRAGMA(clang diagnostic pop)
#elif defined(__GNUC__)
#define MAA_SUPPRESS_CV_WARNINGS_START \
    MAA_DO_PRAGMA(GCC diagnostic push) \
    MAA_DO_PRAGMA(GCC diagnostic ignored "-Wdeprecated-enum-enum-conversion")
#define MAA_SUPPRESS_CV_WARNINGS_END MAA_DO_PRAGMA(GCC diagnostic pop)
#else
#define MAA_SUPPRESS_CV_WARNINGS_START
#define MAA_SUPPRESS_CV_WARNINGS_END
#endif

#ifdef __GNUC__
#define MAA_AUTO_DEDUCED_ZERO_INIT_START \
    MAA_DO_PRAGMA(GCC diagnostic push)   \
    MAA_DO_PRAGMA(GCC diagnostic ignored "-Wmissing-field-initializers")
#define MAA_AUTO_DEDUCED_ZERO_INIT_END MAA_DO_PRAGMA(GCC diagnostic pop)
#elif defined(__clang__)
#define MAA_AUTO_DEDUCED_ZERO_INIT_START \
    MAA_DO_PRAGMA(clang diagnostic push) \
    MAA_DO_PRAGMA(clang diagnostic ignored "-Wmissing-field-initializers")
#define MAA_AUTO_DEDUCED_ZERO_INIT_END MAA_DO_PRAGMA(clang diagnostic pop)
#else
#define MAA_AUTO_DEDUCED_ZERO_INIT_START
#define MAA_AUTO_DEDUCED_ZERO_INIT_END
#endif

#define MAA_NS MaaNS
#define MAA_NS_BEGIN \
    namespace MAA_NS \
    {
#define MAA_NS_END }

#define MAA_RES_NS MAA_NS::ResourceNS
#define MAA_RES_NS_BEGIN \
    namespace MAA_RES_NS \
    {
#define MAA_RES_NS_END }

#define MAA_CTRL_NS MAA_NS::ControllerNS
#define MAA_CTRL_NS_BEGIN \
    namespace MAA_CTRL_NS \
    {
#define MAA_CTRL_NS_END }

#define MAA_VISION_NS MAA_NS::VisionNS
#define MAA_VISION_NS_BEGIN \
    namespace MAA_VISION_NS \
    {
#define MAA_VISION_NS_END }

#define MAA_TASK_NS MAA_NS::TaskNS
#define MAA_TASK_NS_BEGIN \
    namespace MAA_TASK_NS \
    {
#define MAA_TASK_NS_END }

#ifndef NOMINMAX
#define NOMINMAX
#endif

#ifndef MAA_VERSION
#define MAA_VERSION "DEBUG_VERSION"
#endif

#if defined(_MSC_VER)
    // FIXME: remove this
    #pragma warning(disable : 4100)
#endif
