#include "gvas.hpp"

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <limits>
#include <map>
#include <string>
#include <utility>
#include <vector>

namespace fs = std::filesystem;

namespace {

struct SaveRecord {
    gvas::SaveFileMeta meta;
    std::vector<std::uint8_t> bytes;
    bool valid = false;
};

struct PairStats {
    int slot = -1;
    std::size_t lenA = 0;
    std::size_t lenB = 0;
    std::size_t equalPrefix = 0;
    std::size_t comparedPayloadBytes = 0;
    std::size_t equalPayloadBytes = 0;
};

void PrintUsage() {
    std::cout << "Usage:\n"
              << "  payload_study cross-profile <save_dir> <profileA> <profileB> [max_payload_offsets]\n"
              << "  payload_study same-profile <save_dir> <profile> [top_n]\n";
}

bool LoadSaveRecords(const fs::path& saveDir, std::vector<SaveRecord>* out) {
    if (out == nullptr) {
        return false;
    }
    out->clear();
    if (!fs::exists(saveDir) || !fs::is_directory(saveDir)) {
        return false;
    }

    for (const auto& entry : fs::directory_iterator(saveDir)) {
        if (!entry.is_regular_file()) {
            continue;
        }
        gvas::SaveFileMeta meta;
        if (!gvas::ParseSaveFileMeta(entry.path(), &meta)) {
            continue;
        }

        SaveRecord rec;
        rec.meta = meta;
        rec.bytes = gvas::ReadFileBytes(entry.path());

        gvas::Header header;
        if (gvas::ReadHeader(rec.bytes, &header)) {
            std::string checkErr;
            const std::uint16_t mission = gvas::DecodeMission(header);
            if (gvas::ValidateMissionChecks(header, &checkErr) && meta.slot == static_cast<int>(mission)) {
                rec.valid = true;
            }
        }
        out->push_back(std::move(rec));
    }
    return true;
}

std::size_t CommonPrefix(const std::vector<std::uint8_t>& a, const std::vector<std::uint8_t>& b) {
    const std::size_t n = std::min(a.size(), b.size());
    std::size_t i = 0;
    while (i < n && a[i] == b[i]) {
        ++i;
    }
    return i;
}

PairStats ComparePayload(const SaveRecord& a, const SaveRecord& b) {
    PairStats st;
    st.slot = a.meta.slot;
    st.lenA = a.bytes.size();
    st.lenB = b.bytes.size();
    st.equalPrefix = CommonPrefix(a.bytes, b.bytes);

    if (a.bytes.size() <= gvas::kHeaderSize || b.bytes.size() <= gvas::kHeaderSize) {
        return st;
    }

    const std::size_t cmp = std::min(a.bytes.size(), b.bytes.size()) - gvas::kHeaderSize;
    st.comparedPayloadBytes = cmp;
    for (std::size_t i = 0; i < cmp; ++i) {
        if (a.bytes[gvas::kHeaderSize + i] == b.bytes[gvas::kHeaderSize + i]) {
            ++st.equalPayloadBytes;
        }
    }
    return st;
}

int CmdCrossProfile(const fs::path& saveDir, int profileA, int profileB, int maxPayloadOffsets) {
    std::vector<SaveRecord> all;
    if (!LoadSaveRecords(saveDir, &all)) {
        std::cerr << "Failed to read save directory: " << saveDir << "\n";
        return 1;
    }

    std::map<int, SaveRecord> aBySlot;
    std::map<int, SaveRecord> bBySlot;
    for (const auto& rec : all) {
        if (!rec.valid) {
            continue;
        }
        if (rec.meta.profile == profileA) {
            aBySlot[rec.meta.slot] = rec;
        } else if (rec.meta.profile == profileB) {
            bBySlot[rec.meta.slot] = rec;
        }
    }

    std::vector<PairStats> pairs;
    for (const auto& [slot, ra] : aBySlot) {
        auto it = bBySlot.find(slot);
        if (it == bBySlot.end()) {
            continue;
        }
        pairs.push_back(ComparePayload(ra, it->second));
    }
    if (pairs.empty()) {
        std::cerr << "No common valid missions for profiles " << profileA << " and " << profileB << "\n";
        return 1;
    }

    std::cout << "slot,lenA,lenB,equal_prefix,payload_compared,payload_equal,payload_equal_ratio\n";
    for (const auto& st : pairs) {
        const double ratio =
            st.comparedPayloadBytes ? static_cast<double>(st.equalPayloadBytes) / st.comparedPayloadBytes : 0.0;
        std::cout << st.slot << "," << st.lenA << "," << st.lenB << "," << st.equalPrefix << "," << st.comparedPayloadBytes
                  << "," << st.equalPayloadBytes << "," << std::fixed << std::setprecision(6) << ratio << "\n";
    }

    std::size_t minPrefix = std::numeric_limits<std::size_t>::max();
    double sumRatio = 0.0;
    for (const auto& st : pairs) {
        minPrefix = std::min(minPrefix, st.equalPrefix);
        const double ratio =
            st.comparedPayloadBytes ? static_cast<double>(st.equalPayloadBytes) / st.comparedPayloadBytes : 0.0;
        sumRatio += ratio;
    }
    const double avgRatio = sumRatio / static_cast<double>(pairs.size());

    std::cout << "summary,pairs=" << pairs.size() << ",min_prefix=" << minPrefix << ",avg_payload_equal_ratio="
              << std::fixed << std::setprecision(6) << avgRatio << "\n";

    if (maxPayloadOffsets <= 0) {
        return 0;
    }
    std::cout << "offset,pairs_with_byte,equal_pairs,equal_ratio\n";
    for (int off = 0; off < maxPayloadOffsets; ++off) {
        int total = 0;
        int eq = 0;
        for (const auto& [slot, ra] : aBySlot) {
            auto it = bBySlot.find(slot);
            if (it == bBySlot.end()) {
                continue;
            }
            const auto& rb = it->second;
            const std::size_t ia = gvas::kHeaderSize + static_cast<std::size_t>(off);
            if (ia >= ra.bytes.size() || ia >= rb.bytes.size()) {
                continue;
            }
            ++total;
            if (ra.bytes[ia] == rb.bytes[ia]) {
                ++eq;
            }
        }
        if (total == 0) {
            continue;
        }
        const double ratio = static_cast<double>(eq) / static_cast<double>(total);
        std::cout << off << "," << total << "," << eq << "," << std::fixed << std::setprecision(6) << ratio << "\n";
    }

    return 0;
}

int CmdSameProfile(const fs::path& saveDir, int profile, int topN) {
    std::vector<SaveRecord> all;
    if (!LoadSaveRecords(saveDir, &all)) {
        std::cerr << "Failed to read save directory: " << saveDir << "\n";
        return 1;
    }

    std::vector<SaveRecord> rows;
    for (const auto& rec : all) {
        if (rec.valid && rec.meta.profile == profile && rec.meta.slot <= 561) {
            rows.push_back(rec);
        }
    }
    if (rows.size() < 2) {
        std::cerr << "Not enough valid saves for profile " << profile << "\n";
        return 1;
    }

    struct ClosePair {
        int slotA;
        int slotB;
        std::size_t size;
        std::size_t comparedPayload;
        std::size_t diffPayload;
        double diffRatio;
        std::size_t prefix;
    };
    std::vector<ClosePair> out;

    for (std::size_t i = 0; i < rows.size(); ++i) {
        for (std::size_t j = i + 1; j < rows.size(); ++j) {
            if (rows[i].bytes.size() != rows[j].bytes.size()) {
                continue;
            }
            if (rows[i].bytes.size() <= gvas::kHeaderSize) {
                continue;
            }
            const std::size_t compared = rows[i].bytes.size() - gvas::kHeaderSize;
            std::size_t diff = 0;
            for (std::size_t k = 0; k < compared; ++k) {
                if (rows[i].bytes[gvas::kHeaderSize + k] != rows[j].bytes[gvas::kHeaderSize + k]) {
                    ++diff;
                }
            }
            ClosePair p{};
            p.slotA = rows[i].meta.slot;
            p.slotB = rows[j].meta.slot;
            p.size = rows[i].bytes.size();
            p.comparedPayload = compared;
            p.diffPayload = diff;
            p.diffRatio = static_cast<double>(diff) / static_cast<double>(compared);
            p.prefix = CommonPrefix(rows[i].bytes, rows[j].bytes);
            out.push_back(p);
        }
    }

    std::sort(out.begin(), out.end(), [](const ClosePair& a, const ClosePair& b) {
        if (std::fabs(a.diffRatio - b.diffRatio) > 1e-12) {
            return a.diffRatio < b.diffRatio;
        }
        return a.size < b.size;
    });

    if (topN < 1) {
        topN = 1;
    }
    if (topN > static_cast<int>(out.size())) {
        topN = static_cast<int>(out.size());
    }

    std::cout << "slotA,slotB,size,payload_compared,payload_diff,payload_diff_ratio,equal_prefix\n";
    for (int i = 0; i < topN; ++i) {
        const auto& p = out[static_cast<std::size_t>(i)];
        std::cout << p.slotA << "," << p.slotB << "," << p.size << "," << p.comparedPayload << "," << p.diffPayload
                  << "," << std::fixed << std::setprecision(6) << p.diffRatio << "," << p.prefix << "\n";
    }
    return 0;
}

}  // namespace

int main(int argc, char** argv) {
    if (argc < 2) {
        PrintUsage();
        return 1;
    }
    const std::string cmd = argv[1];

    if (cmd == "cross-profile") {
        if (argc < 5 || argc > 6) {
            PrintUsage();
            return 1;
        }
        const fs::path dir = argv[2];
        const int profileA = std::stoi(argv[3]);
        const int profileB = std::stoi(argv[4]);
        int maxOffsets = 128;
        if (argc == 6) {
            maxOffsets = std::stoi(argv[5]);
        }
        return CmdCrossProfile(dir, profileA, profileB, maxOffsets);
    }

    if (cmd == "same-profile") {
        if (argc < 4 || argc > 5) {
            PrintUsage();
            return 1;
        }
        const fs::path dir = argv[2];
        const int profile = std::stoi(argv[3]);
        int topN = 20;
        if (argc == 5) {
            topN = std::stoi(argv[4]);
        }
        return CmdSameProfile(dir, profile, topN);
    }

    PrintUsage();
    return 1;
}

