#ifndef NOMINMAX
#define NOMINMAX
#endif
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "mafia_save.hpp"

#include <windows.h>
#include <commctrl.h>
#include <commdlg.h>

#include <algorithm>
#include <cctype>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <optional>
#include <sstream>
#include <string>
#include <vector>

namespace fs = std::filesystem;

namespace {

constexpr int ID_BTN_OPEN = 1001;
constexpr int ID_BTN_SAVE = 1002;
constexpr int ID_BTN_RESET = 1003;
constexpr int ID_TAB = 1004;

constexpr int ID_EDIT_HP = 1101;
constexpr int ID_EDIT_DATE = 1102;
constexpr int ID_EDIT_TIME = 1103;
constexpr int ID_EDIT_SLOT = 1104;
constexpr int ID_EDIT_MCODE = 1105;
constexpr int ID_EDIT_MNAME = 1106;

constexpr int ID_EDIT_GH_MARKER = 1401;
constexpr int ID_EDIT_GH_FIELD_A = 1402;
constexpr int ID_EDIT_GH_FIELD_B = 1403;
constexpr int ID_EDIT_GH_MISSION_ID = 1404;
constexpr int ID_EDIT_GH_TIMER_ON = 1405;
constexpr int ID_EDIT_GH_TIMER_INTERVAL = 1406;
constexpr int ID_EDIT_GH_TIMER_A = 1407;
constexpr int ID_EDIT_GH_TIMER_B = 1408;
constexpr int ID_EDIT_GH_TIMER_C = 1409;
constexpr int ID_EDIT_GH_SCORE_ON = 1410;
constexpr int ID_EDIT_GH_SCORE_VALUE = 1411;
constexpr int ID_EDIT_GH_SCRIPT_ENTRIES = 1412;
constexpr int ID_EDIT_GH_SCRIPT_CHUNKS = 1413;
constexpr int ID_EDIT_PROG_OFFSET = 1414;
constexpr int ID_EDIT_PROG_VARS = 1415;
constexpr int ID_EDIT_PROG_ACTORS = 1416;
constexpr int ID_EDIT_PROG_FRAMES = 1417;
constexpr int ID_EDIT_PROG_CMD_BLOCK = 1418;
constexpr int ID_EDIT_PROG_VAR_INDEX = 1419;
constexpr int ID_EDIT_PROG_VAR_VALUE = 1420;
constexpr int ID_BTN_PROG_LOAD_VAR = 1421;
constexpr int ID_LIST_PROG_VARS = 1422;
constexpr int ID_BTN_PROG_RELOAD_TABLE = 1423;

constexpr int ID_LIST_ACTORS = 1201;
constexpr int ID_EDIT_ANAME = 1202;
constexpr int ID_EDIT_AMODEL = 1203;
constexpr int ID_EDIT_ATYPE = 1204;
constexpr int ID_EDIT_AIDX = 1205;
constexpr int ID_EDIT_APAYLOAD = 1206;
constexpr int ID_BTN_APPLY_ACTOR = 1207;
constexpr int ID_EDIT_FILTER_NAME = 1208;
constexpr int ID_EDIT_FILTER_TYPE = 1209;
constexpr int ID_BTN_FILTER_APPLY = 1210;
constexpr int ID_BTN_FILTER_CLEAR = 1211;
constexpr int ID_BTN_CLONE_ACTOR = 1212;
constexpr int ID_EDIT_POS_X = 1213;
constexpr int ID_EDIT_POS_Y = 1214;
constexpr int ID_EDIT_POS_Z = 1215;
constexpr int ID_STATIC_COORD_HINT = 1216;
constexpr int ID_EDIT_PSTATE = 1217;
constexpr int ID_EDIT_PID = 1218;
constexpr int ID_EDIT_PACTIVE = 1219;
constexpr int ID_EDIT_PREMOVE = 1220;
constexpr int ID_EDIT_PFRAME = 1221;
constexpr int ID_EDIT_DIR_X = 1222;
constexpr int ID_EDIT_DIR_Y = 1223;
constexpr int ID_EDIT_DIR_Z = 1224;
constexpr int ID_EDIT_ANIM_ID = 1225;
constexpr int ID_EDIT_ROT_W = 1226;
constexpr int ID_EDIT_ROT_X = 1227;
constexpr int ID_EDIT_ROT_Y = 1228;
constexpr int ID_EDIT_ROT_Z = 1229;
constexpr int ID_EDIT_CAR_FUEL = 1230;
constexpr int ID_EDIT_CAR_FLOW = 1231;
constexpr int ID_EDIT_CAR_ENG_NORM = 1232;
constexpr int ID_EDIT_CAR_ENG_CALC = 1233;
constexpr int ID_EDIT_HUMAN_SEAT = 1234;
constexpr int ID_EDIT_HUMAN_CROUCH = 1235;
constexpr int ID_EDIT_HUMAN_AIM = 1236;
constexpr int ID_EDIT_HUMAN_SHOOT_X = 1237;
constexpr int ID_EDIT_HUMAN_SHOOT_Y = 1238;
constexpr int ID_EDIT_HUMAN_SHOOT_Z = 1239;
constexpr int ID_EDIT_CAR_SPEED_LIMIT = 1240;
constexpr int ID_EDIT_CAR_LAST_GEAR = 1241;
constexpr int ID_EDIT_CAR_GEAR = 1242;
constexpr int ID_EDIT_CAR_GEARBOX_FLAG = 1243;
constexpr int ID_EDIT_CAR_DISABLE_ENGINE = 1244;
constexpr int ID_EDIT_CAR_ENGINE_ON = 1245;
constexpr int ID_EDIT_CAR_IS_ENGINE_ON = 1246;
constexpr int ID_EDIT_CAR_ODOMETER = 1247;
constexpr int ID_EDIT_INV_MODE = 1301;
constexpr int ID_EDIT_INV_FLAG = 1302;
constexpr int ID_EDIT_INV_SEL_ID = 1303;
constexpr int ID_EDIT_INV_SEL_LOADED = 1304;
constexpr int ID_EDIT_INV_SEL_HIDDEN = 1305;
constexpr int ID_EDIT_INV_COAT_ID = 1306;
constexpr int ID_EDIT_INV_COAT_LOADED = 1307;
constexpr int ID_EDIT_INV_COAT_HIDDEN = 1308;
constexpr int ID_EDIT_INV_S1_ID = 1309;
constexpr int ID_EDIT_INV_S1_LOADED = 1310;
constexpr int ID_EDIT_INV_S1_HIDDEN = 1311;
constexpr int ID_EDIT_INV_S2_ID = 1312;
constexpr int ID_EDIT_INV_S2_LOADED = 1313;
constexpr int ID_EDIT_INV_S2_HIDDEN = 1314;
constexpr int ID_EDIT_INV_S3_ID = 1315;
constexpr int ID_EDIT_INV_S3_LOADED = 1316;
constexpr int ID_EDIT_INV_S3_HIDDEN = 1317;
constexpr int ID_EDIT_INV_S4_ID = 1318;
constexpr int ID_EDIT_INV_S4_LOADED = 1319;
constexpr int ID_EDIT_INV_S4_HIDDEN = 1320;
constexpr int ID_EDIT_INV_S5_ID = 1321;
constexpr int ID_EDIT_INV_S5_LOADED = 1322;
constexpr int ID_EDIT_INV_S5_HIDDEN = 1323;
constexpr int ID_EDIT_INV_SEL_UNK = 1324;
constexpr int ID_EDIT_INV_COAT_UNK = 1325;
constexpr int ID_EDIT_INV_S1_UNK = 1326;
constexpr int ID_EDIT_INV_S2_UNK = 1327;
constexpr int ID_EDIT_INV_S3_UNK = 1328;
constexpr int ID_EDIT_INV_S4_UNK = 1329;
constexpr int ID_EDIT_INV_S5_UNK = 1330;
constexpr int ID_LIST_CARS = 1501;
constexpr int ID_EDIT_CAR_TAB_NAME = 1502;
constexpr int ID_EDIT_CAR_TAB_MODEL = 1503;
constexpr int ID_EDIT_CAR_TAB_IDX = 1504;
constexpr int ID_EDIT_CAR_TAB_POS_X = 1505;
constexpr int ID_EDIT_CAR_TAB_POS_Y = 1506;
constexpr int ID_EDIT_CAR_TAB_POS_Z = 1507;
constexpr int ID_EDIT_CAR_TAB_ROT_W = 1508;
constexpr int ID_EDIT_CAR_TAB_ROT_X = 1509;
constexpr int ID_EDIT_CAR_TAB_ROT_Y = 1510;
constexpr int ID_EDIT_CAR_TAB_ROT_Z = 1511;
constexpr int ID_EDIT_CAR_TAB_FUEL = 1512;
constexpr int ID_EDIT_CAR_TAB_SPEED = 1513;
constexpr int ID_EDIT_CAR_TAB_ODOMETER = 1514;
constexpr int ID_EDIT_CAR_TAB_ENGINE_ON = 1515;
constexpr int ID_BTN_APPLY_CAR = 1516;
constexpr int ID_LIST_GARAGE = 1521;
constexpr int ID_EDIT_GARAGE_SLOT = 1522;
constexpr int ID_EDIT_GARAGE_A = 1523;
constexpr int ID_EDIT_GARAGE_B = 1524;
constexpr int ID_EDIT_GARAGE_A_HEX = 1525;
constexpr int ID_EDIT_GARAGE_B_HEX = 1526;
constexpr int ID_EDIT_GARAGE_A_LOW16 = 1527;
constexpr int ID_EDIT_GARAGE_B_LOW16 = 1528;
constexpr int ID_EDIT_GARAGE_A_HI8 = 1529;
constexpr int ID_EDIT_GARAGE_B_HI8 = 1530;
constexpr int ID_BTN_APPLY_GARAGE = 1531;
constexpr int ID_BTN_CLEAR_GARAGE = 1532;
constexpr int ID_COMBO_GARAGE_A_CAR = 1533;
constexpr int ID_COMBO_GARAGE_B_CAR = 1534;
constexpr int ID_EDIT_GARAGE_A_FLAGS = 1535;
constexpr int ID_EDIT_GARAGE_B_FLAGS = 1536;
constexpr int ID_BTN_GARAGE_SYNC_B = 1537;
constexpr int ID_EDIT_GARAGE_A_DECODED = 1538;
constexpr int ID_EDIT_GARAGE_B_DECODED = 1539;
constexpr int ID_EDIT_GARAGE_A_COLOR = 1540;
constexpr int ID_EDIT_GARAGE_B_COLOR = 1541;
constexpr int ID_SCROLL_ACTORS = 1701;
constexpr int ID_STATIC_PATH = 1601;
constexpr int ID_STATIC_INFO = 1602;
constexpr int ID_STATIC_STATUS = 1603;
constexpr int ID_STATIC_WARNING = 1604;

struct AppState {
    bool loaded = false;
    fs::path inputPath;
    std::vector<std::uint8_t> raw;
    mafia_save::SaveData save;
    std::vector<std::size_t> actorHeaders;
    std::vector<std::size_t> filteredActorHeaders;
    std::vector<std::size_t> carHeaders;
    int selectedActor = -1;
    int selectedCar = -1;
    int selectedGarageSlot = -1;
    bool garageCatalogLoaded = false;
    bool garageCatalogEmbedded = false;
    fs::path garageCatalogPath;
    std::string filterName;
    std::optional<std::uint32_t> filterType;
    int actorsRightScroll = 0;
    int actorsRightScrollMax = 0;
};

struct GarageCarCatalogEntry {
    std::uint32_t index = 0;
    std::string code;
    std::string model;
    std::string shadow;
    std::string displayName;
};

struct Ui {
    HWND fileLabel = nullptr;
    HWND openBtn = nullptr;
    HWND path = nullptr;
    HWND info = nullptr;
    HWND warning = nullptr;
    HWND status = nullptr;

    HWND tab = nullptr;
    HWND pageMain = nullptr;
    HWND pageActors = nullptr;
    HWND pageMission = nullptr;
    HWND pageCars = nullptr;
    HWND pageGarage = nullptr;
    HWND actorsScroll = nullptr;

    HWND hp = nullptr;
    HWND date = nullptr;
    HWND time = nullptr;
    HWND slot = nullptr;
    HWND mcode = nullptr;
    HWND mname = nullptr;
    HWND mainTitle = nullptr;
    HWND hpLabel = nullptr;
    HWND dateLabel = nullptr;
    HWND timeLabel = nullptr;
    HWND slotLabel = nullptr;
    HWND mcodeLabel = nullptr;
    HWND mnameLabel = nullptr;

    HWND missionTitle = nullptr;
    HWND ghMarker = nullptr;
    HWND ghFieldA = nullptr;
    HWND ghFieldB = nullptr;
    HWND ghMissionId = nullptr;
    HWND ghTimerOn = nullptr;
    HWND ghTimerInterval = nullptr;
    HWND ghTimerA = nullptr;
    HWND ghTimerB = nullptr;
    HWND ghTimerC = nullptr;
    HWND ghScoreOn = nullptr;
    HWND ghScoreValue = nullptr;
    HWND ghScriptEntries = nullptr;
    HWND ghScriptChunks = nullptr;
    HWND progOffset = nullptr;
    HWND progVars = nullptr;
    HWND progActors = nullptr;
    HWND progFrames = nullptr;
    HWND progCmdBlock = nullptr;
    HWND progVarIndex = nullptr;
    HWND progVarValue = nullptr;
    HWND progLoadVar = nullptr;
    HWND progVarsTable = nullptr;
    HWND progVarsTableLabel = nullptr;
    HWND progReloadTable = nullptr;
    HWND missionHint = nullptr;
    HWND missionHelp1 = nullptr;
    HWND missionHelp2 = nullptr;
    HWND ghMarkerLabel = nullptr;
    HWND ghFieldALabel = nullptr;
    HWND ghFieldBLabel = nullptr;
    HWND ghMissionIdLabel = nullptr;
    HWND ghTimerOnLabel = nullptr;
    HWND ghTimerIntervalLabel = nullptr;
    HWND ghTimerALabel = nullptr;
    HWND ghTimerBLabel = nullptr;
    HWND ghTimerCLabel = nullptr;
    HWND ghScoreOnLabel = nullptr;
    HWND ghScoreValueLabel = nullptr;
    HWND ghScriptEntriesLabel = nullptr;
    HWND ghScriptChunksLabel = nullptr;
    HWND progOffsetLabel = nullptr;
    HWND progVarsLabel = nullptr;
    HWND progActorsLabel = nullptr;
    HWND progFramesLabel = nullptr;
    HWND progCmdBlockLabel = nullptr;
    HWND progVarIndexLabel = nullptr;
    HWND progVarValueLabel = nullptr;

    HWND carsTitle = nullptr;
    HWND carsList = nullptr;
    HWND carTabName = nullptr;
    HWND carTabModel = nullptr;
    HWND carTabIdx = nullptr;
    HWND carTabPosX = nullptr;
    HWND carTabPosY = nullptr;
    HWND carTabPosZ = nullptr;
    HWND carTabRotW = nullptr;
    HWND carTabRotX = nullptr;
    HWND carTabRotY = nullptr;
    HWND carTabRotZ = nullptr;
    HWND carTabFuel = nullptr;
    HWND carTabSpeed = nullptr;
    HWND carTabOdometer = nullptr;
    HWND carTabEngineOn = nullptr;
    HWND applyCar = nullptr;
    HWND carsHint = nullptr;
    HWND carTabNameLabel = nullptr;
    HWND carTabModelLabel = nullptr;
    HWND carTabIdxLabel = nullptr;
    HWND carTabPosXLabel = nullptr;
    HWND carTabPosYLabel = nullptr;
    HWND carTabPosZLabel = nullptr;
    HWND carTabRotWLabel = nullptr;
    HWND carTabRotXLabel = nullptr;
    HWND carTabRotYLabel = nullptr;
    HWND carTabRotZLabel = nullptr;
    HWND carTabFuelLabel = nullptr;
    HWND carTabSpeedLabel = nullptr;
    HWND carTabOdometerLabel = nullptr;
    HWND carTabEngineOnLabel = nullptr;
    HWND garageTitle = nullptr;
    HWND garageList = nullptr;
    HWND garageSlot = nullptr;
    HWND garageA = nullptr;
    HWND garageB = nullptr;
    HWND garageAHex = nullptr;
    HWND garageBHex = nullptr;
    HWND garageALow16 = nullptr;
    HWND garageBLow16 = nullptr;
    HWND garageAHi8 = nullptr;
    HWND garageBHi8 = nullptr;
    HWND garageAFlags = nullptr;
    HWND garageBFlags = nullptr;
    HWND garageAColor = nullptr;
    HWND garageBColor = nullptr;
    HWND garageACar = nullptr;
    HWND garageBCar = nullptr;
    HWND garageADecoded = nullptr;
    HWND garageBDecoded = nullptr;
    HWND applyGarage = nullptr;
    HWND clearGarage = nullptr;
    HWND syncGarageB = nullptr;
    HWND garageHint = nullptr;
    HWND garageSlotLabel = nullptr;
    HWND garageALabel = nullptr;
    HWND garageBLabel = nullptr;
    HWND garageAHexLabel = nullptr;
    HWND garageBHexLabel = nullptr;
    HWND garageALow16Label = nullptr;
    HWND garageBLow16Label = nullptr;
    HWND garageAHi8Label = nullptr;
    HWND garageBHi8Label = nullptr;
    HWND garageAFlagsLabel = nullptr;
    HWND garageBFlagsLabel = nullptr;
    HWND garageAColorLabel = nullptr;
    HWND garageBColorLabel = nullptr;
    HWND garageACarLabel = nullptr;
    HWND garageBCarLabel = nullptr;
    HWND garageADecodedLabel = nullptr;
    HWND garageBDecodedLabel = nullptr;

    HWND actors = nullptr;
    HWND aname = nullptr;
    HWND amodel = nullptr;
    HWND atype = nullptr;
    HWND aidx = nullptr;
    HWND apayload = nullptr;
    HWND posx = nullptr;
    HWND posy = nullptr;
    HWND posz = nullptr;
    HWND pstate = nullptr;
    HWND pid = nullptr;
    HWND pactive = nullptr;
    HWND premove = nullptr;
    HWND pframe = nullptr;
    HWND dirx = nullptr;
    HWND diry = nullptr;
    HWND dirz = nullptr;
    HWND animId = nullptr;
    HWND rotw = nullptr;
    HWND rotx = nullptr;
    HWND roty = nullptr;
    HWND rotz = nullptr;
    HWND carFuel = nullptr;
    HWND carFlow = nullptr;
    HWND carEngNorm = nullptr;
    HWND carEngCalc = nullptr;
    HWND carSpeedLimit = nullptr;
    HWND carLastGear = nullptr;
    HWND carGear = nullptr;
    HWND carGearboxFlag = nullptr;
    HWND carDisableEngine = nullptr;
    HWND carEngineOn = nullptr;
    HWND carIsEngineOn = nullptr;
    HWND carOdometer = nullptr;
    HWND invMode = nullptr;
    HWND invFlag = nullptr;
    HWND invSelId = nullptr;
    HWND invSelLoaded = nullptr;
    HWND invSelHidden = nullptr;
    HWND invSelUnk = nullptr;
    HWND invCoatId = nullptr;
    HWND invCoatLoaded = nullptr;
    HWND invCoatHidden = nullptr;
    HWND invCoatUnk = nullptr;
    HWND invS1Id = nullptr;
    HWND invS1Loaded = nullptr;
    HWND invS1Hidden = nullptr;
    HWND invS1Unk = nullptr;
    HWND invS2Id = nullptr;
    HWND invS2Loaded = nullptr;
    HWND invS2Hidden = nullptr;
    HWND invS2Unk = nullptr;
    HWND invS3Id = nullptr;
    HWND invS3Loaded = nullptr;
    HWND invS3Hidden = nullptr;
    HWND invS3Unk = nullptr;
    HWND invS4Id = nullptr;
    HWND invS4Loaded = nullptr;
    HWND invS4Hidden = nullptr;
    HWND invS4Unk = nullptr;
    HWND invS5Id = nullptr;
    HWND invS5Loaded = nullptr;
    HWND invS5Hidden = nullptr;
    HWND invS5Unk = nullptr;
    HWND humanSeat = nullptr;
    HWND humanCrouch = nullptr;
    HWND humanAim = nullptr;
    HWND humanShootX = nullptr;
    HWND humanShootY = nullptr;
    HWND humanShootZ = nullptr;
    HWND coordHint = nullptr;
    HWND applyActor = nullptr;
    HWND filterName = nullptr;
    HWND filterType = nullptr;
    HWND applyFilter = nullptr;
    HWND clearFilter = nullptr;
    HWND cloneActor = nullptr;
    HWND actorsTitle = nullptr;
    HWND filterNameLabel = nullptr;
    HWND filterTypeLabel = nullptr;
    HWND anameLabel = nullptr;
    HWND amodelLabel = nullptr;
    HWND atypeLabel = nullptr;
    HWND aidxLabel = nullptr;
    HWND apayloadLabel = nullptr;
    HWND posxLabel = nullptr;
    HWND posyLabel = nullptr;
    HWND poszLabel = nullptr;
    HWND pstateLabel = nullptr;
    HWND pidLabel = nullptr;
    HWND pactiveLabel = nullptr;
    HWND premoveLabel = nullptr;
    HWND pframeLabel = nullptr;
    HWND dirxLabel = nullptr;
    HWND diryLabel = nullptr;
    HWND dirzLabel = nullptr;
    HWND animIdLabel = nullptr;
    HWND rotwLabel = nullptr;
    HWND rotxLabel = nullptr;
    HWND rotyLabel = nullptr;
    HWND rotzLabel = nullptr;
    HWND carFuelLabel = nullptr;
    HWND carFlowLabel = nullptr;
    HWND carEngNormLabel = nullptr;
    HWND carEngCalcLabel = nullptr;
    HWND carSpeedLimitLabel = nullptr;
    HWND carLastGearLabel = nullptr;
    HWND carGearLabel = nullptr;
    HWND carGearboxFlagLabel = nullptr;
    HWND carDisableEngineLabel = nullptr;
    HWND carEngineOnLabel = nullptr;
    HWND carIsEngineOnLabel = nullptr;
    HWND carOdometerLabel = nullptr;
    HWND invModeLabel = nullptr;
    HWND invFlagLabel = nullptr;
    HWND invSelIdLabel = nullptr;
    HWND invSelLoadedLabel = nullptr;
    HWND invSelHiddenLabel = nullptr;
    HWND invSelUnkLabel = nullptr;
    HWND invCoatIdLabel = nullptr;
    HWND invCoatLoadedLabel = nullptr;
    HWND invCoatHiddenLabel = nullptr;
    HWND invCoatUnkLabel = nullptr;
    HWND invS1IdLabel = nullptr;
    HWND invS1LoadedLabel = nullptr;
    HWND invS1HiddenLabel = nullptr;
    HWND invS1UnkLabel = nullptr;
    HWND invS2IdLabel = nullptr;
    HWND invS2LoadedLabel = nullptr;
    HWND invS2HiddenLabel = nullptr;
    HWND invS2UnkLabel = nullptr;
    HWND invS3IdLabel = nullptr;
    HWND invS3LoadedLabel = nullptr;
    HWND invS3HiddenLabel = nullptr;
    HWND invS3UnkLabel = nullptr;
    HWND invS4IdLabel = nullptr;
    HWND invS4LoadedLabel = nullptr;
    HWND invS4HiddenLabel = nullptr;
    HWND invS4UnkLabel = nullptr;
    HWND invS5IdLabel = nullptr;
    HWND invS5LoadedLabel = nullptr;
    HWND invS5HiddenLabel = nullptr;
    HWND invS5UnkLabel = nullptr;
    HWND humanSeatLabel = nullptr;
    HWND humanCrouchLabel = nullptr;
    HWND humanAimLabel = nullptr;
    HWND humanShootXLabel = nullptr;
    HWND humanShootYLabel = nullptr;
    HWND humanShootZLabel = nullptr;

