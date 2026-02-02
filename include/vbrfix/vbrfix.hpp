#ifndef VBRFIX_HPP
#define VBRFIX_HPP

#include <vector>
#include <string>
#include <variant>
#include <filesystem>
#include <cstdint>
#include <optional>

namespace vbrfix {

    struct FixParams {
        bool always_skip = false;
        bool remove_id3v1 = false;
        bool remove_id3v2 = false;
        bool remove_unknown = false;
        bool remove_lame = false;
        bool keep_lame = true; // default behavior
    };

    using InputData = std::variant<std::filesystem::path, std::vector<uint8_t>>;

    std::vector<uint8_t> fix_mp3(const InputData& input, const FixParams& params = {});

}

#endif