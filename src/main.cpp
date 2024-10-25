#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <filesystem>
#include <fstream>
#include <unordered_map>
#include <vector>

#include "dr_opt.h"

namespace fs = std::filesystem;
using StringArray = std::vector<std::string>;
using TokenArray = std::vector<StringArray>;

static const std::string OUTPUT_OPTION_FLAG = "o";
static const std::string SET_ADDRESS_SYMBOL = "@";
static const char SPACE_DELIMITER = ' ';
static const char TAB_DELIMITER = '\t';
static const std::string DEFAULT_OUTPUT_FILENAME = "out.160";
static const std::string DEFAULT_ENTRY_ADDRESS = "0100";
static const std::string COMMENT_STRING = "//";

// Stop Instructions
static const std::string OP_ERR = "ERR";
static const std::string OP_HLT = "HLT";
static const std::string OP_NOP = "NOP";

// Data Transmission Instructions
static const std::string OP_BLS = "BLS";
static const std::string OP_PTA = "PTA";
static const std::string OP_ATE = "ATE";
static const std::string OP_ATX = "ATX";
static const std::string OP_ETA = "ETA";
static const std::string OP_CTA = "CTA";
static const std::string OP_STP = "STP";
static const std::string OP_LDN = "LDN";
static const std::string OP_LDD = "LDD";
static const std::string OP_LDM = "LDM";
static const std::string OP_LDI = "LDI";
static const std::string OP_LDC = "LDC";
static const std::string OP_LDF = "LDF";
static const std::string OP_LDS = "LDS";
static const std::string OP_LDB = "LDB";
static const std::string OP_STE = "STE";
static const std::string OP_LCN = "LCN";
static const std::string OP_LCD = "LCD";
static const std::string OP_LCM = "LCM";
static const std::string OP_LCI = "LCI";
static const std::string OP_LCC = "LCC";
static const std::string OP_LCF = "LCF";
static const std::string OP_LCS = "LCS";
static const std::string OP_LCB = "LCB";
static const std::string OP_STD = "STD";
static const std::string OP_STM = "STM";
static const std::string OP_STI = "STI";
static const std::string OP_STC = "STC";
static const std::string OP_STF = "STF";
static const std::string OP_STS = "STS";
static const std::string OP_STB = "STB";
static const std::string OP_HWI = "HWI";

// Arithmetic Instructions
static const std::string OP_MUT = "MUT";
static const std::string OP_MUH = "MUH";
static const std::string OP_ADN = "ADN";
static const std::string OP_ADD = "ADD";
static const std::string OP_ADM = "ADM";
static const std::string OP_ADI = "ADI";
static const std::string OP_ADC = "ADC";
static const std::string OP_ADF = "ADF";
static const std::string OP_ADS = "ADS";
static const std::string OP_ADB = "ADB";
static const std::string OP_SBN = "SBN";
static const std::string OP_SBD = "SBD";
static const std::string OP_SBM = "SBM";
static const std::string OP_SBI = "SBI";
static const std::string OP_SBC = "SBC";
static const std::string OP_SBF = "SBF";
static const std::string OP_SBS = "SBS";
static const std::string OP_SBB = "SBB";
static const std::string OP_RAD = "RAD";
static const std::string OP_RAM = "RAM";
static const std::string OP_RAI = "RAI";
static const std::string OP_RAC = "RAC";
static const std::string OP_RAF = "RAF";
static const std::string OP_RAS = "RAS";
static const std::string OP_RAB = "RAB";
static const std::string OP_AOD = "AOD";
static const std::string OP_AOM = "AOM";
static const std::string OP_AOI = "AOI";
static const std::string OP_AOC = "AOC";
static const std::string OP_AOF = "AOF";
static const std::string OP_AOS = "AOS";
static const std::string OP_AOB = "AOB";

// Shift Instructions
static const std::string OP_LS1 = "LS1";
static const std::string OP_LS2 = "LS2";
static const std::string OP_LS3 = "LS3";
static const std::string OP_LS6 = "LS6";
static const std::string OP_RS1 = "RS1";
static const std::string OP_RS2 = "RS2";
static const std::string OP_SRD = "SRD";
static const std::string OP_SRM = "SRM";
static const std::string OP_SRI = "SRI";
static const std::string OP_SRC = "SRC";
static const std::string OP_SRF = "SRF";
static const std::string OP_SRS = "SRS";
static const std::string OP_SRB = "SRB";

// Logical Instructions
static const std::string OP_LPN = "LPN";
static const std::string OP_LPD = "LPD";
static const std::string OP_LPM = "LPM";
static const std::string OP_LPI = "LPI";
static const std::string OP_LPC = "LPC";
static const std::string OP_LPF = "LPF";
static const std::string OP_LPS = "LPS";
static const std::string OP_LPB = "LPB";
static const std::string OP_SCN = "SCN";
static const std::string OP_SCD = "SCD";
static const std::string OP_SCM = "SCM";
static const std::string OP_SCI = "SCI";
static const std::string OP_SCC = "SCC";
static const std::string OP_SCF = "SCF";
static const std::string OP_SCS = "SCS";
static const std::string OP_SCB = "SCB";

// Storage Bank Control Instructions
static const std::string OP_SRJ = "SRJ";
static const std::string OP_SIC = "SIC";
static const std::string OP_IRJ = "IRJ";
static const std::string OP_SDC = "SDC";
static const std::string OP_DRJ = "DRJ";
static const std::string OP_SID = "SID";
static const std::string OP_ACJ = "ACJ";
static const std::string OP_SBU = "SBU";

