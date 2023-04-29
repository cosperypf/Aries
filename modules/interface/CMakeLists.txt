AddModule(interface SHARED juglans PRIVATE juglans_core)

if (TARGET juglans_interface)
  target_compile_options(juglans_interface PRIVATE "-DJUGLANS_EXPORT_API")
endif()
