#pragma once

#ifdef __ANDROID__

#include <ghc/fs_fwd.hpp>

namespace juglans::fs {
using namespace ghc::filesystem;
using ifstream = ghc::filesystem::ifstream;
using ofstream = ghc::filesystem::ofstream;
using fstream = ghc::filesystem::fstream;
}  // namespace juglans::fs

#else

#include <filesystem>

namespace juglans::fs {
using namespace std::filesystem;
using ifstream = std::ifstream;
using ofstream = std::ofstream;
using fstream = std::fstream;
}  // namespace juglans::fs

#endif
