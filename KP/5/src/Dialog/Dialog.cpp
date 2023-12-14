#include "Dialog.h"

bool Dialog::Archiver::TryFlags(std::set<std::string> &flags, std::string &fileName) // TODO добавить -c (вывод в stdout) и - (ввод из stdin) 
{
    if (flags.find("-k") != flags.end()) {
        keep = true;
    }
    if (flags.find("-t") != flags.end()) {  // TODO при добавлении LZW пересмотреть
        if (fileName.substr(fileName.find_last_of(".") + 1) != extension) {
            std::string msg = progName + ": " + fileName + ": not in " + extension + " format";
            throw Dialog::Exception(msg);
        }
        check = true;
        Arifm::Compressor arifmCompressor(fileName, true);
        bool res = arifmCompressor.Uncompress();
        std::string tmpFileName = fileName.substr(0, fileName.find_last_of("."));
        std::filesystem::remove(tmpFileName);
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
        Arifm::Compressor arifmCompressor(fileName, true);
        bool res = arifmCompressor.Uncompress();
        std::string tmpFileName = fileName.substr(0, fileName.find_last_of("."));
        size_t compressedSize = std::filesystem::file_size(fileName);
        size_t uncompressedSize = std::filesystem::file_size(tmpFileName);
        double ratio = ((double) compressedSize / (double) uncompressedSize) * 100;
        std::cout << std::fixed << std::setprecision(2);
        std::cout << std::right << std::setw(20) << compressedSize
                  << std::right << std::setw(20) << uncompressedSize
                  << std::setw(20) << ratio
                  << std::setw(20) << tmpFileName << std::endl;
        std::filesystem::remove(tmpFileName);
    }
    if (flags.find("-d") != flags.end()) {
        if (fileName.substr(fileName.find_last_of(".") + 1) != extension) {
            std::string msg = progName + ": " + fileName + ": unknown suffix -- ignored";
            return false;
        }
        Arifm::Compressor arifmCompressor(fileName, true);
        bool res = arifmCompressor.Uncompress();
        if (!keep) {
            std::filesystem::remove(fileName);
        }
        return res;
    }
    if (!check) {
        Arifm::Compressor arifmCompressor(fileName, false);
        bool res = arifmCompressor.Compress();
        if (!keep) {
            std::filesystem::remove(fileName);
        }
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
                            if (!TryFlags(flags, entryFileName)) {
                                continue;
                            }
                        }
                    }
                }
            } else {
                if (!TryFlags(flags, fileName)) {
                    continue;
                }
            }
            if (ec) {
                throw Dialog::Exception(progName + ": " + ec.message());
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
    argSet = parser.Parse(argc, argv);
    defaultFlags = parser.DefaultFlags();
}

Dialog::Archiver::Archiver()
{
}

Dialog::Archiver::~Archiver()
{
}
