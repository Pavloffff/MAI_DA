#include "Dialog.h"

bool Dialog::Archiver::Archive(std::set<std::string> &flags, std::string &fileName)
{
    if (fileName == "-") {
        consoleInput = true;
    } else {
        consoleInput = false;
    }
    if (flags.find("-1") != flags.end() && 
        flags.find("-9") != flags.end()) {
        std::string msg = progName + ": " + fileName + ": cannot use both -1 and -9 flags";
        throw Dialog::Exception(msg);
    } else if (flags.find("-1") != flags.end()) {
        compressValue = 11;
    } else if (flags.find("-9") != flags.end()) {
        compressValue = 31;
    } else {
        compressValue = 16;
    }
    if (flags.find("-c") != flags.end()) {
        keep = true;
        consoleOutput = true;
    }
    if (flags.find("-k") != flags.end()) {
        keep = true;
    }
    if (flags.find("-t") != flags.end()) {
        if (fileName.substr(fileName.find_last_of(".") + 1) != extension) {
            std::string msg = progName + ": " + fileName + ": not in " + extension + " format";
            throw Dialog::Exception(msg);
        }
        check = true;
        std::string inputFileName = fileName;
        std::string tmpFileName = fileName.substr(0, fileName.find_last_of(".")) + ".tmp";
        std::string outputFileName = fileName.substr(0, fileName.find_last_of("."));
        Arifm::Compressor arifmCompressor(inputFileName, tmpFileName, true, consoleInput, consoleOutput);
        LZW::Compressor LZWCompressor(tmpFileName, outputFileName, true, consoleInput, consoleOutput, compressValue);
        bool res = arifmCompressor.Uncompress();
        res = LZWCompressor.Uncompress();
        std::filesystem::remove(tmpFileName);
        std::filesystem::remove(outputFileName);
        if (!res) {
            std::string msg = progName + ": " + fileName + ": failed integrity check -- invalid archive";
            throw Dialog::Exception(msg);
        }
    }
    if (flags.find("-l") != flags.end()) {
        if (fileName.substr(fileName.find_last_of(".") + 1) != extension) {
            std::string msg = progName + ": " + fileName + ": not in " + extension + " format";
            throw Dialog::Exception(msg);
        }
        if (lFirst) {
            std::cout << std::right << std::setw(20) << "compressed"
                << std::right << std::setw(20) << "uncompressed"
                << std::setw(20) << "ratio"
                << std::setw(20) << "uncompressed_name" << std::endl;
            lFirst = false;
        }
        check = true;
        std::string inputFileName = fileName;
        std::string tmpFileName = fileName.substr(0, fileName.find_last_of(".")) + ".tmp";
        std::string outputFileName = fileName.substr(0, fileName.find_last_of("."));
        Arifm::Compressor arifmCompressor(inputFileName, tmpFileName, true, consoleInput, consoleOutput);
        LZW::Compressor LZWCompressor(tmpFileName, outputFileName, true, consoleInput, consoleOutput, compressValue);
        bool res = arifmCompressor.Uncompress();
        res = LZWCompressor.Uncompress();
        size_t compressedSize = std::filesystem::file_size(fileName);
        size_t uncompressedSize = std::filesystem::file_size(outputFileName);
        double ratio = ((double) compressedSize / (double) uncompressedSize) * 100;
        std::cout << std::fixed << std::setprecision(2);
        std::cout << std::right << std::setw(20) << compressedSize
                  << std::right << std::setw(20) << uncompressedSize
                  << std::setw(20) << ratio
                  << std::setw(20) << outputFileName << std::endl;
        std::filesystem::remove(tmpFileName);
        std::filesystem::remove(outputFileName);
    }
    if (flags.find("-d") != flags.end()) {
        if (fileName.substr(fileName.find_last_of(".") + 1) != extension) {
            std::string msg = progName + ": " + fileName + ": unknown suffix -- ignored";
            return false;
        }
        std::string inputFileName = fileName;
        if (consoleInput) {
            inputFileName = Utils::GenerateRandomString(16);
        }
        std::string tmpFileName = inputFileName.substr(0, fileName.find_last_of(".")) + ".tmp";
        std::string outputFileName = inputFileName.substr(0, fileName.find_last_of("."));
        if (consoleInput) {
            std::cout << progName << ": uncompresed in " << outputFileName << std::endl;
        }
        Arifm::Compressor arifmCompressor(inputFileName, tmpFileName, true, consoleInput, consoleOutput);
        LZW::Compressor LZWCompressor(tmpFileName, outputFileName, true, consoleInput, consoleOutput, compressValue);
        bool res = arifmCompressor.Uncompress();
        res = LZWCompressor.Uncompress();
        if (!keep || consoleInput) {
            std::filesystem::remove(inputFileName);
        }
        if (consoleOutput) {
            std::filesystem::remove(outputFileName);
        }
        std::filesystem::remove(tmpFileName);
        return res;
    }
    if (!check) {
        std::string inputFileName = fileName;
        if (consoleInput) {
            inputFileName = Utils::GenerateRandomString(16);
        }
        std::string tmpFileName = inputFileName + ".tmp";
        std::string outputFileName = inputFileName + ".slzw";
        if (consoleInput) {
            std::cout << progName << ": compresed in " << outputFileName << std::endl;
        }
        LZW::Compressor LZWCompressor(inputFileName, tmpFileName, false, consoleInput, consoleOutput, compressValue);
        Arifm::Compressor arifmCompressor(tmpFileName, outputFileName, false, consoleInput, consoleOutput);
        bool res = LZWCompressor.Compress();
        res = arifmCompressor.Compress();
        if (!keep || consoleInput) {
            std::filesystem::remove(inputFileName);
        }
        if (consoleOutput) {
            std::filesystem::remove(outputFileName);
        }
        std::filesystem::remove(tmpFileName);
        return res;
    }
    return false;
}