    HWND saveBtn = nullptr;
    HWND resetBtn = nullptr;
};

AppState g_state;
Ui g_ui;
HFONT g_font = nullptr;
HBRUSH g_bgBrush = nullptr;
std::vector<GarageCarCatalogEntry> g_garageCatalog;
void LayoutActorsPage();
void LayoutMissionPage();
void LayoutCarsPage();
void LayoutGaragePage();
bool IsActorPairAt(std::size_t headerIdx);

std::string Trim(std::string s) {
    auto isSpace = [](unsigned char c) { return std::isspace(c) != 0; };
    while (!s.empty() && isSpace(static_cast<unsigned char>(s.front()))) {
        s.erase(s.begin());
    }
    while (!s.empty() && isSpace(static_cast<unsigned char>(s.back()))) {
        s.pop_back();
    }
    return s;
}

std::string ToLowerAscii(std::string s) {
    for (char& ch : s) {
        ch = static_cast<char>(std::tolower(static_cast<unsigned char>(ch)));
    }
    return s;
}

bool EndsWithI3d(const std::string& s) {
    const std::string low = ToLowerAscii(s);
    return low.size() >= 4 && low.compare(low.size() - 4, 4, ".i3d") == 0;
}

void SetText(HWND h, const std::string& s) {
    SetWindowTextA(h, s.c_str());
}

std::string GetText(HWND h) {
    const int len = GetWindowTextLengthA(h);
    if (len <= 0) {
        return {};
    }
    std::string s(static_cast<std::size_t>(len), '\0');
    GetWindowTextA(h, s.data(), len + 1);
    return s;
}

void SetFieldVisible(HWND label, HWND edit, bool visible) {
    const int cmd = visible ? SW_SHOW : SW_HIDE;
    if (label != nullptr) {
        ShowWindow(label, cmd);
    }
    if (edit != nullptr) {
        ShowWindow(edit, cmd);
    }
}

void SetStatus(const std::string& s) {
    SetText(g_ui.status, s);
}

void Error(HWND hwnd, const std::string& s) {
    MessageBoxA(hwnd, s.c_str(), "Mafia Save Editor", MB_OK | MB_ICONERROR);
}

bool ParseU32(const std::string& s, std::uint32_t* out, std::string* err, const char* name) {
    char* end = nullptr;
    const unsigned long v = std::strtoul(s.c_str(), &end, 10);
    if (end == nullptr || *end != '\0') {
        if (err != nullptr) {
            *err = std::string(name) + " must be integer";
        }
        return false;
    }
    *out = static_cast<std::uint32_t>(v);
    return true;
}

bool ParseU32Auto(const std::string& sIn, std::uint32_t* out, std::string* err, const char* name) {
    const std::string s = Trim(sIn);
    if (s.empty()) {
        if (err != nullptr) {
            *err = std::string(name) + " is empty";
        }
        return false;
    }
    int base = 10;
    const char* start = s.c_str();
    if (s.size() > 2 && s[0] == '0' && (s[1] == 'x' || s[1] == 'X')) {
        base = 16;
        start += 2;
    }
    char* end = nullptr;
    const unsigned long v = std::strtoul(start, &end, base);
    if (end == nullptr || *end != '\0') {
        if (err != nullptr) {
            *err = std::string(name) + " must be integer (dec or 0xHEX)";
        }
        return false;
    }
    *out = static_cast<std::uint32_t>(v);
    return true;
}

bool ParseI32(const std::string& s, std::int32_t* out, std::string* err, const char* name) {
    char* end = nullptr;
    const long v = std::strtol(s.c_str(), &end, 10);
    if (end == nullptr || *end != '\0') {
        if (err != nullptr) {
            *err = std::string(name) + " must be integer";
        }
        return false;
    }
    *out = static_cast<std::int32_t>(v);
    return true;
}

bool ParseDate(const std::string& s, std::uint32_t* out, std::string* err) {
    int d = 0;
    int m = 0;
    int y = 0;
    if (std::sscanf(s.c_str(), "%d.%d.%d", &d, &m, &y) != 3) {
        if (err != nullptr) {
            *err = "date must be DD.MM.YYYY";
        }
        return false;
    }
    if (d < 1 || d > 31 || m < 1 || m > 12 || y < 1900 || y > 65535) {
        if (err != nullptr) {
            *err = "date out of range";
        }
        return false;
    }
    *out = static_cast<std::uint32_t>(d) | (static_cast<std::uint32_t>(m) << 8) | (static_cast<std::uint32_t>(y) << 16);
    return true;
}

bool ParseTime(const std::string& s, std::uint32_t* out, std::string* err) {
    int hh = 0;
    int mm = 0;
    int ss = 0;
    if (std::sscanf(s.c_str(), "%d:%d:%d", &hh, &mm, &ss) != 3) {
        if (err != nullptr) {
            *err = "time must be HH:MM:SS";
        }
        return false;
    }
    if (hh < 0 || hh > 23 || mm < 0 || mm > 59 || ss < 0 || ss > 59) {
        if (err != nullptr) {
            *err = "time out of range";
        }
        return false;
    }
    *out = static_cast<std::uint32_t>(ss) | (static_cast<std::uint32_t>(mm) << 8) | (static_cast<std::uint32_t>(hh) << 16);
    return true;
}

std::string ReadCStr(const std::vector<std::uint8_t>& data, std::size_t off, std::size_t cap) {
    if (off >= data.size()) {
        return {};
    }
    const std::size_t end = std::min(data.size(), off + cap);
    std::size_t len = 0;
    while (off + len < end && data[off + len] != 0) {
        ++len;
    }
    return std::string(reinterpret_cast<const char*>(data.data() + off), len);
}

bool WriteCStr(std::vector<std::uint8_t>* data, std::size_t off, std::size_t cap, const std::string& value, std::string* err) {
    if (data == nullptr || off + cap > data->size()) {
        if (err != nullptr) {
            *err = "field out of range";
        }
        return false;
    }
    if (value.empty() || value.size() + 1 > cap) {
        if (err != nullptr) {
            *err = "string is empty or too long";
        }
        return false;
    }
    std::fill(data->begin() + static_cast<std::ptrdiff_t>(off),
              data->begin() + static_cast<std::ptrdiff_t>(off + cap),
              static_cast<std::uint8_t>(0));
    for (std::size_t i = 0; i < value.size(); ++i) {
        (*data)[off + i] = static_cast<std::uint8_t>(value[i]);
    }
    return true;
}

float ReadF32LE(const std::vector<std::uint8_t>& data, std::size_t off) {
    const std::uint32_t bits = mafia_save::ReadU32LE(data, off);
    float out = 0.0f;
    std::memcpy(&out, &bits, sizeof(out));
    return out;
}

void WriteF32LE(std::vector<std::uint8_t>* data, std::size_t off, float value) {
    std::uint32_t bits = 0;
    std::memcpy(&bits, &value, sizeof(bits));
    mafia_save::WriteU32LE(data, off, bits);
}

std::string FormatFloat3(float v) {
    if (!std::isfinite(v)) {
        return "nan";
    }
    char buf[64] = {};
    std::snprintf(buf, sizeof(buf), "%.3f", static_cast<double>(v));
    return buf;
}

std::string FormatHex32(std::uint32_t v) {
    char buf[32] = {};
    std::snprintf(buf, sizeof(buf), "0x%08X", static_cast<unsigned>(v));
    return buf;
}

bool ParseF32(const std::string& sIn, float* out, std::string* err, const char* name) {
    if (out == nullptr) {
        if (err != nullptr) {
            *err = "internal null output";
        }
        return false;
    }
    std::string s = Trim(sIn);
    std::replace(s.begin(), s.end(), ',', '.');
    if (s.empty()) {
        if (err != nullptr) {
            *err = std::string(name) + " is empty";
        }
        return false;
    }
    char* end = nullptr;
    const float v = std::strtof(s.c_str(), &end);
    if (end == nullptr || *end != '\0' || !std::isfinite(v)) {
        if (err != nullptr) {
            *err = std::string(name) + " must be finite float";
        }
        return false;
    }
    *out = v;
    return true;
}

bool ParseByteField(const std::string& s, std::uint8_t* out, std::string* err, const char* name, std::uint32_t maxVal) {
    std::uint32_t v = 0;
    if (!ParseU32(Trim(s), &v, err, name)) {
        return false;
    }
    if (v > maxVal) {
        if (err != nullptr) {
            std::ostringstream oss;
            oss << name << " must be <= " << maxVal;
            *err = oss.str();
        }
        return false;
    }
    *out = static_cast<std::uint8_t>(v);
    return true;
}

std::uint16_t ReadU16LE(const std::vector<std::uint8_t>& data, std::size_t off) {
    return static_cast<std::uint16_t>(data[off]) | (static_cast<std::uint16_t>(data[off + 1]) << 8);
}

struct ProgramLayout {
    bool valid = false;
    std::size_t baseOff = 0;
    std::size_t varsOff = 0;
    std::uint16_t regCount = 0;
    std::uint32_t varCount = 0;
    std::uint32_t frameCount = 0;
    std::uint32_t actorCount = 0;
};

constexpr std::size_t kGameHeaderSize = 67;
struct ProgramLocation {
    std::size_t segIdx = mafia_save::kNoIndex;
    ProgramLayout layout;
};

std::optional<ProgramLayout> TryParseProgramLayoutAt(const std::vector<std::uint8_t>& p, std::size_t base) {
    if (base + 39 > p.size() || p[base] != 2u) {
        return std::nullopt;
    }

    ProgramLayout out;
    out.baseOff = base;
    out.regCount = ReadU16LE(p, base + 17);
    out.varCount = mafia_save::ReadU32LE(p, base + 19);
    out.frameCount = mafia_save::ReadU32LE(p, base + 23);
    out.actorCount = mafia_save::ReadU32LE(p, base + 27);

    if (out.regCount > 4096u || out.varCount > 8192u || out.frameCount > 2048u || out.actorCount > 2048u) {
        return std::nullopt;
    }

    std::size_t cur = base + 39;
    if (cur + (2u * static_cast<std::size_t>(out.regCount)) > p.size()) {
        return std::nullopt;
    }
    cur += 2u * static_cast<std::size_t>(out.regCount);

    out.varsOff = cur;
    if (cur + (4u * static_cast<std::size_t>(out.varCount)) > p.size()) {
        return std::nullopt;
    }
    cur += 4u * static_cast<std::size_t>(out.varCount);

    for (std::uint32_t i = 0; i < out.actorCount; ++i) {
        if (cur + 8 > p.size()) {
            return std::nullopt;
        }
        const std::uint32_t nameLen = mafia_save::ReadU32LE(p, cur);
        if (nameLen > 1024u) {
            return std::nullopt;
        }
        cur += 8;
        if (cur + static_cast<std::size_t>(nameLen) > p.size()) {
            return std::nullopt;
        }
        cur += static_cast<std::size_t>(nameLen);
    }

    for (std::uint32_t i = 0; i < out.frameCount; ++i) {
        if (cur + 2 > p.size()) {
            return std::nullopt;
        }
        const std::uint16_t nameLen = ReadU16LE(p, cur);
        cur += 2;
        if (cur + static_cast<std::size_t>(nameLen) > p.size()) {
            return std::nullopt;
        }
        cur += static_cast<std::size_t>(nameLen);
    }

    out.valid = true;
    return out;
}

std::optional<ProgramLayout> DetectProgramLayout(const std::vector<std::uint8_t>& p) {
    if (p.size() < 39) {
        return std::nullopt;
    }
    std::optional<ProgramLayout> best;
    for (std::size_t off = 0; off + 39 <= p.size(); ++off) {
        if (p[off] != 2u) {
            continue;
        }
        const auto cand = TryParseProgramLayoutAt(p, off);
        if (!cand.has_value()) {
            continue;
        }
        if (!best.has_value() || cand->varCount > best->varCount ||
            (cand->varCount == best->varCount && cand->actorCount > best->actorCount)) {
            best = cand;
        }
    }
    return best;
}

bool IsProgramCandidateSegment(const mafia_save::SaveData& save, std::size_t segIdx) {
    if (segIdx >= save.segments.size()) {
        return false;
    }
    if (segIdx == save.idxGamePayload || segIdx == save.idxAiGroups || segIdx == save.idxAiFollow) {
        return true;
    }
    const std::string& n = save.segments[segIdx].name;
    return n.rfind("actor_payload_", 0) == 0 || n.rfind("actor_payload_clone", 0) == 0;
}

std::optional<ProgramLocation> DetectProgramInSave(const mafia_save::SaveData& save) {
    std::optional<ProgramLocation> best;
    for (std::size_t i = 0; i < save.segments.size(); ++i) {
        if (!IsProgramCandidateSegment(save, i)) {
            continue;
        }
        const auto prog = DetectProgramLayout(save.segments[i].plain);
        if (!prog.has_value()) {
            continue;
        }

        if (!best.has_value()) {
            best = ProgramLocation{i, *prog};
            continue;
        }

        const auto& b = best->layout;
        const bool candHasVars = prog->varCount > 0u;
        const bool bestHasVars = b.varCount > 0u;
        if (candHasVars != bestHasVars) {
            if (candHasVars) {
                best = ProgramLocation{i, *prog};
            }
            continue;
        }
        if (prog->varCount != b.varCount) {
            if (prog->varCount > b.varCount) {
                best = ProgramLocation{i, *prog};
            }
            continue;
        }
        if (prog->actorCount != b.actorCount) {
            if (prog->actorCount > b.actorCount) {
                best = ProgramLocation{i, *prog};
            }
            continue;
        }
        if (prog->frameCount != b.frameCount) {
            if (prog->frameCount > b.frameCount) {
                best = ProgramLocation{i, *prog};
            }
            continue;
        }
        if (best->segIdx != save.idxGamePayload && i == save.idxGamePayload) {
            best = ProgramLocation{i, *prog};
        }
    }
    return best;
}

constexpr std::size_t kHumanBlobOff = 13;
constexpr std::size_t kHumanBlobSize = 382;
constexpr std::size_t kInventoryBlobSize = 196;

std::uint32_t ReadInvDw(const std::vector<std::uint8_t>& p, std::size_t invOff, std::size_t idx) {
    return mafia_save::ReadU32LE(p, invOff + (idx * 4));
}

void WriteInvDw(std::vector<std::uint8_t>* p, std::size_t invOff, std::size_t idx, std::uint32_t v) {
    mafia_save::WriteU32LE(p, invOff + (idx * 4), v);
}

bool FindHumanInventoryOffset(const std::vector<std::uint8_t>& p, std::size_t* outInvOff) {
    if (outInvOff == nullptr) {
        return false;
    }
    if (p.size() < kHumanBlobOff + kHumanBlobSize + 16 + kInventoryBlobSize) {
        return false;
    }
    std::size_t cursor = kHumanBlobOff + kHumanBlobSize;
    for (int i = 0; i < 2; ++i) {
        if (cursor + 8 > p.size()) {
            return false;
        }
        const std::uint32_t nameLen = mafia_save::ReadU32LE(p, cursor);
        if (nameLen == 0) {
            cursor += 8;
            continue;
        }
        if (nameLen > 1024) {
            return false;
        }
        const std::size_t chunk = static_cast<std::size_t>(nameLen) + 8;
        if (cursor + chunk > p.size()) {
            return false;
        }
        cursor += chunk;
    }
    if (cursor + kInventoryBlobSize > p.size()) {
        return false;
    }
    *outInvOff = cursor;
    return true;
}

void SetInventoryVisibility(bool visible) {
    SetFieldVisible(g_ui.invModeLabel, g_ui.invMode, visible);
    SetFieldVisible(g_ui.invFlagLabel, g_ui.invFlag, visible);
    SetFieldVisible(g_ui.invSelIdLabel, g_ui.invSelId, visible);
    SetFieldVisible(g_ui.invSelLoadedLabel, g_ui.invSelLoaded, visible);
    SetFieldVisible(g_ui.invSelHiddenLabel, g_ui.invSelHidden, visible);
    SetFieldVisible(g_ui.invSelUnkLabel, g_ui.invSelUnk, visible);
    SetFieldVisible(g_ui.invCoatIdLabel, g_ui.invCoatId, visible);
    SetFieldVisible(g_ui.invCoatLoadedLabel, g_ui.invCoatLoaded, visible);
    SetFieldVisible(g_ui.invCoatHiddenLabel, g_ui.invCoatHidden, visible);
    SetFieldVisible(g_ui.invCoatUnkLabel, g_ui.invCoatUnk, visible);
    SetFieldVisible(g_ui.invS1IdLabel, g_ui.invS1Id, visible);
    SetFieldVisible(g_ui.invS1LoadedLabel, g_ui.invS1Loaded, visible);
    SetFieldVisible(g_ui.invS1HiddenLabel, g_ui.invS1Hidden, visible);
    SetFieldVisible(g_ui.invS1UnkLabel, g_ui.invS1Unk, visible);
    SetFieldVisible(g_ui.invS2IdLabel, g_ui.invS2Id, visible);
    SetFieldVisible(g_ui.invS2LoadedLabel, g_ui.invS2Loaded, visible);
    SetFieldVisible(g_ui.invS2HiddenLabel, g_ui.invS2Hidden, visible);
    SetFieldVisible(g_ui.invS2UnkLabel, g_ui.invS2Unk, visible);
    SetFieldVisible(g_ui.invS3IdLabel, g_ui.invS3Id, visible);
    SetFieldVisible(g_ui.invS3LoadedLabel, g_ui.invS3Loaded, visible);
    SetFieldVisible(g_ui.invS3HiddenLabel, g_ui.invS3Hidden, visible);
    SetFieldVisible(g_ui.invS3UnkLabel, g_ui.invS3Unk, visible);
    SetFieldVisible(g_ui.invS4IdLabel, g_ui.invS4Id, visible);
    SetFieldVisible(g_ui.invS4LoadedLabel, g_ui.invS4Loaded, visible);
    SetFieldVisible(g_ui.invS4HiddenLabel, g_ui.invS4Hidden, visible);
    SetFieldVisible(g_ui.invS4UnkLabel, g_ui.invS4Unk, visible);
    SetFieldVisible(g_ui.invS5IdLabel, g_ui.invS5Id, visible);
    SetFieldVisible(g_ui.invS5LoadedLabel, g_ui.invS5Loaded, visible);
    SetFieldVisible(g_ui.invS5HiddenLabel, g_ui.invS5Hidden, visible);
    SetFieldVisible(g_ui.invS5UnkLabel, g_ui.invS5Unk, visible);
}

void ClearInventoryEdits() {
    SetText(g_ui.invMode, "");
    SetText(g_ui.invFlag, "");
    SetText(g_ui.invSelId, "");
    SetText(g_ui.invSelLoaded, "");
    SetText(g_ui.invSelHidden, "");
    SetText(g_ui.invSelUnk, "");
    SetText(g_ui.invCoatId, "");
    SetText(g_ui.invCoatLoaded, "");
    SetText(g_ui.invCoatHidden, "");
    SetText(g_ui.invCoatUnk, "");
    SetText(g_ui.invS1Id, "");
    SetText(g_ui.invS1Loaded, "");
    SetText(g_ui.invS1Hidden, "");
    SetText(g_ui.invS1Unk, "");
    SetText(g_ui.invS2Id, "");
    SetText(g_ui.invS2Loaded, "");
    SetText(g_ui.invS2Hidden, "");
    SetText(g_ui.invS2Unk, "");
    SetText(g_ui.invS3Id, "");
    SetText(g_ui.invS3Loaded, "");
    SetText(g_ui.invS3Hidden, "");
    SetText(g_ui.invS3Unk, "");
    SetText(g_ui.invS4Id, "");
    SetText(g_ui.invS4Loaded, "");
    SetText(g_ui.invS4Hidden, "");
    SetText(g_ui.invS4Unk, "");
    SetText(g_ui.invS5Id, "");
    SetText(g_ui.invS5Loaded, "");
    SetText(g_ui.invS5Hidden, "");
    SetText(g_ui.invS5Unk, "");
}

void SetInventoryEnabled(bool enabled) {
    const BOOL en = enabled ? TRUE : FALSE;
    EnableWindow(g_ui.invMode, en);
    EnableWindow(g_ui.invFlag, en);
    EnableWindow(g_ui.invSelId, en);
    EnableWindow(g_ui.invSelLoaded, en);
    EnableWindow(g_ui.invSelHidden, en);
    EnableWindow(g_ui.invSelUnk, en);
    EnableWindow(g_ui.invCoatId, en);
    EnableWindow(g_ui.invCoatLoaded, en);
    EnableWindow(g_ui.invCoatHidden, en);
    EnableWindow(g_ui.invCoatUnk, en);
    EnableWindow(g_ui.invS1Id, en);
    EnableWindow(g_ui.invS1Loaded, en);
    EnableWindow(g_ui.invS1Hidden, en);
    EnableWindow(g_ui.invS1Unk, en);
    EnableWindow(g_ui.invS2Id, en);
    EnableWindow(g_ui.invS2Loaded, en);
    EnableWindow(g_ui.invS2Hidden, en);
    EnableWindow(g_ui.invS2Unk, en);
    EnableWindow(g_ui.invS3Id, en);
    EnableWindow(g_ui.invS3Loaded, en);
    EnableWindow(g_ui.invS3Hidden, en);
    EnableWindow(g_ui.invS3Unk, en);
    EnableWindow(g_ui.invS4Id, en);
    EnableWindow(g_ui.invS4Loaded, en);
    EnableWindow(g_ui.invS4Hidden, en);
    EnableWindow(g_ui.invS4Unk, en);
    EnableWindow(g_ui.invS5Id, en);
    EnableWindow(g_ui.invS5Loaded, en);
    EnableWindow(g_ui.invS5Hidden, en);
    EnableWindow(g_ui.invS5Unk, en);
}

void FillInventoryEdits(const std::vector<std::uint8_t>& p, std::size_t invOff) {
    const std::uint32_t modeRaw = ReadInvDw(p, invOff, 0);
    SetText(g_ui.invMode, std::to_string(modeRaw & 0x7Fu));
    SetText(g_ui.invFlag, std::to_string((modeRaw >> 7) & 1u));

    SetText(g_ui.invSelId, std::to_string(ReadInvDw(p, invOff, 1)));
    SetText(g_ui.invSelLoaded, std::to_string(ReadInvDw(p, invOff, 2)));
    SetText(g_ui.invSelHidden, std::to_string(ReadInvDw(p, invOff, 3)));
    SetText(g_ui.invSelUnk, std::to_string(ReadInvDw(p, invOff, 4)));

    SetText(g_ui.invCoatId, std::to_string(ReadInvDw(p, invOff, 29)));
    SetText(g_ui.invCoatLoaded, std::to_string(ReadInvDw(p, invOff, 30)));
    SetText(g_ui.invCoatHidden, std::to_string(ReadInvDw(p, invOff, 31)));
    SetText(g_ui.invCoatUnk, std::to_string(ReadInvDw(p, invOff, 32)));

    SetText(g_ui.invS1Id, std::to_string(ReadInvDw(p, invOff, 9)));
    SetText(g_ui.invS1Loaded, std::to_string(ReadInvDw(p, invOff, 10)));
    SetText(g_ui.invS1Hidden, std::to_string(ReadInvDw(p, invOff, 11)));
    SetText(g_ui.invS1Unk, std::to_string(ReadInvDw(p, invOff, 12)));
    SetText(g_ui.invS2Id, std::to_string(ReadInvDw(p, invOff, 13)));
    SetText(g_ui.invS2Loaded, std::to_string(ReadInvDw(p, invOff, 14)));
    SetText(g_ui.invS2Hidden, std::to_string(ReadInvDw(p, invOff, 15)));
    SetText(g_ui.invS2Unk, std::to_string(ReadInvDw(p, invOff, 16)));
    SetText(g_ui.invS3Id, std::to_string(ReadInvDw(p, invOff, 17)));
    SetText(g_ui.invS3Loaded, std::to_string(ReadInvDw(p, invOff, 18)));
    SetText(g_ui.invS3Hidden, std::to_string(ReadInvDw(p, invOff, 19)));
    SetText(g_ui.invS3Unk, std::to_string(ReadInvDw(p, invOff, 20)));
    SetText(g_ui.invS4Id, std::to_string(ReadInvDw(p, invOff, 21)));
    SetText(g_ui.invS4Loaded, std::to_string(ReadInvDw(p, invOff, 22)));
    SetText(g_ui.invS4Hidden, std::to_string(ReadInvDw(p, invOff, 23)));
    SetText(g_ui.invS4Unk, std::to_string(ReadInvDw(p, invOff, 24)));
    SetText(g_ui.invS5Id, std::to_string(ReadInvDw(p, invOff, 25)));
    SetText(g_ui.invS5Loaded, std::to_string(ReadInvDw(p, invOff, 26)));
    SetText(g_ui.invS5Hidden, std::to_string(ReadInvDw(p, invOff, 27)));
    SetText(g_ui.invS5Unk, std::to_string(ReadInvDw(p, invOff, 28)));
}

struct CoordLayout {
    bool baseSupported = false;
    bool coordsSupported = false;
    bool dirSupported = false;
    bool animSupported = false;
    bool quatSupported = false;
    bool carStateSupported = false;
    bool carDriveSupported = false;
    bool carEngineFlagsSupported = false;
    bool carOdometerSupported = false;
    bool humanStateSupported = false;
    bool humanInventorySupported = false;
    std::size_t stateOff = 1;
    std::size_t idOff = 2;
    std::size_t activeOff = 6;
    std::size_t removeOff = 7;
    std::size_t frameOff = 8;
    std::size_t xOff = 0;
    std::size_t yOff = 0;
    std::size_t zOff = 0;
    std::size_t dirXOff = 0;
    std::size_t dirYOff = 0;
    std::size_t dirZOff = 0;
    std::size_t animIdOff = 0;
    std::size_t quatWOff = 0;
    std::size_t quatXOff = 0;
    std::size_t quatYOff = 0;
    std::size_t quatZOff = 0;
    std::size_t carFuelOff = 0;
    std::size_t carFlowOff = 0;
    std::size_t carEngNormOff = 0;
    std::size_t carEngCalcOff = 0;
    std::size_t carSpeedLimitOff = 0;
    std::size_t carLastGearOff = 0;
    std::size_t carGearOff = 0;
    std::size_t carGearboxFlagOff = 0;
    std::size_t carDisableEngineOff = 0;
    std::size_t carEngineOnOff = 0;
    std::size_t carIsEngineOnOff = 0;
    std::size_t carOdometerOff = 0;
    std::size_t humanSeatOff = 0;
    std::size_t humanCrouchOff = 0;
    std::size_t humanAimOff = 0;
    std::size_t humanShootXOff = 0;
    std::size_t humanShootYOff = 0;
    std::size_t humanShootZOff = 0;
    std::size_t humanInventoryOff = 0;
    std::string hint;
};

std::string FormatDate(std::uint32_t packed) {
    std::ostringstream oss;
    oss << (packed & 0xFFu) << "." << ((packed >> 8) & 0xFFu) << "." << ((packed >> 16) & 0xFFFFu);
    return oss.str();
}

std::string FormatTime(std::uint32_t packed) {
    const int hh = static_cast<int>((packed >> 16) & 0xFFu);
    const int mm = static_cast<int>((packed >> 8) & 0xFFu);
    const int ss = static_cast<int>(packed & 0xFFu);
    char buf[32] = {};
    std::snprintf(buf, sizeof(buf), "%02d:%02d:%02d", hh, mm, ss);
    return buf;
}
void RebuildActorIndex() {
    g_state.actorHeaders.clear();
    for (std::size_t i = 0; i < g_state.save.segments.size(); ++i) {
        const auto& seg = g_state.save.segments[i];
        if (seg.name.rfind("actor_header_", 0) == 0 && seg.plain.size() >= mafia_save::kActorHeaderSize) {
            g_state.actorHeaders.push_back(i);
        }
    }
}

bool MatchesActorFilter(std::size_t segIdx) {
    const auto& h = g_state.save.segments[segIdx].plain;
    const std::string name = ReadCStr(h, 0, 64);
    if (!g_state.filterName.empty()) {
        const std::string f = g_state.filterName;
        if (name.find(f) == std::string::npos) {
            return false;
        }
    }
    if (g_state.filterType.has_value()) {
        const std::uint32_t type = mafia_save::ReadU32LE(h, 128);
        if (type != *g_state.filterType) {
            return false;
        }
    }
    return true;
}

void RebuildFilteredActors() {
    g_state.filteredActorHeaders.clear();
    for (std::size_t segIdx : g_state.actorHeaders) {
        if (MatchesActorFilter(segIdx)) {
            g_state.filteredActorHeaders.push_back(segIdx);
        }
    }
    if (g_state.filteredActorHeaders.empty()) {
        g_state.selectedActor = -1;
    } else if (g_state.selectedActor < 0 || g_state.selectedActor >= static_cast<int>(g_state.filteredActorHeaders.size())) {
        g_state.selectedActor = 0;
    }
}

void RebuildCarIndex() {
    g_state.carHeaders.clear();
    for (std::size_t segIdx : g_state.actorHeaders) {
        if (!IsActorPairAt(segIdx)) {
            continue;
        }
        const auto& h = g_state.save.segments[segIdx].plain;
        const std::uint32_t type = mafia_save::ReadU32LE(h, 128);
        if (type != 4u) {
            continue;
        }
        g_state.carHeaders.push_back(segIdx);
    }

    if (g_state.carHeaders.empty()) {
        g_state.selectedCar = -1;
    } else if (g_state.selectedCar < 0 || g_state.selectedCar >= static_cast<int>(g_state.carHeaders.size())) {
        g_state.selectedCar = 0;
    }
}

std::optional<std::size_t> CurrentSelectedActorSegIdx() {
    if (g_state.selectedActor < 0 || g_state.selectedActor >= static_cast<int>(g_state.filteredActorHeaders.size())) {
        return std::nullopt;
    }
    return g_state.filteredActorHeaders[static_cast<std::size_t>(g_state.selectedActor)];
}

std::optional<std::size_t> CurrentSelectedCarSegIdx() {
    if (g_state.selectedCar < 0 || g_state.selectedCar >= static_cast<int>(g_state.carHeaders.size())) {
        return std::nullopt;
    }
    return g_state.carHeaders[static_cast<std::size_t>(g_state.selectedCar)];
}

bool IsActorPairAt(std::size_t headerIdx) {
    if (headerIdx + 1 >= g_state.save.segments.size()) {
        return false;
    }
    return g_state.save.segments[headerIdx].name.rfind("actor_header_", 0) == 0 &&
           g_state.save.segments[headerIdx + 1].name.rfind("actor_payload_", 0) == 0;
}

CoordLayout DetectCoordLayout(std::size_t headerIdx) {
    CoordLayout layout;
    if (!g_state.loaded || !IsActorPairAt(headerIdx)) {
        return layout;
    }

    const auto& p = g_state.save.segments[headerIdx + 1].plain;
    if (p.size() >= 13 && p[0] == 3u) {
        layout.baseSupported = true;
    }

    if (p.size() >= 42 && p[0] == 3u && p[13] == 6u) {
        layout.coordsSupported = true;
        layout.dirSupported = true;
        layout.animSupported = true;
        layout.xOff = 14;
        layout.yOff = 18;
        layout.zOff = 22;
        layout.dirXOff = 26;
        layout.dirYOff = 30;
        layout.dirZOff = 34;
        layout.animIdOff = 38;
        if (p.size() >= 66) {
            layout.humanStateSupported = true;
            layout.humanSeatOff = 46;
            layout.humanCrouchOff = 50;
            layout.humanAimOff = 51;
            layout.humanShootXOff = 54;
            layout.humanShootYOff = 58;
            layout.humanShootZOff = 62;
        }
        std::size_t invOff = 0;
        if (FindHumanInventoryOffset(p, &invOff)) {
            layout.humanInventorySupported = true;
            layout.humanInventoryOff = invOff;
        }
        layout.hint = "Payload: marker=3, subtype=6 (human/player)";
        return layout;
    }

    if (p.size() >= 18 && p[0] == 3u && p[13] == 9u) {
        if (p.size() >= 49) {
            layout.coordsSupported = true;
            layout.quatSupported = true;
            layout.xOff = 21;
            layout.yOff = 25;
            layout.zOff = 29;
            layout.quatWOff = 33;
            layout.quatXOff = 37;
            layout.quatYOff = 41;
            layout.quatZOff = 45;
            if (p.size() > 308) {
                layout.carStateSupported = true;
                layout.carFuelOff = 304;
                layout.carFlowOff = 211;
                layout.carEngNormOff = 137;
                layout.carEngCalcOff = 141;
            }
            if (p.size() >= 253) {
                layout.carDriveSupported = true;
                layout.carSpeedLimitOff = 215;
                layout.carLastGearOff = 245;
                layout.carGearOff = 249;
            }
            if (p.size() >= 304) {
                layout.carEngineFlagsSupported = true;
                layout.carGearboxFlagOff = 273;
                layout.carDisableEngineOff = 277;
                layout.carEngineOnOff = 298;
                layout.carIsEngineOnOff = 303;
            }
            if (p.size() >= 349) {
                layout.carOdometerSupported = true;
                layout.carOdometerOff = 345;
            }
            layout.hint = "Payload: marker=3, subtype=9 (car mapped)";
            return layout;
        }
        layout.hint = "Payload: marker=3, subtype=9 (car, partial mapping)";
        return layout;
    }

    if (layout.baseSupported) {
        layout.hint = "Payload: marker=3 (base fields available)";
        return layout;
    }
    layout.hint = "Payload: unknown format";
    return layout;
}

std::optional<std::size_t> FindTommyHeaderSegIdx() {
    for (std::size_t segIdx : g_state.actorHeaders) {
        const auto& h = g_state.save.segments[segIdx].plain;
        if (ReadCStr(h, 0, 64) == "Tommy") {
            return segIdx;
        }
    }
    return std::nullopt;
}

void RefreshWarning() {
    if (!g_state.loaded) {
        SetText(g_ui.warning, "");
        return;
    }

    const auto tommy = FindTommyHeaderSegIdx();
    if (!tommy.has_value()) {
        SetText(g_ui.warning, "Warning: actor 'Tommy' not found.");
        return;
    }
    const auto& h = g_state.save.segments[*tommy].plain;
    const std::uint32_t type = mafia_save::ReadU32LE(h, 128);
    if (type != 2u) {
        SetText(g_ui.warning, "Warning: Tommy type is not 2.");
        return;
    }
    SetText(g_ui.warning, "");
}

void FillMain() {
    if (!g_state.loaded) {
        SetText(g_ui.hp, "");
        SetText(g_ui.date, "");
        SetText(g_ui.time, "");
        SetText(g_ui.slot, "");
        SetText(g_ui.mcode, "");
        SetText(g_ui.mname, "");
        return;
    }

    std::string err;
    mafia_save::MetaFields meta;
    if (!mafia_save::ReadMetaFields(g_state.save, &meta, &err)) {
        SetStatus("ReadMetaFields failed: " + err);
        return;
    }

    SetText(g_ui.hp, std::to_string(meta.hpPercent));
    SetText(g_ui.date, FormatDate(meta.packedDate));
    SetText(g_ui.time, FormatTime(meta.packedTime));
    SetText(g_ui.slot, std::to_string(meta.slot));
    SetText(g_ui.mcode, std::to_string(meta.missionCode));
    SetText(g_ui.mname, mafia_save::ReadMissionName(g_state.save, &err));
}

void SetMissionScriptControlsEnabled(bool enabled) {
    const BOOL en = enabled ? TRUE : FALSE;
    EnableWindow(g_ui.progCmdBlock, en);
    EnableWindow(g_ui.progVarIndex, en);
    EnableWindow(g_ui.progVarValue, en);
    EnableWindow(g_ui.progLoadVar, en);
    EnableWindow(g_ui.progVarsTable, en);
    EnableWindow(g_ui.progReloadTable, en);
}

void ClearProgramVarTable() {
    if (g_ui.progVarsTable != nullptr) {
        SendMessageA(g_ui.progVarsTable, LB_RESETCONTENT, 0, 0);
    }
}

void FillProgramVarTable(const std::vector<std::uint8_t>& payload, const ProgramLayout& prog) {
    ClearProgramVarTable();
    if (g_ui.progVarsTable == nullptr || prog.varCount == 0u) {
        return;
    }

    constexpr std::uint32_t kMaxRows = 10000u;
    const std::uint32_t rows = (prog.varCount > kMaxRows) ? kMaxRows : prog.varCount;
    for (std::uint32_t i = 0; i < rows; ++i) {
        const float v = ReadF32LE(payload, prog.varsOff + (static_cast<std::size_t>(i) * 4u));
        std::ostringstream row;
        row << i << " | " << FormatFloat3(v);
        const LRESULT pos = SendMessageA(g_ui.progVarsTable, LB_ADDSTRING, 0, reinterpret_cast<LPARAM>(row.str().c_str()));
        if (pos != LB_ERR && pos != LB_ERRSPACE) {
            SendMessageA(g_ui.progVarsTable, LB_SETITEMDATA, static_cast<WPARAM>(pos), static_cast<LPARAM>(i));
        }
    }
}

bool LoadProgramVarIntoUi(std::string* err) {
    if (!g_state.loaded) {
        if (err != nullptr) {
            *err = "save is not loaded";
        }
        return false;
    }

    const auto where = DetectProgramInSave(g_state.save);
    if (!where.has_value()) {
        if (err != nullptr) {
            *err = "program block not found";
        }
        return false;
    }
    const auto& p = g_state.save.segments[where->segIdx].plain;
    const auto& prog = where->layout;
    if (prog.varCount == 0u) {
        if (err != nullptr) {
            *err = "program has zero variables";
        }
        return false;
    }

    std::uint32_t varIdx = 0;
    if (!ParseU32(Trim(GetText(g_ui.progVarIndex)), &varIdx, err, "Script var index")) {
        return false;
    }
    if (varIdx >= prog.varCount) {
        if (err != nullptr) {
            std::ostringstream oss;
            oss << "Script var index out of range (max " << (prog.varCount - 1u) << ")";
            *err = oss.str();
        }
        return false;
    }

    const float v = ReadF32LE(p, prog.varsOff + (static_cast<std::size_t>(varIdx) * 4u));
    SetText(g_ui.progVarValue, FormatFloat3(v));
    return true;
}

void FillMission() {
    auto clearAll = [&]() {
        SetText(g_ui.ghMarker, "");
        SetText(g_ui.ghFieldA, "");
        SetText(g_ui.ghFieldB, "");
        SetText(g_ui.ghMissionId, "");
        SetText(g_ui.ghTimerOn, "");
        SetText(g_ui.ghTimerInterval, "");
        SetText(g_ui.ghTimerA, "");
        SetText(g_ui.ghTimerB, "");
        SetText(g_ui.ghTimerC, "");
        SetText(g_ui.ghScoreOn, "");
        SetText(g_ui.ghScoreValue, "");
        SetText(g_ui.ghScriptEntries, "");
        SetText(g_ui.ghScriptChunks, "");
        SetText(g_ui.progOffset, "");
        SetText(g_ui.progVars, "");
        SetText(g_ui.progActors, "");
        SetText(g_ui.progFrames, "");
        SetText(g_ui.progCmdBlock, "");
        SetText(g_ui.progVarIndex, "");
        SetText(g_ui.progVarValue, "");
        ClearProgramVarTable();
        SetText(g_ui.missionHint, "Script program: -");
        SetMissionScriptControlsEnabled(false);
    };

    if (!g_state.loaded || g_state.save.idxGamePayload == mafia_save::kNoIndex ||
        g_state.save.idxGamePayload >= g_state.save.segments.size()) {
        clearAll();
        return;
    }

    const auto& p = g_state.save.segments[g_state.save.idxGamePayload].plain;
    if (p.size() < kGameHeaderSize) {
        clearAll();
        SetText(g_ui.missionHint, "Script program: invalid game_payload header");
        return;
    }

    SetText(g_ui.ghMarker, std::to_string(static_cast<unsigned>(p[0])));
    SetText(g_ui.ghFieldA, std::to_string(mafia_save::ReadU32LE(p, 1)));
    SetText(g_ui.ghFieldB, std::to_string(mafia_save::ReadU32LE(p, 5)));
    SetText(g_ui.ghMissionId, std::to_string(mafia_save::ReadU32LE(p, 9)));
    SetText(g_ui.ghTimerOn, std::to_string(static_cast<unsigned>(p[13])));
    SetText(g_ui.ghTimerInterval, std::to_string(mafia_save::ReadU32LE(p, 14)));
    SetText(g_ui.ghTimerA, std::to_string(mafia_save::ReadU32LE(p, 18)));
    SetText(g_ui.ghTimerB, std::to_string(mafia_save::ReadU32LE(p, 22)));
    SetText(g_ui.ghTimerC, std::to_string(mafia_save::ReadU32LE(p, 26)));
    SetText(g_ui.ghScriptEntries, std::to_string(mafia_save::ReadU32LE(p, 42)));
    SetText(g_ui.ghScriptChunks, std::to_string(mafia_save::ReadU32LE(p, 46)));
    SetText(g_ui.ghScoreOn, std::to_string(static_cast<unsigned>(p[62])));
    SetText(g_ui.ghScoreValue, std::to_string(mafia_save::ReadU32LE(p, 63)));

    const auto where = DetectProgramInSave(g_state.save);
    if (!where.has_value()) {
        SetText(g_ui.progOffset, "");
        SetText(g_ui.progVars, "");
        SetText(g_ui.progActors, "");
        SetText(g_ui.progFrames, "");
        SetText(g_ui.progCmdBlock, "");
        SetText(g_ui.progVarIndex, "");
        SetText(g_ui.progVarValue, "");
        ClearProgramVarTable();
        SetText(g_ui.missionHint, "Script program: not found");
        SetMissionScriptControlsEnabled(false);
        return;
    }

    const auto& prog = where->layout;
    const auto& pProg = g_state.save.segments[where->segIdx].plain;
    SetText(g_ui.progOffset, std::to_string(prog.baseOff));
    SetText(g_ui.progVars, std::to_string(prog.varCount));
    SetText(g_ui.progActors, std::to_string(prog.actorCount));
    SetText(g_ui.progFrames, std::to_string(prog.frameCount));
    SetText(g_ui.progCmdBlock, std::to_string(static_cast<unsigned>(pProg[prog.baseOff + 35])));
    FillProgramVarTable(pProg, prog);
    SetMissionScriptControlsEnabled(true);

    std::uint32_t varIdx = 0;
    std::string varText = Trim(GetText(g_ui.progVarIndex));
    if (varText.empty() || !ParseU32(varText, &varIdx, nullptr, "Script var index") || varIdx >= prog.varCount) {
        varIdx = 0;
    }
    if (prog.varCount == 0u) {
        SetText(g_ui.progVarIndex, "");
        SetText(g_ui.progVarValue, "");
        SetText(g_ui.missionHint, "Script program: no float variables");
        return;
    }
    SetText(g_ui.progVarIndex, std::to_string(varIdx));
    SetText(g_ui.progVarValue, FormatFloat3(ReadF32LE(pProg, prog.varsOff + (static_cast<std::size_t>(varIdx) * 4u))));

    std::ostringstream oss;
    oss << "Script source: " << g_state.save.segments[where->segIdx].name << " +" << prog.baseOff << ", vars="
        << prog.varCount;
    SetText(g_ui.missionHint, oss.str());
}

void FillActorEditor() {
    auto setCoreVisibility = [&]() {
        SetFieldVisible(g_ui.anameLabel, g_ui.aname, true);
        SetFieldVisible(g_ui.amodelLabel, g_ui.amodel, true);
        SetFieldVisible(g_ui.atypeLabel, g_ui.atype, true);
        SetFieldVisible(g_ui.aidxLabel, g_ui.aidx, true);
        SetFieldVisible(g_ui.apayloadLabel, g_ui.apayload, true);
    };
    auto setOptionalVisibility = [&](bool base,
                                     bool pos,
                                     bool dir,
                                     bool anim,
                                     bool humanState,
                                     bool humanInventory,
                                     bool quat,
                                     bool carState,
                                     bool carDrive,
                                     bool carEngineFlags,
                                     bool carOdometer) {
        SetFieldVisible(g_ui.pstateLabel, g_ui.pstate, base);
        SetFieldVisible(g_ui.pidLabel, g_ui.pid, base);
        SetFieldVisible(g_ui.pactiveLabel, g_ui.pactive, base);
        SetFieldVisible(g_ui.premoveLabel, g_ui.premove, base);
        SetFieldVisible(g_ui.pframeLabel, g_ui.pframe, base);
        SetFieldVisible(g_ui.posxLabel, g_ui.posx, pos);
        SetFieldVisible(g_ui.posyLabel, g_ui.posy, pos);
        SetFieldVisible(g_ui.poszLabel, g_ui.posz, pos);
        SetFieldVisible(g_ui.dirxLabel, g_ui.dirx, dir);
        SetFieldVisible(g_ui.diryLabel, g_ui.diry, dir);
        SetFieldVisible(g_ui.dirzLabel, g_ui.dirz, dir);
        SetFieldVisible(g_ui.animIdLabel, g_ui.animId, anim);
        SetFieldVisible(g_ui.humanSeatLabel, g_ui.humanSeat, humanState);
        SetFieldVisible(g_ui.humanCrouchLabel, g_ui.humanCrouch, humanState);
        SetFieldVisible(g_ui.humanAimLabel, g_ui.humanAim, humanState);
        SetFieldVisible(g_ui.humanShootXLabel, g_ui.humanShootX, humanState);
        SetFieldVisible(g_ui.humanShootYLabel, g_ui.humanShootY, humanState);
        SetFieldVisible(g_ui.humanShootZLabel, g_ui.humanShootZ, humanState);
        SetFieldVisible(g_ui.rotwLabel, g_ui.rotw, quat);
        SetFieldVisible(g_ui.rotxLabel, g_ui.rotx, quat);
        SetFieldVisible(g_ui.rotyLabel, g_ui.roty, quat);
        SetFieldVisible(g_ui.rotzLabel, g_ui.rotz, quat);
        SetFieldVisible(g_ui.carFuelLabel, g_ui.carFuel, carState);
        SetFieldVisible(g_ui.carFlowLabel, g_ui.carFlow, carState);
        SetFieldVisible(g_ui.carEngNormLabel, g_ui.carEngNorm, carState);
        SetFieldVisible(g_ui.carEngCalcLabel, g_ui.carEngCalc, carState);
        SetFieldVisible(g_ui.carSpeedLimitLabel, g_ui.carSpeedLimit, carDrive);
        SetFieldVisible(g_ui.carLastGearLabel, g_ui.carLastGear, carDrive);
        SetFieldVisible(g_ui.carGearLabel, g_ui.carGear, carDrive);
        SetFieldVisible(g_ui.carGearboxFlagLabel, g_ui.carGearboxFlag, carEngineFlags);
        SetFieldVisible(g_ui.carDisableEngineLabel, g_ui.carDisableEngine, carEngineFlags);
        SetFieldVisible(g_ui.carEngineOnLabel, g_ui.carEngineOn, carEngineFlags);
        SetFieldVisible(g_ui.carIsEngineOnLabel, g_ui.carIsEngineOn, carEngineFlags);
        SetFieldVisible(g_ui.carOdometerLabel, g_ui.carOdometer, carOdometer);
        SetInventoryVisibility(humanInventory);
    };

    setCoreVisibility();
    const auto segIdxOpt = CurrentSelectedActorSegIdx();
    if (!g_state.loaded || !segIdxOpt.has_value()) {
        setOptionalVisibility(false, false, false, false, false, false, false, false, false, false, false);
        SetText(g_ui.aname, "");
        SetText(g_ui.amodel, "");
        SetText(g_ui.atype, "");
        SetText(g_ui.aidx, "");
        SetText(g_ui.apayload, "");
        SetText(g_ui.pstate, "");
        SetText(g_ui.pid, "");
        SetText(g_ui.pactive, "");
        SetText(g_ui.premove, "");
        SetText(g_ui.pframe, "");
        SetText(g_ui.posx, "");
        SetText(g_ui.posy, "");
        SetText(g_ui.posz, "");
        SetText(g_ui.dirx, "");
        SetText(g_ui.diry, "");
        SetText(g_ui.dirz, "");
        SetText(g_ui.animId, "");
        SetText(g_ui.humanSeat, "");
        SetText(g_ui.humanCrouch, "");
        SetText(g_ui.humanAim, "");
        SetText(g_ui.humanShootX, "");
        SetText(g_ui.humanShootY, "");
        SetText(g_ui.humanShootZ, "");
        SetText(g_ui.rotw, "");
        SetText(g_ui.rotx, "");
        SetText(g_ui.roty, "");
        SetText(g_ui.rotz, "");
        SetText(g_ui.carFuel, "");
        SetText(g_ui.carFlow, "");
        SetText(g_ui.carEngNorm, "");
        SetText(g_ui.carEngCalc, "");
        SetText(g_ui.carSpeedLimit, "");
        SetText(g_ui.carLastGear, "");
        SetText(g_ui.carGear, "");
        SetText(g_ui.carGearboxFlag, "");
        SetText(g_ui.carDisableEngine, "");
        SetText(g_ui.carEngineOn, "");
        SetText(g_ui.carIsEngineOn, "");
        SetText(g_ui.carOdometer, "");
        SetText(g_ui.coordHint, "Payload: -");
        EnableWindow(g_ui.pstate, FALSE);
        EnableWindow(g_ui.pid, FALSE);
        EnableWindow(g_ui.pactive, FALSE);
        EnableWindow(g_ui.premove, FALSE);
        EnableWindow(g_ui.pframe, FALSE);
        EnableWindow(g_ui.posx, FALSE);
        EnableWindow(g_ui.posy, FALSE);
        EnableWindow(g_ui.posz, FALSE);
        EnableWindow(g_ui.dirx, FALSE);
        EnableWindow(g_ui.diry, FALSE);
        EnableWindow(g_ui.dirz, FALSE);
        EnableWindow(g_ui.animId, FALSE);
        EnableWindow(g_ui.humanSeat, FALSE);
        EnableWindow(g_ui.humanCrouch, FALSE);
        EnableWindow(g_ui.humanAim, FALSE);
        EnableWindow(g_ui.humanShootX, FALSE);
        EnableWindow(g_ui.humanShootY, FALSE);
        EnableWindow(g_ui.humanShootZ, FALSE);
        EnableWindow(g_ui.rotw, FALSE);
        EnableWindow(g_ui.rotx, FALSE);
        EnableWindow(g_ui.roty, FALSE);
        EnableWindow(g_ui.rotz, FALSE);
        EnableWindow(g_ui.carFuel, FALSE);
        EnableWindow(g_ui.carFlow, FALSE);
        EnableWindow(g_ui.carEngNorm, FALSE);
        EnableWindow(g_ui.carEngCalc, FALSE);
        EnableWindow(g_ui.carSpeedLimit, FALSE);
        EnableWindow(g_ui.carLastGear, FALSE);
        EnableWindow(g_ui.carGear, FALSE);
        EnableWindow(g_ui.carGearboxFlag, FALSE);
        EnableWindow(g_ui.carDisableEngine, FALSE);
        EnableWindow(g_ui.carEngineOn, FALSE);
        EnableWindow(g_ui.carIsEngineOn, FALSE);
        EnableWindow(g_ui.carOdometer, FALSE);
        ClearInventoryEdits();
        SetInventoryEnabled(false);
        LayoutActorsPage();
        return;
    }

    const std::size_t segIdx = *segIdxOpt;
    const auto& h = g_state.save.segments[segIdx].plain;

    SetText(g_ui.aname, ReadCStr(h, 0, 64));
    SetText(g_ui.amodel, ReadCStr(h, 64, 64));
    SetText(g_ui.atype, std::to_string(mafia_save::ReadU32LE(h, 128)));
    SetText(g_ui.aidx, std::to_string(mafia_save::ReadU32LE(h, 136)));
    SetText(g_ui.apayload, std::to_string(mafia_save::ReadU32LE(h, 132)));

    const CoordLayout layout = DetectCoordLayout(segIdx);
    if (!IsActorPairAt(segIdx)) {
        setOptionalVisibility(false, false, false, false, false, false, false, false, false, false, false);
        SetText(g_ui.pstate, "");
        SetText(g_ui.pid, "");
        SetText(g_ui.pactive, "");
        SetText(g_ui.premove, "");
        SetText(g_ui.pframe, "");
        SetText(g_ui.posx, "");
        SetText(g_ui.posy, "");
        SetText(g_ui.posz, "");
        SetText(g_ui.dirx, "");
        SetText(g_ui.diry, "");
        SetText(g_ui.dirz, "");
        SetText(g_ui.animId, "");
        SetText(g_ui.humanSeat, "");
        SetText(g_ui.humanCrouch, "");
        SetText(g_ui.humanAim, "");
        SetText(g_ui.humanShootX, "");
        SetText(g_ui.humanShootY, "");
        SetText(g_ui.humanShootZ, "");
        SetText(g_ui.rotw, "");
        SetText(g_ui.rotx, "");
        SetText(g_ui.roty, "");
        SetText(g_ui.rotz, "");
        SetText(g_ui.carFuel, "");
        SetText(g_ui.carFlow, "");
        SetText(g_ui.carEngNorm, "");
        SetText(g_ui.carEngCalc, "");
        SetText(g_ui.carSpeedLimit, "");
        SetText(g_ui.carLastGear, "");
        SetText(g_ui.carGear, "");
        SetText(g_ui.carGearboxFlag, "");
        SetText(g_ui.carDisableEngine, "");
        SetText(g_ui.carEngineOn, "");
        SetText(g_ui.carIsEngineOn, "");
        SetText(g_ui.carOdometer, "");
        SetText(g_ui.coordHint, "Payload: missing pair");
        EnableWindow(g_ui.pstate, FALSE);
        EnableWindow(g_ui.pid, FALSE);
        EnableWindow(g_ui.pactive, FALSE);
        EnableWindow(g_ui.premove, FALSE);
        EnableWindow(g_ui.pframe, FALSE);
        EnableWindow(g_ui.posx, FALSE);
        EnableWindow(g_ui.posy, FALSE);
        EnableWindow(g_ui.posz, FALSE);
        EnableWindow(g_ui.dirx, FALSE);
        EnableWindow(g_ui.diry, FALSE);
        EnableWindow(g_ui.dirz, FALSE);
        EnableWindow(g_ui.animId, FALSE);
        EnableWindow(g_ui.humanSeat, FALSE);
        EnableWindow(g_ui.humanCrouch, FALSE);
        EnableWindow(g_ui.humanAim, FALSE);
        EnableWindow(g_ui.humanShootX, FALSE);
        EnableWindow(g_ui.humanShootY, FALSE);
        EnableWindow(g_ui.humanShootZ, FALSE);
        EnableWindow(g_ui.rotw, FALSE);
        EnableWindow(g_ui.rotx, FALSE);
        EnableWindow(g_ui.roty, FALSE);
        EnableWindow(g_ui.rotz, FALSE);
        EnableWindow(g_ui.carFuel, FALSE);
        EnableWindow(g_ui.carFlow, FALSE);
        EnableWindow(g_ui.carEngNorm, FALSE);
        EnableWindow(g_ui.carEngCalc, FALSE);
        EnableWindow(g_ui.carSpeedLimit, FALSE);
        EnableWindow(g_ui.carLastGear, FALSE);
        EnableWindow(g_ui.carGear, FALSE);
        EnableWindow(g_ui.carGearboxFlag, FALSE);
        EnableWindow(g_ui.carDisableEngine, FALSE);
        EnableWindow(g_ui.carEngineOn, FALSE);
        EnableWindow(g_ui.carIsEngineOn, FALSE);
        EnableWindow(g_ui.carOdometer, FALSE);
        ClearInventoryEdits();
        SetInventoryEnabled(false);
        LayoutActorsPage();
        return;
    }
    setOptionalVisibility(layout.baseSupported,
                          layout.coordsSupported,
                          layout.dirSupported,
                          layout.animSupported,
                          layout.humanStateSupported,
                          layout.humanInventorySupported,
                          layout.quatSupported,
                          layout.carStateSupported,
                          layout.carDriveSupported,
                          layout.carEngineFlagsSupported,
                          layout.carOdometerSupported);
    const auto& p = g_state.save.segments[segIdx + 1].plain;
    if (layout.baseSupported) {
        SetText(g_ui.pstate, std::to_string(static_cast<unsigned>(p[layout.stateOff])));
        SetText(g_ui.pid, std::to_string(mafia_save::ReadU32LE(p, layout.idOff)));
        SetText(g_ui.pactive, std::to_string(static_cast<unsigned>(p[layout.activeOff])));
        SetText(g_ui.premove, std::to_string(static_cast<unsigned>(p[layout.removeOff])));
        SetText(g_ui.pframe, std::to_string(static_cast<unsigned>(p[layout.frameOff])));
        EnableWindow(g_ui.pstate, TRUE);
        EnableWindow(g_ui.pid, TRUE);
        EnableWindow(g_ui.pactive, TRUE);
        EnableWindow(g_ui.premove, TRUE);
        EnableWindow(g_ui.pframe, TRUE);
    } else {
        SetText(g_ui.pstate, "");
        SetText(g_ui.pid, "");
        SetText(g_ui.pactive, "");
        SetText(g_ui.premove, "");
        SetText(g_ui.pframe, "");
        EnableWindow(g_ui.pstate, FALSE);
        EnableWindow(g_ui.pid, FALSE);
        EnableWindow(g_ui.pactive, FALSE);
        EnableWindow(g_ui.premove, FALSE);
        EnableWindow(g_ui.pframe, FALSE);
    }

    if (layout.coordsSupported) {
        SetText(g_ui.posx, FormatFloat3(ReadF32LE(p, layout.xOff)));
        SetText(g_ui.posy, FormatFloat3(ReadF32LE(p, layout.yOff)));
        SetText(g_ui.posz, FormatFloat3(ReadF32LE(p, layout.zOff)));
        EnableWindow(g_ui.posx, TRUE);
        EnableWindow(g_ui.posy, TRUE);
        EnableWindow(g_ui.posz, TRUE);
    } else {
        SetText(g_ui.posx, "");
        SetText(g_ui.posy, "");
        SetText(g_ui.posz, "");
        SetText(g_ui.coordHint, layout.hint.empty() ? "Coords: -" : layout.hint);
        EnableWindow(g_ui.posx, FALSE);
        EnableWindow(g_ui.posy, FALSE);
        EnableWindow(g_ui.posz, FALSE);
    }

    if (layout.dirSupported) {
        SetText(g_ui.dirx, FormatFloat3(ReadF32LE(p, layout.dirXOff)));
        SetText(g_ui.diry, FormatFloat3(ReadF32LE(p, layout.dirYOff)));
        SetText(g_ui.dirz, FormatFloat3(ReadF32LE(p, layout.dirZOff)));
        EnableWindow(g_ui.dirx, TRUE);
        EnableWindow(g_ui.diry, TRUE);
        EnableWindow(g_ui.dirz, TRUE);
    } else {
        SetText(g_ui.dirx, "");
        SetText(g_ui.diry, "");
        SetText(g_ui.dirz, "");
        EnableWindow(g_ui.dirx, FALSE);
        EnableWindow(g_ui.diry, FALSE);
        EnableWindow(g_ui.dirz, FALSE);
    }

    if (layout.animSupported) {
        SetText(g_ui.animId, std::to_string(mafia_save::ReadU32LE(p, layout.animIdOff)));
        EnableWindow(g_ui.animId, TRUE);
    } else {
        SetText(g_ui.animId, "");
        EnableWindow(g_ui.animId, FALSE);
    }

    if (layout.humanStateSupported) {
        SetText(g_ui.humanSeat, std::to_string(mafia_save::ReadU32LE(p, layout.humanSeatOff)));
        SetText(g_ui.humanCrouch, std::to_string(static_cast<unsigned>(p[layout.humanCrouchOff])));
        SetText(g_ui.humanAim, std::to_string(static_cast<unsigned>(p[layout.humanAimOff])));
        SetText(g_ui.humanShootX, FormatFloat3(ReadF32LE(p, layout.humanShootXOff)));
        SetText(g_ui.humanShootY, FormatFloat3(ReadF32LE(p, layout.humanShootYOff)));
        SetText(g_ui.humanShootZ, FormatFloat3(ReadF32LE(p, layout.humanShootZOff)));
        EnableWindow(g_ui.humanSeat, TRUE);
        EnableWindow(g_ui.humanCrouch, TRUE);
        EnableWindow(g_ui.humanAim, TRUE);
        EnableWindow(g_ui.humanShootX, TRUE);
        EnableWindow(g_ui.humanShootY, TRUE);
        EnableWindow(g_ui.humanShootZ, TRUE);
    } else {
        SetText(g_ui.humanSeat, "");
        SetText(g_ui.humanCrouch, "");
        SetText(g_ui.humanAim, "");
        SetText(g_ui.humanShootX, "");
        SetText(g_ui.humanShootY, "");
        SetText(g_ui.humanShootZ, "");
        EnableWindow(g_ui.humanSeat, FALSE);
        EnableWindow(g_ui.humanCrouch, FALSE);
        EnableWindow(g_ui.humanAim, FALSE);
        EnableWindow(g_ui.humanShootX, FALSE);
        EnableWindow(g_ui.humanShootY, FALSE);
        EnableWindow(g_ui.humanShootZ, FALSE);
    }

    if (layout.humanInventorySupported) {
        FillInventoryEdits(p, layout.humanInventoryOff);
        SetInventoryEnabled(true);
    } else {
        ClearInventoryEdits();
        SetInventoryEnabled(false);
    }

    if (layout.quatSupported) {
        SetText(g_ui.rotw, FormatFloat3(ReadF32LE(p, layout.quatWOff)));
        SetText(g_ui.rotx, FormatFloat3(ReadF32LE(p, layout.quatXOff)));
        SetText(g_ui.roty, FormatFloat3(ReadF32LE(p, layout.quatYOff)));
        SetText(g_ui.rotz, FormatFloat3(ReadF32LE(p, layout.quatZOff)));
        EnableWindow(g_ui.rotw, TRUE);
        EnableWindow(g_ui.rotx, TRUE);
        EnableWindow(g_ui.roty, TRUE);
        EnableWindow(g_ui.rotz, TRUE);
    } else {
        SetText(g_ui.rotw, "");
        SetText(g_ui.rotx, "");
        SetText(g_ui.roty, "");
        SetText(g_ui.rotz, "");
        EnableWindow(g_ui.rotw, FALSE);
        EnableWindow(g_ui.rotx, FALSE);
        EnableWindow(g_ui.roty, FALSE);
        EnableWindow(g_ui.rotz, FALSE);
    }

    if (layout.carStateSupported) {
        SetText(g_ui.carFuel, FormatFloat3(ReadF32LE(p, layout.carFuelOff)));
        SetText(g_ui.carFlow, FormatFloat3(ReadF32LE(p, layout.carFlowOff)));
        SetText(g_ui.carEngNorm, FormatFloat3(ReadF32LE(p, layout.carEngNormOff)));
        SetText(g_ui.carEngCalc, FormatFloat3(ReadF32LE(p, layout.carEngCalcOff)));
        EnableWindow(g_ui.carFuel, TRUE);
        EnableWindow(g_ui.carFlow, TRUE);
        EnableWindow(g_ui.carEngNorm, TRUE);
        EnableWindow(g_ui.carEngCalc, TRUE);
    } else {
        SetText(g_ui.carFuel, "");
        SetText(g_ui.carFlow, "");
        SetText(g_ui.carEngNorm, "");
        SetText(g_ui.carEngCalc, "");
        EnableWindow(g_ui.carFuel, FALSE);
        EnableWindow(g_ui.carFlow, FALSE);
        EnableWindow(g_ui.carEngNorm, FALSE);
        EnableWindow(g_ui.carEngCalc, FALSE);
    }

    if (layout.carDriveSupported) {
        SetText(g_ui.carSpeedLimit, FormatFloat3(ReadF32LE(p, layout.carSpeedLimitOff)));
        SetText(g_ui.carLastGear, std::to_string(static_cast<std::int32_t>(mafia_save::ReadU32LE(p, layout.carLastGearOff))));
        SetText(g_ui.carGear, std::to_string(static_cast<std::int32_t>(mafia_save::ReadU32LE(p, layout.carGearOff))));
        EnableWindow(g_ui.carSpeedLimit, TRUE);
        EnableWindow(g_ui.carLastGear, TRUE);
        EnableWindow(g_ui.carGear, TRUE);
    } else {
        SetText(g_ui.carSpeedLimit, "");
        SetText(g_ui.carLastGear, "");
        SetText(g_ui.carGear, "");
        EnableWindow(g_ui.carSpeedLimit, FALSE);
        EnableWindow(g_ui.carLastGear, FALSE);
        EnableWindow(g_ui.carGear, FALSE);
    }

    if (layout.carEngineFlagsSupported) {
        SetText(g_ui.carGearboxFlag, std::to_string(mafia_save::ReadU32LE(p, layout.carGearboxFlagOff)));
        SetText(g_ui.carDisableEngine, std::to_string(static_cast<unsigned>(p[layout.carDisableEngineOff])));
        SetText(g_ui.carEngineOn, std::to_string(static_cast<unsigned>(p[layout.carEngineOnOff])));
        SetText(g_ui.carIsEngineOn, std::to_string(static_cast<unsigned>(p[layout.carIsEngineOnOff])));
        EnableWindow(g_ui.carGearboxFlag, TRUE);
        EnableWindow(g_ui.carDisableEngine, TRUE);
        EnableWindow(g_ui.carEngineOn, TRUE);
        EnableWindow(g_ui.carIsEngineOn, TRUE);
    } else {
        SetText(g_ui.carGearboxFlag, "");
        SetText(g_ui.carDisableEngine, "");
        SetText(g_ui.carEngineOn, "");
        SetText(g_ui.carIsEngineOn, "");
        EnableWindow(g_ui.carGearboxFlag, FALSE);
        EnableWindow(g_ui.carDisableEngine, FALSE);
        EnableWindow(g_ui.carEngineOn, FALSE);
        EnableWindow(g_ui.carIsEngineOn, FALSE);
    }

    if (layout.carOdometerSupported) {
        SetText(g_ui.carOdometer, FormatFloat3(ReadF32LE(p, layout.carOdometerOff)));
        EnableWindow(g_ui.carOdometer, TRUE);
    } else {
        SetText(g_ui.carOdometer, "");
        EnableWindow(g_ui.carOdometer, FALSE);
    }

    SetText(g_ui.coordHint, layout.hint.empty() ? "Payload: -" : layout.hint);
    LayoutActorsPage();
}

std::string BuildActorRow(std::size_t segIdx) {
    const auto& seg = g_state.save.segments[segIdx];
    const auto& h = seg.plain;
    std::ostringstream oss;
    oss << seg.name << " | " << ReadCStr(h, 0, 64) << " | " << ReadCStr(h, 64, 64)
        << " | t=" << mafia_save::ReadU32LE(h, 128) << " | idx=" << mafia_save::ReadU32LE(h, 136);
    return oss.str();
}

void FillActorList() {
    SendMessageA(g_ui.actors, LB_RESETCONTENT, 0, 0);
    if (!g_state.loaded) {
        return;
    }

    for (std::size_t i = 0; i < g_state.filteredActorHeaders.size(); ++i) {
        const std::string row = BuildActorRow(g_state.filteredActorHeaders[i]);
        SendMessageA(g_ui.actors, LB_ADDSTRING, 0, reinterpret_cast<LPARAM>(row.c_str()));
    }

    if (g_state.selectedActor >= 0 && g_state.selectedActor < static_cast<int>(g_state.filteredActorHeaders.size())) {
        SendMessageA(g_ui.actors, LB_SETCURSEL, static_cast<WPARAM>(g_state.selectedActor), 0);
    }
    FillActorEditor();
}

std::string BuildCarRow(std::size_t segIdx) {
    const auto& h = g_state.save.segments[segIdx].plain;
    std::ostringstream oss;
    oss << ReadCStr(h, 0, 64) << " | " << ReadCStr(h, 64, 64) << " | idx=" << mafia_save::ReadU32LE(h, 136);
    return oss.str();
}

void FillCarEditor() {
    auto clearFields = [&]() {
        SetText(g_ui.carTabName, "");
        SetText(g_ui.carTabModel, "");
        SetText(g_ui.carTabIdx, "");
        SetText(g_ui.carTabPosX, "");
        SetText(g_ui.carTabPosY, "");
        SetText(g_ui.carTabPosZ, "");
        SetText(g_ui.carTabRotW, "");
        SetText(g_ui.carTabRotX, "");
        SetText(g_ui.carTabRotY, "");
        SetText(g_ui.carTabRotZ, "");
        SetText(g_ui.carTabFuel, "");
        SetText(g_ui.carTabSpeed, "");
        SetText(g_ui.carTabOdometer, "");
        SetText(g_ui.carTabEngineOn, "");
        SetText(g_ui.carsHint, "Cars: -");
    };
    auto setEditable = [&](bool pos, bool rot, bool fuel, bool speed, bool odometer, bool engineOn) {
        EnableWindow(g_ui.carTabPosX, pos ? TRUE : FALSE);
        EnableWindow(g_ui.carTabPosY, pos ? TRUE : FALSE);
        EnableWindow(g_ui.carTabPosZ, pos ? TRUE : FALSE);
        EnableWindow(g_ui.carTabRotW, rot ? TRUE : FALSE);
        EnableWindow(g_ui.carTabRotX, rot ? TRUE : FALSE);
        EnableWindow(g_ui.carTabRotY, rot ? TRUE : FALSE);
        EnableWindow(g_ui.carTabRotZ, rot ? TRUE : FALSE);
        EnableWindow(g_ui.carTabFuel, fuel ? TRUE : FALSE);
        EnableWindow(g_ui.carTabSpeed, speed ? TRUE : FALSE);
        EnableWindow(g_ui.carTabOdometer, odometer ? TRUE : FALSE);
        EnableWindow(g_ui.carTabEngineOn, engineOn ? TRUE : FALSE);
    };

    const auto segIdxOpt = CurrentSelectedCarSegIdx();
    if (!g_state.loaded || !segIdxOpt.has_value()) {
        clearFields();
        setEditable(false, false, false, false, false, false);
        return;
    }

    const std::size_t segIdx = *segIdxOpt;
    const auto& h = g_state.save.segments[segIdx].plain;
    SetText(g_ui.carTabName, ReadCStr(h, 0, 64));
    SetText(g_ui.carTabModel, ReadCStr(h, 64, 64));
    SetText(g_ui.carTabIdx, std::to_string(mafia_save::ReadU32LE(h, 136)));

    if (!IsActorPairAt(segIdx)) {
        clearFields();
        SetText(g_ui.carTabName, ReadCStr(h, 0, 64));
        SetText(g_ui.carTabModel, ReadCStr(h, 64, 64));
        SetText(g_ui.carTabIdx, std::to_string(mafia_save::ReadU32LE(h, 136)));
        SetText(g_ui.carsHint, "Cars: payload pair missing");
        setEditable(false, false, false, false, false, false);
        return;
    }

    const auto layout = DetectCoordLayout(segIdx);
    const auto& p = g_state.save.segments[segIdx + 1].plain;

    if (layout.coordsSupported) {
        SetText(g_ui.carTabPosX, FormatFloat3(ReadF32LE(p, layout.xOff)));
        SetText(g_ui.carTabPosY, FormatFloat3(ReadF32LE(p, layout.yOff)));
        SetText(g_ui.carTabPosZ, FormatFloat3(ReadF32LE(p, layout.zOff)));
    } else {
        SetText(g_ui.carTabPosX, "");
        SetText(g_ui.carTabPosY, "");
        SetText(g_ui.carTabPosZ, "");
    }
    if (layout.quatSupported) {
        SetText(g_ui.carTabRotW, FormatFloat3(ReadF32LE(p, layout.quatWOff)));
        SetText(g_ui.carTabRotX, FormatFloat3(ReadF32LE(p, layout.quatXOff)));
        SetText(g_ui.carTabRotY, FormatFloat3(ReadF32LE(p, layout.quatYOff)));
        SetText(g_ui.carTabRotZ, FormatFloat3(ReadF32LE(p, layout.quatZOff)));
    } else {
        SetText(g_ui.carTabRotW, "");
        SetText(g_ui.carTabRotX, "");
        SetText(g_ui.carTabRotY, "");
        SetText(g_ui.carTabRotZ, "");
    }
    if (layout.carStateSupported) {
        SetText(g_ui.carTabFuel, FormatFloat3(ReadF32LE(p, layout.carFuelOff)));
    } else {
        SetText(g_ui.carTabFuel, "");
    }
    if (layout.carDriveSupported) {
        SetText(g_ui.carTabSpeed, FormatFloat3(ReadF32LE(p, layout.carSpeedLimitOff)));
    } else {
        SetText(g_ui.carTabSpeed, "");
    }
    if (layout.carOdometerSupported) {
        SetText(g_ui.carTabOdometer, FormatFloat3(ReadF32LE(p, layout.carOdometerOff)));
    } else {
        SetText(g_ui.carTabOdometer, "");
    }
    if (layout.carEngineFlagsSupported) {
        SetText(g_ui.carTabEngineOn, std::to_string(static_cast<unsigned>(p[layout.carEngineOnOff])));
    } else {
        SetText(g_ui.carTabEngineOn, "");
    }

    setEditable(layout.coordsSupported,
                layout.quatSupported,
                layout.carStateSupported,
                layout.carDriveSupported,
                layout.carOdometerSupported,
                layout.carEngineFlagsSupported);
    SetText(g_ui.carsHint, layout.hint.empty() ? "Cars: mapped fields ready" : layout.hint);
}

void FillCarsList() {
    SendMessageA(g_ui.carsList, LB_RESETCONTENT, 0, 0);
    if (!g_state.loaded) {
        FillCarEditor();
        return;
    }

    for (std::size_t i = 0; i < g_state.carHeaders.size(); ++i) {
        const std::string row = BuildCarRow(g_state.carHeaders[i]);
        SendMessageA(g_ui.carsList, LB_ADDSTRING, 0, reinterpret_cast<LPARAM>(row.c_str()));
    }

    if (g_state.selectedCar >= 0 && g_state.selectedCar < static_cast<int>(g_state.carHeaders.size())) {
        SendMessageA(g_ui.carsList, LB_SETCURSEL, static_cast<WPARAM>(g_state.selectedCar), 0);
    }
    FillCarEditor();
}

constexpr std::size_t kGarageSlotCount = 25;
constexpr std::size_t kGaragePrimaryOff = 40;
constexpr std::size_t kGarageSecondaryOff = 140;
constexpr const char* kEmbeddedGarageCarNames[] = {
    "Bolt Ace Tudor",
    "Bolt Ace Touring",
    "Bolt Ace Runabout",
    "Bolt Ace Pickup",
    "Bolt Ace Fordor",
    "Bolt Ace Coupe",
    "Bolt Model B Tudor",
    "Bolt Model B Roadster",
    "Bolt Model B Pickup",
    "Bolt Model B Fordor",
    "Bolt Model B Delivery",
    "Bolt Model B Coupe",
    "Bolt Model B Cabriolet",
    "Schubert Six",
    "Bolt V8 Coupe",
    "Bolt V8 Fordor",
    "Bolt V8 Roadster",
    "Bolt V8 Touring",
    "Bolt V8 Tudor",
    "Schubert Extra Six Fordor",
    "Schubert Extra Six Tudor",
    "Falconer",
    "Falconer Yellowcar",
    "Crusader Chromium Fordor",
    "Crusader Chromium Tudor",
    "Guardian Terraplane Coupe",
    "Guardian Terraplane Fordor",
    "Guardian Terraplane Tudor",
    "Thor 812 Cabriolet FWD",
    "Thor 810 Phaeton FWD",
    "Thor 810 Sedan FWD",
    "Wright Coupe",
    "Wright Fordor",
    "Bruno Speedster 851",
    "Celeste Marque 500",
    "Lassiter V16 Fordor",
    "Lassiter V16 Phaeton",
    "Lassiter V16 Roadster",
    "Silver Fletcher",
    "Lassiter V16 Appolyon",
    "Manta Prototype",
    "Trautenberg Model J",
    "Carrozella C-Otto 4WD",
    "Brubaker 4WD",
    "Trautenberg Racer 4WD",
    "Caesar 8C Mostro",
    "Bolt Ambulance",
    "Bolt Firetruck",
    "Bolt Hearse",
    "Lassiter V16 Charon",
    "Ulver Airstream Fordor",
    "Ulver Airstream Tudor",
    "Lassiter V16 Police",
    "Schubert Six Police",
    "Schubert Extra 6 Police Fordor",
    "Schubert Extra 6 Police Tudor",
    "Bolt Truck Flatbed",
    "Bolt Truck Covered",
    "Caesar 8C 2300 Racing",
    "Bolt-Thrower",
    "Bolt Truck",
    "HotRod",
    "Wright Coupe Gangster",
    "Falconer Gangster",
    "Trautenberg Model J",
    "Black Dragon 4WD",
    "Mutagen FWD",
    "Flamer",
    "Masseur",
    "Masseur Taxi",
    "Demoniac",
    "Crazy Horse",
    "Bob Mylan 4WD",
    "Disorder 4WD",
    "Speedee 4WD",
    "Luciferion FWD",
    "Black Metal 4WD",
    "Hillbilly 5.1 FWD",
    "Flower Power",
    "Flame Spear 4WD",
    "Manta Taxi FWD",
};

bool IsGarageCodeToken(const std::string& s) {
    if (s.size() < 3 || s.size() > 24) {
        return false;
    }
    for (char ch : s) {
        if (!(std::isalnum(static_cast<unsigned char>(ch)) || ch == '_')) {
            return false;
        }
    }
    return true;
}

bool IsGarageDisplayNameToken(const std::string& s) {
    if (s.size() < 3 || s.size() > 64) {
        return false;
    }
    for (char ch : s) {
        const bool ok = std::isalnum(static_cast<unsigned char>(ch)) || ch == ' ' || ch == '-' || ch == '.' || ch == '\'';
        if (!ok) {
            return false;
        }
    }
    return true;
}

std::vector<std::string> ExtractAsciiRuns(const std::vector<std::uint8_t>& bytes, std::size_t minLen) {
    std::vector<std::string> out;
    std::string cur;
    for (std::uint8_t b : bytes) {
        if (b >= 32u && b <= 126u) {
            cur.push_back(static_cast<char>(b));
        } else {
            if (cur.size() >= minLen) {
                out.push_back(cur);
            }
            cur.clear();
        }
    }
    if (cur.size() >= minLen) {
        out.push_back(cur);
    }
    return out;
}

bool ParseGarageCatalogFromFile(const fs::path& path, std::vector<GarageCarCatalogEntry>* out, std::string* err) {
    if (out == nullptr) {
        if (err != nullptr) {
            *err = "internal null catalog output";
        }
        return false;
    }
    const auto bytes = mafia_save::ReadFileBytes(path);
    if (bytes.empty()) {
        if (err != nullptr) {
            *err = "catalog file is empty or unreadable";
        }
        return false;
    }

    const auto runs = ExtractAsciiRuns(bytes, 4);
    std::vector<GarageCarCatalogEntry> parsed;
    for (std::size_t i = 0; i + 3 < runs.size(); ++i) {
        const std::string& code = runs[i];
        const std::string& model = runs[i + 1];
        const std::string& shadow = runs[i + 2];
        const std::string& name = runs[i + 3];
        if (!IsGarageCodeToken(code) || !EndsWithI3d(model) || !EndsWithI3d(shadow) || !IsGarageDisplayNameToken(name)) {
            continue;
        }
        GarageCarCatalogEntry e;
        e.index = static_cast<std::uint32_t>(parsed.size());
        e.code = code;
        e.model = model;
        e.shadow = shadow;
        e.displayName = name;
        parsed.push_back(std::move(e));
        i += 3;
    }

    if (parsed.size() < 20) {
        if (err != nullptr) {
            *err = "catalog parse produced too few entries";
        }
        return false;
    }
    *out = std::move(parsed);
    return true;
}

void LoadEmbeddedGarageCatalog(std::vector<GarageCarCatalogEntry>* out) {
    if (out == nullptr) {
        return;
    }
    out->clear();
    out->reserve(sizeof(kEmbeddedGarageCarNames) / sizeof(kEmbeddedGarageCarNames[0]));
    for (std::size_t i = 0; i < (sizeof(kEmbeddedGarageCarNames) / sizeof(kEmbeddedGarageCarNames[0])); ++i) {
        GarageCarCatalogEntry e;
        e.index = static_cast<std::uint32_t>(i);
        e.displayName = kEmbeddedGarageCarNames[i];
        out->push_back(std::move(e));
    }
}

void RefreshGarageCatalog() {
    g_garageCatalog.clear();
    g_state.garageCatalogLoaded = false;
    g_state.garageCatalogEmbedded = false;
    g_state.garageCatalogPath.clear();

    auto addUnique = [](std::vector<fs::path>* out, const fs::path& p) {
        const fs::path norm = p.lexically_normal();
        for (const auto& e : *out) {
            if (e.lexically_normal() == norm) {
                return;
            }
        }
        out->push_back(norm);
    };

    std::vector<fs::path> roots;
    addUnique(&roots, fs::current_path());

    if (!g_state.inputPath.empty()) {
        const fs::path saveDir = g_state.inputPath.parent_path();
        if (!saveDir.empty()) {
            addUnique(&roots, saveDir);
            addUnique(&roots, saveDir.parent_path());
            addUnique(&roots, saveDir.parent_path().parent_path());
        }
    }

    char exePathBuf[MAX_PATH] = {};
    const DWORD exeLen = GetModuleFileNameA(nullptr, exePathBuf, MAX_PATH);
    if (exeLen > 0 && exeLen < MAX_PATH) {
        fs::path exeDir = fs::path(exePathBuf).parent_path();
        for (int i = 0; i < 5 && !exeDir.empty(); ++i) {
            addUnique(&roots, exeDir);
            exeDir = exeDir.parent_path();
        }
    }

    std::vector<fs::path> candidates;
    for (const auto& root : roots) {
        addUnique(&candidates, root / "Mafia" / "tables" / "carindex.def");
        addUnique(&candidates, root / "Mafia" / "tables" / "carcyclopedia.def");
        addUnique(&candidates, root / "tables" / "carindex.def");
        addUnique(&candidates, root / "tables" / "carcyclopedia.def");
    }

    for (const auto& p : candidates) {
        if (!fs::exists(p)) {
            continue;
        }
        std::string err;
        std::vector<GarageCarCatalogEntry> parsed;
        if (ParseGarageCatalogFromFile(p, &parsed, &err)) {
            g_garageCatalog = std::move(parsed);
            g_state.garageCatalogLoaded = true;
            g_state.garageCatalogEmbedded = false;
            g_state.garageCatalogPath = p;
            return;
        }
    }

    LoadEmbeddedGarageCatalog(&g_garageCatalog);
    if (!g_garageCatalog.empty()) {
        g_state.garageCatalogLoaded = true;
        g_state.garageCatalogEmbedded = true;
        g_state.garageCatalogPath.clear();
    }
}

const GarageCarCatalogEntry* FindGarageCarByIndex(std::uint32_t idx) {
    if (idx < g_garageCatalog.size() && g_garageCatalog[idx].index == idx) {
        return &g_garageCatalog[idx];
    }
    for (const auto& e : g_garageCatalog) {
        if (e.index == idx) {
            return &e;
        }
    }
    return nullptr;
}

std::string GarageCatalogEntryText(const GarageCarCatalogEntry& e) {
    std::ostringstream oss;
    oss << "[" << e.index << "] " << e.displayName;
    if (!e.code.empty()) {
        oss << " (" << e.code << ")";
    }
    return oss.str();
}

std::string GarageDecodedText(std::uint32_t value) {
    if (value == 0u) {
        return "empty";
    }
    const std::uint32_t idx = value & 0xFFFFu;
    const std::uint32_t hi = (value >> 24) & 0xFFu;
    const std::uint32_t color = (value >> 16) & 0xFFu;
    std::ostringstream oss;
    oss << "idx=" << idx;
    if (const auto* car = FindGarageCarByIndex(idx); car != nullptr) {
        oss << " " << car->displayName;
    } else {
        oss << " <unknown>";
    }
    oss << " | color=" << color;
    oss << " | flag=" << hi;
    return oss.str();
}

std::string GarageRowSummary(std::uint32_t value) {
    if (value == 0u) {
        return "empty";
    }
    const std::uint32_t idx = value & 0xFFFFu;
    const std::uint32_t color = (value >> 16) & 0xFFu;
    const std::uint32_t flag = (value >> 24) & 0xFFu;
    std::ostringstream oss;
    if (const auto* car = FindGarageCarByIndex(idx); car != nullptr) {
        oss << "#" << idx << " " << car->displayName;
    } else {
        oss << "#" << idx;
    }
    if (color != 0u) {
        oss << " c=" << color;
    }
    if (flag != 0u) {
        oss << " f=" << flag;
    }
    return oss.str();
}

void PopulateGarageCarCombo(HWND combo) {
    SendMessageA(combo, CB_RESETCONTENT, 0, 0);
    const LRESULT nonePos = SendMessageA(combo, CB_ADDSTRING, 0, reinterpret_cast<LPARAM>("<keep raw index>"));
    if (nonePos != CB_ERR && nonePos != CB_ERRSPACE) {
        SendMessageA(combo, CB_SETITEMDATA, static_cast<WPARAM>(nonePos), static_cast<LPARAM>(-1));
    }
    if (!g_garageCatalog.empty()) {
        for (const auto& e : g_garageCatalog) {
            const std::string text = GarageCatalogEntryText(e);
            const LRESULT pos = SendMessageA(combo, CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(text.c_str()));
            if (pos != CB_ERR && pos != CB_ERRSPACE) {
                SendMessageA(combo, CB_SETITEMDATA, static_cast<WPARAM>(pos), static_cast<LPARAM>(e.index));
            }
        }
    } else {
        for (int i = 0; i <= 255; ++i) {
            std::ostringstream oss;
            oss << "[" << i << "] idx only";
            const std::string text = oss.str();
            const LRESULT pos = SendMessageA(combo, CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(text.c_str()));
            if (pos != CB_ERR && pos != CB_ERRSPACE) {
                SendMessageA(combo, CB_SETITEMDATA, static_cast<WPARAM>(pos), static_cast<LPARAM>(i));
            }
        }
    }
    SendMessageA(combo, CB_SETCURSEL, 0, 0);
}

int FindComboItemByData(HWND combo, std::int32_t wanted) {
    const LRESULT count = SendMessageA(combo, CB_GETCOUNT, 0, 0);
    for (LRESULT i = 0; i < count; ++i) {
        const LRESULT data = SendMessageA(combo, CB_GETITEMDATA, static_cast<WPARAM>(i), 0);
        if (static_cast<std::int32_t>(data) == wanted) {
            return static_cast<int>(i);
        }
    }
    return 0;
}

bool HasGarageInfoData() {
    if (!g_state.loaded) {
        return false;
    }
    if (g_state.save.idxInfo == mafia_save::kNoIndex || g_state.save.idxInfo >= g_state.save.segments.size()) {
        return false;
    }
    const auto& info = g_state.save.segments[g_state.save.idxInfo].plain;
    return info.size() >= (kGarageSecondaryOff + kGarageSlotCount * 4);
}

std::uint32_t ReadGaragePrimary(int slot) {
    const auto& info = g_state.save.segments[g_state.save.idxInfo].plain;
    return mafia_save::ReadU32LE(info, kGaragePrimaryOff + static_cast<std::size_t>(slot) * 4);
}

std::uint32_t ReadGarageSecondary(int slot) {
    const auto& info = g_state.save.segments[g_state.save.idxInfo].plain;
    return mafia_save::ReadU32LE(info, kGarageSecondaryOff + static_cast<std::size_t>(slot) * 4);
}

void WriteGaragePrimary(int slot, std::uint32_t value) {
    auto& info = g_state.save.segments[g_state.save.idxInfo].plain;
    mafia_save::WriteU32LE(&info, kGaragePrimaryOff + static_cast<std::size_t>(slot) * 4, value);
}

void WriteGarageSecondary(int slot, std::uint32_t value) {
    auto& info = g_state.save.segments[g_state.save.idxInfo].plain;
    mafia_save::WriteU32LE(&info, kGarageSecondaryOff + static_cast<std::size_t>(slot) * 4, value);
}

std::string BuildGarageRow(int slot) {
    std::ostringstream oss;
    oss << "slot_" << slot;
    if (!HasGarageInfoData()) {
        oss << " | n/a";
        return oss.str();
    }
    const std::uint32_t a = ReadGaragePrimary(slot);
    const std::uint32_t b = ReadGarageSecondary(slot);
    oss << " | A: " << GarageRowSummary(a);
    oss << " | B: " << GarageRowSummary(b);
    return oss.str();
}

void FillGarageEditor() {
    auto clearAll = [&]() {
        SetText(g_ui.garageSlot, "");
        SetText(g_ui.garageA, "");
        SetText(g_ui.garageB, "");
        SetText(g_ui.garageAHex, "");
        SetText(g_ui.garageBHex, "");
        SetText(g_ui.garageALow16, "");
        SetText(g_ui.garageBLow16, "");
        SetText(g_ui.garageAHi8, "");
        SetText(g_ui.garageBHi8, "");
        SetText(g_ui.garageAFlags, "");
        SetText(g_ui.garageBFlags, "");
        SetText(g_ui.garageAColor, "");
        SetText(g_ui.garageBColor, "");
        SetText(g_ui.garageADecoded, "");
        SetText(g_ui.garageBDecoded, "");
        SendMessageA(g_ui.garageACar, CB_SETCURSEL, 0, 0);
        SendMessageA(g_ui.garageBCar, CB_SETCURSEL, 0, 0);
        SetText(g_ui.garageHint, "Garage: -");
    };

    if (!HasGarageInfoData() || g_state.selectedGarageSlot < 0 ||
        g_state.selectedGarageSlot >= static_cast<int>(kGarageSlotCount)) {
        clearAll();
        return;
    }

    const int slot = g_state.selectedGarageSlot;
    const std::uint32_t a = ReadGaragePrimary(slot);
    const std::uint32_t b = ReadGarageSecondary(slot);

    SetText(g_ui.garageSlot, std::to_string(slot));
    SetText(g_ui.garageA, std::to_string(a));
    SetText(g_ui.garageB, std::to_string(b));
    SetText(g_ui.garageAHex, FormatHex32(a));
    SetText(g_ui.garageBHex, FormatHex32(b));
    SetText(g_ui.garageALow16, std::to_string(a & 0xFFFFu));
    SetText(g_ui.garageBLow16, std::to_string(b & 0xFFFFu));
    SetText(g_ui.garageAHi8, std::to_string((a >> 24) & 0xFFu));
    SetText(g_ui.garageBHi8, std::to_string((b >> 24) & 0xFFu));
    SetText(g_ui.garageAFlags, std::to_string((a >> 24) & 0xFFu));
    SetText(g_ui.garageBFlags, std::to_string((b >> 24) & 0xFFu));
    SetText(g_ui.garageAColor, std::to_string((a >> 16) & 0xFFu));
    SetText(g_ui.garageBColor, std::to_string((b >> 16) & 0xFFu));
    SetText(g_ui.garageADecoded, GarageDecodedText(a));
    SetText(g_ui.garageBDecoded, GarageDecodedText(b));

    const int selA = FindComboItemByData(g_ui.garageACar, static_cast<std::int32_t>(a & 0xFFFFu));
    const int selB = FindComboItemByData(g_ui.garageBCar, static_cast<std::int32_t>(b & 0xFFFFu));
    SendMessageA(g_ui.garageACar, CB_SETCURSEL, static_cast<WPARAM>(selA), 0);
    SendMessageA(g_ui.garageBCar, CB_SETCURSEL, static_cast<WPARAM>(selB), 0);

    std::ostringstream hint;
    hint << "Garage: info264 [40..139]=A, [140..239]=B";
    if (g_state.garageCatalogLoaded) {
        if (g_state.garageCatalogEmbedded) {
            hint << " | catalog: embedded (" << g_garageCatalog.size() << " cars)";
        } else {
            hint << " | carindex loaded: " << g_garageCatalog.size() << " cars (" << g_state.garageCatalogPath.string()
                 << ")";
        }
    } else {
        hint << " | carindex not loaded";
    }
    SetText(g_ui.garageHint, hint.str());
}

void RefreshGaragePreviewFromFields() {
    std::uint32_t a = 0;
    std::uint32_t b = 0;
    std::string err;
    const bool aOk = ParseU32Auto(GetText(g_ui.garageA), &a, &err, "Garage A");
    err.clear();
    const bool bOk = ParseU32Auto(GetText(g_ui.garageB), &b, &err, "Garage B");

    std::uint8_t aFlag = static_cast<std::uint8_t>((a >> 24) & 0xFFu);
    std::uint8_t bFlag = static_cast<std::uint8_t>((b >> 24) & 0xFFu);
    std::uint8_t aColor = static_cast<std::uint8_t>((a >> 16) & 0xFFu);
    std::uint8_t bColor = static_cast<std::uint8_t>((b >> 16) & 0xFFu);
    err.clear();
    ParseByteField(GetText(g_ui.garageAFlags), &aFlag, &err, "Garage A flag", 255);
    err.clear();
    ParseByteField(GetText(g_ui.garageBFlags), &bFlag, &err, "Garage B flag", 255);
    err.clear();
    ParseByteField(GetText(g_ui.garageAColor), &aColor, &err, "Garage A color", 255);
    err.clear();
    ParseByteField(GetText(g_ui.garageBColor), &bColor, &err, "Garage B color", 255);
    a = (a & 0xFF00FFFFu) | (static_cast<std::uint32_t>(aColor) << 16);
    b = (b & 0xFF00FFFFu) | (static_cast<std::uint32_t>(bColor) << 16);
    a = (a & 0x00FFFFFFu) | (static_cast<std::uint32_t>(aFlag) << 24);
    b = (b & 0x00FFFFFFu) | (static_cast<std::uint32_t>(bFlag) << 24);

    SetText(g_ui.garageAHex, aOk ? FormatHex32(a) : "invalid");
    SetText(g_ui.garageBHex, bOk ? FormatHex32(b) : "invalid");
    SetText(g_ui.garageALow16, aOk ? std::to_string(a & 0xFFFFu) : "");
    SetText(g_ui.garageBLow16, bOk ? std::to_string(b & 0xFFFFu) : "");
    SetText(g_ui.garageAHi8, aOk ? std::to_string((a >> 24) & 0xFFu) : "");
    SetText(g_ui.garageBHi8, bOk ? std::to_string((b >> 24) & 0xFFu) : "");
    SetText(g_ui.garageADecoded, aOk ? GarageDecodedText(a) : "invalid");
    SetText(g_ui.garageBDecoded, bOk ? GarageDecodedText(b) : "invalid");

    if (aOk) {
        const int selA = FindComboItemByData(g_ui.garageACar, static_cast<std::int32_t>(a & 0xFFFFu));
        SendMessageA(g_ui.garageACar, CB_SETCURSEL, static_cast<WPARAM>(selA), 0);
    }
    if (bOk) {
        const int selB = FindComboItemByData(g_ui.garageBCar, static_cast<std::int32_t>(b & 0xFFFFu));
        SendMessageA(g_ui.garageBCar, CB_SETCURSEL, static_cast<WPARAM>(selB), 0);
    }
}

void UpdateGarageRawFromCombo(HWND combo, HWND rawField) {
    const LRESULT sel = SendMessageA(combo, CB_GETCURSEL, 0, 0);
    if (sel == CB_ERR) {
        return;
    }
    const LRESULT itemData = SendMessageA(combo, CB_GETITEMDATA, static_cast<WPARAM>(sel), 0);
    if (itemData < 0) {
        return;
    }
    std::uint32_t raw = 0;
    std::string err;
    ParseU32Auto(GetText(rawField), &raw, &err, "garage raw");
    raw = (raw & 0xFFFF0000u) | (static_cast<std::uint32_t>(itemData) & 0xFFFFu);
    SetText(rawField, std::to_string(raw));
}

void FillGarageList() {
    if (!g_state.garageCatalogLoaded) {
        RefreshGarageCatalog();
    }
    PopulateGarageCarCombo(g_ui.garageACar);
    PopulateGarageCarCombo(g_ui.garageBCar);

    SendMessageA(g_ui.garageList, LB_RESETCONTENT, 0, 0);
    if (!g_state.loaded || !HasGarageInfoData()) {
        FillGarageEditor();
        return;
    }

    for (int i = 0; i < static_cast<int>(kGarageSlotCount); ++i) {
        const std::string row = BuildGarageRow(i);
        SendMessageA(g_ui.garageList, LB_ADDSTRING, 0, reinterpret_cast<LPARAM>(row.c_str()));
    }
    if (g_state.selectedGarageSlot < 0 || g_state.selectedGarageSlot >= static_cast<int>(kGarageSlotCount)) {
        g_state.selectedGarageSlot = 0;
    }
    SendMessageA(g_ui.garageList, LB_SETCURSEL, static_cast<WPARAM>(g_state.selectedGarageSlot), 0);
    FillGarageEditor();
}

bool ApplyGarageEdits(std::string* err) {
    if (!HasGarageInfoData()) {
        return true;
    }
    if (g_state.selectedGarageSlot < 0 || g_state.selectedGarageSlot >= static_cast<int>(kGarageSlotCount)) {
        if (err != nullptr) {
            *err = "garage slot is not selected";
        }
        return false;
    }

    std::uint32_t a = 0;
    std::uint32_t b = 0;
    if (!ParseU32Auto(GetText(g_ui.garageA), &a, err, "Garage A")) {
        return false;
    }
    if (!ParseU32Auto(GetText(g_ui.garageB), &b, err, "Garage B")) {
        return false;
    }

    std::uint8_t aFlag = 0;
    std::uint8_t bFlag = 0;
    std::uint8_t aColor = 0;
    std::uint8_t bColor = 0;
    if (!ParseByteField(GetText(g_ui.garageAFlags), &aFlag, err, "Garage A flag", 255)) {
        return false;
    }
    if (!ParseByteField(GetText(g_ui.garageBFlags), &bFlag, err, "Garage B flag", 255)) {
        return false;
    }
    if (!ParseByteField(GetText(g_ui.garageAColor), &aColor, err, "Garage A color", 255)) {
        return false;
    }
    if (!ParseByteField(GetText(g_ui.garageBColor), &bColor, err, "Garage B color", 255)) {
        return false;
    }
    a = (a & 0xFF00FFFFu) | (static_cast<std::uint32_t>(aColor) << 16);
    b = (b & 0xFF00FFFFu) | (static_cast<std::uint32_t>(bColor) << 16);
    a = (a & 0x00FFFFFFu) | (static_cast<std::uint32_t>(aFlag) << 24);
    b = (b & 0x00FFFFFFu) | (static_cast<std::uint32_t>(bFlag) << 24);

    const LRESULT selA = SendMessageA(g_ui.garageACar, CB_GETCURSEL, 0, 0);
    if (selA != CB_ERR) {
        const LRESULT data = SendMessageA(g_ui.garageACar, CB_GETITEMDATA, static_cast<WPARAM>(selA), 0);
        if (data >= 0) {
            a = (a & 0xFFFF0000u) | (static_cast<std::uint32_t>(data) & 0xFFFFu);
        }
    }
    const LRESULT selB = SendMessageA(g_ui.garageBCar, CB_GETCURSEL, 0, 0);
    if (selB != CB_ERR) {
        const LRESULT data = SendMessageA(g_ui.garageBCar, CB_GETITEMDATA, static_cast<WPARAM>(selB), 0);
        if (data >= 0) {
            b = (b & 0xFFFF0000u) | (static_cast<std::uint32_t>(data) & 0xFFFFu);
        }
    }

    WriteGaragePrimary(g_state.selectedGarageSlot, a);
    WriteGarageSecondary(g_state.selectedGarageSlot, b);
    return true;
}

void RefreshInfo() {
    if (!g_state.loaded) {
        SetText(g_ui.path, "No file loaded");
        SetText(g_ui.info, "Mission: -");
        return;
    }

    SetText(g_ui.path, g_state.inputPath.string());

    std::string err;
    mafia_save::MetaFields meta;
    if (!mafia_save::ReadMetaFields(g_state.save, &meta, &err)) {
        SetText(g_ui.info, "Meta read error: " + err);
        return;
    }

    const std::string mission = mafia_save::ReadMissionName(g_state.save, &err);
    std::ostringstream oss;
    oss << "Mission=" << mission << " | slot=" << meta.slot << " | code=" << meta.missionCode
        << " | actors=" << g_state.actorHeaders.size() << " | visible=" << g_state.filteredActorHeaders.size();
    SetText(g_ui.info, oss.str());
}

void SetEnabledMain(bool enabled) {
    const HWND arr[] = {g_ui.hp, g_ui.date, g_ui.time, g_ui.slot, g_ui.mcode, g_ui.mname};
    for (HWND h : arr) {
        EnableWindow(h, enabled ? TRUE : FALSE);
    }
}

void SetEnabledMission(bool enabled) {
    const HWND arr[] = {g_ui.ghFieldA,       g_ui.ghFieldB,      g_ui.ghMissionId,   g_ui.ghTimerOn,
                        g_ui.ghTimerInterval, g_ui.ghTimerA,      g_ui.ghTimerB,      g_ui.ghTimerC,
                        g_ui.ghScoreOn,      g_ui.ghScoreValue};
    for (HWND h : arr) {
        EnableWindow(h, enabled ? TRUE : FALSE);
    }
    if (!enabled) {
        SetMissionScriptControlsEnabled(false);
    }
}

void SetEnabledCars(bool enabled) {
    const HWND arr[] = {g_ui.carsList,    g_ui.carTabName,    g_ui.carTabModel,   g_ui.carTabIdx,
                        g_ui.carTabPosX,  g_ui.carTabPosY,    g_ui.carTabPosZ,    g_ui.carTabRotW,
                        g_ui.carTabRotX,  g_ui.carTabRotY,    g_ui.carTabRotZ,    g_ui.carTabFuel,
                        g_ui.carTabSpeed, g_ui.carTabOdometer, g_ui.carTabEngineOn, g_ui.applyCar};
    for (HWND h : arr) {
        EnableWindow(h, enabled ? TRUE : FALSE);
    }
}

void SetEnabledGarage(bool enabled) {
    const HWND arr[] = {g_ui.garageList,  g_ui.garageSlot,  g_ui.garageA,      g_ui.garageB,
                        g_ui.garageAHex,  g_ui.garageBHex,  g_ui.garageALow16, g_ui.garageBLow16,
                        g_ui.garageAHi8,  g_ui.garageBHi8,  g_ui.garageAFlags, g_ui.garageBFlags,
                        g_ui.garageAColor, g_ui.garageBColor,
                        g_ui.garageACar,  g_ui.garageBCar,  g_ui.garageADecoded, g_ui.garageBDecoded,
                        g_ui.applyGarage, g_ui.clearGarage, g_ui.syncGarageB};
    for (HWND h : arr) {
        EnableWindow(h, enabled ? TRUE : FALSE);
    }
}

void SetEnabledActors(bool enabled) {
    const HWND arr[] = {g_ui.actorsScroll, g_ui.actors,      g_ui.aname,       g_ui.amodel,      g_ui.atype,
                        g_ui.aidx,        g_ui.apayload,    g_ui.pstate,      g_ui.pid,
                        g_ui.pactive,     g_ui.premove,     g_ui.pframe,      g_ui.posx,
                        g_ui.posy,        g_ui.posz,        g_ui.dirx,        g_ui.diry,
                        g_ui.dirz,        g_ui.animId,      g_ui.humanSeat,   g_ui.humanCrouch,
                        g_ui.humanAim,    g_ui.humanShootX, g_ui.humanShootY, g_ui.humanShootZ,
                        g_ui.rotw,        g_ui.rotx,        g_ui.roty,        g_ui.rotz,
                        g_ui.carFuel,     g_ui.carFlow,     g_ui.carEngNorm,  g_ui.carEngCalc,
                        g_ui.carSpeedLimit, g_ui.carLastGear, g_ui.carGear, g_ui.carGearboxFlag,
                        g_ui.carDisableEngine, g_ui.carEngineOn, g_ui.carIsEngineOn, g_ui.carOdometer,
                        g_ui.invMode,     g_ui.invFlag,     g_ui.invSelId,    g_ui.invSelLoaded,
                        g_ui.invSelHidden, g_ui.invSelUnk,  g_ui.invCoatId,   g_ui.invCoatLoaded,
                        g_ui.invCoatHidden, g_ui.invCoatUnk, g_ui.invS1Id,    g_ui.invS1Loaded,
                        g_ui.invS1Hidden, g_ui.invS1Unk,    g_ui.invS2Id,     g_ui.invS2Loaded,
                        g_ui.invS2Hidden, g_ui.invS2Unk,    g_ui.invS3Id,     g_ui.invS3Loaded,
                        g_ui.invS3Hidden, g_ui.invS3Unk,    g_ui.invS4Id,     g_ui.invS4Loaded,
                        g_ui.invS4Hidden, g_ui.invS4Unk,    g_ui.invS5Id,     g_ui.invS5Loaded,
                        g_ui.invS5Hidden, g_ui.invS5Unk,
                        g_ui.applyActor,  g_ui.filterName,  g_ui.filterType,  g_ui.applyFilter,
                        g_ui.clearFilter, g_ui.cloneActor};
    for (HWND h : arr) {
        EnableWindow(h, enabled ? TRUE : FALSE);
    }
}

void FillAll() {
    SetText(g_ui.filterName, g_state.filterName);
    SetText(g_ui.filterType, g_state.filterType.has_value() ? std::to_string(*g_state.filterType) : "");
    RefreshInfo();
    RefreshWarning();
    FillMain();
    FillMission();
    FillActorList();
    FillCarsList();
    FillGarageList();

    const bool on = g_state.loaded;
    SetEnabledMain(on);
    SetEnabledMission(on);
    SetEnabledActors(on);
    SetEnabledCars(on);
    SetEnabledGarage(on && HasGarageInfoData());
    EnableWindow(g_ui.saveBtn, on ? TRUE : FALSE);
    EnableWindow(g_ui.resetBtn, on ? TRUE : FALSE);
}

bool LoadFile(HWND hwnd, const fs::path& path) {
    const auto raw = mafia_save::ReadFileBytes(path);
    if (raw.empty()) {
        Error(hwnd, "Failed to read save file");
        return false;
    }

    mafia_save::SaveData parsed;
    std::string err;
    if (!mafia_save::ParseSave(raw, &parsed, &err)) {
        Error(hwnd, "ParseSave failed: " + err);
        return false;
    }

    g_state.loaded = true;
    g_state.inputPath = path;
    g_state.raw = raw;
    g_state.save = std::move(parsed);
    g_state.selectedActor = 0;
    g_state.selectedCar = 0;
    g_state.selectedGarageSlot = 0;
    g_state.filterName.clear();
    g_state.filterType.reset();
    g_state.actorsRightScroll = 0;
    g_state.actorsRightScrollMax = 0;
    RefreshGarageCatalog();
    RebuildActorIndex();
    RebuildFilteredActors();
    RebuildCarIndex();
    FillAll();
    SetStatus("Loaded: " + path.string());
    return true;
}

std::optional<fs::path> ChooseFile(HWND hwnd, bool saveMode, const std::string& suggestedName) {
    char fileName[MAX_PATH] = {};
    if (!suggestedName.empty()) {
        std::snprintf(fileName, sizeof(fileName), "%s", suggestedName.c_str());
    }

    OPENFILENAMEA ofn = {};
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hwnd;
    ofn.lpstrFilter = "Mafia save files\0mafia*.*\0All files\0*.*\0\0";
    ofn.lpstrFile = fileName;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_HIDEREADONLY;

    if (saveMode) {
        ofn.Flags |= OFN_OVERWRITEPROMPT;
        if (!GetSaveFileNameA(&ofn)) {
            return std::nullopt;
        }
    } else {
        ofn.Flags |= OFN_FILEMUSTEXIST;
        if (!GetOpenFileNameA(&ofn)) {
            return std::nullopt;
        }
    }

    return fs::path(ofn.lpstrFile);
}

bool ApplyInventoryEdits(std::vector<std::uint8_t>* p, std::size_t invOff, std::string* err) {
    if (p == nullptr || invOff + kInventoryBlobSize > p->size()) {
        if (err != nullptr) {
            *err = "inventory block out of range";
        }
        return false;
    }

    std::uint32_t mode = 0;
    std::uint8_t flag = 0;
    if (!ParseU32(Trim(GetText(g_ui.invMode)), &mode, err, "Inv Mode(0)")) {
        return false;
    }
    if (mode > 127u) {
        if (err != nullptr) {
            *err = "Inv Mode(0) must be <= 127";
        }
        return false;
    }
    if (!ParseByteField(GetText(g_ui.invFlag), &flag, err, "Inv Flag(bit7)", 1)) {
        return false;
    }

    std::uint32_t selId = 0;
    std::uint32_t selLoaded = 0;
    std::uint32_t selHidden = 0;
    std::uint32_t selUnk = 0;
    std::uint32_t coatId = 0;
    std::uint32_t coatLoaded = 0;
    std::uint32_t coatHidden = 0;
    std::uint32_t coatUnk = 0;
    std::uint32_t s1Id = 0;
    std::uint32_t s1Loaded = 0;
    std::uint32_t s1Hidden = 0;
    std::uint32_t s1Unk = 0;
    std::uint32_t s2Id = 0;
    std::uint32_t s2Loaded = 0;
    std::uint32_t s2Hidden = 0;
    std::uint32_t s2Unk = 0;
    std::uint32_t s3Id = 0;
    std::uint32_t s3Loaded = 0;
    std::uint32_t s3Hidden = 0;
    std::uint32_t s3Unk = 0;
    std::uint32_t s4Id = 0;
    std::uint32_t s4Loaded = 0;
    std::uint32_t s4Hidden = 0;
    std::uint32_t s4Unk = 0;
    std::uint32_t s5Id = 0;
    std::uint32_t s5Loaded = 0;
    std::uint32_t s5Hidden = 0;
    std::uint32_t s5Unk = 0;

    if (!ParseU32(Trim(GetText(g_ui.invSelId)), &selId, err, "Inv Sel ID")) {
        return false;
    }
    if (!ParseU32(Trim(GetText(g_ui.invSelLoaded)), &selLoaded, err, "Inv Sel AmmoLoaded")) {
        return false;
    }
    if (!ParseU32(Trim(GetText(g_ui.invSelHidden)), &selHidden, err, "Inv Sel AmmoHidden")) {
        return false;
    }
    if (!ParseU32(Trim(GetText(g_ui.invSelUnk)), &selUnk, err, "Inv Sel Unk")) {
        return false;
    }
    if (!ParseU32(Trim(GetText(g_ui.invCoatId)), &coatId, err, "Inv Coat ID")) {
        return false;
    }
    if (!ParseU32(Trim(GetText(g_ui.invCoatLoaded)), &coatLoaded, err, "Inv Coat AmmoLoaded")) {
        return false;
    }
    if (!ParseU32(Trim(GetText(g_ui.invCoatHidden)), &coatHidden, err, "Inv Coat AmmoHidden")) {
        return false;
    }
    if (!ParseU32(Trim(GetText(g_ui.invCoatUnk)), &coatUnk, err, "Inv Coat Unk")) {
        return false;
    }
    if (!ParseU32(Trim(GetText(g_ui.invS1Id)), &s1Id, err, "Inv S1 ID")) {
        return false;
    }
    if (!ParseU32(Trim(GetText(g_ui.invS1Loaded)), &s1Loaded, err, "Inv S1 AmmoLoaded")) {
        return false;
    }
    if (!ParseU32(Trim(GetText(g_ui.invS1Hidden)), &s1Hidden, err, "Inv S1 AmmoHidden")) {
        return false;
    }
    if (!ParseU32(Trim(GetText(g_ui.invS1Unk)), &s1Unk, err, "Inv S1 Unk")) {
        return false;
    }
    if (!ParseU32(Trim(GetText(g_ui.invS2Id)), &s2Id, err, "Inv S2 ID")) {
        return false;
    }
    if (!ParseU32(Trim(GetText(g_ui.invS2Loaded)), &s2Loaded, err, "Inv S2 AmmoLoaded")) {
        return false;
    }
    if (!ParseU32(Trim(GetText(g_ui.invS2Hidden)), &s2Hidden, err, "Inv S2 AmmoHidden")) {
        return false;
    }
    if (!ParseU32(Trim(GetText(g_ui.invS2Unk)), &s2Unk, err, "Inv S2 Unk")) {
        return false;
    }
    if (!ParseU32(Trim(GetText(g_ui.invS3Id)), &s3Id, err, "Inv S3 ID")) {
        return false;
    }
    if (!ParseU32(Trim(GetText(g_ui.invS3Loaded)), &s3Loaded, err, "Inv S3 AmmoLoaded")) {
        return false;
    }
    if (!ParseU32(Trim(GetText(g_ui.invS3Hidden)), &s3Hidden, err, "Inv S3 AmmoHidden")) {
        return false;
    }
    if (!ParseU32(Trim(GetText(g_ui.invS3Unk)), &s3Unk, err, "Inv S3 Unk")) {
        return false;
    }
    if (!ParseU32(Trim(GetText(g_ui.invS4Id)), &s4Id, err, "Inv S4 ID")) {
        return false;
    }
    if (!ParseU32(Trim(GetText(g_ui.invS4Loaded)), &s4Loaded, err, "Inv S4 AmmoLoaded")) {
        return false;
    }
    if (!ParseU32(Trim(GetText(g_ui.invS4Hidden)), &s4Hidden, err, "Inv S4 AmmoHidden")) {
        return false;
    }
    if (!ParseU32(Trim(GetText(g_ui.invS4Unk)), &s4Unk, err, "Inv S4 Unk")) {
        return false;
    }
    if (!ParseU32(Trim(GetText(g_ui.invS5Id)), &s5Id, err, "Inv S5 ID")) {
        return false;
    }
    if (!ParseU32(Trim(GetText(g_ui.invS5Loaded)), &s5Loaded, err, "Inv S5 AmmoLoaded")) {
        return false;
    }
    if (!ParseU32(Trim(GetText(g_ui.invS5Hidden)), &s5Hidden, err, "Inv S5 AmmoHidden")) {
        return false;
    }
    if (!ParseU32(Trim(GetText(g_ui.invS5Unk)), &s5Unk, err, "Inv S5 Unk")) {
        return false;
    }

    const std::uint32_t modeRaw = (mode & 0x7Fu) | (static_cast<std::uint32_t>(flag) << 7);
    WriteInvDw(p, invOff, 0, modeRaw);
    WriteInvDw(p, invOff, 1, selId);
    WriteInvDw(p, invOff, 2, selLoaded);
    WriteInvDw(p, invOff, 3, selHidden);
    WriteInvDw(p, invOff, 4, selUnk);
    WriteInvDw(p, invOff, 9, s1Id);
    WriteInvDw(p, invOff, 10, s1Loaded);
    WriteInvDw(p, invOff, 11, s1Hidden);
    WriteInvDw(p, invOff, 12, s1Unk);
    WriteInvDw(p, invOff, 13, s2Id);
    WriteInvDw(p, invOff, 14, s2Loaded);
    WriteInvDw(p, invOff, 15, s2Hidden);
    WriteInvDw(p, invOff, 16, s2Unk);
    WriteInvDw(p, invOff, 17, s3Id);
    WriteInvDw(p, invOff, 18, s3Loaded);
    WriteInvDw(p, invOff, 19, s3Hidden);
    WriteInvDw(p, invOff, 20, s3Unk);
    WriteInvDw(p, invOff, 21, s4Id);
    WriteInvDw(p, invOff, 22, s4Loaded);
    WriteInvDw(p, invOff, 23, s4Hidden);
    WriteInvDw(p, invOff, 24, s4Unk);
    WriteInvDw(p, invOff, 25, s5Id);
    WriteInvDw(p, invOff, 26, s5Loaded);
    WriteInvDw(p, invOff, 27, s5Hidden);
    WriteInvDw(p, invOff, 28, s5Unk);
    WriteInvDw(p, invOff, 29, coatId);
    WriteInvDw(p, invOff, 30, coatLoaded);
    WriteInvDw(p, invOff, 31, coatHidden);
    WriteInvDw(p, invOff, 32, coatUnk);
    return true;
}

bool ApplyActorEdits(std::string* err) {
    const auto segIdxOpt = CurrentSelectedActorSegIdx();
    if (!g_state.loaded || !segIdxOpt.has_value()) {
        return true;
    }

    const std::size_t segIdx = *segIdxOpt;
    auto& h = g_state.save.segments[segIdx].plain;

    const std::string name = Trim(GetText(g_ui.aname));
    const std::string model = Trim(GetText(g_ui.amodel));
    std::uint32_t type = 0;
    std::uint32_t idx = 0;

    if (!WriteCStr(&h, 0, 64, name, err)) {
        return false;
    }
    if (!WriteCStr(&h, 64, 64, model, err)) {
        return false;
    }
    if (!ParseU32(Trim(GetText(g_ui.atype)), &type, err, "Actor type")) {
        return false;
    }
    if (!ParseU32(Trim(GetText(g_ui.aidx)), &idx, err, "Actor idx")) {
        return false;
    }

    mafia_save::WriteU32LE(&h, 128, type);
    mafia_save::WriteU32LE(&h, 136, idx);

    const CoordLayout layout = DetectCoordLayout(segIdx);
    if (IsActorPairAt(segIdx) && layout.baseSupported) {
        auto& p = g_state.save.segments[segIdx + 1].plain;
        std::uint8_t state = 0;
        std::uint32_t runtimeId = 0;
        std::uint8_t isActive = 0;
        std::uint8_t doRemove = 0;
        std::uint8_t frameOn = 0;

        if (!ParseByteField(GetText(g_ui.pstate), &state, err, "Payload state", 255)) {
            return false;
        }
        if (!ParseU32(Trim(GetText(g_ui.pid)), &runtimeId, err, "Payload id")) {
            return false;
        }
        if (!ParseByteField(GetText(g_ui.pactive), &isActive, err, "Payload is_active", 1)) {
            return false;
        }
        if (!ParseByteField(GetText(g_ui.premove), &doRemove, err, "Payload do_remove", 1)) {
            return false;
        }
        if (!ParseByteField(GetText(g_ui.pframe), &frameOn, err, "Payload frame_on", 1)) {
            return false;
        }

        p[layout.stateOff] = state;
        mafia_save::WriteU32LE(&p, layout.idOff, runtimeId);
        p[layout.activeOff] = isActive;
        p[layout.removeOff] = doRemove;
        p[layout.frameOff] = frameOn;
    }

    if (IsActorPairAt(segIdx) && layout.coordsSupported) {
        float x = 0.0f;
        float y = 0.0f;
        float z = 0.0f;
        if (!ParseF32(GetText(g_ui.posx), &x, err, "Pos X")) {
            return false;
        }
        if (!ParseF32(GetText(g_ui.posy), &y, err, "Pos Y")) {
            return false;
        }
        if (!ParseF32(GetText(g_ui.posz), &z, err, "Pos Z")) {
            return false;
        }
        auto& p = g_state.save.segments[segIdx + 1].plain;
        WriteF32LE(&p, layout.xOff, x);
        WriteF32LE(&p, layout.yOff, y);
        WriteF32LE(&p, layout.zOff, z);
    }

    if (IsActorPairAt(segIdx) && layout.dirSupported) {
        float dx = 0.0f;
        float dy = 0.0f;
        float dz = 0.0f;
        if (!ParseF32(GetText(g_ui.dirx), &dx, err, "Dir X")) {
            return false;
        }
        if (!ParseF32(GetText(g_ui.diry), &dy, err, "Dir Y")) {
            return false;
        }
        if (!ParseF32(GetText(g_ui.dirz), &dz, err, "Dir Z")) {
            return false;
        }
        auto& p = g_state.save.segments[segIdx + 1].plain;
        WriteF32LE(&p, layout.dirXOff, dx);
        WriteF32LE(&p, layout.dirYOff, dy);
        WriteF32LE(&p, layout.dirZOff, dz);
    }

    if (IsActorPairAt(segIdx) && layout.animSupported) {
        std::uint32_t anim = 0;
        if (!ParseU32(Trim(GetText(g_ui.animId)), &anim, err, "Anim ID")) {
            return false;
        }
        auto& p = g_state.save.segments[segIdx + 1].plain;
        mafia_save::WriteU32LE(&p, layout.animIdOff, anim);
    }

    if (IsActorPairAt(segIdx) && layout.humanStateSupported) {
        std::uint32_t seatId = 0;
        std::uint8_t crouching = 0;
        std::uint8_t aiming = 0;
        float shootX = 0.0f;
        float shootY = 0.0f;
        float shootZ = 0.0f;
        if (!ParseU32(Trim(GetText(g_ui.humanSeat)), &seatId, err, "Seat ID(46)")) {
            return false;
        }
        if (!ParseByteField(GetText(g_ui.humanCrouch), &crouching, err, "Crouching(50)", 255)) {
            return false;
        }
        if (!ParseByteField(GetText(g_ui.humanAim), &aiming, err, "Aiming(51)", 255)) {
            return false;
        }
        if (!ParseF32(GetText(g_ui.humanShootX), &shootX, err, "ShootTarget X(54)")) {
            return false;
        }
        if (!ParseF32(GetText(g_ui.humanShootY), &shootY, err, "ShootTarget Y(58)")) {
            return false;
        }
        if (!ParseF32(GetText(g_ui.humanShootZ), &shootZ, err, "ShootTarget Z(62)")) {
            return false;
        }
        auto& p = g_state.save.segments[segIdx + 1].plain;
        mafia_save::WriteU32LE(&p, layout.humanSeatOff, seatId);
        p[layout.humanCrouchOff] = crouching;
        p[layout.humanAimOff] = aiming;
        WriteF32LE(&p, layout.humanShootXOff, shootX);
        WriteF32LE(&p, layout.humanShootYOff, shootY);
        WriteF32LE(&p, layout.humanShootZOff, shootZ);
    }

    if (IsActorPairAt(segIdx) && layout.humanInventorySupported) {
        auto& p = g_state.save.segments[segIdx + 1].plain;
        if (!ApplyInventoryEdits(&p, layout.humanInventoryOff, err)) {
            return false;
        }
    }

    if (IsActorPairAt(segIdx) && layout.quatSupported) {
        float qw = 0.0f;
        float qx = 0.0f;
        float qy = 0.0f;
        float qz = 0.0f;
        if (!ParseF32(GetText(g_ui.rotw), &qw, err, "Rot W")) {
            return false;
        }
        if (!ParseF32(GetText(g_ui.rotx), &qx, err, "Rot X")) {
            return false;
        }
        if (!ParseF32(GetText(g_ui.roty), &qy, err, "Rot Y")) {
            return false;
        }
        if (!ParseF32(GetText(g_ui.rotz), &qz, err, "Rot Z")) {
            return false;
        }
        auto& p = g_state.save.segments[segIdx + 1].plain;
        WriteF32LE(&p, layout.quatWOff, qw);
        WriteF32LE(&p, layout.quatXOff, qx);
        WriteF32LE(&p, layout.quatYOff, qy);
        WriteF32LE(&p, layout.quatZOff, qz);
    }

    if (IsActorPairAt(segIdx) && layout.carStateSupported) {
        float fuel = 0.0f;
        float flow = 0.0f;
        float engNorm = 0.0f;
        float engCalc = 0.0f;
        if (!ParseF32(GetText(g_ui.carFuel), &fuel, err, "Car Fuel(ofs304)")) {
            return false;
        }
        if (!ParseF32(GetText(g_ui.carFlow), &flow, err, "Car FuelFlow(ofs211)")) {
            return false;
        }
        if (!ParseF32(GetText(g_ui.carEngNorm), &engNorm, err, "Car EngNorm(ofs137)")) {
            return false;
        }
        if (!ParseF32(GetText(g_ui.carEngCalc), &engCalc, err, "Car EngCalc(ofs141)")) {
            return false;
        }
        auto& p = g_state.save.segments[segIdx + 1].plain;
        WriteF32LE(&p, layout.carFuelOff, fuel);
        WriteF32LE(&p, layout.carFlowOff, flow);
        WriteF32LE(&p, layout.carEngNormOff, engNorm);
        WriteF32LE(&p, layout.carEngCalcOff, engCalc);
    }

    if (IsActorPairAt(segIdx) && layout.carDriveSupported) {
        float speedLimit = 0.0f;
        std::int32_t lastGear = 0;
        std::int32_t gear = 0;
        if (!ParseF32(GetText(g_ui.carSpeedLimit), &speedLimit, err, "Car SpeedLimit(215)")) {
            return false;
        }
        if (!ParseI32(Trim(GetText(g_ui.carLastGear)), &lastGear, err, "Car LastGear(245)")) {
            return false;
        }
        if (!ParseI32(Trim(GetText(g_ui.carGear)), &gear, err, "Car Gear(249)")) {
            return false;
        }
        auto& p = g_state.save.segments[segIdx + 1].plain;
        WriteF32LE(&p, layout.carSpeedLimitOff, speedLimit);
        mafia_save::WriteU32LE(&p, layout.carLastGearOff, static_cast<std::uint32_t>(lastGear));
        mafia_save::WriteU32LE(&p, layout.carGearOff, static_cast<std::uint32_t>(gear));
    }

    if (IsActorPairAt(segIdx) && layout.carEngineFlagsSupported) {
        std::uint32_t gearboxFlag = 0;
        std::uint8_t disableEngine = 0;
        std::uint8_t engineOn = 0;
        std::uint8_t isEngineOn = 0;
        if (!ParseU32(Trim(GetText(g_ui.carGearboxFlag)), &gearboxFlag, err, "Car EngineGearboxFlag(273)")) {
            return false;
        }
        if (!ParseByteField(GetText(g_ui.carDisableEngine), &disableEngine, err, "Car InstantDisableEngine(277)", 255)) {
            return false;
        }
        if (!ParseByteField(GetText(g_ui.carEngineOn), &engineOn, err, "Car EngineON(298)", 255)) {
            return false;
        }
        if (!ParseByteField(GetText(g_ui.carIsEngineOn), &isEngineOn, err, "Car IsEngineOn(303)", 255)) {
            return false;
        }
        auto& p = g_state.save.segments[segIdx + 1].plain;
        mafia_save::WriteU32LE(&p, layout.carGearboxFlagOff, gearboxFlag);
        p[layout.carDisableEngineOff] = disableEngine;
        p[layout.carEngineOnOff] = engineOn;
        p[layout.carIsEngineOnOff] = isEngineOn;
    }

    if (IsActorPairAt(segIdx) && layout.carOdometerSupported) {
        float odometer = 0.0f;
        if (!ParseF32(GetText(g_ui.carOdometer), &odometer, err, "Car Odometer(345)")) {
            return false;
        }
        auto& p = g_state.save.segments[segIdx + 1].plain;
        WriteF32LE(&p, layout.carOdometerOff, odometer);
    }
    return true;
}

bool ApplyCarEdits(std::string* err) {
    const auto segIdxOpt = CurrentSelectedCarSegIdx();
    if (!g_state.loaded || !segIdxOpt.has_value()) {
        return true;
    }
    const std::size_t segIdx = *segIdxOpt;
    if (!IsActorPairAt(segIdx)) {
        return true;
    }

    const auto layout = DetectCoordLayout(segIdx);
    auto& p = g_state.save.segments[segIdx + 1].plain;

    if (layout.coordsSupported) {
        float x = 0.0f;
        float y = 0.0f;
        float z = 0.0f;
        if (!ParseF32(GetText(g_ui.carTabPosX), &x, err, "Car Pos X")) {
            return false;
        }
        if (!ParseF32(GetText(g_ui.carTabPosY), &y, err, "Car Pos Y")) {
            return false;
        }
        if (!ParseF32(GetText(g_ui.carTabPosZ), &z, err, "Car Pos Z")) {
            return false;
        }
        WriteF32LE(&p, layout.xOff, x);
        WriteF32LE(&p, layout.yOff, y);
        WriteF32LE(&p, layout.zOff, z);
    }

    if (layout.quatSupported) {
        float qw = 0.0f;
        float qx = 0.0f;
        float qy = 0.0f;
        float qz = 0.0f;
        if (!ParseF32(GetText(g_ui.carTabRotW), &qw, err, "Car Rot W")) {
            return false;
        }
        if (!ParseF32(GetText(g_ui.carTabRotX), &qx, err, "Car Rot X")) {
            return false;
        }
        if (!ParseF32(GetText(g_ui.carTabRotY), &qy, err, "Car Rot Y")) {
            return false;
        }
        if (!ParseF32(GetText(g_ui.carTabRotZ), &qz, err, "Car Rot Z")) {
            return false;
        }
        WriteF32LE(&p, layout.quatWOff, qw);
        WriteF32LE(&p, layout.quatXOff, qx);
        WriteF32LE(&p, layout.quatYOff, qy);
        WriteF32LE(&p, layout.quatZOff, qz);
    }

    if (layout.carStateSupported) {
        float fuel = 0.0f;
        if (!ParseF32(GetText(g_ui.carTabFuel), &fuel, err, "Car Fuel")) {
            return false;
        }
        WriteF32LE(&p, layout.carFuelOff, fuel);
    }

    if (layout.carDriveSupported) {
        float speed = 0.0f;
        if (!ParseF32(GetText(g_ui.carTabSpeed), &speed, err, "Car SpeedLimit")) {
            return false;
        }
        WriteF32LE(&p, layout.carSpeedLimitOff, speed);
    }

    if (layout.carOdometerSupported) {
        float odo = 0.0f;
        if (!ParseF32(GetText(g_ui.carTabOdometer), &odo, err, "Car Odometer")) {
            return false;
        }
        WriteF32LE(&p, layout.carOdometerOff, odo);
    }

    if (layout.carEngineFlagsSupported) {
        std::uint8_t engineOn = 0;
        if (!ParseByteField(GetText(g_ui.carTabEngineOn), &engineOn, err, "Car EngineON", 255)) {
            return false;
        }
        p[layout.carEngineOnOff] = engineOn;
    }
    return true;
}

bool ApplyMissionEdits(mafia_save::SaveData* edited, std::string* err) {
    if (edited == nullptr) {
        if (err != nullptr) {
            *err = "internal null save pointer";
        }
        return false;
    }
    if (edited->idxGamePayload == mafia_save::kNoIndex || edited->idxGamePayload >= edited->segments.size()) {
        if (err != nullptr) {
            *err = "game_payload segment is missing";
        }
        return false;
    }

    auto& p = edited->segments[edited->idxGamePayload].plain;
    if (p.size() < kGameHeaderSize) {
        if (err != nullptr) {
            *err = "game_payload header is too small";
        }
        return false;
    }

    std::uint32_t fieldA = 0;
    std::uint32_t fieldB = 0;
    std::uint32_t missionId = 0;
    std::uint8_t timerOn = 0;
    std::uint32_t timerInterval = 0;
    std::uint32_t timerA = 0;
    std::uint32_t timerB = 0;
    std::uint32_t timerC = 0;
    std::uint8_t scoreOn = 0;
    std::uint32_t scoreValue = 0;

    if (!ParseU32(Trim(GetText(g_ui.ghFieldA)), &fieldA, err, "Game field A(11020)")) {
        return false;
    }
    if (!ParseU32(Trim(GetText(g_ui.ghFieldB)), &fieldB, err, "Game field B(11024)")) {
        return false;
    }
    if (!ParseU32(Trim(GetText(g_ui.ghMissionId)), &missionId, err, "Mission ID(payload)")) {
        return false;
    }
    if (!ParseByteField(GetText(g_ui.ghTimerOn), &timerOn, err, "Timer enabled(13)", 1)) {
        return false;
    }
    if (!ParseU32(Trim(GetText(g_ui.ghTimerInterval)), &timerInterval, err, "Timer interval(14)")) {
        return false;
    }
    if (!ParseU32(Trim(GetText(g_ui.ghTimerA)), &timerA, err, "Timer A(18)")) {
        return false;
    }
    if (!ParseU32(Trim(GetText(g_ui.ghTimerB)), &timerB, err, "Timer B(22)")) {
        return false;
    }
    if (!ParseU32(Trim(GetText(g_ui.ghTimerC)), &timerC, err, "Timer C(26)")) {
        return false;
    }
    if (!ParseByteField(GetText(g_ui.ghScoreOn), &scoreOn, err, "Score enabled(62)", 1)) {
        return false;
    }
    if (!ParseU32(Trim(GetText(g_ui.ghScoreValue)), &scoreValue, err, "Score value(63)")) {
        return false;
    }

    mafia_save::WriteU32LE(&p, 1, fieldA);
    mafia_save::WriteU32LE(&p, 5, fieldB);
    mafia_save::WriteU32LE(&p, 9, missionId);
    p[13] = timerOn;
    mafia_save::WriteU32LE(&p, 14, timerInterval);
    mafia_save::WriteU32LE(&p, 18, timerA);
    mafia_save::WriteU32LE(&p, 22, timerB);
    mafia_save::WriteU32LE(&p, 26, timerC);
    p[62] = scoreOn;
    mafia_save::WriteU32LE(&p, 63, scoreValue);

    const auto where = DetectProgramInSave(*edited);
    if (!where.has_value()) {
        return true;
    }

    if (where->segIdx >= edited->segments.size()) {
        return true;
    }
    auto& pProg = edited->segments[where->segIdx].plain;
    const auto& prog = where->layout;
    if (prog.baseOff + 36 > pProg.size()) {
        return true;
    }

    std::uint8_t cmdBlock = 0;
    if (!ParseByteField(GetText(g_ui.progCmdBlock), &cmdBlock, err, "Pause script(35)", 1)) {
        return false;
    }
    pProg[prog.baseOff + 35] = cmdBlock;

    const std::string idxText = Trim(GetText(g_ui.progVarIndex));
    const std::string valText = Trim(GetText(g_ui.progVarValue));
    if (idxText.empty() && valText.empty()) {
        return true;
    }

    if (prog.varCount == 0u) {
        if (err != nullptr) {
            *err = "program has zero variables";
        }
        return false;
    }

    std::uint32_t varIdx = 0;
    float varValue = 0.0f;
    if (!ParseU32(idxText, &varIdx, err, "Script var index")) {
        return false;
    }
    if (varIdx >= prog.varCount) {
        if (err != nullptr) {
            std::ostringstream oss;
            oss << "Script var index out of range (max " << (prog.varCount - 1u) << ")";
            *err = oss.str();
        }
        return false;
    }
    if (!ParseF32(valText, &varValue, err, "Script var value")) {
        return false;
    }
    WriteF32LE(&pProg, prog.varsOff + (static_cast<std::size_t>(varIdx) * 4u), varValue);
    return true;
}

bool BuildEditedSave(mafia_save::SaveData* out, std::string* err) {
    if (out == nullptr) {
        return false;
    }

    mafia_save::SaveData edited = g_state.save;
    if (edited.idxMeta == mafia_save::kNoIndex || edited.idxMeta >= edited.segments.size()) {
        if (err != nullptr) {
            *err = "meta32 block missing";
        }
        return false;
    }
    if (edited.idxInfo == mafia_save::kNoIndex || edited.idxInfo >= edited.segments.size()) {
        if (err != nullptr) {
            *err = "info264 block missing";
        }
        return false;
    }

    auto& meta = edited.segments[edited.idxMeta].plain;
    auto& info = edited.segments[edited.idxInfo].plain;

    std::uint32_t hp = 0;
    std::uint32_t date = 0;
    std::uint32_t time = 0;
    std::uint32_t slot = 0;
    std::uint32_t mcode = 0;

    if (!ParseU32(Trim(GetText(g_ui.hp)), &hp, err, "HP") || hp > 1000u) {
        if (err != nullptr && err->empty()) {
            *err = "HP must be <= 1000";
        }
        return false;
    }
    if (!ParseDate(Trim(GetText(g_ui.date)), &date, err)) {
        return false;
    }
    if (!ParseTime(Trim(GetText(g_ui.time)), &time, err)) {
        return false;
    }
    if (!ParseU32(Trim(GetText(g_ui.slot)), &slot, err, "Slot")) {
        return false;
    }
    if (!ParseU32(Trim(GetText(g_ui.mcode)), &mcode, err, "Mission code")) {
        return false;
    }

    const std::string mission = Trim(GetText(g_ui.mname));
    if (!WriteCStr(&info, 0, 32, mission, err)) {
        return false;
    }

    mafia_save::WriteU32LE(&meta, 16, hp);
    mafia_save::WriteU32LE(&meta, 12, date);
    mafia_save::WriteU32LE(&meta, 8, time);
    mafia_save::WriteU32LE(&meta, 0, slot);
    mafia_save::WriteU32LE(&meta, 28, mcode);

    if (!ApplyMissionEdits(&edited, err)) {
        return false;
    }

    *out = std::move(edited);
    return true;
}

bool ApplyFilterFromUi(std::string* err) {
    g_state.filterName = Trim(GetText(g_ui.filterName));
    const std::string typeText = Trim(GetText(g_ui.filterType));
    if (typeText.empty()) {
        g_state.filterType.reset();
    } else {
        std::uint32_t t = 0;
        if (!ParseU32(typeText, &t, err, "Filter type")) {
            return false;
        }
        g_state.filterType = t;
    }
    RebuildFilteredActors();
    return true;
}

bool CloneSelectedActor(std::string* err) {
    const auto segIdxOpt = CurrentSelectedActorSegIdx();
    if (!segIdxOpt.has_value()) {
        if (err != nullptr) {
            *err = "no actor selected";
        }
        return false;
    }

    const std::size_t headerIdx = *segIdxOpt;
    if (headerIdx + 1 >= g_state.save.segments.size()) {
        if (err != nullptr) {
            *err = "selected actor payload segment is missing";
        }
        return false;
    }
    if (g_state.save.segments[headerIdx].name.rfind("actor_header_", 0) != 0 ||
        g_state.save.segments[headerIdx + 1].name.rfind("actor_payload_", 0) != 0) {
        if (err != nullptr) {
            *err = "selected segment pair is not actor header/payload";
        }
        return false;
    }

    mafia_save::Segment newHeader = g_state.save.segments[headerIdx];
    mafia_save::Segment newPayload = g_state.save.segments[headerIdx + 1];
    newHeader.name = "actor_header_clone";
    newPayload.name = "actor_payload_clone";

    auto it = g_state.save.segments.begin() + static_cast<std::ptrdiff_t>(headerIdx + 2);
    it = g_state.save.segments.insert(it, newHeader);
    g_state.save.segments.insert(it + 1, newPayload);
    return true;
}
HWND MakeLabel(HWND parent, const char* text, int x, int y, int w, int h, int id = 0) {
    return CreateWindowExA(0, "STATIC", text, WS_CHILD | WS_VISIBLE, x, y, w, h, parent,
                           reinterpret_cast<HMENU>(static_cast<INT_PTR>(id)), nullptr, nullptr);
}

HWND MakeEdit(HWND parent, const char* text, int x, int y, int w, int h, int id, DWORD extraStyle = 0) {
    return CreateWindowExA(WS_EX_CLIENTEDGE, "EDIT", text, WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL | extraStyle,
                           x, y, w, h, parent, reinterpret_cast<HMENU>(static_cast<INT_PTR>(id)), nullptr, nullptr);
}

HWND MakeCombo(HWND parent, int x, int y, int w, int h, int id, DWORD extraStyle = 0) {
    return CreateWindowExA(0, "COMBOBOX", "", WS_CHILD | WS_VISIBLE | WS_VSCROLL | CBS_DROPDOWNLIST | extraStyle,
                           x, y, w, h, parent, reinterpret_cast<HMENU>(static_cast<INT_PTR>(id)), nullptr, nullptr);
}

HWND MakeButton(HWND parent, const char* text, int x, int y, int w, int h, int id) {
    return CreateWindowExA(0, "BUTTON", text, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, x, y, w, h, parent,
                           reinterpret_cast<HMENU>(static_cast<INT_PTR>(id)), nullptr, nullptr);
}

BOOL CALLBACK FontCb(HWND child, LPARAM) {
    SendMessageA(child, WM_SETFONT, reinterpret_cast<WPARAM>(g_font), TRUE);
    return TRUE;
}

LRESULT CALLBACK PageForwardProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    if (msg == WM_COMMAND || msg == WM_NOTIFY) {
        return SendMessageA(GetParent(hwnd), msg, wParam, lParam);
    }
    if (msg == WM_VSCROLL || msg == WM_MOUSEWHEEL) {
        const LPARAM src = (lParam == 0) ? reinterpret_cast<LPARAM>(hwnd) : lParam;
        return SendMessageA(GetParent(hwnd), msg, wParam, src);
    }
    const auto oldProc = reinterpret_cast<WNDPROC>(GetWindowLongPtrA(hwnd, GWLP_USERDATA));
    if (oldProc != nullptr) {
        return CallWindowProcA(oldProc, hwnd, msg, wParam, lParam);
    }
    return DefWindowProcA(hwnd, msg, wParam, lParam);
}

