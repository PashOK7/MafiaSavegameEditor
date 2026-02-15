#include "gvas.hpp"

#include <fstream>
#include <regex>
#include <sstream>

namespace gvas {

namespace {

std::uint32_t ReadU32LE(const std::vector<std::uint8_t>& b, std::size_t off) {
    return static_cast<std::uint32_t>(b[off]) | (static_cast<std::uint32_t>(b[off + 1]) << 8) |
           (static_cast<std::uint32_t>(b[off + 2]) << 16) | (static_cast<std::uint32_t>(b[off + 3]) << 24);
}

void WriteU32LE(std::vector<std::uint8_t>* b, std::size_t off, std::uint32_t v) {
    (*b)[off] = static_cast<std::uint8_t>(v & 0xFFu);
    (*b)[off + 1] = static_cast<std::uint8_t>((v >> 8) & 0xFFu);
    (*b)[off + 2] = static_cast<std::uint8_t>((v >> 16) & 0xFFu);
    (*b)[off + 3] = static_cast<std::uint8_t>((v >> 24) & 0xFFu);
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

bool ParseSaveFileMeta(const fs::path& path, SaveFileMeta* out) {
    if (out == nullptr) {
        return false;
    }
    std::regex nameRe(R"(^mafia(\d+)\.(\d+)$)", std::regex::icase);
    std::smatch m;
    const std::string name = path.filename().string();
    if (!std::regex_match(name, m, nameRe)) {
        return false;
    }
    out->path = path;
    out->profile = std::stoi(m[1].str());
    out->slot = std::stoi(m[2].str());
    return true;
}

bool HasMagic(const std::vector<std::uint8_t>& bytes) {
    return bytes.size() >= 4 && bytes[0] == 'G' && bytes[1] == 'v' && bytes[2] == 'a' && bytes[3] == 'S';
}

bool ReadHeader(const std::vector<std::uint8_t>& bytes, Header* out, std::string* error) {
    if (out == nullptr) {
        if (error != nullptr) {
            *error = "null output header";
        }
        return false;
    }
    if (bytes.size() < kHeaderSize) {
        if (error != nullptr) {
            *error = "file is smaller than GvaS header size";
        }
        return false;
    }
    if (!HasMagic(bytes)) {
        if (error != nullptr) {
            *error = "missing GvaS magic";
        }
        return false;
    }

    out->w04 = ReadU32LE(bytes, 0x04);
    out->version = ReadU32LE(bytes, 0x08);
    out->w0c = ReadU32LE(bytes, 0x0C);
    out->w10 = ReadU32LE(bytes, 0x10);
    out->w14 = ReadU32LE(bytes, 0x14);
    out->w18 = ReadU32LE(bytes, 0x18);
    out->w1c = ReadU32LE(bytes, 0x1C);
    out->d0 = ReadU32LE(bytes, 0x20);
    out->d1 = ReadU32LE(bytes, 0x24);
    out->d2 = ReadU32LE(bytes, 0x28);
    out->d3 = ReadU32LE(bytes, 0x2C);
    out->d4 = ReadU32LE(bytes, 0x30);
    out->d5 = ReadU32LE(bytes, 0x34);
    return true;
}

bool WriteHeader(const Header& header, std::vector<std::uint8_t>* bytes, std::string* error) {
    if (bytes == nullptr) {
        if (error != nullptr) {
            *error = "null output byte vector";
        }
        return false;
    }
    if (bytes->size() < kHeaderSize) {
        if (error != nullptr) {
            *error = "file is smaller than GvaS header size";
        }
        return false;
    }
    if (!HasMagic(*bytes)) {
        if (error != nullptr) {
            *error = "missing GvaS magic";
        }
        return false;
    }

    WriteU32LE(bytes, 0x04, header.w04);
    WriteU32LE(bytes, 0x08, header.version);
    WriteU32LE(bytes, 0x0C, header.w0c);
    WriteU32LE(bytes, 0x10, header.w10);
    WriteU32LE(bytes, 0x14, header.w14);
    WriteU32LE(bytes, 0x18, header.w18);
    WriteU32LE(bytes, 0x1C, header.w1c);
    WriteU32LE(bytes, 0x20, header.d0);
    WriteU32LE(bytes, 0x24, header.d1);
    WriteU32LE(bytes, 0x28, header.d2);
    WriteU32LE(bytes, 0x2C, header.d3);
    WriteU32LE(bytes, 0x30, header.d4);
    WriteU32LE(bytes, 0x34, header.d5);
    return true;
}

std::uint16_t DecodeMissionFromD0(std::uint32_t d0) {
    const std::uint16_t hi = static_cast<std::uint16_t>(((d0 >> 8) & 0xFFu) ^ 0x1Cu);
    const std::uint16_t lo = static_cast<std::uint16_t>((d0 & 0xFFu) ^ 0x30u);
    return static_cast<std::uint16_t>((hi << 8) | lo);
}

std::uint16_t DecodeMissionFromD1(std::uint32_t d1) {
    return static_cast<std::uint16_t>(d1 - 0x29889D95u);
}

std::uint16_t DecodeMissionFromD2(std::uint32_t d2) {
    return static_cast<std::uint16_t>((d2 - 0x81061EFAu) / 2u);
}

std::uint16_t DecodeMission(const Header& header) {
    return DecodeMissionFromD0(header.d0);
}

bool ValidateMissionChecks(const Header& header, std::string* error) {
    const std::uint16_t m0 = DecodeMissionFromD0(header.d0);
    const std::uint16_t m1 = DecodeMissionFromD1(header.d1);
    const std::uint16_t m2 = DecodeMissionFromD2(header.d2);
    const std::uint16_t m5 = static_cast<std::uint16_t>((header.d5 - 0x877ECA39u) / 8u);

    if (m0 == m1 && m0 == m2 && m0 == m5) {
        return true;
    }

    if (error != nullptr) {
        std::ostringstream oss;
        oss << "mission mismatch: d0=" << m0 << " d1=" << m1 << " d2=" << m2 << " d5=" << m5;
        *error = oss.str();
    }
    return false;
}

void EncodeMissionCore(Header* header, std::uint16_t mission) {
    if (header == nullptr) {
        return;
    }
    const std::uint32_t hi = static_cast<std::uint32_t>((((mission >> 8) & 0xFFu) ^ 0x1Cu) << 8);
    const std::uint32_t lo = static_cast<std::uint32_t>((mission & 0xFFu) ^ 0x30u);
    header->d0 = 0xD20B0000u | hi | lo;
    header->d1 = 0x29889D95u + static_cast<std::uint32_t>(mission);
    header->d2 = 0x81061EFAu + static_cast<std::uint32_t>(mission) * 2u;
    header->d5 = 0x877ECA39u + static_cast<std::uint32_t>(mission) * 8u;
}

std::map<std::uint16_t, MissionWord34> BuildMissionWord34Table(const fs::path& directory) {
    std::map<std::uint16_t, MissionWord34> table;
    if (!fs::exists(directory) || !fs::is_directory(directory)) {
        return table;
    }

    for (const auto& entry : fs::directory_iterator(directory)) {
        if (!entry.is_regular_file()) {
            continue;
        }
        SaveFileMeta meta;
        if (!ParseSaveFileMeta(entry.path(), &meta)) {
            continue;
        }

        const auto bytes = ReadFileBytes(entry.path());
        Header h;
        if (!ReadHeader(bytes, &h)) {
            continue;
        }

        std::string checkError;
        if (!ValidateMissionChecks(h, &checkError)) {
            continue;
        }

        const std::uint16_t mission = DecodeMission(h);
        if (meta.slot != static_cast<int>(mission)) {
            continue;
        }

        auto& rec = table[mission];
        if (rec.sourceCount == 0) {
            rec.d3 = h.d3;
            rec.d4 = h.d4;
            rec.sourceCount = 1;
            continue;
        }

        if (rec.d3 != h.d3 || rec.d4 != h.d4) {
            rec.conflict = true;
        }
        ++rec.sourceCount;
    }
    return table;
}

bool TryApplyMissionWord34(Header* header,
                           std::uint16_t mission,
                           const std::map<std::uint16_t, MissionWord34>& table,
                           std::string* message) {
    if (header == nullptr) {
        if (message != nullptr) {
            *message = "null header";
        }
        return false;
    }

    const auto it = table.find(mission);
    if (it == table.end()) {
        if (message != nullptr) {
            *message = "mission is not present in d3/d4 lookup table";
        }
        return false;
    }
    if (it->second.conflict) {
        if (message != nullptr) {
            *message = "d3/d4 lookup has conflicting values for this mission";
        }
        return false;
    }
    header->d3 = it->second.d3;
    header->d4 = it->second.d4;
    if (message != nullptr) {
        *message = "applied d3/d4 from lookup table";
    }
    return true;
}

}  // namespace gvas