// Jump Instructions
static const std::string OP_ZJF = "ZJF";
static const std::string OP_NZF = "NZF";
static const std::string OP_PJF = "PJF";
static const std::string OP_NJF = "NJF";
static const std::string OP_ZJB = "ZJB";
static const std::string OP_NZB = "NZB";
static const std::string OP_PJB = "PJB";
static const std::string OP_NJB = "NJB";
static const std::string OP_JPI = "JPI";
static const std::string OP_JPR = "JPR";
static const std::string OP_JFI = "JFI";

// Input/Output Instructions
static const std::string OP_CBC = "CBC";
static const std::string OP_CIL = "CIL";
static const std::string OP_IBI = "IBI";
static const std::string OP_IBO = "IBO";
static const std::string OP_INP = "INP";
static const std::string OP_OUT = "OUT";
static const std::string OP_OTN = "OTN";
static const std::string OP_INA = "INA";
static const std::string OP_OTA = "OTA";
static const std::string OP_EXC = "EXC";
static const std::string OP_EXF = "EXF";

// Selective Stop and Jump Instructions
static const std::string OP_SLS = "SLS";
static const std::string OP_SLJ = "SLJ";
static const std::string OP_SJS = "SJS";

// Stop Instructions
static const std::string OPCODE_ERR = "0000";
static const std::string OPCODE_HLT = "7700";
static const std::string OPCODE_NOP = "000";

// Data Transmission Instructions
static const std::string OPCODE_BLS = "0100";
static const std::string OPCODE_PTA = "0101";
static const std::string OPCODE_ATE = "0105";
static const std::string OPCODE_ATX = "0106";
static const std::string OPCODE_ETA = "0107";
static const std::string OPCODE_CTA = "0130";
static const std::string OPCODE_STP = "015";
static const std::string OPCODE_LDN = "04";
static const std::string OPCODE_LDD = "20";
static const std::string OPCODE_LDM = "2100";
static const std::string OPCODE_LDI = "21";
static const std::string OPCODE_LDC = "2200";
static const std::string OPCODE_LDF = "22";
static const std::string OPCODE_LDS = "2300";
static const std::string OPCODE_LDB = "23";
static const std::string OPCODE_STE = "016";
static const std::string OPCODE_LCN = "05";
static const std::string OPCODE_LCD = "24";
static const std::string OPCODE_LCM = "2500";
static const std::string OPCODE_LCI = "25";
static const std::string OPCODE_LCC = "2600";
static const std::string OPCODE_LCF = "26";
static const std::string OPCODE_LCS = "2700";
static const std::string OPCODE_LCB = "27";
static const std::string OPCODE_STD = "40";
static const std::string OPCODE_STM = "4100";
static const std::string OPCODE_STI = "41";
static const std::string OPCODE_STC = "4200";
static const std::string OPCODE_STF = "42";
static const std::string OPCODE_STS = "4300";
static const std::string OPCODE_STB = "43";
static const std::string OPCODE_HWI = "76";

// Arithmetic Instructions
static const std::string OPCODE_MUT = "0112";
static const std::string OPCODE_MUH = "0113";
static const std::string OPCODE_ADN = "06";
static const std::string OPCODE_ADD = "30";
static const std::string OPCODE_ADM = "3100";
static const std::string OPCODE_ADI = "31";
static const std::string OPCODE_ADC = "3200";
static const std::string OPCODE_ADF = "32";
static const std::string OPCODE_ADS = "3300";
static const std::string OPCODE_ADB = "33";
static const std::string OPCODE_SBN = "07";
static const std::string OPCODE_SBD = "34";
static const std::string OPCODE_SBM = "3500";
static const std::string OPCODE_SBI = "35";
static const std::string OPCODE_SBC = "3600";
static const std::string OPCODE_SBF = "36";
static const std::string OPCODE_SBS = "3700";
static const std::string OPCODE_SBB = "37";
static const std::string OPCODE_RAD = "50";
static const std::string OPCODE_RAM = "5100";
static const std::string OPCODE_RAI = "51";
static const std::string OPCODE_RAC = "5200";
static const std::string OPCODE_RAF = "52";
static const std::string OPCODE_RAS = "5300";
static const std::string OPCODE_RAB = "53";
static const std::string OPCODE_AOD = "54";
static const std::string OPCODE_AOM = "5500";
static const std::string OPCODE_AOI = "55";
static const std::string OPCODE_AOC = "5600";
static const std::string OPCODE_AOF = "56";
static const std::string OPCODE_AOS = "5700";
static const std::string OPCODE_AOB = "57";

// Shift Instructions
static const std::string OPCODE_LS1 = "0102";
static const std::string OPCODE_LS2 = "0103";
static const std::string OPCODE_LS3 = "0110";
static const std::string OPCODE_LS6 = "0111";
static const std::string OPCODE_RS1 = "0114";
static const std::string OPCODE_RS2 = "0115";
static const std::string OPCODE_SRD = "44";
static const std::string OPCODE_SRM = "4500";
static const std::string OPCODE_SRI = "45";
static const std::string OPCODE_SRC = "4600";
static const std::string OPCODE_SRF = "46";
static const std::string OPCODE_SRS = "4700";
static const std::string OPCODE_SRB = "47";

// Logical Instructions
static const std::string OPCODE_LPN = "02";
static const std::string OPCODE_LPD = "10";
static const std::string OPCODE_LPM = "1100";
static const std::string OPCODE_LPI = "11";
static const std::string OPCODE_LPC = "1200";
static const std::string OPCODE_LPF = "12";
static const std::string OPCODE_LPS = "1300";
static const std::string OPCODE_LPB = "13";
static const std::string OPCODE_SCN = "03";
static const std::string OPCODE_SCD = "14";
static const std::string OPCODE_SCM = "1500";
static const std::string OPCODE_SCI = "15";
static const std::string OPCODE_SCC = "1600";
static const std::string OPCODE_SCF = "16";
static const std::string OPCODE_SCS = "1700";
static const std::string OPCODE_SCB = "17";