void SubclassPageForward(HWND page) {
    const auto old = reinterpret_cast<WNDPROC>(SetWindowLongPtrA(page, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(PageForwardProc)));
    SetWindowLongPtrA(page, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(old));
}

void ShowTab(int index) {
    ShowWindow(g_ui.pageMain, index == 0 ? SW_SHOW : SW_HIDE);
    ShowWindow(g_ui.pageActors, index == 1 ? SW_SHOW : SW_HIDE);
    ShowWindow(g_ui.pageCars, index == 2 ? SW_SHOW : SW_HIDE);
    ShowWindow(g_ui.pageGarage, index == 3 ? SW_SHOW : SW_HIDE);
    ShowWindow(g_ui.pageMission, index == 4 ? SW_SHOW : SW_HIDE);
    if (index == 1) {
        LayoutActorsPage();
    }
    if (index == 2) {
        LayoutCarsPage();
    }
    if (index == 3) {
        LayoutGaragePage();
    }
    if (index == 4) {
        LayoutMissionPage();
    }
}

void UpdateActorsRightScrollBar(int viewportHeight, int contentHeight) {
    if (g_ui.actorsScroll == nullptr) {
        return;
    }
    const int vp = std::max(0, viewportHeight);
    const int content = std::max(0, contentHeight);
    const int maxPos = std::max(0, content - vp);
    g_state.actorsRightScrollMax = maxPos;
    if (g_state.actorsRightScroll > maxPos) {
        g_state.actorsRightScroll = maxPos;
    }
    if (g_state.actorsRightScroll < 0) {
        g_state.actorsRightScroll = 0;
    }

    SCROLLINFO si = {};
    si.cbSize = sizeof(si);
    si.fMask = SIF_RANGE | SIF_PAGE | SIF_POS;
    si.nMin = 0;
    si.nMax = std::max(0, content - 1);
    si.nPage = static_cast<UINT>(vp);
    si.nPos = g_state.actorsRightScroll;
    SetScrollInfo(g_ui.actorsScroll, SB_CTL, &si, TRUE);
    ShowWindow(g_ui.actorsScroll, maxPos > 0 ? SW_SHOW : SW_HIDE);
}