void Dialog::Archiver::Run()
{
    std::set<std::string> flags;
    std::vector<std::string> fileNames;

    for (auto arg : argSet) {
        if (defaultFlags.find(arg) != defaultFlags.end()) {
            flags.insert(arg);
        } else {
            fileNames.push_back(arg);
        }
    }
    if (fileNames.empty() && fileCheck) {
        fileNames.push_back("-");
    }
    
    if (flags.find("-h") != flags.end()) {
        Help();
        return;
    }

    try {
        for (auto fileName : fileNames) {
            std::filesystem::path filePath(fileName);
            std::error_code ec;
            if (std::filesystem::is_directory(filePath, ec)) {
                if ((flags.find("-r") == flags.end())) {
                    std::string msg = progName + ": " + fileName + " is a directory -- ignored";
                    std::cerr << msg << std::endl;
                } else {
                    for (const auto &dirEntry : std::filesystem::recursive_directory_iterator(filePath)) {
                        std::stringstream sstream;
                        sstream << dirEntry;
                        std::string entryFileName;
                        sstream >> entryFileName;
                        entryFileName = entryFileName.substr(1, entryFileName.size() - 2);
                        std::filesystem::path entryFilePath(entryFileName);
                        if (!std::filesystem::is_directory(entryFilePath, ec)) {
                            if (!Archive(flags, entryFileName)) {
                                continue;
                            }
                        }
                    }
                }
            } else {
                if (!Archive(flags, fileName)) {
                    continue;
                }
            }
        }
    } catch (const Dialog::Exception &ex) {
        Dialog::Exception::LogException(ex);
        exit(-1);
    }
}

void Dialog::Archiver::Help()
{
    std::cout << "Usage: " << progName << " [OPTION]... [FILE]..." << std::endl;
    std::cout << "Compress or uncompress FILEs (by default, compress FILES in-place)." << std::endl;
    std::cout << std::endl;
    std::cout << "Mandatory arguments for short options." << std::endl;
    std::cout << std::endl;
    std::cout << "  -c\twrite on standard output, keep original files unchanged" << std::endl;
    std::cout << "  -d\tuncompress" << std::endl;
    std::cout << "  -h\tgive this help" << std::endl;
    std::cout << "  -k\tkeep (don't delete) input files" << std::endl;
    std::cout << "  -l\tlist compressed file contents" << std::endl;
    std::cout << "  -r\toperate recursively on directories" << std::endl;
    std::cout << "  -t\ttest compressed file integrity" << std::endl;
    std::cout << "  -1\tcompress faster" << std::endl;
    std::cout << "  -9\tcompress better" << std::endl;
    std::cout << std::endl;
    std::cout << "With no FILE, or when FILE is -, read standard input." << std::endl;
}

Dialog::Archiver::Archiver(int argc, char **argv)
{
    progName = argv[0];
    Dialog::ArgParser parser;
    auto parsed = parser.Parse(argc, argv);
    argSet = parsed.second;
    fileCheck = parsed.first;
    defaultFlags = parser.DefaultFlags();
}

Dialog::Archiver::Archiver()
{
}

Dialog::Archiver::~Archiver()
{
}