// Storage Bank Control Instructions
static const std::string OPCODE_SRJ = "001";
static const std::string OPCODE_SIC = "002";
static const std::string OPCODE_IRJ = "003";
static const std::string OPCODE_SDC = "004";
static const std::string OPCODE_DRJ = "005";
static const std::string OPCODE_SID = "006";
static const std::string OPCODE_ACJ = "007";
static const std::string OPCODE_SBU = "014";

// Jump Instructions
static const std::string OPCODE_ZJF = "60";
static const std::string OPCODE_NZF = "61";
static const std::string OPCODE_PJF = "62";
static const std::string OPCODE_NJF = "63";
static const std::string OPCODE_ZJB = "64";
static const std::string OPCODE_NZB = "65";
static const std::string OPCODE_PJB = "66";
static const std::string OPCODE_NJB = "67";
static const std::string OPCODE_JPI = "70";
static const std::string OPCODE_JPR = "7100";
static const std::string OPCODE_JFI = "71";

// Input/Output Instructions
static const std::string OPCODE_CBC = "0104";
static const std::string OPCODE_CIL = "0120";
static const std::string OPCODE_IBI = "7200";
static const std::string OPCODE_IBO = "7300";
static const std::string OPCODE_INP = "72";
static const std::string OPCODE_OUT = "73";
static const std::string OPCODE_OTN = "74";
static const std::string OPCODE_INA = "7600";
static const std::string OPCODE_OTA = "7677";
static const std::string OPCODE_EXC = "7500";
static const std::string OPCODE_EXF = "75";

// Selective Stop and Jump Instructions
static const std::string OPCODE_SLS = "770";
static const std::string OPCODE_SLJ = "77";
static const std::string OPCODE_SJS = "77";

static const std::unordered_map<std::string, std::string> OPCODE_MAPPING = {
    {OP_ERR, OPCODE_ERR},
    {OP_HLT, OPCODE_HLT},
    {OP_NOP, OPCODE_NOP},
    {OP_BLS, OPCODE_BLS},
    {OP_PTA, OPCODE_PTA},
    {OP_ATE, OPCODE_ATE},
    {OP_ATX, OPCODE_ATX},
    {OP_ETA, OPCODE_ETA},
    {OP_CTA, OPCODE_CTA},
    {OP_STP, OPCODE_STP},
    {OP_LDN, OPCODE_LDN},
    {OP_LDD, OPCODE_LDD},
    {OP_LDM, OPCODE_LDM},
    {OP_LDI, OPCODE_LDI},
    {OP_LDC, OPCODE_LDC},
    {OP_LDF, OPCODE_LDF},
    {OP_LDS, OPCODE_LDS},
    {OP_LDB, OPCODE_LDB},
    {OP_STE, OPCODE_STE},
    {OP_LCN, OPCODE_LCN},
    {OP_LCD, OPCODE_LCD},
    {OP_LCM, OPCODE_LCM},
    {OP_LCI, OPCODE_LCI},
    {OP_LCC, OPCODE_LCC},
    {OP_LCF, OPCODE_LCF},
    {OP_LCS, OPCODE_LCS},
    {OP_LCB, OPCODE_LCB},
    {OP_STD, OPCODE_STD},
    {OP_STM, OPCODE_STM},
    {OP_STI, OPCODE_STI},
    {OP_STC, OPCODE_STC},
    {OP_STF, OPCODE_STF},
    {OP_STS, OPCODE_STS},
    {OP_STB, OPCODE_STB},
    {OP_HWI, OPCODE_HWI},
    {OP_MUT, OPCODE_MUT},
    {OP_MUH, OPCODE_MUH},
    {OP_ADN, OPCODE_ADN},
    {OP_ADD, OPCODE_ADD},
    {OP_ADM, OPCODE_ADM},
    {OP_ADI, OPCODE_ADI},
    {OP_ADC, OPCODE_ADC},
    {OP_ADF, OPCODE_ADF},
    {OP_ADS, OPCODE_ADS},
    {OP_ADB, OPCODE_ADB},
    {OP_SBN, OPCODE_SBN},
    {OP_SBD, OPCODE_SBD},
    {OP_SBM, OPCODE_SBM},
    {OP_SBI, OPCODE_SBI},
    {OP_SBC, OPCODE_SBC},
    {OP_SBF, OPCODE_SBF},
    {OP_SBS, OPCODE_SBS},
    {OP_SBB, OPCODE_SBB},
    {OP_RAD, OPCODE_RAD},
    {OP_RAM, OPCODE_RAM},
    {OP_RAI, OPCODE_RAI},
    {OP_RAC, OPCODE_RAC},
    {OP_RAF, OPCODE_RAF},
    {OP_RAS, OPCODE_RAS},
    {OP_RAB, OPCODE_RAB},
    {OP_AOD, OPCODE_AOD},
    {OP_AOM, OPCODE_AOM},
    {OP_AOI, OPCODE_AOI},
    {OP_AOC, OPCODE_AOC},
    {OP_AOF, OPCODE_AOF},
    {OP_AOS, OPCODE_AOS},
    {OP_AOB, OPCODE_AOB},
    {OP_LS1, OPCODE_LS1},
    {OP_LS2, OPCODE_LS2},
    {OP_LS3, OPCODE_LS3},
    {OP_LS6, OPCODE_LS6},
    {OP_RS1, OPCODE_RS1},
    {OP_RS2, OPCODE_RS2},
    {OP_SRD, OPCODE_SRD},
    {OP_SRM, OPCODE_SRM},
    {OP_SRI, OPCODE_SRI},
    {OP_SRC, OPCODE_SRC},
    {OP_SRF, OPCODE_SRF},
    {OP_SRS, OPCODE_SRS},
    {OP_SRB, OPCODE_SRB},
    {OP_LPN, OPCODE_LPN},
    {OP_LPD, OPCODE_LPD},
    {OP_LPM, OPCODE_LPM},
    {OP_LPI, OPCODE_LPI},
    {OP_LPC, OPCODE_LPC},
    {OP_LPF, OPCODE_LPF},
    {OP_LPS, OPCODE_LPS},
    {OP_LPB, OPCODE_LPB},
    {OP_SCN, OPCODE_SCN},
    {OP_SCD, OPCODE_SCD},
    {OP_SCM, OPCODE_SCM},
    {OP_SCI, OPCODE_SCI},
    {OP_SCC, OPCODE_SCC},
    {OP_SCF, OPCODE_SCF},
    {OP_SCS, OPCODE_SCS},
    {OP_SCB, OPCODE_SCB},
    {OP_SRJ, OPCODE_SRJ},
    {OP_SIC, OPCODE_SIC},
    {OP_IRJ, OPCODE_IRJ},
    {OP_SDC, OPCODE_SDC},
    {OP_DRJ, OPCODE_DRJ},
    {OP_SID, OPCODE_SID},
    {OP_ACJ, OPCODE_ACJ},
    {OP_SBU, OPCODE_SBU},
    {OP_ZJF, OPCODE_ZJF},
    {OP_NZF, OPCODE_NZF},
    {OP_PJF, OPCODE_PJF},
    {OP_NJF, OPCODE_NJF},
    {OP_ZJB, OPCODE_ZJB},
    {OP_NZB, OPCODE_NZB},
    {OP_PJB, OPCODE_PJB},
    {OP_NJB, OPCODE_NJB},
    {OP_JPI, OPCODE_JPI},
    {OP_JPR, OPCODE_JPR},
    {OP_JFI, OPCODE_JFI},
    {OP_CBC, OPCODE_CBC},
    {OP_CIL, OPCODE_CIL},
    {OP_IBI, OPCODE_IBI},
    {OP_IBO, OPCODE_IBO},
    {OP_INP, OPCODE_INP},
    {OP_OUT, OPCODE_OUT},
    {OP_OTN, OPCODE_OTN},
    {OP_INA, OPCODE_INA},
    {OP_OTA, OPCODE_OTA},
    {OP_EXC, OPCODE_EXC},
    {OP_EXF, OPCODE_EXF},
    {OP_SLS, OPCODE_SLS},
    {OP_SLJ, OPCODE_SLJ},
    {OP_SJS, OPCODE_SJS}
};

