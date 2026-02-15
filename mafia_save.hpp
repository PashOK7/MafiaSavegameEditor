#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <string>
#include <vector>

namespace mafia_save {

namespace fs = std::filesystem;

constexpr std::size_t kFileHeaderSize = 24;
constexpr std::size_t kBlockHeadSize = 24;
constexpr std::size_t kBlockMetaSize = 32;
constexpr std::size_t kBlockInfoSize = 264;
constexpr std::size_t kActorHeaderSize = 140;
constexpr std::size_t kNoIndex = static_cast<std::size_t>(-1);

struct Segment {
    std::string name;
    std::vector<std::uint8_t> plain;
};

struct SaveData {
    std::array<std::uint8_t, kFileHeaderSize> fileHeader{};
    std::vector<Segment> segments;

    std::size_t idxHead = kNoIndex;
    std::size_t idxMeta = kNoIndex;
    std::size_t idxInfo = kNoIndex;
    std::size_t idxGamePayload = kNoIndex;
    std::size_t idxAiGroups = kNoIndex;
    std::size_t idxAiFollow = kNoIndex;

    std::size_t actorCount = 0;
    std::size_t rawSize = 0;
};

struct MetaFields {
    std::uint32_t slot = 0;
    std::uint32_t unknown1 = 0;
    std::uint32_t packedTime = 0;
    std::uint32_t packedDate = 0;
    std::uint32_t hpPercent = 0;
    std::uint32_t unknown5 = 0;
    std::uint32_t unknown6 = 0;
    std::uint32_t missionCode = 0;
};

std::vector<std::uint8_t> ReadFileBytes(const fs::path& path);
bool WriteFileBytes(const fs::path& path, const std::vector<std::uint8_t>& bytes);

bool ParseSave(const std::vector<std::uint8_t>& raw, SaveData* out, std::string* error = nullptr);
bool BuildRaw(const SaveData& save, std::vector<std::uint8_t>* out, std::string* error = nullptr);

std::uint32_t ReadU32LE(const std::vector<std::uint8_t>& bytes, std::size_t offset);
void WriteU32LE(std::vector<std::uint8_t>* bytes, std::size_t offset, std::uint32_t value);

bool ReadMetaFields(const SaveData& save, MetaFields* out, std::string* error = nullptr);
bool WriteHpPercent(SaveData* save, std::uint32_t hpPercent, std::string* error = nullptr);
bool XorGamePayloadByte(SaveData* save, std::size_t payloadOffset, std::uint8_t mask, std::string* error = nullptr);
bool XorFileOffsetByte(SaveData* save, std::size_t fileOffset, std::uint8_t mask, std::string* error = nullptr);
bool SetFileOffsetByte(SaveData* save, std::size_t fileOffset, std::uint8_t value, std::string* error = nullptr);

std::string ReadMissionName(const SaveData& save, std::string* error = nullptr);
std::uint32_t ReadMainPayloadSize(const SaveData& save, std::string* error = nullptr);
std::uint32_t ReadAiGroupsSize(const SaveData& save, std::string* error = nullptr);
std::uint32_t ReadAiFollowSize(const SaveData& save, std::string* error = nullptr);

}  // namespace mafia_save
