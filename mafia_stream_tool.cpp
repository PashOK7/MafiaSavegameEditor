#include "mafia_save.hpp"

#include <cstdlib>
#include <filesystem>
#include <iomanip>
#include <iostream>
#include <optional>
#include <sstream>
#include <string>
#include <vector>

namespace fs = std::filesystem;

namespace {

void PrintUsage() {
    std::cout << "Usage:\n"
              << "  mafia_stream_tool inspect <save_file>\n"
              << "  mafia_stream_tool set-hp <input_file> <output_file> <hp_percent>\n"
              << "  mafia_stream_tool batch005 <base_file> <output_dir>\n"
              << "  mafia_stream_tool batch006 <base_file> <output_dir>\n"
              << "  mafia_stream_tool batch007 <base_file> <output_dir>\n"
              << "  mafia_stream_tool batch008 <base_file> <output_dir>\n";
}

std::optional<std::uint32_t> ParseU32(const std::string& s) {
    char* end = nullptr;
    const unsigned long v = std::strtoul(s.c_str(), &end, 10);
    if (end == nullptr || *end != '\0' || v > 0xFFFFFFFFul) {
        return std::nullopt;
    }
    return static_cast<std::uint32_t>(v);
}

void DecodePackedTime(std::uint32_t packed, int* hour, int* minute, int* second) {
    if (hour != nullptr) {
        *hour = static_cast<int>((packed >> 16) & 0xFFu);
    }
    if (minute != nullptr) {
        *minute = static_cast<int>((packed >> 8) & 0xFFu);
    }
    if (second != nullptr) {
        *second = static_cast<int>(packed & 0xFFu);
    }
}

void DecodePackedDate(std::uint32_t packed, int* day, int* month, int* year) {
    if (day != nullptr) {
        *day = static_cast<int>(packed & 0xFFu);
    }
    if (month != nullptr) {
        *month = static_cast<int>((packed >> 8) & 0xFFu);
    }
    if (year != nullptr) {
        *year = static_cast<int>((packed >> 16) & 0xFFFFu);
    }
}

std::size_t CountDiffBytes(const std::vector<std::uint8_t>& a, const std::vector<std::uint8_t>& b) {
    const std::size_t n = std::min(a.size(), b.size());
    std::size_t diff = 0;
    for (std::size_t i = 0; i < n; ++i) {
        if (a[i] != b[i]) {
            ++diff;
        }
    }
    diff += (a.size() > b.size()) ? (a.size() - b.size()) : (b.size() - a.size());
    return diff;
}

int CmdInspect(const fs::path& savePath) {
    const auto raw = mafia_save::ReadFileBytes(savePath);
    if (raw.empty()) {
        std::cerr << "Failed to read save file: " << savePath << "\n";
        return 1;
    }

    mafia_save::SaveData save;
    std::string err;
    if (!mafia_save::ParseSave(raw, &save, &err)) {
        std::cerr << "ParseSave failed: " << err << "\n";
        return 1;
    }

    mafia_save::MetaFields meta;
    if (!mafia_save::ReadMetaFields(save, &meta, &err)) {
        std::cerr << "ReadMetaFields failed: " << err << "\n";
        return 1;
    }

    const std::string missionName = mafia_save::ReadMissionName(save, &err);
    if (!err.empty()) {
        std::cerr << "ReadMissionName warning: " << err << "\n";
    }

    const std::uint32_t mainSize = mafia_save::ReadMainPayloadSize(save, &err);
    const std::uint32_t aiGroupsSize = mafia_save::ReadAiGroupsSize(save, &err);
    const std::uint32_t aiFollowSize = mafia_save::ReadAiFollowSize(save, &err);

    int hh = 0;
    int mm = 0;
    int ss = 0;
    int dd = 0;
    int mo = 0;
    int yy = 0;
    DecodePackedTime(meta.packedTime, &hh, &mm, &ss);
    DecodePackedDate(meta.packedDate, &dd, &mo, &yy);

    std::cout << "file=" << savePath.string() << "\n";
    std::cout << "raw_size=" << raw.size() << " segments=" << save.segments.size() << " actors=" << save.actorCount << "\n";
    std::cout << "mission_name=" << missionName << "\n";
    std::cout << "slot(meta32[0])=" << meta.slot << " mission_code(meta32[7])=" << meta.missionCode << "\n";
    std::cout << "hp_percent(meta32[4])=" << meta.hpPercent << "\n";
    std::cout << "date(meta32[3])=" << dd << "." << mo << "." << yy << "\n";
    std::cout << "time(meta32[2])=" << std::setfill('0') << std::setw(2) << hh << ":" << std::setw(2) << mm << ":"
              << std::setw(2) << ss << std::setfill(' ') << "\n";
    std::cout << "payload_sizes: game=" << mainSize << " ai_groups=" << aiGroupsSize << " ai_follow=" << aiFollowSize << "\n";
    std::size_t abs = mafia_save::kFileHeaderSize;
    for (std::size_t i = 0; i < save.segments.size(); ++i) {
        const auto& seg = save.segments[i];
        const std::size_t start = abs;
        const std::size_t end = seg.plain.empty() ? abs : (abs + seg.plain.size() - 1);
        std::cout << "segment[" << i << "] " << seg.name << " size=" << seg.plain.size() << " abs=0x" << std::hex
                  << start << "..0x" << end << std::dec << "\n";
        abs += seg.plain.size();
    }

    for (std::size_t i = 0; i < save.segments.size(); ++i) {
        const auto& seg = save.segments[i];
        if (seg.name.rfind("actor_header_", 0) != 0 || seg.plain.size() < 140) {
            continue;
        }
        std::size_t nameLen = 0;
        while (nameLen < 64 && seg.plain[nameLen] != 0) {
            ++nameLen;
        }
        std::size_t modelLen = 0;
        while (modelLen < 64 && seg.plain[64 + modelLen] != 0) {
            ++modelLen;
        }
        const std::string actorName(reinterpret_cast<const char*>(seg.plain.data()), nameLen);
        const std::string modelName(reinterpret_cast<const char*>(seg.plain.data() + 64), modelLen);
        const std::uint32_t actorType = mafia_save::ReadU32LE(seg.plain, 128);
        const std::uint32_t payloadSize = mafia_save::ReadU32LE(seg.plain, 132);
        const std::uint32_t slotIndex = mafia_save::ReadU32LE(seg.plain, 136);
        std::cout << "actor_header: " << seg.name << " actor=\"" << actorName << "\" model=\"" << modelName
                  << "\" type=" << actorType << " payload=" << payloadSize << " idx=" << slotIndex << "\n";
    }
    return 0;
}

bool WriteModified(const mafia_save::SaveData& save, const fs::path& outPath, std::string* errOut) {
    std::vector<std::uint8_t> raw;
    std::string err;
    if (!mafia_save::BuildRaw(save, &raw, &err)) {
        if (errOut != nullptr) {
            *errOut = err;
        }
        return false;
    }
    if (!mafia_save::WriteFileBytes(outPath, raw)) {
        if (errOut != nullptr) {
            *errOut = "failed to write file";
        }
        return false;
    }
    return true;
}

int CmdSetHp(const fs::path& inPath, const fs::path& outPath, std::uint32_t hpPercent) {
    const auto raw = mafia_save::ReadFileBytes(inPath);
    if (raw.empty()) {
        std::cerr << "Failed to read input file: " << inPath << "\n";
        return 1;
    }

    mafia_save::SaveData save;
    std::string err;
    if (!mafia_save::ParseSave(raw, &save, &err)) {
        std::cerr << "ParseSave failed: " << err << "\n";
        return 1;
    }

    if (!mafia_save::WriteHpPercent(&save, hpPercent, &err)) {
        std::cerr << "WriteHpPercent failed: " << err << "\n";
        return 1;
    }

    if (!WriteModified(save, outPath, &err)) {
        std::cerr << "Write failed: " << err << "\n";
        return 1;
    }

    std::cout << "input=" << inPath.string() << "\n";
    std::cout << "output=" << outPath.string() << "\n";
    std::cout << "new_hp_percent=" << hpPercent << "\n";
    return 0;
}

int CmdBatch005(const fs::path& basePath, const fs::path& outDir) {
    const auto raw = mafia_save::ReadFileBytes(basePath);
    if (raw.empty()) {
        std::cerr << "Failed to read base save file: " << basePath << "\n";
        return 1;
    }

    mafia_save::SaveData base;
    std::string err;
    if (!mafia_save::ParseSave(raw, &base, &err)) {
        std::cerr << "ParseSave failed: " << err << "\n";
        return 1;
    }

    fs::create_directories(outDir);

    auto writeVariant = [&](const std::string& fileName, const mafia_save::SaveData& variant) -> bool {
        const fs::path outPath = outDir / fileName;
        std::string localErr;
        if (!WriteModified(variant, outPath, &localErr)) {
            std::cerr << "Failed to write " << outPath << ": " << localErr << "\n";
            return false;
        }
        return true;
    };

    mafia_save::SaveData v22 = base;
    if (!writeVariant("mafia004.230_22", v22)) {
        return 1;
    }

    mafia_save::SaveData v23 = base;
    if (!mafia_save::XorFileOffsetByte(&v23, 0x400u, 0x01u, &err)) {
        std::cerr << "Failed to patch v23: " << err << "\n";
        return 1;
    }
    if (!writeVariant("mafia004.230_23", v23)) {
        return 1;
    }

    mafia_save::SaveData v24 = base;
    if (!mafia_save::XorFileOffsetByte(&v24, 0x800u, 0x01u, &err)) {
        std::cerr << "Failed to patch v24: " << err << "\n";
        return 1;
    }
    if (!writeVariant("mafia004.230_24", v24)) {
        return 1;
    }

    std::vector<std::uint8_t> raw22;
    std::vector<std::uint8_t> raw23;
    std::vector<std::uint8_t> raw24;
    if (!mafia_save::BuildRaw(v22, &raw22) || !mafia_save::BuildRaw(v23, &raw23) || !mafia_save::BuildRaw(v24, &raw24)) {
        std::cerr << "Internal error while rebuilding variants" << "\n";
        return 1;
    }

    const fs::path notePath = outDir / "RESULTS_SERIES_005_plan.txt";
    std::vector<std::string> lines;
    lines.push_back("Compact batch 005 (3 tests, encrypted stream-aware edits)");
    lines.push_back("");
    lines.push_back("Base file: " + basePath.string());
    lines.push_back("Generation method: decrypt -> edit plaintext -> re-encrypt full stream");
    lines.push_back("");
    lines.push_back("- mafia004.230_22: control roundtrip, no plaintext edits");
    lines.push_back("  Purpose: verify parser/encrypter stability (should behave exactly like base save).");
    lines.push_back("  Byte diff vs base: " + std::to_string(CountDiffBytes(raw, raw22)));
    lines.push_back("");
    lines.push_back("- mafia004.230_23: XOR plaintext byte at absolute file offset 0x400 with 0x01");
    lines.push_back("  Purpose: compare with old _20 result, but now without stream desync.");
    lines.push_back("  Byte diff vs base: " + std::to_string(CountDiffBytes(raw, raw23)));
    lines.push_back("");
    lines.push_back("- mafia004.230_24: XOR plaintext byte at absolute file offset 0x800 with 0x01");
    lines.push_back("  Purpose: compare with old _21 result, but now without stream desync.");
    lines.push_back("  Byte diff vs base: " + std::to_string(CountDiffBytes(raw, raw24)));
    lines.push_back("");
    lines.push_back("What to report:");
    lines.push_back("1) menu title/image/stats");
    lines.push_back("2) load progress stage (percent)");
    lines.push_back("3) whether mission starts, and if yes, where/with what behavior");

    std::string content;
    for (const auto& line : lines) {
        content += line;
        content += "\n";
    }
    if (!mafia_save::WriteFileBytes(notePath, std::vector<std::uint8_t>(content.begin(), content.end()))) {
        std::cerr << "Failed to write note file: " << notePath << "\n";
        return 1;
    }

    std::cout << "Generated: " << (outDir / "mafia004.230_22").string() << "\n";
    std::cout << "Generated: " << (outDir / "mafia004.230_23").string() << "\n";
    std::cout << "Generated: " << (outDir / "mafia004.230_24").string() << "\n";
    std::cout << "Plan note: " << notePath.string() << "\n";
    return 0;
}

int CmdBatch006(const fs::path& basePath, const fs::path& outDir) {
    const auto raw = mafia_save::ReadFileBytes(basePath);
    if (raw.empty()) {
        std::cerr << "Failed to read base save file: " << basePath << "\n";
        return 1;
    }

    mafia_save::SaveData base;
    std::string err;
    if (!mafia_save::ParseSave(raw, &base, &err)) {
        std::cerr << "ParseSave failed: " << err << "\n";
        return 1;
    }

    std::size_t actorHeader0Abs = static_cast<std::size_t>(-1);
    std::size_t actorHeader1Abs = static_cast<std::size_t>(-1);
    std::size_t abs = mafia_save::kFileHeaderSize;
    for (const auto& seg : base.segments) {
        if (seg.name == "actor_header_0") {
            actorHeader0Abs = abs;
        } else if (seg.name == "actor_header_1") {
            actorHeader1Abs = abs;
        }
        abs += seg.plain.size();
    }
    if (actorHeader0Abs == static_cast<std::size_t>(-1) || actorHeader1Abs == static_cast<std::size_t>(-1)) {
        std::cerr << "Expected actor_header_0 and actor_header_1 are missing in base file\n";
        return 1;
    }

    fs::create_directories(outDir);
    auto writeVariant = [&](const std::string& fileName, const mafia_save::SaveData& variant) -> bool {
        const fs::path outPath = outDir / fileName;
        std::string localErr;
        if (!WriteModified(variant, outPath, &localErr)) {
            std::cerr << "Failed to write " << outPath << ": " << localErr << "\n";
            return false;
        }
        return true;
    };

    mafia_save::SaveData v25 = base;
    if (!writeVariant("mafia004.230_25", v25)) {
        return 1;
    }

    mafia_save::SaveData v26 = base;
    if (!mafia_save::SetFileOffsetByte(&v26, actorHeader0Abs, 0x00, &err)) {
        std::cerr << "Failed to patch v26: " << err << "\n";
        return 1;
    }
    if (!writeVariant("mafia004.230_26", v26)) {
        return 1;
    }

    mafia_save::SaveData v27 = base;
    if (!mafia_save::SetFileOffsetByte(&v27, actorHeader1Abs, 0x00, &err)) {
        std::cerr << "Failed to patch v27: " << err << "\n";
        return 1;
    }
    if (!writeVariant("mafia004.230_27", v27)) {
        return 1;
    }

    std::vector<std::uint8_t> raw25;
    std::vector<std::uint8_t> raw26;
    std::vector<std::uint8_t> raw27;
    if (!mafia_save::BuildRaw(v25, &raw25) || !mafia_save::BuildRaw(v26, &raw26) || !mafia_save::BuildRaw(v27, &raw27)) {
        std::cerr << "Internal error while rebuilding variants\n";
        return 1;
    }

    const fs::path notePath = outDir / "RESULTS_SERIES_006_plan.txt";
    std::vector<std::string> lines;
    lines.push_back("Compact batch 006 (actor-header mapping tests)");
    lines.push_back("");
    lines.push_back("Base file: " + basePath.string());
    lines.push_back("Based on recovered SaveProc/SaveGameLoad actor mapping by actor name.");
    lines.push_back("");
    lines.push_back("- mafia004.230_25: control copy of base");
    lines.push_back("  Byte diff vs base: " + std::to_string(CountDiffBytes(raw, raw25)));
    lines.push_back("");
    lines.push_back("- mafia004.230_26: actor_header_0 name first byte set to 0x00");
    lines.push_back("  Absolute offset: 0x" + [&]() { std::ostringstream oss; oss << std::hex << actorHeader0Abs; return oss.str(); }());
    lines.push_back("  Byte diff vs base: " + std::to_string(CountDiffBytes(raw, raw26)));
    lines.push_back("");
    lines.push_back("- mafia004.230_27: actor_header_1 name first byte set to 0x00");
    lines.push_back("  Absolute offset: 0x" + [&]() { std::ostringstream oss; oss << std::hex << actorHeader1Abs; return oss.str(); }());
    lines.push_back("  Byte diff vs base: " + std::to_string(CountDiffBytes(raw, raw27)));
    lines.push_back("");
    lines.push_back("What to report:");
    lines.push_back("1) menu title/image/stats");
    lines.push_back("2) load progress stage");
    lines.push_back("3) whether mission starts, and if yes, any broken scripts/softlock/fail state");

    std::string content;
    for (const auto& line : lines) {
        content += line;
        content += "\n";
    }
    if (!mafia_save::WriteFileBytes(notePath, std::vector<std::uint8_t>(content.begin(), content.end()))) {
        std::cerr << "Failed to write note file: " << notePath << "\n";
        return 1;
    }

    std::cout << "Generated: " << (outDir / "mafia004.230_25").string() << "\n";
    std::cout << "Generated: " << (outDir / "mafia004.230_26").string() << "\n";
    std::cout << "Generated: " << (outDir / "mafia004.230_27").string() << "\n";
    std::cout << "Plan note: " << notePath.string() << "\n";
    return 0;
}

int CmdBatch007(const fs::path& basePath, const fs::path& outDir) {
    const auto raw = mafia_save::ReadFileBytes(basePath);
    if (raw.empty()) {
        std::cerr << "Failed to read base save file: " << basePath << "\n";
        return 1;
    }

    mafia_save::SaveData base;
    std::string err;
    if (!mafia_save::ParseSave(raw, &base, &err)) {
        std::cerr << "ParseSave failed: " << err << "\n";
        return 1;
    }

    std::size_t actorHeader1Abs = static_cast<std::size_t>(-1);
    std::size_t abs = mafia_save::kFileHeaderSize;
    for (const auto& seg : base.segments) {
        if (seg.name == "actor_header_1") {
            actorHeader1Abs = abs;
            break;
        }
        abs += seg.plain.size();
    }
    if (actorHeader1Abs == static_cast<std::size_t>(-1)) {
        std::cerr << "actor_header_1 is missing in base file\n";
        return 1;
    }

    auto setU32AtFileOffset = [&](mafia_save::SaveData* s, std::size_t off, std::uint32_t v) -> bool {
        for (int i = 0; i < 4; ++i) {
            const std::uint8_t b = static_cast<std::uint8_t>((v >> (i * 8)) & 0xFFu);
            if (!mafia_save::SetFileOffsetByte(s, off + static_cast<std::size_t>(i), b, &err)) {
                return false;
            }
        }
        return true;
    };

    fs::create_directories(outDir);
    auto writeVariant = [&](const std::string& fileName, const mafia_save::SaveData& variant) -> bool {
        const fs::path outPath = outDir / fileName;
        std::string localErr;
        if (!WriteModified(variant, outPath, &localErr)) {
            std::cerr << "Failed to write " << outPath << ": " << localErr << "\n";
            return false;
        }
        return true;
    };

    const std::size_t offModel = actorHeader1Abs + 64;
    const std::size_t offType = actorHeader1Abs + 128;
    const std::size_t offIdx = actorHeader1Abs + 136;

    mafia_save::SaveData v28 = base;
    if (!mafia_save::SetFileOffsetByte(&v28, offModel, 0x00, &err)) {
        std::cerr << "Failed to patch v28: " << err << "\n";
        return 1;
    }
    if (!writeVariant("mafia004.230_28", v28)) {
        return 1;
    }

    mafia_save::SaveData v29 = base;
    if (!setU32AtFileOffset(&v29, offType, 4u)) {
        std::cerr << "Failed to patch v29 type: " << err << "\n";
        return 1;
    }
    if (!writeVariant("mafia004.230_29", v29)) {
        return 1;
    }

    mafia_save::SaveData v30 = base;
    if (!setU32AtFileOffset(&v30, offIdx, 0u)) {
        std::cerr << "Failed to patch v30 idx: " << err << "\n";
        return 1;
    }
    if (!writeVariant("mafia004.230_30", v30)) {
        return 1;
    }

    std::vector<std::uint8_t> raw28;
    std::vector<std::uint8_t> raw29;
    std::vector<std::uint8_t> raw30;
    if (!mafia_save::BuildRaw(v28, &raw28) || !mafia_save::BuildRaw(v29, &raw29) || !mafia_save::BuildRaw(v30, &raw30)) {
        std::cerr << "Internal error while rebuilding variants\n";
        return 1;
    }

    const fs::path notePath = outDir / "RESULTS_SERIES_007_plan.txt";
    std::vector<std::string> lines;
    lines.push_back("Compact batch 007 (Tommy header field isolation)");
    lines.push_back("");
    lines.push_back("Base file: " + basePath.string());
    lines.push_back("actor_header_1 base offset: 0x" + [&]() { std::ostringstream oss; oss << std::hex << actorHeader1Abs; return oss.str(); }());
    lines.push_back("");
    lines.push_back("- mafia004.230_28: actor_header_1 model first byte -> 0x00");
    lines.push_back("  Offset: 0x" + [&]() { std::ostringstream oss; oss << std::hex << offModel; return oss.str(); }());
    lines.push_back("  Hypothesis: model name is less critical than actor name for matching.");
    lines.push_back("  Byte diff vs base: " + std::to_string(CountDiffBytes(raw, raw28)));
    lines.push_back("");
    lines.push_back("- mafia004.230_29: actor_header_1 type 2 -> 4");
    lines.push_back("  Offset: 0x" + [&]() { std::ostringstream oss; oss << std::hex << offType; return oss.str(); }());
    lines.push_back("  Hypothesis: wrong actor type should change load/apply path strongly.");
    lines.push_back("  Byte diff vs base: " + std::to_string(CountDiffBytes(raw, raw29)));
    lines.push_back("");
    lines.push_back("- mafia004.230_30: actor_header_1 idx 0xFFFFFFFF -> 0");
    lines.push_back("  Offset: 0x" + [&]() { std::ostringstream oss; oss << std::hex << offIdx; return oss.str(); }());
    lines.push_back("  Hypothesis: Tommy may be written into car slot table, causing script/state anomalies.");
    lines.push_back("  Byte diff vs base: " + std::to_string(CountDiffBytes(raw, raw30)));
    lines.push_back("");
    lines.push_back("What to report:");
    lines.push_back("1) menu title/image/stats");
    lines.push_back("2) load progress stage");
    lines.push_back("3) mission start/fail/softlock details");

    std::string content;
    for (const auto& line : lines) {
        content += line;
        content += "\n";
    }
    if (!mafia_save::WriteFileBytes(notePath, std::vector<std::uint8_t>(content.begin(), content.end()))) {
        std::cerr << "Failed to write note file: " << notePath << "\n";
        return 1;
    }

    std::cout << "Generated: " << (outDir / "mafia004.230_28").string() << "\n";
    std::cout << "Generated: " << (outDir / "mafia004.230_29").string() << "\n";
    std::cout << "Generated: " << (outDir / "mafia004.230_30").string() << "\n";
    std::cout << "Plan note: " << notePath.string() << "\n";
    return 0;
}

int CmdBatch008(const fs::path& basePath, const fs::path& outDir) {
    const auto raw = mafia_save::ReadFileBytes(basePath);
    if (raw.empty()) {
        std::cerr << "Failed to read base save file: " << basePath << "\n";
        return 1;
    }

    mafia_save::SaveData base;
    std::string err;
    if (!mafia_save::ParseSave(raw, &base, &err)) {
        std::cerr << "ParseSave failed: " << err << "\n";
        return 1;
    }

    std::size_t actorHeader1Abs = static_cast<std::size_t>(-1);
    std::size_t abs = mafia_save::kFileHeaderSize;
    for (const auto& seg : base.segments) {
        if (seg.name == "actor_header_1") {
            actorHeader1Abs = abs;
            break;
        }
        abs += seg.plain.size();
    }
    if (actorHeader1Abs == static_cast<std::size_t>(-1)) {
        std::cerr << "actor_header_1 is missing in base file\n";
        return 1;
    }

    auto setU32AtFileOffset = [&](mafia_save::SaveData* s, std::size_t off, std::uint32_t v) -> bool {
        for (int i = 0; i < 4; ++i) {
            const std::uint8_t b = static_cast<std::uint8_t>((v >> (i * 8)) & 0xFFu);
            if (!mafia_save::SetFileOffsetByte(s, off + static_cast<std::size_t>(i), b, &err)) {
                return false;
            }
        }
        return true;
    };

    auto setCStringAtFileOffset = [&](mafia_save::SaveData* s, std::size_t off, std::size_t cap, const std::string& str) -> bool {
        if (str.size() + 1 > cap) {
            err = "string is too long for fixed-size field";
            return false;
        }
        for (std::size_t i = 0; i < cap; ++i) {
            std::uint8_t b = 0;
            if (i < str.size()) {
                b = static_cast<std::uint8_t>(str[i]);
            }
            if (!mafia_save::SetFileOffsetByte(s, off + i, b, &err)) {
                return false;
            }
        }
        return true;
    };

    fs::create_directories(outDir);
    auto writeVariant = [&](const std::string& fileName, const mafia_save::SaveData& variant) -> bool {
        const fs::path outPath = outDir / fileName;
        std::string localErr;
        if (!WriteModified(variant, outPath, &localErr)) {
            std::cerr << "Failed to write " << outPath << ": " << localErr << "\n";
            return false;
        }
        return true;
    };

    const std::size_t offName = actorHeader1Abs + 0;
    const std::size_t offModel = actorHeader1Abs + 64;
    const std::size_t offType = actorHeader1Abs + 128;

    mafia_save::SaveData v31 = base;
    if (!setCStringAtFileOffset(&v31, offName, 64, "Jommy")) {
        std::cerr << "Failed to patch v31 name: " << err << "\n";
        return 1;
    }
    if (!writeVariant("mafia004.230_31", v31)) {
        return 1;
    }

    mafia_save::SaveData v32 = base;
    if (!setU32AtFileOffset(&v32, offType, 27u)) {
        std::cerr << "Failed to patch v32 type: " << err << "\n";
        return 1;
    }
    if (!writeVariant("mafia004.230_32", v32)) {
        return 1;
    }

    mafia_save::SaveData v33 = base;
    if (!setCStringAtFileOffset(&v33, offModel, 64, "Tommy.i3d")) {
        std::cerr << "Failed to patch v33 model: " << err << "\n";
        return 1;
    }
    if (!writeVariant("mafia004.230_33", v33)) {
        return 1;
    }

    std::vector<std::uint8_t> raw31;
    std::vector<std::uint8_t> raw32;
    std::vector<std::uint8_t> raw33;
    if (!mafia_save::BuildRaw(v31, &raw31) || !mafia_save::BuildRaw(v32, &raw32) || !mafia_save::BuildRaw(v33, &raw33)) {
        std::cerr << "Internal error while rebuilding variants\n";
        return 1;
    }

    const fs::path notePath = outDir / "RESULTS_SERIES_008_plan.txt";
    std::vector<std::string> lines;
    lines.push_back("Compact batch 008 (Tommy strict-field tests)");
    lines.push_back("");
    lines.push_back("Base file: " + basePath.string());
    lines.push_back("actor_header_1 base offset: 0x" + [&]() { std::ostringstream oss; oss << std::hex << actorHeader1Abs; return oss.str(); }());
    lines.push_back("");
    lines.push_back("- mafia004.230_31: Tommy name -> Jommy");
    lines.push_back("  Offset: 0x" + [&]() { std::ostringstream oss; oss << std::hex << offName; return oss.str(); }());
    lines.push_back("  Hypothesis: even one-char mismatch should break actor matching like _27.");
    lines.push_back("  Byte diff vs base: " + std::to_string(CountDiffBytes(raw, raw31)));
    lines.push_back("");
    lines.push_back("- mafia004.230_32: Tommy type 2 -> 27");
    lines.push_back("  Offset: 0x" + [&]() { std::ostringstream oss; oss << std::hex << offType; return oss.str(); }());
    lines.push_back("  Hypothesis: type 27 may behave closer to human/player than type 4.");
    lines.push_back("  Byte diff vs base: " + std::to_string(CountDiffBytes(raw, raw32)));
    lines.push_back("");
    lines.push_back("- mafia004.230_33: Tommy model -> Tommy.i3d");
    lines.push_back("  Offset: 0x" + [&]() { std::ostringstream oss; oss << std::hex << offModel; return oss.str(); }());
    lines.push_back("  Hypothesis: valid non-empty model should avoid invisibility from _28.");
    lines.push_back("  Byte diff vs base: " + std::to_string(CountDiffBytes(raw, raw33)));
    lines.push_back("");
    lines.push_back("What to report:");
    lines.push_back("1) menu title/image/stats");
    lines.push_back("2) mission behavior on start");
    lines.push_back("3) player body visibility, controls, fail/restart behavior");

    std::string content;
    for (const auto& line : lines) {
        content += line;
        content += "\n";
    }
    if (!mafia_save::WriteFileBytes(notePath, std::vector<std::uint8_t>(content.begin(), content.end()))) {
        std::cerr << "Failed to write note file: " << notePath << "\n";
        return 1;
    }

    std::cout << "Generated: " << (outDir / "mafia004.230_31").string() << "\n";
    std::cout << "Generated: " << (outDir / "mafia004.230_32").string() << "\n";
    std::cout << "Generated: " << (outDir / "mafia004.230_33").string() << "\n";
    std::cout << "Plan note: " << notePath.string() << "\n";
    return 0;
}

}  // namespace