static bool asmFail = false;

static StringArray splitStringByDelimiter(const std::string& s, char c) {
    std::string e;
    StringArray sv;
    std::stringstream ss(s);
    while(std::getline(ss, e, c)) {
        sv.push_back(e);
    }
    return sv;
}

static void printUsage() {
    std::cout << "asm160 Usage" << std::endl << std::endl
    << "\t160asm filename.asm [-o output.160]" << std::endl << std::endl
    << "\tfilename.asm - input assembler file path" << std::endl
    << "\t-o output.160 - output 160 machine code file path" << std::endl << std::endl
    << "If output file path is not specified, the output file will be in the same directory as the input file."
    << std::endl;
}

static bool validCommandSyntax() {
    int argCount = dr::getoptc();
    int saCount = dr::getstandalonec();
    if(argCount > 1 || saCount != 1)
        return false;
    return true;
}

static bool opcodeDoesNotExist(const std::string& opcode) {
    return OPCODE_MAPPING.find(opcode) == OPCODE_MAPPING.end();
}

static bool isComment(const std::string& code) {
    return code == COMMENT_STRING;
}


static bool isSingleOctalDigit(const std::string& code) {
    if(code.size() != 1) return false;
    char c = code[0];
    return c >= '0' || c <= '7';
}

static bool isTwoOctalDigits(const std::string& code) {
    if(code.size() != 2) return false;
    for( char c : code) {
        if(c < '0' || c > '7') {
            return false;
        }
    }
    return true;
}

static bool isFourOctalDigits(const std::string& code) {
    if(code.size() != 4) return false;
    for( char c : code) {
        if(c < '0' || c > '7') {
            return false;
        }
    }
    return true;
}

static bool isMachineCodeLiteral(const StringArray& instruction) {
    return (isFourOctalDigits(instruction[0]) && instruction.size() == 1)
        || (isFourOctalDigits(instruction[0]) && instruction.size() > 1 && instruction[1] == COMMENT_STRING);
}

static void printSyntaxError(const std::string& op) {
    std::cout << "Incorrect syntax for " + op + " instruction." << std::endl;
}

static bool isSyntaxGood_FFEE(const StringArray& instruction) {
    return instruction.size() == 1 || isComment(instruction.at(1));
}

static bool isSyntaxGood_FFEX(const StringArray& instruction) {
    return (instruction.size() == 2 || (instruction.size() > 2 && instruction.at(2) == "//")) && isSingleOctalDigit(instruction.at(1));
}

static bool isSyntaxGood_FFXX(const StringArray& instruction) {
    return (instruction.size() == 2 || (instruction.size() > 2 && instruction.at(2) == "//")) && isTwoOctalDigits(instruction.at(1));
}

static bool isSyntaxGood_FFEE_YYYY(const StringArray& instruction) {
    return (instruction.size() == 2 || (instruction.size() > 2 && instruction.at(2) == "//")) && isFourOctalDigits(instruction.at(1));
}