void LayoutMainPage() {
    if (g_ui.pageMain == nullptr) {
        return;
    }
    RECT rc = {};
    GetClientRect(g_ui.pageMain, &rc);
    const int w = rc.right - rc.left;

    const int margin = 16;
    const int labelW = 140;
    const int rowH = 24;
    const int rowGap = 10;
    const int baseEditW = std::max(220, w - (margin * 2) - labelW - 12);

    int y = 12;
    MoveWindow(g_ui.mainTitle, margin, y, w - margin * 2, 20, TRUE);
    y += 30;

    auto placeRow = [&](HWND label, HWND edit, int editW) {
        MoveWindow(label, margin, y + 2, labelW, 20, TRUE);
        MoveWindow(edit, margin + labelW + 8, y, std::min(baseEditW, editW), rowH, TRUE);
        y += rowH + rowGap;
    };

    placeRow(g_ui.hpLabel, g_ui.hp, 180);
    placeRow(g_ui.dateLabel, g_ui.date, 220);
    placeRow(g_ui.timeLabel, g_ui.time, 220);
    placeRow(g_ui.slotLabel, g_ui.slot, 160);
    placeRow(g_ui.mcodeLabel, g_ui.mcode, 180);
    placeRow(g_ui.mnameLabel, g_ui.mname, std::max(280, baseEditW));
}

