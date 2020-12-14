#pragma once
#include <cinttypes>
#include <cstring>

namespace embot {
namespace prot {
namespace can {
namespace analog {
namespace periodic {

// the management of commands

enum class ADCsaturation { NONE = 0, LOW = 1, HIGH = 2 };
} // namespace periodic
} // namespace analog
} // namespace can
} // namespace prot
}; // namespace embot

inline static const std::uint8_t numOfChannels = 6;
inline static const std::uint8_t numOfSets = 3;

inline static const std::uint32_t validityKey = 0xdead0001;
inline static const std::uint32_t validityKey_v00 = 0x7777dead;