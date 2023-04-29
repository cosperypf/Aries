#pragma once

#ifdef _WIN32

#ifdef JUGLANS_EXPORT_API
#define JUGLANS_INTERFACE __declspec(dllexport)
#else
#define JUGLANS_INTERFACE __declspec(dllimport)
#endif

#else

#ifdef JUGLANS_EXPORT_API
#define JUGLANS_INTERFACE __attribute__((visibility("default")))
#else
#define JUGLANS_INTERFACE
#endif

#endif

#define JUGLANS_INTERFACE_CAPI extern "C" JUGLANS_INTERFACE


struct JUGLANS_INTERFACE InterfaceStructDemo {
  InterfaceStructDemo() {}
};
struct JUGLANS_INTERFACE InterfaceClassDemo {
  InterfaceClassDemo(){};
  ~InterfaceClassDemo(){};
};