void LayoutMissionPage() {
    if (g_ui.pageMission == nullptr) {
        return;
    }
    RECT rc = {};
    GetClientRect(g_ui.pageMission, &rc);
    const int w = rc.right - rc.left;
    const int h = rc.bottom - rc.top;

    const int margin = 16;
    const int gap = 14;
    const int rowH = 24;
    const int rowGap = 6;
    const int titleH = 22;
    const int totalW = std::max(300, w - margin * 2);
    const int colW = std::max(230, (totalW - gap * 2) / 3);
    const int col1X = margin;
    const int col2X = col1X + colW + gap;
    const int col3X = col2X + colW + gap;
    const int labelW = 112;
    const int editW = std::max(100, colW - labelW - 8);

    MoveWindow(g_ui.missionTitle, margin, 10, w - margin * 2, titleH, TRUE);

    int y1 = 40;
    auto col1Row = [&](HWND label, HWND edit) {
        MoveWindow(label, col1X, y1 + 2, labelW, 20, TRUE);
        MoveWindow(edit, col1X + labelW + 8, y1, editW, rowH, TRUE);
        y1 += rowH + rowGap;
    };
    col1Row(g_ui.ghMarkerLabel, g_ui.ghMarker);
    col1Row(g_ui.ghMissionIdLabel, g_ui.ghMissionId);
    col1Row(g_ui.ghTimerOnLabel, g_ui.ghTimerOn);
    col1Row(g_ui.ghTimerIntervalLabel, g_ui.ghTimerInterval);
    col1Row(g_ui.ghTimerALabel, g_ui.ghTimerA);
    col1Row(g_ui.ghTimerBLabel, g_ui.ghTimerB);
    col1Row(g_ui.ghTimerCLabel, g_ui.ghTimerC);

    int y2 = 40;
    auto col2Row = [&](HWND label, HWND edit) {
        MoveWindow(label, col2X, y2 + 2, labelW, 20, TRUE);
        MoveWindow(edit, col2X + labelW + 8, y2, editW, rowH, TRUE);
        y2 += rowH + rowGap;
    };
    col2Row(g_ui.ghFieldALabel, g_ui.ghFieldA);
    col2Row(g_ui.ghFieldBLabel, g_ui.ghFieldB);
    col2Row(g_ui.ghScoreOnLabel, g_ui.ghScoreOn);
    col2Row(g_ui.ghScoreValueLabel, g_ui.ghScoreValue);
    col2Row(g_ui.ghScriptEntriesLabel, g_ui.ghScriptEntries);
    col2Row(g_ui.ghScriptChunksLabel, g_ui.ghScriptChunks);
    col2Row(g_ui.progOffsetLabel, g_ui.progOffset);

    int y3 = 40;
    auto col3Row = [&](HWND label, HWND edit) {
        MoveWindow(label, col3X, y3 + 2, labelW, 20, TRUE);
        MoveWindow(edit, col3X + labelW + 8, y3, editW, rowH, TRUE);
        y3 += rowH + rowGap;
    };
    col3Row(g_ui.progVarsLabel, g_ui.progVars);
    col3Row(g_ui.progActorsLabel, g_ui.progActors);
    col3Row(g_ui.progFramesLabel, g_ui.progFrames);
    col3Row(g_ui.progCmdBlockLabel, g_ui.progCmdBlock);
    col3Row(g_ui.progVarIndexLabel, g_ui.progVarIndex);
    col3Row(g_ui.progVarValueLabel, g_ui.progVarValue);

    MoveWindow(g_ui.progLoadVar, col3X + labelW + 8, y3 + 2, 130, 28, TRUE);
    MoveWindow(g_ui.progReloadTable, col3X + labelW + 144, y3 + 2, 130, 28, TRUE);
    const int hintY = std::min(h - 24, y3 + 40);
    MoveWindow(g_ui.missionHint, col3X, hintY, colW, 20, TRUE);
    MoveWindow(g_ui.missionHelp1, margin, std::min(h - 44, hintY + 24), w - margin * 2, 18, TRUE);
    MoveWindow(g_ui.missionHelp2, margin, std::min(h - 24, hintY + 42), w - margin * 2, 18, TRUE);

    const int tableY = std::min(h - 170, hintY + 66);
    const int tableLabelY = std::max(40, tableY);
    const int tableTop = tableLabelY + 20;
    const int tableH = std::max(90, h - tableTop - 12);
    MoveWindow(g_ui.progVarsTableLabel, margin, tableLabelY, std::max(300, w - margin * 2), 18, TRUE);
    MoveWindow(g_ui.progVarsTable, margin, tableTop, std::max(300, w - margin * 2), tableH, TRUE);
}

