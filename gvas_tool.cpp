#include "gvas.hpp"

#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <map>
#include <optional>
#include <string>

namespace fs = std::filesystem;

namespace {

void PrintUsage() {
    std::cout << "Usage:\n"
              << "  gvas_tool inspect <save_file>\n"
              << "  gvas_tool set-mission <input_file> <output_file> <mission_id> [--table-dir <save_dir>] "
                 "[--preserve-word34]\n";
}

std::optional<std::uint16_t> ParseMissionId(const std::string& s) {
    char* end = nullptr;
    const long v = std::strtol(s.c_str(), &end, 10);
    if (end == nullptr || *end != '\0' || v < 0 || v > 65535) {
        return std::nullopt;
    }
    return static_cast<std::uint16_t>(v);
}

int CmdInspect(const fs::path& filePath) {
    const auto bytes = gvas::ReadFileBytes(filePath);
    if (bytes.empty()) {
        std::cerr << "Failed to read file or file is empty: " << filePath << "\n";
        return 1;
    }

    gvas::Header h;
    std::string err;
    if (!gvas::ReadHeader(bytes, &h, &err)) {
        std::cerr << "ReadHeader failed: " << err << "\n";
        return 1;
    }

    const auto mission0 = gvas::DecodeMissionFromD0(h.d0);
    const auto mission1 = gvas::DecodeMissionFromD1(h.d1);
    const auto mission2 = gvas::DecodeMissionFromD2(h.d2);
    std::string checkErr;
    const bool okChecks = gvas::ValidateMissionChecks(h, &checkErr);

    std::cout << "file=" << filePath.string() << "\n";
    std::cout << "size=" << bytes.size() << " header_size=" << gvas::kHeaderSize
              << " payload_size=" << (bytes.size() - gvas::kHeaderSize) << "\n";
    std::cout << "version=" << h.version << "\n";
    std::cout << std::hex << std::uppercase << std::setfill('0');
    std::cout << "d0=0x" << std::setw(8) << h.d0 << " d1=0x" << std::setw(8) << h.d1 << " d2=0x" << std::setw(8)
              << h.d2 << " d3=0x" << std::setw(8) << h.d3 << " d4=0x" << std::setw(8) << h.d4 << " d5=0x"
              << std::setw(8) << h.d5 << "\n";
    std::cout << std::dec;
    std::cout << "mission_from_d0=" << mission0 << " mission_from_d1=" << mission1 << " mission_from_d2=" << mission2
              << "\n";
    std::cout << "checks=" << (okChecks ? "OK" : "FAIL") << "\n";
    if (!okChecks) {
        std::cout << "check_error=" << checkErr << "\n";
    }
    return 0;
}

int CmdSetMission(int argc, char** argv) {
    if (argc < 5) {
        PrintUsage();
        return 1;
    }

    const fs::path inPath = argv[2];
    const fs::path outPath = argv[3];
    const auto missionOpt = ParseMissionId(argv[4]);
    if (!missionOpt.has_value()) {
        std::cerr << "Invalid mission id: " << argv[4] << "\n";
        return 1;
    }
    const std::uint16_t mission = *missionOpt;

    fs::path tableDir = inPath.parent_path().empty() ? fs::path(".") : inPath.parent_path();
    bool preserveWord34 = false;
    for (int i = 5; i < argc; ++i) {
        const std::string arg = argv[i];
        if (arg == "--preserve-word34") {
            preserveWord34 = true;
            continue;
        }
        if (arg == "--table-dir") {
            if (i + 1 >= argc) {
                std::cerr << "--table-dir requires a path\n";
                return 1;
            }
            tableDir = argv[++i];
            continue;
        }
        std::cerr << "Unknown argument: " << arg << "\n";
        return 1;
    }

    auto bytes = gvas::ReadFileBytes(inPath);
    if (bytes.empty()) {
        std::cerr << "Failed to read input file: " << inPath << "\n";
        return 1;
    }

    gvas::Header h;
    std::string err;
    if (!gvas::ReadHeader(bytes, &h, &err)) {
        std::cerr << "ReadHeader failed: " << err << "\n";
        return 1;
    }

    const std::uint16_t oldMission = gvas::DecodeMission(h);
    gvas::EncodeMissionCore(&h, mission);

    std::string wordMessage;
    const auto table = gvas::BuildMissionWord34Table(tableDir);
    bool wordApplied = gvas::TryApplyMissionWord34(&h, mission, table, &wordMessage);
    if (!wordApplied && !preserveWord34) {
        std::cerr << "Cannot set mission safely: " << wordMessage
                  << ". Use --preserve-word34 to keep previous d3/d4.\n";
        return 1;
    }

    if (!wordApplied && preserveWord34) {
        wordMessage = "kept existing d3/d4 from input file";
    }

    if (!gvas::WriteHeader(h, &bytes, &err)) {
        std::cerr << "WriteHeader failed: " << err << "\n";
        return 1;
    }
    if (!gvas::WriteFileBytes(outPath, bytes)) {
        std::cerr << "Failed to write output file: " << outPath << "\n";
        return 1;
    }

    std::string checkErr;
    const bool ok = gvas::ValidateMissionChecks(h, &checkErr);
    std::cout << "input=" << inPath.string() << "\n";
    std::cout << "output=" << outPath.string() << "\n";
    std::cout << "old_mission=" << oldMission << " new_mission=" << mission << "\n";
    std::cout << "d3d4=" << wordMessage << "\n";
    std::cout << "header_checks_after_write=" << (ok ? "OK" : ("FAIL: " + checkErr)) << "\n";
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
    if (cmd == "set-mission") {
        return CmdSetMission(argc, argv);
    }

    PrintUsage();
    return 1;
}

