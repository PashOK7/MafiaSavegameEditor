#include <cstdint>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <regex>
#include <sstream>
#include <string>
#include <vector>

namespace fs = std::filesystem;

static std::vector<std::uint8_t> ReadFileBytes(const fs::path& path) {
    std::ifstream in(path, std::ios::binary);
    if (!in) {
        return {};
    }
    in.seekg(0, std::ios::end);
    const std::streamsize size = in.tellg();
    in.seekg(0, std::ios::beg);

    std::vector<std::uint8_t> bytes(static_cast<size_t>(size));
    if (size > 0) {
        in.read(reinterpret_cast<char*>(bytes.data()), size);
    }
    return bytes;
}

static std::uint16_t ReadU16LE(const std::vector<std::uint8_t>& b, size_t off) {
    return static_cast<std::uint16_t>(b[off] | (static_cast<std::uint16_t>(b[off + 1]) << 8));
}

static std::map<int, std::string> LoadMissionNames(const fs::path& missionsPath) {
    std::map<int, std::string> out;
    std::ifstream in(missionsPath);
    if (!in) {
        return out;
    }

    std::regex rowRe(R"(^\s*(\d{3})\s+(.+?)\s*$)");
    std::string line;
    while (std::getline(in, line)) {
        std::smatch m;
        if (!std::regex_match(line, m, rowRe)) {
            continue;
        }
        const int id = std::stoi(m[1].str());
        out[id] = m[2].str();
    }
    return out;
}

struct SaveInfo {
    fs::path path;
    int profileFromName = -1;
    int slotFromName = -1;
    std::uint16_t missionDecoded = 0;
    std::uint16_t missionCheckD1 = 0;
    std::uint16_t missionCheckD2 = 0;
    bool validGvaS = false;
    bool hasHeader = false;
};

static SaveInfo ParseGvaSSave(const fs::path& path) {
    SaveInfo info;
    info.path = path;

    std::regex nameRe(R"(^mafia(\d+)\.(\d+)$)", std::regex::icase);
    std::smatch nameMatch;
    const std::string baseName = path.filename().string();
    if (std::regex_match(baseName, nameMatch, nameRe)) {
        info.profileFromName = std::stoi(nameMatch[1].str());
        info.slotFromName = std::stoi(nameMatch[2].str());
    }

    const auto bytes = ReadFileBytes(path);
    if (bytes.size() < 44) {
        return info;
    }

    info.hasHeader = true;
    info.validGvaS = (bytes[0] == 'G' && bytes[1] == 'v' && bytes[2] == 'a' && bytes[3] == 'S');
    if (!info.validGvaS) {
        return info;
    }

    const std::uint8_t d0b0 = bytes[32];
    const std::uint8_t d0b1 = bytes[33];
    const std::uint16_t d1 = ReadU16LE(bytes, 36);
    const std::uint16_t d2 = ReadU16LE(bytes, 40);

    const std::uint16_t missionFromD0 =
        static_cast<std::uint16_t>(((static_cast<std::uint16_t>(d0b1 ^ 0x1C) << 8) | (d0b0 ^ 0x30)));
    const std::uint16_t missionFromD1 = static_cast<std::uint16_t>(d1 - 0x9D95);
    const std::uint16_t missionFromD2Half = static_cast<std::uint16_t>((d2 - 0x1EFA) / 2);

    info.missionDecoded = missionFromD0;
    info.missionCheckD1 = missionFromD1;
    info.missionCheckD2 = missionFromD2Half;

    return info;
}

int main(int argc, char** argv) {
    const fs::path saveDir = (argc > 1) ? fs::path(argv[1]) : fs::path("savegame");
    const fs::path missionsPath = (argc > 2) ? fs::path(argv[2]) : fs::path("missions.txt");

    const auto missionNames = LoadMissionNames(missionsPath);
    if (!fs::exists(saveDir) || !fs::is_directory(saveDir)) {
        std::cerr << "Save directory not found: " << saveDir << "\n";
        return 1;
    }

    std::vector<SaveInfo> rows;
    std::regex fileNameRe(R"(^mafia\d+\.\d+$)", std::regex::icase);
    for (const auto& entry : fs::directory_iterator(saveDir)) {
        if (!entry.is_regular_file()) {
            continue;
        }
        const std::string baseName = entry.path().filename().string();
        if (!std::regex_match(baseName, fileNameRe)) {
            continue;
        }
        rows.push_back(ParseGvaSSave(entry.path()));
    }

    std::sort(rows.begin(), rows.end(), [](const SaveInfo& a, const SaveInfo& b) {
        return a.path.filename().string() < b.path.filename().string();
    });

    std::cout << "file,size,profile,slot,decoded_mission,d1_mission,d2_mission,mission_name,notes\n";
    for (const auto& r : rows) {
        const auto fileName = r.path.filename().string();
        const std::uintmax_t fileSize = fs::file_size(r.path);

        std::string missionName;
        if (r.validGvaS) {
            auto it = missionNames.find(r.missionDecoded);
            if (it != missionNames.end()) {
                missionName = it->second;
            }
        }

        std::vector<std::string> notes;
        if (!r.hasHeader) {
            notes.emplace_back("too_small");
        } else if (!r.validGvaS) {
            notes.emplace_back("not_gvas");
        } else {
            if (r.slotFromName >= 0 && r.slotFromName != static_cast<int>(r.missionDecoded)) {
                notes.emplace_back("slot_mismatch");
            }
            if (r.missionDecoded != r.missionCheckD1 || r.missionDecoded != r.missionCheckD2) {
                notes.emplace_back("header_check_failed");
            }
            if (missionName.empty()) {
                notes.emplace_back("unknown_mission_name");
            }
        }

        std::ostringstream noteJoin;
        for (size_t i = 0; i < notes.size(); ++i) {
            if (i) {
                noteJoin << '|';
            }
            noteJoin << notes[i];
        }

        std::cout << fileName << "," << fileSize << "," << r.profileFromName << "," << r.slotFromName << ","
                  << r.missionDecoded << "," << r.missionCheckD1 << "," << r.missionCheckD2 << ","
                  << missionName << "," << noteJoin.str() << "\n";
    }

    return 0;
}