void LayoutActorsPage() {
    if (g_ui.pageActors == nullptr) {
        return;
    }
    RECT rc = {};
    GetClientRect(g_ui.pageActors, &rc);
    const int w = rc.right - rc.left;
    const int h = rc.bottom - rc.top;

    const int margin = 16;
    const int gap = 14;
    const int rowH = 24;
    const int btnW = 76;
    const int sbW = GetSystemMetrics(SM_CXVSCROLL);
    const int scrollBarReserve = sbW + 6;
    const int filterY = 38;
    const int rightTop = filterY;
    const int rightViewportH = std::max(0, h - margin - rightTop);

    int rightW = std::max(300, w / 3);
    const int maxRightW = std::max(260, w - (margin * 3) - 280 - scrollBarReserve);
    rightW = std::min(rightW, maxRightW);
    int rightX = w - margin - scrollBarReserve - rightW;
    int leftX = margin;
    int leftW = rightX - gap - leftX;
    if (leftW < 280) {
        leftW = 280;
        rightX = leftX + leftW + gap;
        rightW = std::max(260, w - margin - scrollBarReserve - rightX);
    }

    if (g_ui.actorsScroll != nullptr) {
        MoveWindow(g_ui.actorsScroll, w - margin - sbW, rightTop, sbW, rightViewportH, TRUE);
    }

    MoveWindow(g_ui.actorsTitle, margin, 12, w - margin * 2, 20, TRUE);

    MoveWindow(g_ui.filterNameLabel, leftX, filterY + 2, 80, 20, TRUE);
    int x = leftX + 88;
    const int typeLblW = 70;
    const int typeEditW = 78;
    const int reserved = 12 + typeLblW + 8 + typeEditW + 8 + btnW + 8 + btnW;
    const int nameW = std::max(120, leftW - (x - leftX) - reserved);
    MoveWindow(g_ui.filterName, x, filterY, nameW, rowH, TRUE);
    x += nameW + 12;
    MoveWindow(g_ui.filterTypeLabel, x, filterY + 2, typeLblW, 20, TRUE);
    x += typeLblW + 8;
    MoveWindow(g_ui.filterType, x, filterY, typeEditW, rowH, TRUE);
    x += typeEditW + 8;
    MoveWindow(g_ui.applyFilter, x, filterY, btnW, rowH, TRUE);
    x += btnW + 8;
    MoveWindow(g_ui.clearFilter, x, filterY, btnW, rowH, TRUE);

    const int listY = filterY + rowH + 10;
    const int listH = std::max(120, h - listY - margin);
    MoveWindow(g_ui.actors, leftX, listY, leftW, listH, TRUE);

    const int labelW = 96;
    const int editW = std::max(140, rightW - labelW - 10);

    const auto rowVisible = [](HWND label, HWND edit) -> bool {
        return label != nullptr && edit != nullptr && IsWindowVisible(label) && IsWindowVisible(edit);
    };
    int visibleRows = 0;
    const auto addRowIfVisible = [&](HWND label, HWND edit) {
        if (rowVisible(label, edit)) {
            ++visibleRows;
        }
    };
    addRowIfVisible(g_ui.anameLabel, g_ui.aname);
    addRowIfVisible(g_ui.amodelLabel, g_ui.amodel);
    addRowIfVisible(g_ui.atypeLabel, g_ui.atype);
    addRowIfVisible(g_ui.aidxLabel, g_ui.aidx);
    addRowIfVisible(g_ui.apayloadLabel, g_ui.apayload);
    addRowIfVisible(g_ui.pstateLabel, g_ui.pstate);
    addRowIfVisible(g_ui.pidLabel, g_ui.pid);
    addRowIfVisible(g_ui.pactiveLabel, g_ui.pactive);
    addRowIfVisible(g_ui.premoveLabel, g_ui.premove);
    addRowIfVisible(g_ui.pframeLabel, g_ui.pframe);
    addRowIfVisible(g_ui.posxLabel, g_ui.posx);
    addRowIfVisible(g_ui.posyLabel, g_ui.posy);
    addRowIfVisible(g_ui.poszLabel, g_ui.posz);
    addRowIfVisible(g_ui.dirxLabel, g_ui.dirx);
    addRowIfVisible(g_ui.diryLabel, g_ui.diry);
    addRowIfVisible(g_ui.dirzLabel, g_ui.dirz);
    addRowIfVisible(g_ui.animIdLabel, g_ui.animId);
    addRowIfVisible(g_ui.humanSeatLabel, g_ui.humanSeat);
    addRowIfVisible(g_ui.humanCrouchLabel, g_ui.humanCrouch);
    addRowIfVisible(g_ui.humanAimLabel, g_ui.humanAim);
    addRowIfVisible(g_ui.humanShootXLabel, g_ui.humanShootX);
    addRowIfVisible(g_ui.humanShootYLabel, g_ui.humanShootY);
    addRowIfVisible(g_ui.humanShootZLabel, g_ui.humanShootZ);
    addRowIfVisible(g_ui.rotwLabel, g_ui.rotw);
    addRowIfVisible(g_ui.rotxLabel, g_ui.rotx);
    addRowIfVisible(g_ui.rotyLabel, g_ui.roty);
    addRowIfVisible(g_ui.rotzLabel, g_ui.rotz);
    addRowIfVisible(g_ui.carFuelLabel, g_ui.carFuel);
    addRowIfVisible(g_ui.carFlowLabel, g_ui.carFlow);
    addRowIfVisible(g_ui.carEngNormLabel, g_ui.carEngNorm);
    addRowIfVisible(g_ui.carEngCalcLabel, g_ui.carEngCalc);
    addRowIfVisible(g_ui.carSpeedLimitLabel, g_ui.carSpeedLimit);
    addRowIfVisible(g_ui.carLastGearLabel, g_ui.carLastGear);
    addRowIfVisible(g_ui.carGearLabel, g_ui.carGear);
    addRowIfVisible(g_ui.carGearboxFlagLabel, g_ui.carGearboxFlag);
    addRowIfVisible(g_ui.carDisableEngineLabel, g_ui.carDisableEngine);
    addRowIfVisible(g_ui.carEngineOnLabel, g_ui.carEngineOn);
    addRowIfVisible(g_ui.carIsEngineOnLabel, g_ui.carIsEngineOn);
    addRowIfVisible(g_ui.carOdometerLabel, g_ui.carOdometer);
    addRowIfVisible(g_ui.invModeLabel, g_ui.invMode);
    addRowIfVisible(g_ui.invFlagLabel, g_ui.invFlag);
    addRowIfVisible(g_ui.invSelIdLabel, g_ui.invSelId);
    addRowIfVisible(g_ui.invSelLoadedLabel, g_ui.invSelLoaded);
    addRowIfVisible(g_ui.invSelHiddenLabel, g_ui.invSelHidden);
    addRowIfVisible(g_ui.invSelUnkLabel, g_ui.invSelUnk);
    addRowIfVisible(g_ui.invCoatIdLabel, g_ui.invCoatId);
    addRowIfVisible(g_ui.invCoatLoadedLabel, g_ui.invCoatLoaded);
    addRowIfVisible(g_ui.invCoatHiddenLabel, g_ui.invCoatHidden);
    addRowIfVisible(g_ui.invCoatUnkLabel, g_ui.invCoatUnk);
    addRowIfVisible(g_ui.invS1IdLabel, g_ui.invS1Id);
    addRowIfVisible(g_ui.invS1LoadedLabel, g_ui.invS1Loaded);
    addRowIfVisible(g_ui.invS1HiddenLabel, g_ui.invS1Hidden);
    addRowIfVisible(g_ui.invS1UnkLabel, g_ui.invS1Unk);
    addRowIfVisible(g_ui.invS2IdLabel, g_ui.invS2Id);
    addRowIfVisible(g_ui.invS2LoadedLabel, g_ui.invS2Loaded);
    addRowIfVisible(g_ui.invS2HiddenLabel, g_ui.invS2Hidden);
    addRowIfVisible(g_ui.invS2UnkLabel, g_ui.invS2Unk);
    addRowIfVisible(g_ui.invS3IdLabel, g_ui.invS3Id);
    addRowIfVisible(g_ui.invS3LoadedLabel, g_ui.invS3Loaded);
    addRowIfVisible(g_ui.invS3HiddenLabel, g_ui.invS3Hidden);
    addRowIfVisible(g_ui.invS3UnkLabel, g_ui.invS3Unk);
    addRowIfVisible(g_ui.invS4IdLabel, g_ui.invS4Id);
    addRowIfVisible(g_ui.invS4LoadedLabel, g_ui.invS4Loaded);
    addRowIfVisible(g_ui.invS4HiddenLabel, g_ui.invS4Hidden);
    addRowIfVisible(g_ui.invS4UnkLabel, g_ui.invS4Unk);
    addRowIfVisible(g_ui.invS5IdLabel, g_ui.invS5Id);
    addRowIfVisible(g_ui.invS5LoadedLabel, g_ui.invS5Loaded);
    addRowIfVisible(g_ui.invS5HiddenLabel, g_ui.invS5Hidden);
    addRowIfVisible(g_ui.invS5UnkLabel, g_ui.invS5Unk);

    const int rightContentH = visibleRows * 32 + 24 + 8 + 28 + 4;
    UpdateActorsRightScrollBar(rightViewportH, rightContentH);
    const int scrollY = g_state.actorsRightScroll;
    int y = 0;

    auto placeRightRow = [&](HWND label, HWND edit, int width) {
        if (!rowVisible(label, edit)) {
            return;
        }
        const int drawY = rightTop + y - scrollY;
        MoveWindow(label, rightX, drawY + 2, labelW, 20, TRUE);
        MoveWindow(edit, rightX + labelW + 8, drawY, std::min(editW, width), rowH, TRUE);
        y += 32;
    };

    placeRightRow(g_ui.anameLabel, g_ui.aname, editW);
    placeRightRow(g_ui.amodelLabel, g_ui.amodel, editW);
    placeRightRow(g_ui.atypeLabel, g_ui.atype, 170);
    placeRightRow(g_ui.aidxLabel, g_ui.aidx, 170);
    placeRightRow(g_ui.apayloadLabel, g_ui.apayload, 170);
    placeRightRow(g_ui.pstateLabel, g_ui.pstate, 170);
    placeRightRow(g_ui.pidLabel, g_ui.pid, 170);
    placeRightRow(g_ui.pactiveLabel, g_ui.pactive, 170);
    placeRightRow(g_ui.premoveLabel, g_ui.premove, 170);
    placeRightRow(g_ui.pframeLabel, g_ui.pframe, 170);
    placeRightRow(g_ui.posxLabel, g_ui.posx, 170);
    placeRightRow(g_ui.posyLabel, g_ui.posy, 170);
    placeRightRow(g_ui.poszLabel, g_ui.posz, 170);
    placeRightRow(g_ui.dirxLabel, g_ui.dirx, 170);
    placeRightRow(g_ui.diryLabel, g_ui.diry, 170);
    placeRightRow(g_ui.dirzLabel, g_ui.dirz, 170);
    placeRightRow(g_ui.animIdLabel, g_ui.animId, 170);
    placeRightRow(g_ui.humanSeatLabel, g_ui.humanSeat, 170);
    placeRightRow(g_ui.humanCrouchLabel, g_ui.humanCrouch, 170);
    placeRightRow(g_ui.humanAimLabel, g_ui.humanAim, 170);
    placeRightRow(g_ui.humanShootXLabel, g_ui.humanShootX, 170);
    placeRightRow(g_ui.humanShootYLabel, g_ui.humanShootY, 170);
    placeRightRow(g_ui.humanShootZLabel, g_ui.humanShootZ, 170);
    placeRightRow(g_ui.rotwLabel, g_ui.rotw, 170);
    placeRightRow(g_ui.rotxLabel, g_ui.rotx, 170);
    placeRightRow(g_ui.rotyLabel, g_ui.roty, 170);
    placeRightRow(g_ui.rotzLabel, g_ui.rotz, 170);
    placeRightRow(g_ui.carFuelLabel, g_ui.carFuel, 170);
    placeRightRow(g_ui.carFlowLabel, g_ui.carFlow, 170);
    placeRightRow(g_ui.carEngNormLabel, g_ui.carEngNorm, 170);
    placeRightRow(g_ui.carEngCalcLabel, g_ui.carEngCalc, 170);
    placeRightRow(g_ui.carSpeedLimitLabel, g_ui.carSpeedLimit, 170);
    placeRightRow(g_ui.carLastGearLabel, g_ui.carLastGear, 170);
    placeRightRow(g_ui.carGearLabel, g_ui.carGear, 170);
    placeRightRow(g_ui.carGearboxFlagLabel, g_ui.carGearboxFlag, 170);
    placeRightRow(g_ui.carDisableEngineLabel, g_ui.carDisableEngine, 170);
    placeRightRow(g_ui.carEngineOnLabel, g_ui.carEngineOn, 170);
    placeRightRow(g_ui.carIsEngineOnLabel, g_ui.carIsEngineOn, 170);
    placeRightRow(g_ui.carOdometerLabel, g_ui.carOdometer, 170);
    placeRightRow(g_ui.invModeLabel, g_ui.invMode, 170);
    placeRightRow(g_ui.invFlagLabel, g_ui.invFlag, 170);
    placeRightRow(g_ui.invSelIdLabel, g_ui.invSelId, 170);
    placeRightRow(g_ui.invSelLoadedLabel, g_ui.invSelLoaded, 170);
    placeRightRow(g_ui.invSelHiddenLabel, g_ui.invSelHidden, 170);
    placeRightRow(g_ui.invSelUnkLabel, g_ui.invSelUnk, 170);
    placeRightRow(g_ui.invCoatIdLabel, g_ui.invCoatId, 170);
    placeRightRow(g_ui.invCoatLoadedLabel, g_ui.invCoatLoaded, 170);
    placeRightRow(g_ui.invCoatHiddenLabel, g_ui.invCoatHidden, 170);
    placeRightRow(g_ui.invCoatUnkLabel, g_ui.invCoatUnk, 170);
    placeRightRow(g_ui.invS1IdLabel, g_ui.invS1Id, 170);
    placeRightRow(g_ui.invS1LoadedLabel, g_ui.invS1Loaded, 170);
    placeRightRow(g_ui.invS1HiddenLabel, g_ui.invS1Hidden, 170);
    placeRightRow(g_ui.invS1UnkLabel, g_ui.invS1Unk, 170);
    placeRightRow(g_ui.invS2IdLabel, g_ui.invS2Id, 170);
    placeRightRow(g_ui.invS2LoadedLabel, g_ui.invS2Loaded, 170);
    placeRightRow(g_ui.invS2HiddenLabel, g_ui.invS2Hidden, 170);
    placeRightRow(g_ui.invS2UnkLabel, g_ui.invS2Unk, 170);
    placeRightRow(g_ui.invS3IdLabel, g_ui.invS3Id, 170);
    placeRightRow(g_ui.invS3LoadedLabel, g_ui.invS3Loaded, 170);
    placeRightRow(g_ui.invS3HiddenLabel, g_ui.invS3Hidden, 170);
    placeRightRow(g_ui.invS3UnkLabel, g_ui.invS3Unk, 170);
    placeRightRow(g_ui.invS4IdLabel, g_ui.invS4Id, 170);
    placeRightRow(g_ui.invS4LoadedLabel, g_ui.invS4Loaded, 170);
    placeRightRow(g_ui.invS4HiddenLabel, g_ui.invS4Hidden, 170);
    placeRightRow(g_ui.invS4UnkLabel, g_ui.invS4Unk, 170);
    placeRightRow(g_ui.invS5IdLabel, g_ui.invS5Id, 170);
    placeRightRow(g_ui.invS5LoadedLabel, g_ui.invS5Loaded, 170);
    placeRightRow(g_ui.invS5HiddenLabel, g_ui.invS5Hidden, 170);
    placeRightRow(g_ui.invS5UnkLabel, g_ui.invS5Unk, 170);

    MoveWindow(g_ui.coordHint, rightX, rightTop + y - scrollY + 2, rightW, 20, TRUE);
    y += 24;

    const int actionY = rightTop + y - scrollY + 8;
    const int actionW = 110;
    const int actionGap = 10;
    const int applyX = rightX + rightW - actionW;
    const int cloneX = std::max(rightX, applyX - actionGap - actionW);
    MoveWindow(g_ui.cloneActor, cloneX, actionY, actionW, 28, TRUE);
    MoveWindow(g_ui.applyActor, applyX, actionY, actionW, 28, TRUE);
}

void LayoutCarsPage() {
    if (g_ui.pageCars == nullptr) {
        return;
    }
    RECT rc = {};
    GetClientRect(g_ui.pageCars, &rc);
    const int w = rc.right - rc.left;
    const int h = rc.bottom - rc.top;

    const int margin = 16;
    const int gap = 14;
    const int rowH = 24;
    const int rowGap = 8;

    int leftW = std::max(300, w / 2 - 20);
    leftW = std::min(leftW, std::max(260, w - margin * 2 - 260 - gap));
    int rightW = std::max(260, w - margin * 2 - gap - leftW);
    const int leftX = margin;
    const int rightX = leftX + leftW + gap;

    MoveWindow(g_ui.carsTitle, margin, 12, w - margin * 2, 20, TRUE);
    const int listY = 40;
    const int listH = std::max(120, h - margin - listY);
    MoveWindow(g_ui.carsList, leftX, listY, leftW, listH, TRUE);

    const int labelW = 110;
    const int editW = std::max(120, rightW - labelW - 8);
    int y = 40;

    const auto placeRow = [&](HWND label, HWND edit, int customW = -1) {
        MoveWindow(label, rightX, y + 2, labelW, 20, TRUE);
        const int wEdit = (customW > 0) ? std::min(editW, customW) : editW;
        MoveWindow(edit, rightX + labelW + 8, y, wEdit, rowH, TRUE);
        y += rowH + rowGap;
    };

    placeRow(g_ui.carTabNameLabel, g_ui.carTabName);
    placeRow(g_ui.carTabModelLabel, g_ui.carTabModel);
    placeRow(g_ui.carTabIdxLabel, g_ui.carTabIdx, 180);
    y += 6;
    placeRow(g_ui.carTabPosXLabel, g_ui.carTabPosX, 180);
    placeRow(g_ui.carTabPosYLabel, g_ui.carTabPosY, 180);
    placeRow(g_ui.carTabPosZLabel, g_ui.carTabPosZ, 180);
    y += 6;
    placeRow(g_ui.carTabRotWLabel, g_ui.carTabRotW, 180);
    placeRow(g_ui.carTabRotXLabel, g_ui.carTabRotX, 180);
    placeRow(g_ui.carTabRotYLabel, g_ui.carTabRotY, 180);
    placeRow(g_ui.carTabRotZLabel, g_ui.carTabRotZ, 180);
    y += 6;
    placeRow(g_ui.carTabFuelLabel, g_ui.carTabFuel, 180);
    placeRow(g_ui.carTabSpeedLabel, g_ui.carTabSpeed, 180);
    placeRow(g_ui.carTabOdometerLabel, g_ui.carTabOdometer, 180);
    placeRow(g_ui.carTabEngineOnLabel, g_ui.carTabEngineOn, 180);

    const int btnW = 110;
    const int btnY = y + 8;
    MoveWindow(g_ui.applyCar, rightX + rightW - btnW, btnY, btnW, 28, TRUE);
    MoveWindow(g_ui.carsHint, rightX, btnY + 4, std::max(120, rightW - btnW - 12), 20, TRUE);
}

void LayoutGaragePage() {
    if (g_ui.pageGarage == nullptr) {
        return;
    }
    RECT rc = {};
    GetClientRect(g_ui.pageGarage, &rc);
    const int w = rc.right - rc.left;
    const int h = rc.bottom - rc.top;

    const int margin = 16;
    const int gap = 14;
    const int rowH = 24;
    const int rowGap = 6;
    const int labelW = 128;

    int leftW = std::max(360, w / 2 - 20);
    leftW = std::min(leftW, std::max(300, w - margin * 2 - 360 - gap));
    const int rightW = std::max(340, w - margin * 2 - gap - leftW);
    const int leftX = margin;
    const int rightX = leftX + leftW + gap;

    MoveWindow(g_ui.garageTitle, margin, 12, w - margin * 2, 20, TRUE);
    MoveWindow(g_ui.garageList, leftX, 40, leftW, std::max(120, h - margin - 40), TRUE);

    int y = 40;
    const int editW = std::max(120, rightW - labelW - 8);
    const auto placeRow = [&](HWND label, HWND edit, int customW = -1) {
        MoveWindow(label, rightX, y + 2, labelW, 20, TRUE);
        const int useW = (customW > 0) ? std::min(editW, customW) : editW;
        const int ctrlH = (edit == g_ui.garageACar || edit == g_ui.garageBCar) ? 220 : rowH;
        MoveWindow(edit, rightX + labelW + 8, y, useW, ctrlH, TRUE);
        y += rowH + rowGap;
    };

    placeRow(g_ui.garageSlotLabel, g_ui.garageSlot, 90);
    placeRow(g_ui.garageACarLabel, g_ui.garageACar, std::max(200, rightW - labelW - 8));
    placeRow(g_ui.garageBCarLabel, g_ui.garageBCar, std::max(200, rightW - labelW - 8));
    y += 2;
    placeRow(g_ui.garageALabel, g_ui.garageA, 190);
    placeRow(g_ui.garageBLabel, g_ui.garageB, 190);
    placeRow(g_ui.garageAColorLabel, g_ui.garageAColor, 90);
    placeRow(g_ui.garageBColorLabel, g_ui.garageBColor, 90);
    placeRow(g_ui.garageAFlagsLabel, g_ui.garageAFlags, 90);
    placeRow(g_ui.garageBFlagsLabel, g_ui.garageBFlags, 90);
    y += 2;
    placeRow(g_ui.garageADecodedLabel, g_ui.garageADecoded, std::max(200, rightW - labelW - 8));
    placeRow(g_ui.garageBDecodedLabel, g_ui.garageBDecoded, std::max(200, rightW - labelW - 8));
    y += 2;
    placeRow(g_ui.garageAHexLabel, g_ui.garageAHex, 190);
    placeRow(g_ui.garageBHexLabel, g_ui.garageBHex, 190);
    placeRow(g_ui.garageALow16Label, g_ui.garageALow16, 90);
    placeRow(g_ui.garageBLow16Label, g_ui.garageBLow16, 90);
    placeRow(g_ui.garageAHi8Label, g_ui.garageAHi8, 90);
    placeRow(g_ui.garageBHi8Label, g_ui.garageBHi8, 90);

    const int btnW = 102;
    const int btnY = y + 8;
    const int btnGap = 8;
    const int applyX = rightX + rightW - btnW;
    const int clearX = applyX - btnW - btnGap;
    const int syncX = clearX - btnW - btnGap;
    MoveWindow(g_ui.syncGarageB, syncX, btnY, btnW, 28, TRUE);
    MoveWindow(g_ui.clearGarage, clearX, btnY, btnW, 28, TRUE);
    MoveWindow(g_ui.applyGarage, applyX, btnY, btnW, 28, TRUE);
    MoveWindow(g_ui.garageHint, rightX, btnY + 4, std::max(120, rightW - btnW * 3 - btnGap * 2 - 12), 20, TRUE);
}

