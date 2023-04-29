AddModule(core DEPENDS spdlog OPTIONAL ghc_filesystem)

target_compile_options(juglans_core PUBLIC "$<$<CONFIG:Debug>:-DJUGLANS_DEBUG>")