static bool isSyntaxGood_FFX0_YYYY(const StringArray& instruction) {
    return (instruction.size() == 3 || (instruction.size() > 3 && instruction.at(3) == "//")) && isSingleOctalDigit(instruction.at(1)) && isFourOctalDigits(instruction.at(2));
}

static bool isSyntaxGood_FFXX_YYYY(const StringArray& instruction) {
    return (instruction.size() == 3 || (instruction.size() > 3 && instruction.at(3) == "//")) && isTwoOctalDigits(instruction.at(1)) && isFourOctalDigits(instruction.at(2));
}

static bool isSyntaxGood_A_XXXX(const StringArray& instruction) {
    return isSyntaxGood_FFEE_YYYY(instruction);
}

static std::string instructionToMachineCode(const StringArray& instruction) {
    const std::string& op = instruction[0];
    const std::string& opcode = OPCODE_MAPPING.at(op);

    if (op == OP_ERR) {
        if(isSyntaxGood_FFEE(instruction))
            return opcode + "\n";
    } else if (op == OP_HLT) {
        if(isSyntaxGood_FFEE(instruction))
            return opcode + "\n";
    } else if (op == OP_NOP) {
        if(isSyntaxGood_FFEX(instruction))
            return opcode + instruction[1] + "\n";
        if(isSyntaxGood_FFEE(instruction))
            return opcode + "1\n";
    } else if (op == OP_BLS) {
        if(isSyntaxGood_FFEE_YYYY(instruction))
            return opcode + "\n" + instruction[1] + "\n";
    } else if (op == OP_PTA) {
        if(isSyntaxGood_FFEE(instruction))
            return opcode + "\n";
    } else if (op == OP_ATE) {
        if(isSyntaxGood_FFEE_YYYY(instruction))
            return opcode + "\n" + instruction[1] + "\n";
    } else if (op == OP_ATX) {
        if(isSyntaxGood_FFEE_YYYY(instruction))
            return opcode + "\n" + instruction[1] + "\n";
    } else if (op == OP_ETA) {
        if(isSyntaxGood_FFEE(instruction))
            return opcode + "\n";
    } else if (op == OP_CTA) {
        if(isSyntaxGood_FFEE(instruction))
            return opcode + "\n";
    } else if (op == OP_STP) {
        if(isSyntaxGood_FFEX(instruction))
            return opcode + instruction[1] + "\n";
    } else if (op == OP_LDN) {
        if(isSyntaxGood_FFXX(instruction))
            return opcode + instruction[1] + "\n";
    } else if (op == OP_LDD) {
        if(isSyntaxGood_FFXX(instruction))
            return opcode + instruction[1] + "\n";
    } else if (op == OP_LDM) {
        if(isSyntaxGood_FFEE_YYYY(instruction))
            return opcode + "\n" + instruction[1] + "\n";
    } else if (op == OP_LDI) {
        if(isSyntaxGood_FFXX(instruction))
            return opcode + instruction[1] + "\n";
    } else if (op == OP_LDC) {
        if(isSyntaxGood_FFEE_YYYY(instruction))
            return opcode + "\n" + instruction[1] + "\n";
    } else if (op == OP_LDF) {
        if(isSyntaxGood_FFXX(instruction))
            return opcode + instruction[1] + "\n";
    } else if (op == OP_LDS) {
        if(isSyntaxGood_FFEE(instruction))
            return opcode + "\n";
    } else if (op == OP_LDB) {
        if(isSyntaxGood_FFXX(instruction))
            return opcode + instruction[1] + "\n";
    } else if (op == OP_STE) {
        if(isSyntaxGood_FFEX(instruction))
            return opcode + instruction[1] + "\n";
    } else if (op == OP_LCN) {
        if(isSyntaxGood_FFXX(instruction))
            return opcode + instruction[1] + "\n";
    } else if (op == OP_LCD) {
        if(isSyntaxGood_FFXX(instruction))
            return opcode + instruction[1] + "\n";
    } else if (op == OP_LCM) {
        if(isSyntaxGood_FFEE_YYYY(instruction))
            return opcode + "\n" + instruction[1] + "\n";
    } else if (op == OP_LCI) {
        if(isSyntaxGood_FFXX(instruction))
            return opcode + instruction[1] + "\n";
    } else if (op == OP_LCC) {
        if(isSyntaxGood_FFEE_YYYY(instruction))
            return opcode + "\n" + instruction[1] + "\n";
    } else if (op == OP_LCF) {
        if(isSyntaxGood_FFXX(instruction))
            return opcode + instruction[1] + "\n";
    } else if (op == OP_LCS) {
        if(isSyntaxGood_FFEE(instruction))
            return opcode + "\n";
    } else if (op == OP_LCB) {
        if(isSyntaxGood_FFXX(instruction))
            return opcode + instruction[1] + "\n";
    } else if (op == OP_STD) {
        if(isSyntaxGood_FFXX(instruction))
            return opcode + instruction[1] + "\n";
    } else if (op == OP_STM) {
        if(isSyntaxGood_FFEE_YYYY(instruction))
            return opcode + "\n" + instruction[1] + "\n";
    } else if (op == OP_STI) {
        if(isSyntaxGood_FFXX(instruction))
            return opcode + instruction[1] + "\n";
    } else if (op == OP_STC) {
        if(isSyntaxGood_FFEE_YYYY(instruction))
            return opcode + "\n" + instruction[1] + "\n";
    } else if (op == OP_STF) {
        if(isSyntaxGood_FFXX(instruction))
            return opcode + instruction[1] + "\n";
    } else if (op == OP_STS) {
        if(isSyntaxGood_FFEE(instruction))
            return opcode + "\n";
    } else if (op == OP_STB) {
        if(isSyntaxGood_FFXX(instruction))
            return opcode + instruction[1] + "\n";
    } else if (op == OP_HWI) {
        if(isSyntaxGood_FFXX(instruction))
            return opcode + instruction[1] + "\n";
    } else if (op == OP_MUT) {
        if(isSyntaxGood_FFEE(instruction))
            return opcode + "\n";
    } else if (op == OP_MUH) {
        if(isSyntaxGood_FFEE(instruction))
            return opcode + "\n";
    } else if (op == OP_ADN) {
        if(isSyntaxGood_FFXX(instruction))
            return opcode + instruction[1] + "\n";
    } else if (op == OP_ADD) {
        if(isSyntaxGood_FFXX(instruction))
            return opcode + instruction[1] + "\n";
    } else if (op == OP_ADM) {
        if(isSyntaxGood_FFEE_YYYY(instruction))
            return opcode + "\n" + instruction[1] + "\n";
    } else if (op == OP_ADI) {
        if(isSyntaxGood_FFXX(instruction))
            return opcode + instruction[1] + "\n";
    } else if (op == OP_ADC) {
        if(isSyntaxGood_FFEE_YYYY(instruction))
            return opcode + "\n" + instruction[1] + "\n";
    } else if (op == OP_ADF) {
        if(isSyntaxGood_FFXX(instruction))
            return opcode + instruction[1] + "\n";
    } else if (op == OP_ADS) {
        if(isSyntaxGood_FFEE(instruction))
            return opcode + "\n";
    } else if (op == OP_ADB) {
        if(isSyntaxGood_FFXX(instruction))
            return opcode + instruction[1] + "\n";
    } else if (op == OP_SBN) {
        if(isSyntaxGood_FFXX(instruction))
            return opcode + instruction[1] + "\n";
    } else if (op == OP_SBD) {
        if(isSyntaxGood_FFXX(instruction))
            return opcode + instruction[1] + "\n";
    } else if (op == OP_SBM) {
        if(isSyntaxGood_FFEE_YYYY(instruction))
            return opcode + "\n" + instruction[1] + "\n";
    } else if (op == OP_SBI) {
        if(isSyntaxGood_FFXX(instruction))
            return opcode + instruction[1] + "\n";
    } else if (op == OP_SBC) {
        if(isSyntaxGood_FFEE_YYYY(instruction))
            return opcode + "\n" + instruction[1] + "\n";
    } else if (op == OP_SBF) {
        if(isSyntaxGood_FFXX(instruction))
            return opcode + instruction[1] + "\n";
    } else if (op == OP_SBS) {
        if(isSyntaxGood_FFEE(instruction))
            return opcode + "\n";
    } else if (op == OP_SBB) {
        if(isSyntaxGood_FFXX(instruction))
            return opcode + instruction[1] + "\n";
    } else if (op == OP_RAD) {
        if(isSyntaxGood_FFXX(instruction))
            return opcode + instruction[1] + "\n";
    } else if (op == OP_RAM) {
        if(isSyntaxGood_FFEE_YYYY(instruction))
            return opcode + "\n" + instruction[1] + "\n";
    } else if (op == OP_RAI) {
        if(isSyntaxGood_FFXX(instruction))
            return opcode + instruction[1] + "\n";
    } else if (op == OP_RAC) {
        if(isSyntaxGood_FFEE_YYYY(instruction))
            return opcode + "\n" + instruction[1] + "\n";
    } else if (op == OP_RAF) {
        if(isSyntaxGood_FFXX(instruction))
            return opcode + instruction[1] + "\n";
    } else if (op == OP_RAS) {
        if(isSyntaxGood_FFEE(instruction))
            return opcode + "\n";
    } else if (op == OP_RAB) {
        if(isSyntaxGood_FFXX(instruction))
            return opcode + instruction[1] + "\n";
    } else if (op == OP_AOD) {
        if(isSyntaxGood_FFXX(instruction))
            return opcode + instruction[1] + "\n";
    } else if (op == OP_AOM) {
        if(isSyntaxGood_FFEE_YYYY(instruction))
            return opcode + "\n" + instruction[1] + "\n";
    } else if (op == OP_AOI) {
        if(isSyntaxGood_FFXX(instruction))
            return opcode + instruction[1] + "\n";
    } else if (op == OP_AOC) {
        if(isSyntaxGood_FFEE_YYYY(instruction))
            return opcode + "\n" + instruction[1] + "\n";
    } else if (op == OP_AOF) {
        if(isSyntaxGood_FFXX(instruction))
            return opcode + instruction[1] + "\n";
    } else if (op == OP_AOS) {
        if(isSyntaxGood_FFEE(instruction))
            return opcode + "\n";
    } else if (op == OP_AOB) {
        if(isSyntaxGood_FFXX(instruction))
            return opcode + instruction[1] + "\n";
    } else if (op == OP_LS1) {
        if(isSyntaxGood_FFEE(instruction))
            return opcode + "\n";
    } else if (op == OP_LS2) {
        if(isSyntaxGood_FFEE(instruction))
            return opcode + "\n";
    } else if (op == OP_LS3) {
        if(isSyntaxGood_FFEE(instruction))
            return opcode + "\n";
    } else if (op == OP_LS6) {
        if(isSyntaxGood_FFEE(instruction))
            return opcode + "\n";
    } else if (op == OP_RS1) {
        if(isSyntaxGood_FFEE(instruction))
            return opcode + "\n";
    } else if (op == OP_RS2) {
        if(isSyntaxGood_FFEE(instruction))
            return opcode + "\n";
    } else if (op == OP_SRD) {
        if(isSyntaxGood_FFXX(instruction))
            return opcode + instruction[1] + "\n";
    } else if (op == OP_SRM) {
        if(isSyntaxGood_FFEE_YYYY(instruction))
            return opcode + "\n" + instruction[1] + "\n";
    } else if (op == OP_SRI) {
        if(isSyntaxGood_FFXX(instruction))
            return opcode + instruction[1] + "\n";
    } else if (op == OP_SRC) {
        if(isSyntaxGood_FFEE_YYYY(instruction))
            return opcode + "\n" + instruction[1] + "\n";
    } else if (op == OP_SRF) {
        if(isSyntaxGood_FFXX(instruction))
            return opcode + instruction[1] + "\n";
    } else if (op == OP_SRS) {
        if(isSyntaxGood_FFEE(instruction))
            return opcode + "\n";
    } else if (op == OP_SRB) {
        if(isSyntaxGood_FFXX(instruction))
            return opcode + instruction[1] + "\n";
    } else if (op == OP_LPN) {
        if(isSyntaxGood_FFXX(instruction))
            return opcode + instruction[1] + "\n";
    } else if (op == OP_LPD) {
        if(isSyntaxGood_FFXX(instruction))
            return opcode + instruction[1] + "\n";
    } else if (op == OP_LPM) {
        if(isSyntaxGood_FFEE_YYYY(instruction))
            return opcode + "\n" + instruction[1] + "\n";
    } else if (op == OP_LPI) {
        if(isSyntaxGood_FFXX(instruction))
            return opcode + instruction[1] + "\n";
    } else if (op == OP_LPC) {
        if(isSyntaxGood_FFEE_YYYY(instruction))
            return opcode + "\n" + instruction[1] + "\n";
    } else if (op == OP_LPF) {
        if(isSyntaxGood_FFXX(instruction))
            return opcode + instruction[1] + "\n";
    } else if (op == OP_LPS) {
        if(isSyntaxGood_FFEE(instruction))
            return opcode + "\n";
    } else if (op == OP_LPB) {
        if(isSyntaxGood_FFXX(instruction))
            return opcode + instruction[1] + "\n";
    } else if (op == OP_SCN) {
        if(isSyntaxGood_FFXX(instruction))
            return opcode + instruction[1] + "\n";
    } else if (op == OP_SCD) {
        if(isSyntaxGood_FFXX(instruction))
            return opcode + instruction[1] + "\n";
    } else if (op == OP_SCM) {
        if(isSyntaxGood_FFEE_YYYY(instruction))
            return opcode + "\n" + instruction[1] + "\n";
    } else if (op == OP_SCI) {
        if(isSyntaxGood_FFXX(instruction))
            return opcode + instruction[1] + "\n";
    } else if (op == OP_SCC) {
        if(isSyntaxGood_FFEE_YYYY(instruction))
            return opcode + "\n" + instruction[1] + "\n";
    } else if (op == OP_SCF) {
        if(isSyntaxGood_FFXX(instruction))
            return opcode + instruction[1] + "\n";
    } else if (op == OP_SCS) {
        if(isSyntaxGood_FFEE(instruction))
            return opcode + "\n";
    } else if (op == OP_SCB) {
        if(isSyntaxGood_FFXX(instruction))
            return opcode + instruction[1] + "\n";
    } else if (op == OP_SRJ) {
        if(isSyntaxGood_FFEX(instruction))
            return opcode + instruction[1] + "\n";
    } else if (op == OP_SIC) {
        if(isSyntaxGood_FFEX(instruction))
            return opcode + instruction[1] + "\n";
    } else if (op == OP_IRJ) {
        if(isSyntaxGood_FFEX(instruction))
            return opcode + instruction[1] + "\n";
    } else if (op == OP_SDC) {
        if(isSyntaxGood_FFEX(instruction))
            return opcode + instruction[1] + "\n";
    } else if (op == OP_DRJ) {
        if(isSyntaxGood_FFEX(instruction))
            return opcode + instruction[1] + "\n";
    } else if (op == OP_SID) {
        if(isSyntaxGood_FFEX(instruction))
            return opcode + instruction[1] + "\n";
    } else if (op == OP_ACJ) {
        if(isSyntaxGood_FFEX(instruction))
            return opcode + instruction[1] + "\n";
    } else if (op == OP_SBU) {
        if(isSyntaxGood_FFEX(instruction))
            return opcode + instruction[1] + "\n";
    } else if (op == OP_ZJF) {
        if(isSyntaxGood_FFXX(instruction))
            return opcode + instruction[1] + "\n";
    } else if (op == OP_NZF) {
        if(isSyntaxGood_FFXX(instruction))
            return opcode + instruction[1] + "\n";
    } else if (op == OP_PJF) {
        if(isSyntaxGood_FFXX(instruction))
            return opcode + instruction[1] + "\n";
    } else if (op == OP_NJF) {
        if(isSyntaxGood_FFXX(instruction))
            return opcode + instruction[1] + "\n";
    } else if (op == OP_ZJB) {
        if(isSyntaxGood_FFXX(instruction))
            return opcode + instruction[1] + "\n";
    } else if (op == OP_NZB) {
        if(isSyntaxGood_FFXX(instruction))
            return opcode + instruction[1] + "\n";
    } else if (op == OP_PJB) {
        if(isSyntaxGood_FFXX(instruction))
            return opcode + instruction[1] + "\n";
    } else if (op == OP_NJB) {
        if(isSyntaxGood_FFXX(instruction))
            return opcode + instruction[1] + "\n";
    } else if (op == OP_JPI) {
        if(isSyntaxGood_FFXX(instruction))
            return opcode + instruction[1] + "\n";
    } else if (op == OP_JPR) {
        if(isSyntaxGood_FFEE_YYYY(instruction))
            return opcode + "\n" + instruction[1] + "\n";
    } else if (op == OP_JFI) {
        if(isSyntaxGood_FFXX(instruction))
            return opcode + instruction[1] + "\n";
    } else if (op == OP_CBC) {
        if(isSyntaxGood_FFEE(instruction))
            return opcode + "\n";
    } else if (op == OP_CIL) {
        if(isSyntaxGood_FFEE(instruction))
            return opcode + "\n";
    } else if (op == OP_IBI) {
        if(isSyntaxGood_FFEE_YYYY(instruction))
            return opcode + "\n" + instruction[1] + "\n";
    } else if (op == OP_IBO) {
        if(isSyntaxGood_FFEE_YYYY(instruction))
            return opcode + "\n" + instruction[1] + "\n";
    } else if (op == OP_INP) {
        if(isSyntaxGood_FFXX_YYYY(instruction))
            return opcode + instruction[1] + "\n" + instruction[2] + "\n";
    } else if (op == OP_OUT) {
        if(isSyntaxGood_FFXX_YYYY(instruction))
            return opcode + instruction[1] + "\n" + instruction[2] + "\n";
    } else if (op == OP_OTN) {
        if(isSyntaxGood_FFXX(instruction))
            return opcode + instruction[1] + "\n";
    } else if (op == OP_INA) {
        if(isSyntaxGood_FFEE(instruction))
            return opcode + "\n";
    } else if (op == OP_OTA) {
        if(isSyntaxGood_FFEE(instruction))
            return opcode + "\n";
    } else if (op == OP_EXC) {
        if(isSyntaxGood_FFEE_YYYY(instruction))
            return opcode + "\n" + instruction[1] + "\n";
    } else if (op == OP_EXF) {
        if(isSyntaxGood_FFXX(instruction))
            return opcode + instruction[1] + "\n";
    } else if (op == OP_SLS) {
        if(isSyntaxGood_FFEX(instruction))
            return opcode + instruction[1] + "\n";
    } else if (op == OP_SLJ) {
        if(isSyntaxGood_FFX0_YYYY(instruction))
            return opcode + instruction[1] + "0\n" + instruction[2] + "\n";
    } else if (op == OP_SJS) {
        if(isSyntaxGood_FFXX_YYYY(instruction))
            return opcode + instruction[1] + "\n" + instruction[2] + "\n";
    } else {
        std::cout << "Operation is unknown." << std::endl;
        return "";
    }
    printSyntaxError(op);
    return "";
}