void LayoutWindow(HWND hwnd) {
    if (g_ui.tab == nullptr) {
        return;
    }
    RECT rc = {};
    GetClientRect(hwnd, &rc);
    const int w = rc.right - rc.left;
    const int h = rc.bottom - rc.top;

    const int margin = 16;
    const int topY = 10;
    const int btnW = 110;
    const int btnH = 28;

    MoveWindow(g_ui.fileLabel, margin, topY + 4, 40, 20, TRUE);
    MoveWindow(g_ui.openBtn, w - margin - btnW, topY, btnW, btnH, TRUE);
    MoveWindow(g_ui.path, margin + 44, topY + 4, w - (margin * 3) - btnW - 44, 20, TRUE);

    const int infoY = topY + 34;
    MoveWindow(g_ui.info, margin, infoY, w - margin * 2, 20, TRUE);
    MoveWindow(g_ui.warning, margin, infoY + 20, w - margin * 2, 20, TRUE);

    const int bottomY = h - margin - 30;
    const int tabY = infoY + 44;
    const int tabH = std::max(200, bottomY - tabY - 8);
    MoveWindow(g_ui.tab, margin, tabY, w - margin * 2, tabH, TRUE);

    RECT trc = {};
    GetWindowRect(g_ui.tab, &trc);
    MapWindowPoints(nullptr, hwnd, reinterpret_cast<LPPOINT>(&trc), 2);
    TabCtrl_AdjustRect(g_ui.tab, FALSE, &trc);
    MoveWindow(g_ui.pageMain, trc.left, trc.top, trc.right - trc.left, trc.bottom - trc.top, TRUE);
    MoveWindow(g_ui.pageActors, trc.left, trc.top, trc.right - trc.left, trc.bottom - trc.top, TRUE);
    MoveWindow(g_ui.pageCars, trc.left, trc.top, trc.right - trc.left, trc.bottom - trc.top, TRUE);
    MoveWindow(g_ui.pageGarage, trc.left, trc.top, trc.right - trc.left, trc.bottom - trc.top, TRUE);
    MoveWindow(g_ui.pageMission, trc.left, trc.top, trc.right - trc.left, trc.bottom - trc.top, TRUE);

    const int saveX = w - margin - btnW;
    const int resetX = saveX - 12 - btnW;
    MoveWindow(g_ui.resetBtn, resetX, bottomY, btnW, 30, TRUE);
    MoveWindow(g_ui.saveBtn, saveX, bottomY, btnW, 30, TRUE);
    MoveWindow(g_ui.status, margin, bottomY + 6, std::max(120, resetX - margin - 12), 20, TRUE);

    LayoutMainPage();
    LayoutMissionPage();
    LayoutActorsPage();
    LayoutCarsPage();
    LayoutGaragePage();
}

