#pragma once
#include "../dependencies/EnumBitmask.hpp" // IWYU pragma: export
#include <algorithm>                       // IWYU pragma: export
#include <any>                             // IWYU pragma: export
#include <array>                           // IWYU pragma: export
#include <bitset>                          // IWYU pragma: export
#include <chrono>                          // IWYU pragma: export
#include <cmath>                           // IWYU pragma: export
#include <compare>                         // IWYU pragma: export
#include <concepts>                        // IWYU pragma: export
#include <cstdint>                         // IWYU pragma: export
#include <cstdlib>                         // IWYU pragma: export
#include <exception>                       // IWYU pragma: export
#include <filesystem>                      // IWYU pragma: export
#include <forward_list>                    // IWYU pragma: export
#include <fstream>                         // IWYU pragma: export
#include <functional>                      // IWYU pragma: export
#include <glm/ext.hpp>                     // IWYU pragma: export
#include <glm/glm.hpp>                     // IWYU pragma: export
#include <iostream>                        // IWYU pragma: export
#include <limits>                          // IWYU pragma: export
#include <list>                            // IWYU pragma: export
#include <map>                             // IWYU pragma: export
#include <memory>                          // IWYU pragma: export
#include <mutex>                           // IWYU pragma: export
#include <numbers>                         // IWYU pragma: export
#include <numeric>                         // IWYU pragma: export
#include <optional>                        // IWYU pragma: export
#include <ranges>                          // IWYU pragma: export
#include <set>                             // IWYU pragma: export
#include <span>                            // IWYU pragma: export
#include <stdexcept>                       // IWYU pragma: export
#include <string>                          // IWYU pragma: export
#include <string_view>                     // IWYU pragma: export
#include <thread>                          // IWYU pragma: export
#include <tuple>                           // IWYU pragma: export
#include <type_traits>                     // IWYU pragma: export
#include <unordered_map>                   // IWYU pragma: export
#include <unordered_set>                   // IWYU pragma: export
#include <utility>                         // IWYU pragma: export
#include <variant>                         // IWYU pragma: export
#include <vector>                          // IWYU pragma: export

#ifdef TR_HAS_STD_FORMAT
#include <format> // IWYU pragma: export
#define TR_FMT std
#define TR_FORMAT_STRING std::format_string
#else
#include <fmt/base.h>
#include <fmt/chrono.h>
#include <fmt/format.h>
#define TR_FMT fmt
#define TR_FORMAT_STRING fmt::fstring
#endif