static TokenArray tokenize(std::ifstream& asmFile) {
    TokenArray ta;
    StringArray sa;
    std::string asmLine;
    std::stringstream ss;
    for(int i = 0; std::getline(asmFile, asmLine); ++i) {
        // If empty line, skip
        if(asmLine.size() == 0)
            continue;
        
        // Replace all tabs with spaces
        std::replace(asmLine.begin(), asmLine.end(), TAB_DELIMITER, SPACE_DELIMITER);

        sa = splitStringByDelimiter(asmLine, SPACE_DELIMITER);

        // Remove whitespace from string array
        StringArray::iterator sai = sa.begin();
        while(sai != sa.end()) {
            if(*sai == "")
                sai = sa.erase(sai);
            else
                ++sai;
        }

        // If empty string array, skip
        if(sa.size() == 0)
            continue;
        
        // Add string array to tokenized array
        ta.push_back(sa);
    }
    return ta;
}

static std::string assemble(std::ifstream& asmFile) {
    std::stringstream mc;
    std::string asmLine, mcLine;
    StringArray instruction;
    for(int i = 0; std::getline(asmFile, asmLine); ++i) {
        instruction = splitStringByDelimiter(asmLine, ' ');
        if(i == 0 && instruction[0] != SET_ADDRESS_SYMBOL) {
            mc << SET_ADDRESS_SYMBOL + "0100\n";
        }
        if(instruction.size() < 1) {
            // Assuming whitespace empty line
            continue;
        }
        if(instruction[0] == SET_ADDRESS_SYMBOL) {
            if(isSyntaxGood_A_XXXX(instruction)) {
                mc << SET_ADDRESS_SYMBOL + instruction[1] + "\n";
            } else {
                std::cerr << "Error: set address syntax incorrect at line " << i + 1 << std::endl;
                asmFail = true;
            }
            continue;
        }
        const std::string& opcode = instruction[0];
        if(isComment(opcode.substr(0, 2))) {
            // Line comment ignored
            continue;
        }
        if(isMachineCodeLiteral(instruction)) {
            mc << opcode + "\n";
            continue;
        }
        if(opcodeDoesNotExist(opcode)) {
            std::cerr << "Error: opcode not recognized at line " << i + 1 << std::endl;
            asmFail = true;
            continue;
        }
        mcLine = instructionToMachineCode(instruction);
        if(mcLine == "") {
            std::cerr << "Error: instruction syntax incorrect at line " << i + 1 << std::endl;
            asmFail = true;
            continue;
        }
        mc << mcLine;
    }
    if(asmFail) exit(EXIT_FAILURE);
    return mc.str();
}

int main(int argc, char *args[]) {
    dr::setopt(argc, args);

    if(!validCommandSyntax()) {
        printUsage();
        return EXIT_FAILURE;
    }

    fs::path asmPath = dr::getsa(0);
    if(!fs::exists(asmPath))
        return EXIT_FAILURE;

    fs::path mcPath;
    if(dr::hasopt(OUTPUT_OPTION_FLAG))
        mcPath = fs::path(dr::getopt(OUTPUT_OPTION_FLAG));
    else
        mcPath = asmPath.parent_path() / DEFAULT_OUTPUT_FILENAME;
    
    std::ifstream asmFile(asmPath);
    std::string asmString = assemble(asmFile);
    asmFile.close();

    std::ofstream mcFile(mcPath);
    mcFile << asmString;
    mcFile.close();

    return EXIT_SUCCESS;
}