void CreatePages(HWND hwnd) {
    RECT rc = {};
    GetWindowRect(g_ui.tab, &rc);
    MapWindowPoints(nullptr, hwnd, reinterpret_cast<LPPOINT>(&rc), 2);
    TabCtrl_AdjustRect(g_ui.tab, FALSE, &rc);

    g_ui.pageMain = CreateWindowExA(0, "STATIC", "", WS_CHILD | WS_VISIBLE, rc.left, rc.top, rc.right - rc.left,
                                    rc.bottom - rc.top, hwnd, nullptr, nullptr, nullptr);
    g_ui.pageActors = CreateWindowExA(0, "STATIC", "", WS_CHILD, rc.left, rc.top, rc.right - rc.left,
                                      rc.bottom - rc.top, hwnd, nullptr, nullptr, nullptr);
    g_ui.pageCars = CreateWindowExA(0, "STATIC", "", WS_CHILD, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top,
                                    hwnd, nullptr, nullptr, nullptr);
    g_ui.pageGarage = CreateWindowExA(0, "STATIC", "", WS_CHILD, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top,
                                      hwnd, nullptr, nullptr, nullptr);
    g_ui.pageMission = CreateWindowExA(0, "STATIC", "", WS_CHILD, rc.left, rc.top, rc.right - rc.left,
                                       rc.bottom - rc.top, hwnd, nullptr, nullptr, nullptr);
    SubclassPageForward(g_ui.pageMain);
    SubclassPageForward(g_ui.pageActors);
    SubclassPageForward(g_ui.pageCars);
    SubclassPageForward(g_ui.pageGarage);
    SubclassPageForward(g_ui.pageMission);

    g_ui.mainTitle = MakeLabel(g_ui.pageMain, "Main Save Fields", 16, 12, 200, 20);
    g_ui.hpLabel = MakeLabel(g_ui.pageMain, "HP %:", 16, 44, 130, 20);
    g_ui.hp = MakeEdit(g_ui.pageMain, "", 150, 42, 140, 24, ID_EDIT_HP);
    g_ui.dateLabel = MakeLabel(g_ui.pageMain, "Date (DD.MM.YYYY):", 16, 76, 130, 20);
    g_ui.date = MakeEdit(g_ui.pageMain, "", 150, 74, 180, 24, ID_EDIT_DATE);
    g_ui.timeLabel = MakeLabel(g_ui.pageMain, "Time (HH:MM:SS):", 16, 108, 130, 20);
    g_ui.time = MakeEdit(g_ui.pageMain, "", 150, 106, 180, 24, ID_EDIT_TIME);
    g_ui.slotLabel = MakeLabel(g_ui.pageMain, "Slot:", 16, 140, 130, 20);
    g_ui.slot = MakeEdit(g_ui.pageMain, "", 150, 138, 140, 24, ID_EDIT_SLOT);
    g_ui.mcodeLabel = MakeLabel(g_ui.pageMain, "Mission code:", 16, 172, 130, 20);
    g_ui.mcode = MakeEdit(g_ui.pageMain, "", 150, 170, 140, 24, ID_EDIT_MCODE);
    g_ui.mnameLabel = MakeLabel(g_ui.pageMain, "Mission name:", 16, 204, 130, 20);
    g_ui.mname = MakeEdit(g_ui.pageMain, "", 150, 202, 360, 24, ID_EDIT_MNAME);

    g_ui.missionTitle = MakeLabel(g_ui.pageMission, "Mission / Script State (advanced)", 16, 12, 360, 20);
    g_ui.ghMarkerLabel = MakeLabel(g_ui.pageMission, "Payload marker:", 16, 44, 130, 20);
    g_ui.ghMarker = MakeEdit(g_ui.pageMission, "", 150, 42, 140, 24, ID_EDIT_GH_MARKER, ES_READONLY);
    g_ui.ghFieldALabel = MakeLabel(g_ui.pageMission, "Game field A*:", 16, 76, 130, 20);
    g_ui.ghFieldA = MakeEdit(g_ui.pageMission, "", 150, 74, 140, 24, ID_EDIT_GH_FIELD_A);
    g_ui.ghFieldBLabel = MakeLabel(g_ui.pageMission, "Game field B*:", 16, 108, 130, 20);
    g_ui.ghFieldB = MakeEdit(g_ui.pageMission, "", 150, 106, 140, 24, ID_EDIT_GH_FIELD_B);
    g_ui.ghMissionIdLabel = MakeLabel(g_ui.pageMission, "Mission ID:", 16, 140, 130, 20);
    g_ui.ghMissionId = MakeEdit(g_ui.pageMission, "", 150, 138, 140, 24, ID_EDIT_GH_MISSION_ID);
    g_ui.ghTimerOnLabel = MakeLabel(g_ui.pageMission, "Timer enabled:", 16, 172, 130, 20);
    g_ui.ghTimerOn = MakeEdit(g_ui.pageMission, "", 150, 170, 140, 24, ID_EDIT_GH_TIMER_ON);
    g_ui.ghTimerIntervalLabel = MakeLabel(g_ui.pageMission, "Timer interval:", 16, 204, 130, 20);
    g_ui.ghTimerInterval = MakeEdit(g_ui.pageMission, "", 150, 202, 140, 24, ID_EDIT_GH_TIMER_INTERVAL);
    g_ui.ghTimerALabel = MakeLabel(g_ui.pageMission, "Timer value A:", 16, 236, 130, 20);
    g_ui.ghTimerA = MakeEdit(g_ui.pageMission, "", 150, 234, 140, 24, ID_EDIT_GH_TIMER_A);
    g_ui.ghTimerBLabel = MakeLabel(g_ui.pageMission, "Timer value B:", 16, 268, 130, 20);
    g_ui.ghTimerB = MakeEdit(g_ui.pageMission, "", 150, 266, 140, 24, ID_EDIT_GH_TIMER_B);
    g_ui.ghTimerCLabel = MakeLabel(g_ui.pageMission, "Timer value C:", 16, 300, 130, 20);
    g_ui.ghTimerC = MakeEdit(g_ui.pageMission, "", 150, 298, 140, 24, ID_EDIT_GH_TIMER_C);
    g_ui.ghScoreOnLabel = MakeLabel(g_ui.pageMission, "Score enabled:", 16, 332, 130, 20);
    g_ui.ghScoreOn = MakeEdit(g_ui.pageMission, "", 150, 330, 140, 24, ID_EDIT_GH_SCORE_ON);
    g_ui.ghScoreValueLabel = MakeLabel(g_ui.pageMission, "Score value:", 16, 364, 130, 20);
    g_ui.ghScoreValue = MakeEdit(g_ui.pageMission, "", 150, 362, 140, 24, ID_EDIT_GH_SCORE_VALUE);
    g_ui.ghScriptEntriesLabel = MakeLabel(g_ui.pageMission, "Script entries:", 16, 396, 130, 20);
    g_ui.ghScriptEntries = MakeEdit(g_ui.pageMission, "", 150, 394, 140, 24, ID_EDIT_GH_SCRIPT_ENTRIES, ES_READONLY);
    g_ui.ghScriptChunksLabel = MakeLabel(g_ui.pageMission, "Script chunks:", 16, 428, 130, 20);
    g_ui.ghScriptChunks = MakeEdit(g_ui.pageMission, "", 150, 426, 140, 24, ID_EDIT_GH_SCRIPT_CHUNKS, ES_READONLY);

    g_ui.progOffsetLabel = MakeLabel(g_ui.pageMission, "Program offset:", 500, 44, 130, 20);
    g_ui.progOffset = MakeEdit(g_ui.pageMission, "", 634, 42, 160, 24, ID_EDIT_PROG_OFFSET, ES_READONLY);
    g_ui.progVarsLabel = MakeLabel(g_ui.pageMission, "Script vars:", 500, 76, 130, 20);
    g_ui.progVars = MakeEdit(g_ui.pageMission, "", 634, 74, 160, 24, ID_EDIT_PROG_VARS, ES_READONLY);
    g_ui.progActorsLabel = MakeLabel(g_ui.pageMission, "Script actors:", 500, 108, 130, 20);
    g_ui.progActors = MakeEdit(g_ui.pageMission, "", 634, 106, 160, 24, ID_EDIT_PROG_ACTORS, ES_READONLY);
    g_ui.progFramesLabel = MakeLabel(g_ui.pageMission, "Script frames:", 500, 140, 130, 20);
    g_ui.progFrames = MakeEdit(g_ui.pageMission, "", 634, 138, 160, 24, ID_EDIT_PROG_FRAMES, ES_READONLY);
    g_ui.progCmdBlockLabel = MakeLabel(g_ui.pageMission, "Pause script:", 500, 172, 130, 20);
    g_ui.progCmdBlock = MakeEdit(g_ui.pageMission, "", 634, 170, 160, 24, ID_EDIT_PROG_CMD_BLOCK);
    g_ui.progVarIndexLabel = MakeLabel(g_ui.pageMission, "Script var #:", 500, 204, 130, 20);
    g_ui.progVarIndex = MakeEdit(g_ui.pageMission, "", 634, 202, 160, 24, ID_EDIT_PROG_VAR_INDEX);
    g_ui.progVarValueLabel = MakeLabel(g_ui.pageMission, "Script value:", 500, 236, 130, 20);
    g_ui.progVarValue = MakeEdit(g_ui.pageMission, "", 634, 234, 160, 24, ID_EDIT_PROG_VAR_VALUE);
    g_ui.progLoadVar = MakeButton(g_ui.pageMission, "Read Script Var", 634, 268, 130, 28, ID_BTN_PROG_LOAD_VAR);
    g_ui.progReloadTable = MakeButton(g_ui.pageMission, "Reload Table", 770, 268, 130, 28, ID_BTN_PROG_RELOAD_TABLE);
    g_ui.progVarsTableLabel = MakeLabel(g_ui.pageMission, "Script vars table (index | value):", 16, 388, 320, 18);
    g_ui.progVarsTable = CreateWindowExA(WS_EX_CLIENTEDGE,
                                          "LISTBOX",
                                          "",
                                          WS_CHILD | WS_VISIBLE | LBS_NOTIFY | WS_VSCROLL,
                                          16,
                                          408,
                                          860,
                                          130,
                                          g_ui.pageMission,
                                          reinterpret_cast<HMENU>(static_cast<INT_PTR>(ID_LIST_PROG_VARS)),
                                          nullptr,
                                          nullptr);
    g_ui.missionHint = MakeLabel(g_ui.pageMission, "Script program: -", 500, 304, 420, 20);
    g_ui.missionHelp1 = MakeLabel(g_ui.pageMission, "Script var = internal float variable of mission script.", 16, 334, 920, 18);
    g_ui.missionHelp2 =
        MakeLabel(g_ui.pageMission, "How to edit: enter Script var # -> Read Script Var -> change Script value -> Save As...",
                  16, 352, 920, 18);

    g_ui.carsTitle = MakeLabel(g_ui.pageCars, "Cars", 16, 12, 200, 20);
    g_ui.carsList = CreateWindowExA(WS_EX_CLIENTEDGE,
                                    "LISTBOX",
                                    "",
                                    WS_CHILD | WS_VISIBLE | LBS_NOTIFY | WS_VSCROLL,
                                    16,
                                    40,
                                    440,
                                    320,
                                    g_ui.pageCars,
                                    reinterpret_cast<HMENU>(static_cast<INT_PTR>(ID_LIST_CARS)),
                                    nullptr,
                                    nullptr);
    g_ui.carTabNameLabel = MakeLabel(g_ui.pageCars, "Car name:", 476, 42, 110, 20);
    g_ui.carTabName = MakeEdit(g_ui.pageCars, "", 592, 40, 260, 24, ID_EDIT_CAR_TAB_NAME, ES_READONLY);
    g_ui.carTabModelLabel = MakeLabel(g_ui.pageCars, "Car model:", 476, 74, 110, 20);
    g_ui.carTabModel = MakeEdit(g_ui.pageCars, "", 592, 72, 260, 24, ID_EDIT_CAR_TAB_MODEL, ES_READONLY);
    g_ui.carTabIdxLabel = MakeLabel(g_ui.pageCars, "Car idx:", 476, 106, 110, 20);
    g_ui.carTabIdx = MakeEdit(g_ui.pageCars, "", 592, 104, 180, 24, ID_EDIT_CAR_TAB_IDX, ES_READONLY);
    g_ui.carTabPosXLabel = MakeLabel(g_ui.pageCars, "Pos X:", 476, 146, 110, 20);
    g_ui.carTabPosX = MakeEdit(g_ui.pageCars, "", 592, 144, 180, 24, ID_EDIT_CAR_TAB_POS_X);
    g_ui.carTabPosYLabel = MakeLabel(g_ui.pageCars, "Pos Y:", 476, 178, 110, 20);
    g_ui.carTabPosY = MakeEdit(g_ui.pageCars, "", 592, 176, 180, 24, ID_EDIT_CAR_TAB_POS_Y);
    g_ui.carTabPosZLabel = MakeLabel(g_ui.pageCars, "Pos Z:", 476, 210, 110, 20);
    g_ui.carTabPosZ = MakeEdit(g_ui.pageCars, "", 592, 208, 180, 24, ID_EDIT_CAR_TAB_POS_Z);
    g_ui.carTabRotWLabel = MakeLabel(g_ui.pageCars, "Rot W:", 476, 250, 110, 20);
    g_ui.carTabRotW = MakeEdit(g_ui.pageCars, "", 592, 248, 180, 24, ID_EDIT_CAR_TAB_ROT_W);
    g_ui.carTabRotXLabel = MakeLabel(g_ui.pageCars, "Rot X:", 476, 282, 110, 20);
    g_ui.carTabRotX = MakeEdit(g_ui.pageCars, "", 592, 280, 180, 24, ID_EDIT_CAR_TAB_ROT_X);
    g_ui.carTabRotYLabel = MakeLabel(g_ui.pageCars, "Rot Y:", 476, 314, 110, 20);
    g_ui.carTabRotY = MakeEdit(g_ui.pageCars, "", 592, 312, 180, 24, ID_EDIT_CAR_TAB_ROT_Y);
    g_ui.carTabRotZLabel = MakeLabel(g_ui.pageCars, "Rot Z:", 476, 346, 110, 20);
    g_ui.carTabRotZ = MakeEdit(g_ui.pageCars, "", 592, 344, 180, 24, ID_EDIT_CAR_TAB_ROT_Z);
    g_ui.carTabFuelLabel = MakeLabel(g_ui.pageCars, "Fuel:", 476, 386, 110, 20);
    g_ui.carTabFuel = MakeEdit(g_ui.pageCars, "", 592, 384, 180, 24, ID_EDIT_CAR_TAB_FUEL);
    g_ui.carTabSpeedLabel = MakeLabel(g_ui.pageCars, "Speed limit:", 476, 418, 110, 20);
    g_ui.carTabSpeed = MakeEdit(g_ui.pageCars, "", 592, 416, 180, 24, ID_EDIT_CAR_TAB_SPEED);
    g_ui.carTabOdometerLabel = MakeLabel(g_ui.pageCars, "Odometer:", 476, 450, 110, 20);
    g_ui.carTabOdometer = MakeEdit(g_ui.pageCars, "", 592, 448, 180, 24, ID_EDIT_CAR_TAB_ODOMETER);
    g_ui.carTabEngineOnLabel = MakeLabel(g_ui.pageCars, "Engine ON:", 476, 482, 110, 20);
    g_ui.carTabEngineOn = MakeEdit(g_ui.pageCars, "", 592, 480, 180, 24, ID_EDIT_CAR_TAB_ENGINE_ON);
    g_ui.applyCar = MakeButton(g_ui.pageCars, "Apply Car", 742, 514, 110, 28, ID_BTN_APPLY_CAR);
    g_ui.carsHint = MakeLabel(g_ui.pageCars, "Cars: -", 476, 518, 252, 20);

    g_ui.garageTitle = MakeLabel(g_ui.pageGarage, "Garage (Salieri bar)", 16, 12, 260, 20);
    g_ui.garageList = CreateWindowExA(WS_EX_CLIENTEDGE,
                                      "LISTBOX",
                                      "",
                                      WS_CHILD | WS_VISIBLE | LBS_NOTIFY | WS_VSCROLL,
                                      16,
                                      40,
                                      440,
                                      320,
                                      g_ui.pageGarage,
                                      reinterpret_cast<HMENU>(static_cast<INT_PTR>(ID_LIST_GARAGE)),
                                      nullptr,
                                      nullptr);
    g_ui.garageSlotLabel = MakeLabel(g_ui.pageGarage, "Slot:", 476, 42, 130, 20);
    g_ui.garageSlot = MakeEdit(g_ui.pageGarage, "", 614, 40, 120, 24, ID_EDIT_GARAGE_SLOT, ES_READONLY);
    g_ui.garageACarLabel = MakeLabel(g_ui.pageGarage, "Primary car:", 476, 74, 130, 20);
    g_ui.garageACar = MakeCombo(g_ui.pageGarage, 614, 72, 280, 220, ID_COMBO_GARAGE_A_CAR);
    g_ui.garageBCarLabel = MakeLabel(g_ui.pageGarage, "Secondary car:", 476, 106, 130, 20);
    g_ui.garageBCar = MakeCombo(g_ui.pageGarage, 614, 104, 280, 220, ID_COMBO_GARAGE_B_CAR);
    g_ui.garageALabel = MakeLabel(g_ui.pageGarage, "Primary A (u32):", 476, 142, 130, 20);
    g_ui.garageA = MakeEdit(g_ui.pageGarage, "", 614, 140, 190, 24, ID_EDIT_GARAGE_A);
    g_ui.garageBLabel = MakeLabel(g_ui.pageGarage, "Secondary B (u32):", 476, 174, 130, 20);
    g_ui.garageB = MakeEdit(g_ui.pageGarage, "", 614, 172, 190, 24, ID_EDIT_GARAGE_B);
    g_ui.garageAColorLabel = MakeLabel(g_ui.pageGarage, "Primary color:", 476, 206, 130, 20);
    g_ui.garageAColor = MakeEdit(g_ui.pageGarage, "", 614, 204, 90, 24, ID_EDIT_GARAGE_A_COLOR);
    g_ui.garageBColorLabel = MakeLabel(g_ui.pageGarage, "Secondary color:", 476, 238, 130, 20);
    g_ui.garageBColor = MakeEdit(g_ui.pageGarage, "", 614, 236, 90, 24, ID_EDIT_GARAGE_B_COLOR);
    g_ui.garageAFlagsLabel = MakeLabel(g_ui.pageGarage, "Primary flag:", 476, 206, 130, 20);
    g_ui.garageAFlags = MakeEdit(g_ui.pageGarage, "", 614, 204, 90, 24, ID_EDIT_GARAGE_A_FLAGS);
    g_ui.garageBFlagsLabel = MakeLabel(g_ui.pageGarage, "Secondary flag:", 476, 238, 130, 20);
    g_ui.garageBFlags = MakeEdit(g_ui.pageGarage, "", 614, 236, 90, 24, ID_EDIT_GARAGE_B_FLAGS);
    g_ui.garageADecodedLabel = MakeLabel(g_ui.pageGarage, "Primary decoded:", 476, 270, 130, 20);
    g_ui.garageADecoded = MakeEdit(g_ui.pageGarage, "", 614, 268, 320, 24, ID_EDIT_GARAGE_A_DECODED, ES_READONLY);
    g_ui.garageBDecodedLabel = MakeLabel(g_ui.pageGarage, "Secondary decoded:", 476, 302, 130, 20);
    g_ui.garageBDecoded = MakeEdit(g_ui.pageGarage, "", 614, 300, 320, 24, ID_EDIT_GARAGE_B_DECODED, ES_READONLY);
    g_ui.garageAHexLabel = MakeLabel(g_ui.pageGarage, "Primary A (hex):", 476, 334, 130, 20);
    g_ui.garageAHex = MakeEdit(g_ui.pageGarage, "", 614, 332, 190, 24, ID_EDIT_GARAGE_A_HEX, ES_READONLY);
    g_ui.garageBHexLabel = MakeLabel(g_ui.pageGarage, "Secondary B (hex):", 476, 366, 130, 20);
    g_ui.garageBHex = MakeEdit(g_ui.pageGarage, "", 614, 364, 190, 24, ID_EDIT_GARAGE_B_HEX, ES_READONLY);
    g_ui.garageALow16Label = MakeLabel(g_ui.pageGarage, "Primary idx low16:", 476, 398, 130, 20);
    g_ui.garageALow16 = MakeEdit(g_ui.pageGarage, "", 614, 396, 90, 24, ID_EDIT_GARAGE_A_LOW16, ES_READONLY);
    g_ui.garageBLow16Label = MakeLabel(g_ui.pageGarage, "Secondary idx low16:", 476, 430, 130, 20);
    g_ui.garageBLow16 = MakeEdit(g_ui.pageGarage, "", 614, 428, 90, 24, ID_EDIT_GARAGE_B_LOW16, ES_READONLY);
    g_ui.garageAHi8Label = MakeLabel(g_ui.pageGarage, "Primary hi8:", 476, 462, 130, 20);
    g_ui.garageAHi8 = MakeEdit(g_ui.pageGarage, "", 614, 460, 90, 24, ID_EDIT_GARAGE_A_HI8, ES_READONLY);
    g_ui.garageBHi8Label = MakeLabel(g_ui.pageGarage, "Secondary hi8:", 476, 494, 130, 20);
    g_ui.garageBHi8 = MakeEdit(g_ui.pageGarage, "", 614, 492, 90, 24, ID_EDIT_GARAGE_B_HI8, ES_READONLY);
    g_ui.syncGarageB = MakeButton(g_ui.pageGarage, "B = A", 510, 528, 90, 28, ID_BTN_GARAGE_SYNC_B);
    g_ui.clearGarage = MakeButton(g_ui.pageGarage, "Clear Slot", 622, 528, 110, 28, ID_BTN_CLEAR_GARAGE);
    g_ui.applyGarage = MakeButton(g_ui.pageGarage, "Apply Slot", 742, 528, 110, 28, ID_BTN_APPLY_GARAGE);
    g_ui.garageHint = MakeLabel(g_ui.pageGarage, "Garage: -", 476, 532, 140, 20);

    g_ui.actorsTitle = MakeLabel(g_ui.pageActors, "Actors", 16, 12, 200, 20);
    g_ui.actorsScroll = CreateWindowExA(0, "SCROLLBAR", "", WS_CHILD | SBS_VERT, 0, 0, 16, 100, g_ui.pageActors,
                                        reinterpret_cast<HMENU>(static_cast<INT_PTR>(ID_SCROLL_ACTORS)), nullptr, nullptr);
    g_ui.filterNameLabel = MakeLabel(g_ui.pageActors, "Filter name:", 16, 40, 90, 20);
    g_ui.filterName = MakeEdit(g_ui.pageActors, "", 108, 38, 180, 24, ID_EDIT_FILTER_NAME);
    g_ui.filterTypeLabel = MakeLabel(g_ui.pageActors, "Filter type:", 300, 40, 80, 20);
    g_ui.filterType = MakeEdit(g_ui.pageActors, "", 382, 38, 80, 24, ID_EDIT_FILTER_TYPE);
    g_ui.applyFilter = MakeButton(g_ui.pageActors, "Apply", 472, 38, 70, 24, ID_BTN_FILTER_APPLY);
    g_ui.clearFilter = MakeButton(g_ui.pageActors, "Clear", 548, 38, 70, 24, ID_BTN_FILTER_CLEAR);

    g_ui.actors = CreateWindowExA(WS_EX_CLIENTEDGE, "LISTBOX", "", WS_CHILD | WS_VISIBLE | LBS_NOTIFY | WS_VSCROLL,
                                  16, 70, 470, 216, g_ui.pageActors,
                                  reinterpret_cast<HMENU>(static_cast<INT_PTR>(ID_LIST_ACTORS)), nullptr, nullptr);
    g_ui.anameLabel = MakeLabel(g_ui.pageActors, "Actor name:", 510, 44, 120, 20);
    g_ui.aname = MakeEdit(g_ui.pageActors, "", 640, 42, 260, 24, ID_EDIT_ANAME);
    g_ui.amodelLabel = MakeLabel(g_ui.pageActors, "Actor model:", 510, 76, 120, 20);
    g_ui.amodel = MakeEdit(g_ui.pageActors, "", 640, 74, 260, 24, ID_EDIT_AMODEL);
    g_ui.atypeLabel = MakeLabel(g_ui.pageActors, "Actor type:", 510, 108, 120, 20);
    g_ui.atype = MakeEdit(g_ui.pageActors, "", 640, 106, 140, 24, ID_EDIT_ATYPE);
    g_ui.aidxLabel = MakeLabel(g_ui.pageActors, "Actor idx:", 510, 140, 120, 20);
    g_ui.aidx = MakeEdit(g_ui.pageActors, "", 640, 138, 140, 24, ID_EDIT_AIDX);
    g_ui.apayloadLabel = MakeLabel(g_ui.pageActors, "Payload size:", 510, 172, 120, 20);
    g_ui.apayload = MakeEdit(g_ui.pageActors, "", 640, 170, 140, 24, ID_EDIT_APAYLOAD, ES_READONLY);
    g_ui.pstateLabel = MakeLabel(g_ui.pageActors, "Payload state:", 510, 204, 120, 20);
    g_ui.pstate = MakeEdit(g_ui.pageActors, "", 640, 202, 140, 24, ID_EDIT_PSTATE);
    g_ui.pidLabel = MakeLabel(g_ui.pageActors, "Payload id:", 510, 236, 120, 20);
    g_ui.pid = MakeEdit(g_ui.pageActors, "", 640, 234, 140, 24, ID_EDIT_PID);
    g_ui.pactiveLabel = MakeLabel(g_ui.pageActors, "Is active:", 510, 268, 120, 20);
    g_ui.pactive = MakeEdit(g_ui.pageActors, "", 640, 266, 140, 24, ID_EDIT_PACTIVE);
    g_ui.premoveLabel = MakeLabel(g_ui.pageActors, "Do remove:", 510, 300, 120, 20);
    g_ui.premove = MakeEdit(g_ui.pageActors, "", 640, 298, 140, 24, ID_EDIT_PREMOVE);
    g_ui.pframeLabel = MakeLabel(g_ui.pageActors, "Frame on:", 510, 332, 120, 20);
    g_ui.pframe = MakeEdit(g_ui.pageActors, "", 640, 330, 140, 24, ID_EDIT_PFRAME);
    g_ui.posxLabel = MakeLabel(g_ui.pageActors, "Pos X:", 510, 364, 120, 20);
    g_ui.posx = MakeEdit(g_ui.pageActors, "", 640, 362, 140, 24, ID_EDIT_POS_X);
    g_ui.posyLabel = MakeLabel(g_ui.pageActors, "Pos Y:", 510, 396, 120, 20);
    g_ui.posy = MakeEdit(g_ui.pageActors, "", 640, 394, 140, 24, ID_EDIT_POS_Y);
    g_ui.poszLabel = MakeLabel(g_ui.pageActors, "Pos Z:", 510, 428, 120, 20);
    g_ui.posz = MakeEdit(g_ui.pageActors, "", 640, 426, 140, 24, ID_EDIT_POS_Z);
    g_ui.dirxLabel = MakeLabel(g_ui.pageActors, "Dir X:", 510, 460, 120, 20);
    g_ui.dirx = MakeEdit(g_ui.pageActors, "", 640, 458, 140, 24, ID_EDIT_DIR_X);
    g_ui.diryLabel = MakeLabel(g_ui.pageActors, "Dir Y:", 510, 492, 120, 20);
    g_ui.diry = MakeEdit(g_ui.pageActors, "", 640, 490, 140, 24, ID_EDIT_DIR_Y);
    g_ui.dirzLabel = MakeLabel(g_ui.pageActors, "Dir Z:", 510, 524, 120, 20);
    g_ui.dirz = MakeEdit(g_ui.pageActors, "", 640, 522, 140, 24, ID_EDIT_DIR_Z);
    g_ui.animIdLabel = MakeLabel(g_ui.pageActors, "Anim ID:", 510, 556, 120, 20);
    g_ui.animId = MakeEdit(g_ui.pageActors, "", 640, 554, 140, 24, ID_EDIT_ANIM_ID);
    g_ui.humanSeatLabel = MakeLabel(g_ui.pageActors, "Seat ID (46):", 510, 588, 120, 20);
    g_ui.humanSeat = MakeEdit(g_ui.pageActors, "", 640, 586, 140, 24, ID_EDIT_HUMAN_SEAT);
    g_ui.humanCrouchLabel = MakeLabel(g_ui.pageActors, "Crouching (50):", 510, 620, 120, 20);
    g_ui.humanCrouch = MakeEdit(g_ui.pageActors, "", 640, 618, 140, 24, ID_EDIT_HUMAN_CROUCH);
    g_ui.humanAimLabel = MakeLabel(g_ui.pageActors, "Aiming (51):", 510, 652, 120, 20);
    g_ui.humanAim = MakeEdit(g_ui.pageActors, "", 640, 650, 140, 24, ID_EDIT_HUMAN_AIM);
    g_ui.humanShootXLabel = MakeLabel(g_ui.pageActors, "Shoot X (54):", 510, 684, 120, 20);
    g_ui.humanShootX = MakeEdit(g_ui.pageActors, "", 640, 682, 140, 24, ID_EDIT_HUMAN_SHOOT_X);
    g_ui.humanShootYLabel = MakeLabel(g_ui.pageActors, "Shoot Y (58):", 510, 716, 120, 20);
    g_ui.humanShootY = MakeEdit(g_ui.pageActors, "", 640, 714, 140, 24, ID_EDIT_HUMAN_SHOOT_Y);
    g_ui.humanShootZLabel = MakeLabel(g_ui.pageActors, "Shoot Z (62):", 510, 748, 120, 20);
    g_ui.humanShootZ = MakeEdit(g_ui.pageActors, "", 640, 746, 140, 24, ID_EDIT_HUMAN_SHOOT_Z);
    g_ui.rotwLabel = MakeLabel(g_ui.pageActors, "Rot W:", 510, 780, 120, 20);
    g_ui.rotw = MakeEdit(g_ui.pageActors, "", 640, 778, 140, 24, ID_EDIT_ROT_W);
    g_ui.rotxLabel = MakeLabel(g_ui.pageActors, "Rot X:", 510, 812, 120, 20);
    g_ui.rotx = MakeEdit(g_ui.pageActors, "", 640, 810, 140, 24, ID_EDIT_ROT_X);
    g_ui.rotyLabel = MakeLabel(g_ui.pageActors, "Rot Y:", 510, 844, 120, 20);
    g_ui.roty = MakeEdit(g_ui.pageActors, "", 640, 842, 140, 24, ID_EDIT_ROT_Y);
    g_ui.rotzLabel = MakeLabel(g_ui.pageActors, "Rot Z:", 510, 876, 120, 20);
    g_ui.rotz = MakeEdit(g_ui.pageActors, "", 640, 874, 140, 24, ID_EDIT_ROT_Z);
    g_ui.carFuelLabel = MakeLabel(g_ui.pageActors, "Car Fuel* (304):", 510, 908, 120, 20);
    g_ui.carFuel = MakeEdit(g_ui.pageActors, "", 640, 906, 140, 24, ID_EDIT_CAR_FUEL);
    g_ui.carFlowLabel = MakeLabel(g_ui.pageActors, "Fuel Flow* (211):", 510, 940, 120, 20);
    g_ui.carFlow = MakeEdit(g_ui.pageActors, "", 640, 938, 140, 24, ID_EDIT_CAR_FLOW);
    g_ui.carEngNormLabel = MakeLabel(g_ui.pageActors, "EngNorm* (137):", 510, 972, 120, 20);
    g_ui.carEngNorm = MakeEdit(g_ui.pageActors, "", 640, 970, 140, 24, ID_EDIT_CAR_ENG_NORM);
    g_ui.carEngCalcLabel = MakeLabel(g_ui.pageActors, "EngCalc* (141):", 510, 1004, 120, 20);
    g_ui.carEngCalc = MakeEdit(g_ui.pageActors, "", 640, 1002, 140, 24, ID_EDIT_CAR_ENG_CALC);
    g_ui.carSpeedLimitLabel = MakeLabel(g_ui.pageActors, "SpeedLimit* (215):", 510, 1036, 120, 20);
    g_ui.carSpeedLimit = MakeEdit(g_ui.pageActors, "", 640, 1034, 140, 24, ID_EDIT_CAR_SPEED_LIMIT);
    g_ui.carLastGearLabel = MakeLabel(g_ui.pageActors, "LastGear* (245):", 510, 1068, 120, 20);
    g_ui.carLastGear = MakeEdit(g_ui.pageActors, "", 640, 1066, 140, 24, ID_EDIT_CAR_LAST_GEAR);
    g_ui.carGearLabel = MakeLabel(g_ui.pageActors, "Gear* (249):", 510, 1100, 120, 20);
    g_ui.carGear = MakeEdit(g_ui.pageActors, "", 640, 1098, 140, 24, ID_EDIT_CAR_GEAR);
    g_ui.carGearboxFlagLabel = MakeLabel(g_ui.pageActors, "GearboxFlg* (273):", 510, 1132, 120, 20);
    g_ui.carGearboxFlag = MakeEdit(g_ui.pageActors, "", 640, 1130, 140, 24, ID_EDIT_CAR_GEARBOX_FLAG);
    g_ui.carDisableEngineLabel = MakeLabel(g_ui.pageActors, "DisableEng* (277):", 510, 1164, 120, 20);
    g_ui.carDisableEngine = MakeEdit(g_ui.pageActors, "", 640, 1162, 140, 24, ID_EDIT_CAR_DISABLE_ENGINE);
    g_ui.carEngineOnLabel = MakeLabel(g_ui.pageActors, "EngineON* (298):", 510, 1196, 120, 20);
    g_ui.carEngineOn = MakeEdit(g_ui.pageActors, "", 640, 1194, 140, 24, ID_EDIT_CAR_ENGINE_ON);
    g_ui.carIsEngineOnLabel = MakeLabel(g_ui.pageActors, "IsEngineOn* (303):", 510, 1228, 120, 20);
    g_ui.carIsEngineOn = MakeEdit(g_ui.pageActors, "", 640, 1226, 140, 24, ID_EDIT_CAR_IS_ENGINE_ON);
    g_ui.carOdometerLabel = MakeLabel(g_ui.pageActors, "Odometer* (345):", 510, 1260, 120, 20);
    g_ui.carOdometer = MakeEdit(g_ui.pageActors, "", 640, 1258, 140, 24, ID_EDIT_CAR_ODOMETER);
    g_ui.invModeLabel = MakeLabel(g_ui.pageActors, "Inv Mode (0):", 510, 1292, 120, 20);
    g_ui.invMode = MakeEdit(g_ui.pageActors, "", 640, 1290, 140, 24, ID_EDIT_INV_MODE);
    g_ui.invFlagLabel = MakeLabel(g_ui.pageActors, "Inv Flag (7):", 510, 1324, 120, 20);
    g_ui.invFlag = MakeEdit(g_ui.pageActors, "", 640, 1322, 140, 24, ID_EDIT_INV_FLAG);
    g_ui.invSelIdLabel = MakeLabel(g_ui.pageActors, "Sel ID:", 510, 1356, 120, 20);
    g_ui.invSelId = MakeEdit(g_ui.pageActors, "", 640, 1354, 140, 24, ID_EDIT_INV_SEL_ID);
    g_ui.invSelLoadedLabel = MakeLabel(g_ui.pageActors, "Sel Ammo L:", 510, 1388, 120, 20);
    g_ui.invSelLoaded = MakeEdit(g_ui.pageActors, "", 640, 1386, 140, 24, ID_EDIT_INV_SEL_LOADED);
    g_ui.invSelHiddenLabel = MakeLabel(g_ui.pageActors, "Sel Ammo H:", 510, 1420, 120, 20);
    g_ui.invSelHidden = MakeEdit(g_ui.pageActors, "", 640, 1418, 140, 24, ID_EDIT_INV_SEL_HIDDEN);
    g_ui.invSelUnkLabel = MakeLabel(g_ui.pageActors, "Sel Unk:", 510, 1452, 120, 20);
    g_ui.invSelUnk = MakeEdit(g_ui.pageActors, "", 640, 1450, 140, 24, ID_EDIT_INV_SEL_UNK);
    g_ui.invCoatIdLabel = MakeLabel(g_ui.pageActors, "Coat ID:", 510, 1484, 120, 20);
    g_ui.invCoatId = MakeEdit(g_ui.pageActors, "", 640, 1482, 140, 24, ID_EDIT_INV_COAT_ID);
    g_ui.invCoatLoadedLabel = MakeLabel(g_ui.pageActors, "Coat Ammo L:", 510, 1516, 120, 20);
    g_ui.invCoatLoaded = MakeEdit(g_ui.pageActors, "", 640, 1514, 140, 24, ID_EDIT_INV_COAT_LOADED);
    g_ui.invCoatHiddenLabel = MakeLabel(g_ui.pageActors, "Coat Ammo H:", 510, 1548, 120, 20);
    g_ui.invCoatHidden = MakeEdit(g_ui.pageActors, "", 640, 1546, 140, 24, ID_EDIT_INV_COAT_HIDDEN);
    g_ui.invCoatUnkLabel = MakeLabel(g_ui.pageActors, "Coat Unk:", 510, 1580, 120, 20);
    g_ui.invCoatUnk = MakeEdit(g_ui.pageActors, "", 640, 1578, 140, 24, ID_EDIT_INV_COAT_UNK);
    g_ui.invS1IdLabel = MakeLabel(g_ui.pageActors, "Slot1 ID:", 510, 1612, 120, 20);
    g_ui.invS1Id = MakeEdit(g_ui.pageActors, "", 640, 1610, 140, 24, ID_EDIT_INV_S1_ID);
    g_ui.invS1LoadedLabel = MakeLabel(g_ui.pageActors, "Slot1 Ammo L:", 510, 1644, 120, 20);
    g_ui.invS1Loaded = MakeEdit(g_ui.pageActors, "", 640, 1642, 140, 24, ID_EDIT_INV_S1_LOADED);
    g_ui.invS1HiddenLabel = MakeLabel(g_ui.pageActors, "Slot1 Ammo H:", 510, 1676, 120, 20);
    g_ui.invS1Hidden = MakeEdit(g_ui.pageActors, "", 640, 1674, 140, 24, ID_EDIT_INV_S1_HIDDEN);
    g_ui.invS1UnkLabel = MakeLabel(g_ui.pageActors, "Slot1 Unk:", 510, 1708, 120, 20);
    g_ui.invS1Unk = MakeEdit(g_ui.pageActors, "", 640, 1706, 140, 24, ID_EDIT_INV_S1_UNK);
    g_ui.invS2IdLabel = MakeLabel(g_ui.pageActors, "Slot2 ID:", 510, 1740, 120, 20);
    g_ui.invS2Id = MakeEdit(g_ui.pageActors, "", 640, 1738, 140, 24, ID_EDIT_INV_S2_ID);
    g_ui.invS2LoadedLabel = MakeLabel(g_ui.pageActors, "Slot2 Ammo L:", 510, 1772, 120, 20);
    g_ui.invS2Loaded = MakeEdit(g_ui.pageActors, "", 640, 1770, 140, 24, ID_EDIT_INV_S2_LOADED);
    g_ui.invS2HiddenLabel = MakeLabel(g_ui.pageActors, "Slot2 Ammo H:", 510, 1804, 120, 20);
    g_ui.invS2Hidden = MakeEdit(g_ui.pageActors, "", 640, 1802, 140, 24, ID_EDIT_INV_S2_HIDDEN);
    g_ui.invS2UnkLabel = MakeLabel(g_ui.pageActors, "Slot2 Unk:", 510, 1836, 120, 20);
    g_ui.invS2Unk = MakeEdit(g_ui.pageActors, "", 640, 1834, 140, 24, ID_EDIT_INV_S2_UNK);
    g_ui.invS3IdLabel = MakeLabel(g_ui.pageActors, "Slot3 ID:", 510, 1868, 120, 20);
    g_ui.invS3Id = MakeEdit(g_ui.pageActors, "", 640, 1866, 140, 24, ID_EDIT_INV_S3_ID);
    g_ui.invS3LoadedLabel = MakeLabel(g_ui.pageActors, "Slot3 Ammo L:", 510, 1900, 120, 20);
    g_ui.invS3Loaded = MakeEdit(g_ui.pageActors, "", 640, 1898, 140, 24, ID_EDIT_INV_S3_LOADED);
    g_ui.invS3HiddenLabel = MakeLabel(g_ui.pageActors, "Slot3 Ammo H:", 510, 1932, 120, 20);
    g_ui.invS3Hidden = MakeEdit(g_ui.pageActors, "", 640, 1930, 140, 24, ID_EDIT_INV_S3_HIDDEN);
    g_ui.invS3UnkLabel = MakeLabel(g_ui.pageActors, "Slot3 Unk:", 510, 1964, 120, 20);
    g_ui.invS3Unk = MakeEdit(g_ui.pageActors, "", 640, 1962, 140, 24, ID_EDIT_INV_S3_UNK);
    g_ui.invS4IdLabel = MakeLabel(g_ui.pageActors, "Slot4 ID:", 510, 1996, 120, 20);
    g_ui.invS4Id = MakeEdit(g_ui.pageActors, "", 640, 1994, 140, 24, ID_EDIT_INV_S4_ID);
    g_ui.invS4LoadedLabel = MakeLabel(g_ui.pageActors, "Slot4 Ammo L:", 510, 2028, 120, 20);
    g_ui.invS4Loaded = MakeEdit(g_ui.pageActors, "", 640, 2026, 140, 24, ID_EDIT_INV_S4_LOADED);
    g_ui.invS4HiddenLabel = MakeLabel(g_ui.pageActors, "Slot4 Ammo H:", 510, 2060, 120, 20);
    g_ui.invS4Hidden = MakeEdit(g_ui.pageActors, "", 640, 2058, 140, 24, ID_EDIT_INV_S4_HIDDEN);
    g_ui.invS4UnkLabel = MakeLabel(g_ui.pageActors, "Slot4 Unk:", 510, 2092, 120, 20);
    g_ui.invS4Unk = MakeEdit(g_ui.pageActors, "", 640, 2090, 140, 24, ID_EDIT_INV_S4_UNK);
    g_ui.invS5IdLabel = MakeLabel(g_ui.pageActors, "Slot5 ID:", 510, 2124, 120, 20);
    g_ui.invS5Id = MakeEdit(g_ui.pageActors, "", 640, 2122, 140, 24, ID_EDIT_INV_S5_ID);
    g_ui.invS5LoadedLabel = MakeLabel(g_ui.pageActors, "Slot5 Ammo L:", 510, 2156, 120, 20);
    g_ui.invS5Loaded = MakeEdit(g_ui.pageActors, "", 640, 2154, 140, 24, ID_EDIT_INV_S5_LOADED);
    g_ui.invS5HiddenLabel = MakeLabel(g_ui.pageActors, "Slot5 Ammo H:", 510, 2188, 120, 20);
    g_ui.invS5Hidden = MakeEdit(g_ui.pageActors, "", 640, 2186, 140, 24, ID_EDIT_INV_S5_HIDDEN);
    g_ui.invS5UnkLabel = MakeLabel(g_ui.pageActors, "Slot5 Unk:", 510, 2220, 120, 20);
    g_ui.invS5Unk = MakeEdit(g_ui.pageActors, "", 640, 2218, 140, 24, ID_EDIT_INV_S5_UNK);
    g_ui.coordHint = MakeLabel(g_ui.pageActors, "Payload: -", 510, 2252, 390, 20, ID_STATIC_COORD_HINT);
    g_ui.applyActor = MakeButton(g_ui.pageActors, "Apply Actor", 790, 202, 110, 28, ID_BTN_APPLY_ACTOR);
    g_ui.cloneActor = MakeButton(g_ui.pageActors, "Clone Actor", 670, 202, 110, 28, ID_BTN_CLONE_ACTOR);

    ShowTab(0);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_CREATE: {
        g_font = static_cast<HFONT>(GetStockObject(DEFAULT_GUI_FONT));
        g_bgBrush = CreateSolidBrush(GetSysColor(COLOR_BTNFACE));

        g_ui.fileLabel = MakeLabel(hwnd, "File:", 16, 14, 40, 20);
        g_ui.path = MakeLabel(hwnd, "No file loaded", 60, 14, 760, 20, ID_STATIC_PATH);
        g_ui.openBtn = MakeButton(hwnd, "Open...", 840, 10, 110, 28, ID_BTN_OPEN);

        g_ui.info = MakeLabel(hwnd, "Mission: -", 16, 42, 940, 20, ID_STATIC_INFO);
        g_ui.warning = MakeLabel(hwnd, "", 16, 60, 940, 20, ID_STATIC_WARNING);

        g_ui.tab = CreateWindowExA(0, WC_TABCONTROLA, "", WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS, 16, 86, 940, 302,
                                   hwnd, reinterpret_cast<HMENU>(static_cast<INT_PTR>(ID_TAB)), nullptr, nullptr);

        TCITEMA item = {};
        item.mask = TCIF_TEXT;
        item.pszText = const_cast<char*>("Main");
        TabCtrl_InsertItem(g_ui.tab, 0, &item);
        item.pszText = const_cast<char*>("Actors");
        TabCtrl_InsertItem(g_ui.tab, 1, &item);
        item.pszText = const_cast<char*>("Cars");
        TabCtrl_InsertItem(g_ui.tab, 2, &item);
        item.pszText = const_cast<char*>("Garage");
        TabCtrl_InsertItem(g_ui.tab, 3, &item);
        item.pszText = const_cast<char*>("Mission/Script");
        TabCtrl_InsertItem(g_ui.tab, 4, &item);

        CreatePages(hwnd);

        g_ui.resetBtn = MakeButton(hwnd, "Reset Form", 710, 396, 110, 30, ID_BTN_RESET);
        g_ui.saveBtn = MakeButton(hwnd, "Save As...", 840, 396, 110, 30, ID_BTN_SAVE);
        g_ui.status = MakeLabel(hwnd, "Ready", 16, 402, 680, 20, ID_STATIC_STATUS);

        EnumChildWindows(hwnd, FontCb, 0);
        LayoutWindow(hwnd);
        FillAll();
        return 0;
    }
    case WM_COMMAND: {
        const int id = LOWORD(wParam);
        const int code = HIWORD(wParam);

        if (id == ID_BTN_OPEN && code == BN_CLICKED) {
            const auto path = ChooseFile(hwnd, false, "");
            if (path.has_value()) {
                LoadFile(hwnd, *path);
            }
            return 0;
        }
        if (id == ID_BTN_RESET && code == BN_CLICKED) {
            FillAll();
            SetStatus("Form reset to loaded values");
            return 0;
        }
        if (id == ID_LIST_ACTORS && code == LBN_SELCHANGE) {
            const LRESULT sel = SendMessageA(g_ui.actors, LB_GETCURSEL, 0, 0);
            g_state.selectedActor = (sel == LB_ERR) ? -1 : static_cast<int>(sel);
            FillActorEditor();
            return 0;
        }
        if (id == ID_LIST_CARS && code == LBN_SELCHANGE) {
            const LRESULT sel = SendMessageA(g_ui.carsList, LB_GETCURSEL, 0, 0);
            g_state.selectedCar = (sel == LB_ERR) ? -1 : static_cast<int>(sel);
            FillCarEditor();
            return 0;
        }
        if (id == ID_LIST_GARAGE && code == LBN_SELCHANGE) {
            const LRESULT sel = SendMessageA(g_ui.garageList, LB_GETCURSEL, 0, 0);
            g_state.selectedGarageSlot = (sel == LB_ERR) ? -1 : static_cast<int>(sel);
            FillGarageEditor();
            return 0;
        }
        if (id == ID_COMBO_GARAGE_A_CAR && code == CBN_SELCHANGE) {
            UpdateGarageRawFromCombo(g_ui.garageACar, g_ui.garageA);
            RefreshGaragePreviewFromFields();
            return 0;
        }
        if (id == ID_COMBO_GARAGE_B_CAR && code == CBN_SELCHANGE) {
            UpdateGarageRawFromCombo(g_ui.garageBCar, g_ui.garageB);
            RefreshGaragePreviewFromFields();
            return 0;
        }
        if ((id == ID_EDIT_GARAGE_A || id == ID_EDIT_GARAGE_B || id == ID_EDIT_GARAGE_A_FLAGS ||
             id == ID_EDIT_GARAGE_B_FLAGS || id == ID_EDIT_GARAGE_A_COLOR || id == ID_EDIT_GARAGE_B_COLOR) &&
            code == EN_CHANGE) {
            RefreshGaragePreviewFromFields();
            return 0;
        }
        if (id == ID_BTN_FILTER_APPLY && code == BN_CLICKED) {
            std::string err;
            if (!ApplyFilterFromUi(&err)) {
                Error(hwnd, "Filter error: " + err);
                return 0;
            }
            FillActorList();
            RefreshInfo();
            SetStatus("Filter applied");
            return 0;
        }
        if (id == ID_BTN_FILTER_CLEAR && code == BN_CLICKED) {
            g_state.filterName.clear();
            g_state.filterType.reset();
            SetText(g_ui.filterName, "");
            SetText(g_ui.filterType, "");
            RebuildFilteredActors();
            FillActorList();
            RefreshInfo();
            SetStatus("Filter cleared");
            return 0;
        }
        if (id == ID_BTN_APPLY_ACTOR && code == BN_CLICKED) {
            std::string err;
            if (!ApplyActorEdits(&err)) {
                Error(hwnd, "Actor apply failed: " + err);
                return 0;
            }
            RebuildActorIndex();
            RebuildFilteredActors();
            RebuildCarIndex();
            FillActorList();
            FillCarsList();
            RefreshInfo();
            RefreshWarning();
            SetStatus("Actor fields applied");
            return 0;
        }
        if (id == ID_BTN_APPLY_CAR && code == BN_CLICKED) {
            std::string err;
            if (!ApplyCarEdits(&err)) {
                Error(hwnd, "Car apply failed: " + err);
                return 0;
            }
            RebuildActorIndex();
            RebuildFilteredActors();
            RebuildCarIndex();
            FillActorList();
            FillCarsList();
            RefreshInfo();
            RefreshWarning();
            SetStatus("Car fields applied");
            return 0;
        }
        if (id == ID_BTN_APPLY_GARAGE && code == BN_CLICKED) {
            std::string err;
            if (!ApplyGarageEdits(&err)) {
                Error(hwnd, "Garage apply failed: " + err);
                return 0;
            }
            FillGarageList();
            SetStatus("Garage slot applied");
            return 0;
        }
        if (id == ID_BTN_CLEAR_GARAGE && code == BN_CLICKED) {
            if (!HasGarageInfoData() || g_state.selectedGarageSlot < 0 ||
                g_state.selectedGarageSlot >= static_cast<int>(kGarageSlotCount)) {
                return 0;
            }
            SetText(g_ui.garageA, "0");
            SetText(g_ui.garageB, "0");
            SetText(g_ui.garageAColor, "0");
            SetText(g_ui.garageBColor, "0");
            SetText(g_ui.garageAFlags, "0");
            SetText(g_ui.garageBFlags, "0");
            std::string err;
            if (!ApplyGarageEdits(&err)) {
                Error(hwnd, "Garage clear failed: " + err);
                return 0;
            }
            FillGarageList();
            SetStatus("Garage slot cleared");
            return 0;
        }
        if (id == ID_BTN_GARAGE_SYNC_B && code == BN_CLICKED) {
            SetText(g_ui.garageB, GetText(g_ui.garageA));
            SetText(g_ui.garageBColor, GetText(g_ui.garageAColor));
            SetText(g_ui.garageBFlags, GetText(g_ui.garageAFlags));
            const LRESULT selA = SendMessageA(g_ui.garageACar, CB_GETCURSEL, 0, 0);
            if (selA != CB_ERR) {
                SendMessageA(g_ui.garageBCar, CB_SETCURSEL, static_cast<WPARAM>(selA), 0);
            }
            RefreshGaragePreviewFromFields();
            SetStatus("Garage: copied A to B (not saved yet)");
            return 0;
        }
        if (id == ID_BTN_CLONE_ACTOR && code == BN_CLICKED) {
            std::string err;
            if (!CloneSelectedActor(&err)) {
                Error(hwnd, "Clone failed: " + err);
                return 0;
            }
            RebuildActorIndex();
            RebuildFilteredActors();
            RebuildCarIndex();
            FillActorList();
            FillCarsList();
            RefreshInfo();
            RefreshWarning();
            SetStatus("Actor cloned");
            return 0;
        }
        if (id == ID_BTN_PROG_LOAD_VAR && code == BN_CLICKED) {
            std::string err;
            if (!LoadProgramVarIntoUi(&err)) {
                Error(hwnd, "Load var failed: " + err);
                return 0;
            }
            SetStatus("Script variable loaded");
            return 0;
        }
        if (id == ID_BTN_PROG_RELOAD_TABLE && code == BN_CLICKED) {
            FillMission();
            SetStatus("Script vars table reloaded");
            return 0;
        }
        if (id == ID_LIST_PROG_VARS && code == LBN_SELCHANGE) {
            const LRESULT sel = SendMessageA(g_ui.progVarsTable, LB_GETCURSEL, 0, 0);
            if (sel == LB_ERR) {
                return 0;
            }
            const LRESULT itemData = SendMessageA(g_ui.progVarsTable, LB_GETITEMDATA, static_cast<WPARAM>(sel), 0);
            if (itemData == LB_ERR) {
                return 0;
            }
            SetText(g_ui.progVarIndex, std::to_string(static_cast<std::uint32_t>(itemData)));
            std::string err;
            if (!LoadProgramVarIntoUi(&err)) {
                Error(hwnd, "Load var failed: " + err);
                return 0;
            }
            SetStatus("Script var selected from table");
            return 0;
        }
        if (id == ID_BTN_SAVE && code == BN_CLICKED) {
            if (!g_state.loaded) {
                Error(hwnd, "Load a save file first");
                return 0;
            }

            std::string err;
            if (!ApplyActorEdits(&err)) {
                Error(hwnd, "Actor fields invalid: " + err);
                return 0;
            }
            if (!ApplyCarEdits(&err)) {
                Error(hwnd, "Car fields invalid: " + err);
                return 0;
            }
            if (!ApplyGarageEdits(&err)) {
                Error(hwnd, "Garage fields invalid: " + err);
                return 0;
            }

            mafia_save::SaveData edited;
            if (!BuildEditedSave(&edited, &err)) {
                Error(hwnd, "Main fields invalid: " + err);
                return 0;
            }

            const auto outPath = ChooseFile(hwnd, true, g_state.inputPath.filename().string());
            if (!outPath.has_value()) {
                return 0;
            }

            std::vector<std::uint8_t> outRaw;
            if (!mafia_save::BuildRaw(edited, &outRaw, &err)) {
                Error(hwnd, "BuildRaw failed: " + err);
                return 0;
            }
            if (!mafia_save::WriteFileBytes(*outPath, outRaw)) {
                Error(hwnd, "Failed to write output file");
                return 0;
            }

            g_state.save = std::move(edited);
            g_state.raw = outRaw;
            g_state.inputPath = *outPath;
            RebuildActorIndex();
            RebuildFilteredActors();
            RebuildCarIndex();
            FillAll();
            SetStatus("Saved: " + outPath->string());
            return 0;
        }
        return 0;
    }
    case WM_VSCROLL: {
        const HWND src = reinterpret_cast<HWND>(lParam);
        if (src != g_ui.actorsScroll && src != g_ui.pageActors) {
            return 0;
        }
        SCROLLINFO si = {};
        si.cbSize = sizeof(si);
        si.fMask = SIF_ALL;
        GetScrollInfo(g_ui.actorsScroll, SB_CTL, &si);

        int pos = g_state.actorsRightScroll;
        switch (LOWORD(wParam)) {
        case SB_TOP:
            pos = 0;
            break;
        case SB_BOTTOM:
            pos = g_state.actorsRightScrollMax;
            break;
        case SB_LINEUP:
            pos -= 20;
            break;
        case SB_LINEDOWN:
            pos += 20;
            break;
        case SB_PAGEUP:
            pos -= static_cast<int>(si.nPage);
            break;
        case SB_PAGEDOWN:
            pos += static_cast<int>(si.nPage);
            break;
        case SB_THUMBPOSITION:
        case SB_THUMBTRACK:
            pos = si.nTrackPos;
            break;
        default:
            break;
        }
        pos = std::clamp(pos, 0, g_state.actorsRightScrollMax);
        if (pos != g_state.actorsRightScroll) {
            g_state.actorsRightScroll = pos;
            LayoutActorsPage();
        }
        return 0;
    }
    case WM_MOUSEWHEEL:
        if (g_ui.pageActors != nullptr && IsWindowVisible(g_ui.pageActors) && g_state.actorsRightScrollMax > 0) {
            const int delta = GET_WHEEL_DELTA_WPARAM(wParam);
            const int step = 32;
            const int next = g_state.actorsRightScroll - ((delta / WHEEL_DELTA) * step);
            const int clamped = std::clamp(next, 0, g_state.actorsRightScrollMax);
            if (clamped != g_state.actorsRightScroll) {
                g_state.actorsRightScroll = clamped;
                LayoutActorsPage();
            }
            return 0;
        }
        return DefWindowProcA(hwnd, msg, wParam, lParam);
    case WM_GETMINMAXINFO: {
        auto* mmi = reinterpret_cast<MINMAXINFO*>(lParam);
        mmi->ptMinTrackSize.x = 940;
        mmi->ptMinTrackSize.y = 560;
        return 0;
    }
    case WM_SIZE:
        if (wParam != SIZE_MINIMIZED) {
            LayoutWindow(hwnd);
        }
        return 0;
    case WM_NOTIFY: {
        const auto* hdr = reinterpret_cast<const NMHDR*>(lParam);
        if (hdr != nullptr && hdr->idFrom == static_cast<UINT_PTR>(ID_TAB) && hdr->code == TCN_SELCHANGE) {
            ShowTab(TabCtrl_GetCurSel(g_ui.tab));
            return 0;
        }
        return 0;
    }
    case WM_CTLCOLORSTATIC: {
        HDC hdc = reinterpret_cast<HDC>(wParam);
        HWND ctrl = reinterpret_cast<HWND>(lParam);
        if (ctrl == g_ui.warning) {
            SetTextColor(hdc, RGB(190, 20, 20));
            SetBkMode(hdc, TRANSPARENT);
            return reinterpret_cast<LRESULT>(g_bgBrush != nullptr ? g_bgBrush : GetSysColorBrush(COLOR_BTNFACE));
        }
        return DefWindowProcA(hwnd, msg, wParam, lParam);
    }
    case WM_DESTROY:
        if (g_bgBrush != nullptr) {
            DeleteObject(g_bgBrush);
            g_bgBrush = nullptr;
        }
        PostQuitMessage(0);
        return 0;
    default:
        return DefWindowProcA(hwnd, msg, wParam, lParam);
    }
}

}  // namespace

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow) {
    INITCOMMONCONTROLSEX icc = {};
    icc.dwSize = sizeof(icc);
    icc.dwICC = ICC_TAB_CLASSES;
    InitCommonControlsEx(&icc);

    const char* cls = "MafiaSaveEditorWnd";
    WNDCLASSA wc = {};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_BTNFACE + 1);
    wc.lpszClassName = cls;
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);

    if (!RegisterClassA(&wc)) {
        MessageBoxA(nullptr, "Failed to register window class", "Mafia Save Editor", MB_OK | MB_ICONERROR);
        return 1;
    }

    HWND hwnd = CreateWindowExA(0, cls, "Mafia Save Editor (Mafia 1 2002)",
                                WS_OVERLAPPEDWINDOW,
                                CW_USEDEFAULT, CW_USEDEFAULT, 1120, 620,
                                nullptr, nullptr, hInstance, nullptr);
    if (hwnd == nullptr) {
        MessageBoxA(nullptr, "Failed to create main window", "Mafia Save Editor", MB_OK | MB_ICONERROR);
        return 1;
    }

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    MSG msg = {};
    while (GetMessageA(&msg, nullptr, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessageA(&msg);
    }
    return static_cast<int>(msg.wParam);
}
