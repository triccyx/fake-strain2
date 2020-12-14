#pragma once

namespace embot { namespace prot { namespace can { namespace analog { namespace periodic {
        

    // the management of commands   

    enum class ADCsaturation { NONE = 0, LOW = 1, HIGH = 2 };
}}}}};

  inline static const std::uint8_t numOfChannels = 6;
  inline static const std::uint8_t numOfSets = 3;