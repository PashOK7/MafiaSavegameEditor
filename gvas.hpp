#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <map>
#include <optional>
#include <string>
#include <vector>

namespace gvas {

namespace fs = std::filesystem;

constexpr std::size_t kHeaderSize = 56;

struct Header {
    std::uint32_t w04 = 0;
    std::uint32_t version = 0;
    std::uint32_t w0c = 0;
    std::uint32_t w10 = 0;
    std::uint32_t w14 = 0;
    std::uint32_t w18 = 0;
    std::uint32_t w1c = 0;
    std::uint32_t d0 = 0;
    std::uint32_t d1 = 0;
    std::uint32_t d2 = 0;
    std::uint32_t d3 = 0;
    std::uint32_t d4 = 0;
    std::uint32_t d5 = 0;
};

struct MissionWord34 {
    std::uint32_t d3 = 0;
    std::uint32_t d4 = 0;
    std::size_t sourceCount = 0;
    bool conflict = false;
};

struct SaveFileMeta {
    fs::path path;
    int profile = -1;
    int slot = -1;
};

std::vector<std::uint8_t> ReadFileBytes(const fs::path& path);
bool WriteFileBytes(const fs::path& path, const std::vector<std::uint8_t>& bytes);

bool ParseSaveFileMeta(const fs::path& path, SaveFileMeta* out);

bool ReadHeader(const std::vector<std::uint8_t>& bytes, Header* out, std::string* error = nullptr);
bool WriteHeader(const Header& header, std::vector<std::uint8_t>* bytes, std::string* error = nullptr);
bool HasMagic(const std::vector<std::uint8_t>& bytes);

std::uint16_t DecodeMissionFromD0(std::uint32_t d0);
std::uint16_t DecodeMissionFromD1(std::uint32_t d1);
std::uint16_t DecodeMissionFromD2(std::uint32_t d2);
std::uint16_t DecodeMission(const Header& header);

bool ValidateMissionChecks(const Header& header, std::string* error = nullptr);
void EncodeMissionCore(Header* header, std::uint16_t mission);

std::map<std::uint16_t, MissionWord34> BuildMissionWord34Table(const fs::path& directory);
bool TryApplyMissionWord34(Header* header,
                           std::uint16_t mission,
                           const std::map<std::uint16_t, MissionWord34>& table,
                           std::string* message = nullptr);

}  // namespace gvas

