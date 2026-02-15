#include "mafia_save.hpp"

#include <algorithm>
#include <fstream>
#include <sstream>

namespace mafia_save {

namespace {

struct CipherState {
    std::uint32_t key1 = 0x23101976u;
    std::uint32_t key2 = 0x10072002u;
};

std::uint32_t ReadU32LERaw(const std::uint8_t* data) {
    return static_cast<std::uint32_t>(data[0]) | (static_cast<std::uint32_t>(data[1]) << 8) |
           (static_cast<std::uint32_t>(data[2]) << 16) | (static_cast<std::uint32_t>(data[3]) << 24);
}

void WriteU32LERaw(std::uint8_t* data, std::uint32_t value) {
    data[0] = static_cast<std::uint8_t>(value & 0xFFu);
    data[1] = static_cast<std::uint8_t>((value >> 8) & 0xFFu);
    data[2] = static_cast<std::uint8_t>((value >> 16) & 0xFFu);
    data[3] = static_cast<std::uint8_t>((value >> 24) & 0xFFu);
}

void DecryptInPlace(std::vector<std::uint8_t>* bytes, CipherState* state) {
    if (bytes == nullptr || state == nullptr) {
        return;
    }
    const std::size_t fullWords = bytes->size() / 4;
    for (std::size_t i = 0; i < fullWords; ++i) {
        const std::size_t off = i * 4;
        const std::uint32_t cipher = ReadU32LERaw(bytes->data() + off);
        const std::uint32_t plain = state->key1 ^ cipher;
        WriteU32LERaw(bytes->data() + off, plain);
        state->key2 += plain;
        state->key1 += state->key2;
    }
}

void EncryptInPlace(std::vector<std::uint8_t>* bytes, CipherState* state) {
    if (bytes == nullptr || state == nullptr) {
        return;
    }
    const std::size_t fullWords = bytes->size() / 4;
    for (std::size_t i = 0; i < fullWords; ++i) {
        const std::size_t off = i * 4;
        const std::uint32_t plain = ReadU32LERaw(bytes->data() + off);
        state->key2 += plain;
        const std::uint32_t cipher = plain ^ state->key1;
        WriteU32LERaw(bytes->data() + off, cipher);
        state->key1 += state->key2;
    }
}

bool ReadEncryptedSegment(const std::vector<std::uint8_t>& raw,
                          std::size_t* cursor,
                          std::size_t size,
                          const std::string& name,
                          CipherState* state,
                          SaveData* out,
                          std::string* error) {
    if (cursor == nullptr || state == nullptr || out == nullptr) {
        if (error != nullptr) {
            *error = "internal null pointer while reading segment";
        }
        return false;
    }
    if (*cursor + size > raw.size()) {
        if (error != nullptr) {
            std::ostringstream oss;
            oss << "segment '" << name << "' exceeds file size";
            *error = oss.str();
        }
        return false;
    }

    Segment seg;
    seg.name = name;
    seg.plain.assign(raw.begin() + static_cast<std::ptrdiff_t>(*cursor),
                     raw.begin() + static_cast<std::ptrdiff_t>(*cursor + size));
    DecryptInPlace(&seg.plain, state);
    out->segments.push_back(std::move(seg));
    *cursor += size;
    return true;
}

const std::vector<std::uint8_t>* GetSegment(const SaveData& save, std::size_t idx, std::string* error) {
    if (idx == kNoIndex || idx >= save.segments.size()) {
        if (error != nullptr) {
            *error = "requested segment index is missing";
        }
        return nullptr;
    }
    return &save.segments[idx].plain;
}

std::uint32_t ReadInfoField(const SaveData& save, std::size_t offset, std::string* error) {
    const auto* info = GetSegment(save, save.idxInfo, error);
    if (info == nullptr) {
        return 0;
    }
    if (offset + 4 > info->size()) {
        if (error != nullptr) {
            *error = "info block offset out of range";
        }
        return 0;
    }
    return ReadU32LE(*info, offset);
}

}  // namespace

std::vector<std::uint8_t> ReadFileBytes(const fs::path& path) {
    std::ifstream in(path, std::ios::binary);
    if (!in) {
        return {};
    }
    in.seekg(0, std::ios::end);
    const auto size = in.tellg();
    in.seekg(0, std::ios::beg);

    std::vector<std::uint8_t> bytes(static_cast<std::size_t>(size));
    if (size > 0) {
        in.read(reinterpret_cast<char*>(bytes.data()), size);
    }
    return bytes;
}

bool WriteFileBytes(const fs::path& path, const std::vector<std::uint8_t>& bytes) {
    std::ofstream out(path, std::ios::binary | std::ios::trunc);
    if (!out) {
        return false;
    }
    if (!bytes.empty()) {
        out.write(reinterpret_cast<const char*>(bytes.data()), static_cast<std::streamsize>(bytes.size()));
    }
    return static_cast<bool>(out);
}

bool ParseSave(const std::vector<std::uint8_t>& raw, SaveData* out, std::string* error) {
    if (out == nullptr) {
        if (error != nullptr) {
            *error = "null output save struct";
        }
        return false;
    }
    if (raw.size() < kFileHeaderSize) {
        if (error != nullptr) {
            *error = "file is too small for 24-byte header";
        }
        return false;
    }

    SaveData parsed;
    parsed.rawSize = raw.size();
    std::copy(raw.begin(), raw.begin() + static_cast<std::ptrdiff_t>(kFileHeaderSize), parsed.fileHeader.begin());

    std::size_t cursor = kFileHeaderSize;
    CipherState state;

    parsed.idxHead = parsed.segments.size();
    if (!ReadEncryptedSegment(raw, &cursor, kBlockHeadSize, "head24", &state, &parsed, error)) {
        return false;
    }

    parsed.idxMeta = parsed.segments.size();
    if (!ReadEncryptedSegment(raw, &cursor, kBlockMetaSize, "meta32", &state, &parsed, error)) {
        return false;
    }

    parsed.idxInfo = parsed.segments.size();
    if (!ReadEncryptedSegment(raw, &cursor, kBlockInfoSize, "info264", &state, &parsed, error)) {
        return false;
    }

    const auto mainSize = ReadMainPayloadSize(parsed, error);
    const auto aiGroupsSize = ReadAiGroupsSize(parsed, error);
    const auto aiFollowSize = ReadAiFollowSize(parsed, error);

    parsed.idxGamePayload = parsed.segments.size();
    if (!ReadEncryptedSegment(raw, &cursor, static_cast<std::size_t>(mainSize), "game_payload", &state, &parsed, error)) {
        return false;
    }

    if (aiGroupsSize > 0) {
        parsed.idxAiGroups = parsed.segments.size();
        if (!ReadEncryptedSegment(raw,
                                  &cursor,
                                  static_cast<std::size_t>(aiGroupsSize),
                                  "ai_groups_payload",
                                  &state,
                                  &parsed,
                                  error)) {
            return false;
        }
    }

    if (aiFollowSize > 0) {
        parsed.idxAiFollow = parsed.segments.size();
        if (!ReadEncryptedSegment(raw,
                                  &cursor,
                                  static_cast<std::size_t>(aiFollowSize),
                                  "ai_follow_payload",
                                  &state,
                                  &parsed,
                                  error)) {
            return false;
        }
    }

    std::size_t actorIndex = 0;
    while (cursor < raw.size()) {
        if (raw.size() - cursor < kActorHeaderSize) {
            if (error != nullptr) {
                *error = "trailing bytes are smaller than actor header";
            }
            return false;
        }

        const std::string headerName = "actor_header_" + std::to_string(actorIndex);
        const std::size_t hdrIdx = parsed.segments.size();
        if (!ReadEncryptedSegment(raw, &cursor, kActorHeaderSize, headerName, &state, &parsed, error)) {
            return false;
        }
        const auto payloadSize = ReadU32LE(parsed.segments[hdrIdx].plain, 132);
        if (cursor + payloadSize > raw.size()) {
            if (error != nullptr) {
                std::ostringstream oss;
                oss << "actor payload exceeds file at actor " << actorIndex;
                *error = oss.str();
            }
            return false;
        }

        const std::string payloadName = "actor_payload_" + std::to_string(actorIndex);
        if (!ReadEncryptedSegment(raw, &cursor, payloadSize, payloadName, &state, &parsed, error)) {
            return false;
        }
        ++actorIndex;
    }

    parsed.actorCount = actorIndex;
    *out = std::move(parsed);
    return true;
}

bool BuildRaw(const SaveData& save, std::vector<std::uint8_t>* out, std::string* error) {
    if (out == nullptr) {
        if (error != nullptr) {
            *error = "null output byte vector";
        }
        return false;
    }

    std::vector<std::uint8_t> raw;
    raw.insert(raw.end(), save.fileHeader.begin(), save.fileHeader.end());

    CipherState state;
    for (const auto& seg : save.segments) {
        std::vector<std::uint8_t> cipher = seg.plain;
        EncryptInPlace(&cipher, &state);
        raw.insert(raw.end(), cipher.begin(), cipher.end());
    }

    *out = std::move(raw);
    return true;
}

std::uint32_t ReadU32LE(const std::vector<std::uint8_t>& bytes, std::size_t offset) {
    return static_cast<std::uint32_t>(bytes[offset]) | (static_cast<std::uint32_t>(bytes[offset + 1]) << 8) |
           (static_cast<std::uint32_t>(bytes[offset + 2]) << 16) |
           (static_cast<std::uint32_t>(bytes[offset + 3]) << 24);
}

void WriteU32LE(std::vector<std::uint8_t>* bytes, std::size_t offset, std::uint32_t value) {
    (*bytes)[offset] = static_cast<std::uint8_t>(value & 0xFFu);
    (*bytes)[offset + 1] = static_cast<std::uint8_t>((value >> 8) & 0xFFu);
    (*bytes)[offset + 2] = static_cast<std::uint8_t>((value >> 16) & 0xFFu);
    (*bytes)[offset + 3] = static_cast<std::uint8_t>((value >> 24) & 0xFFu);
}

bool ReadMetaFields(const SaveData& save, MetaFields* out, std::string* error) {
    if (out == nullptr) {
        if (error != nullptr) {
            *error = "null output meta struct";
        }
        return false;
    }
    const auto* meta = GetSegment(save, save.idxMeta, error);
    if (meta == nullptr) {
        return false;
    }
    if (meta->size() < kBlockMetaSize) {
        if (error != nullptr) {
            *error = "meta32 block is too small";
        }
        return false;
    }

    out->slot = ReadU32LE(*meta, 0);
    out->unknown1 = ReadU32LE(*meta, 4);
    out->packedTime = ReadU32LE(*meta, 8);
    out->packedDate = ReadU32LE(*meta, 12);
    out->hpPercent = ReadU32LE(*meta, 16);
    out->unknown5 = ReadU32LE(*meta, 20);
    out->unknown6 = ReadU32LE(*meta, 24);
    out->missionCode = ReadU32LE(*meta, 28);
    return true;
}

bool WriteHpPercent(SaveData* save, std::uint32_t hpPercent, std::string* error) {
    if (save == nullptr) {
        if (error != nullptr) {
            *error = "null save pointer";
        }
        return false;
    }
    if (save->idxMeta == kNoIndex || save->idxMeta >= save->segments.size()) {
        if (error != nullptr) {
            *error = "meta32 block is missing";
        }
        return false;
    }
    auto& meta = save->segments[save->idxMeta].plain;
    if (meta.size() < kBlockMetaSize) {
        if (error != nullptr) {
            *error = "meta32 block is too small";
        }
        return false;
    }
    WriteU32LE(&meta, 16, hpPercent);
    return true;
}

bool XorGamePayloadByte(SaveData* save, std::size_t payloadOffset, std::uint8_t mask, std::string* error) {
    if (save == nullptr) {
        if (error != nullptr) {
            *error = "null save pointer";
        }
        return false;
    }
    if (save->idxGamePayload == kNoIndex || save->idxGamePayload >= save->segments.size()) {
        if (error != nullptr) {
            *error = "game payload segment is missing";
        }
        return false;
    }
    auto& payload = save->segments[save->idxGamePayload].plain;
    if (payloadOffset >= payload.size()) {
        if (error != nullptr) {
            std::ostringstream oss;
            oss << "payload offset " << payloadOffset << " is out of range (size=" << payload.size() << ")";
            *error = oss.str();
        }
        return false;
    }
    payload[payloadOffset] ^= mask;
    return true;
}

bool XorFileOffsetByte(SaveData* save, std::size_t fileOffset, std::uint8_t mask, std::string* error) {
    if (save == nullptr) {
        if (error != nullptr) {
            *error = "null save pointer";
        }
        return false;
    }
    if (fileOffset >= save->rawSize) {
        if (error != nullptr) {
            std::ostringstream oss;
            oss << "file offset " << fileOffset << " is out of range (size=" << save->rawSize << ")";
            *error = oss.str();
        }
        return false;
    }

    if (fileOffset < kFileHeaderSize) {
        save->fileHeader[fileOffset] ^= mask;
        return true;
    }

    std::size_t streamOffset = fileOffset - kFileHeaderSize;
    for (auto& seg : save->segments) {
        if (streamOffset < seg.plain.size()) {
            seg.plain[streamOffset] ^= mask;
            return true;
        }
        streamOffset -= seg.plain.size();
    }

    if (error != nullptr) {
        *error = "failed to map file offset to decrypted segments";
    }
    return false;
}

bool SetFileOffsetByte(SaveData* save, std::size_t fileOffset, std::uint8_t value, std::string* error) {
    if (save == nullptr) {
        if (error != nullptr) {
            *error = "null save pointer";
        }
        return false;
    }
    if (fileOffset >= save->rawSize) {
        if (error != nullptr) {
            std::ostringstream oss;
            oss << "file offset " << fileOffset << " is out of range (size=" << save->rawSize << ")";
            *error = oss.str();
        }
        return false;
    }

    if (fileOffset < kFileHeaderSize) {
        save->fileHeader[fileOffset] = value;
        return true;
    }

    std::size_t streamOffset = fileOffset - kFileHeaderSize;
    for (auto& seg : save->segments) {
        if (streamOffset < seg.plain.size()) {
            seg.plain[streamOffset] = value;
            return true;
        }
        streamOffset -= seg.plain.size();
    }

    if (error != nullptr) {
        *error = "failed to map file offset to decrypted segments";
    }
    return false;
}

std::string ReadMissionName(const SaveData& save, std::string* error) {
    const auto* info = GetSegment(save, save.idxInfo, error);
    if (info == nullptr) {
        return {};
    }
    const std::size_t cap = std::min<std::size_t>(32, info->size());
    std::size_t len = 0;
    while (len < cap && (*info)[len] != 0) {
        ++len;
    }
    return std::string(reinterpret_cast<const char*>(info->data()), len);
}

std::uint32_t ReadMainPayloadSize(const SaveData& save, std::string* error) {
    return ReadInfoField(save, 32, error);
}

std::uint32_t ReadAiGroupsSize(const SaveData& save, std::string* error) {
    return ReadInfoField(save, 240, error);
}

std::uint32_t ReadAiFollowSize(const SaveData& save, std::string* error) {
    return ReadInfoField(save, 244, error);
}

}  // namespace mafia_save