int main(int argc, char** argv) {
    if (argc < 2) {
        PrintUsage();
        return 1;
    }

    const std::string cmd = argv[1];

    if (cmd == "inspect") {
        if (argc != 3) {
            PrintUsage();
            return 1;
        }
        return CmdInspect(argv[2]);
    }

    if (cmd == "set-hp") {
        if (argc != 5) {
            PrintUsage();
            return 1;
        }
        const auto hpOpt = ParseU32(argv[4]);
        if (!hpOpt.has_value()) {
            std::cerr << "Invalid hp_percent: " << argv[4] << "\n";
            return 1;
        }
        return CmdSetHp(argv[2], argv[3], *hpOpt);
    }

    if (cmd == "batch005") {
        if (argc != 4) {
            PrintUsage();
            return 1;
        }
        return CmdBatch005(argv[2], argv[3]);
    }

    if (cmd == "batch006") {
        if (argc != 4) {
            PrintUsage();
            return 1;
        }
        return CmdBatch006(argv[2], argv[3]);
    }

    if (cmd == "batch007") {
        if (argc != 4) {
            PrintUsage();
            return 1;
        }
        return CmdBatch007(argv[2], argv[3]);
    }

    if (cmd == "batch008") {
        if (argc != 4) {
            PrintUsage();
            return 1;
        }
        return CmdBatch008(argv[2], argv[3]);
    }

    PrintUsage();
    return 1;
}
