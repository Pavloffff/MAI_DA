#include "Dialog.h"

bool Dialog::Archiver::TryFlags(std::set<std::string> &flags, std::string &fileName)
{
    if (flags.find("-d") != flags.end()) {
        if (fileName.substr(fileName.find_last_of(".") + 1) != extension) {
            std::string msg = progName + ": " + fileName + ": unknown suffix -- ignored";
            std::cerr << msg << std::endl;
            return false;
        }
        uncompress = true;
    }
    if (flags.find("-l") != flags.end()) {
        if (fileName.substr(fileName.find_last_of(".") + 1) != extension) {
            std::string msg = progName + ": " + fileName + ": not in " + extension + " format";
            throw Dialog::Exception(msg);
        }
        // TODO : вот тут cout заголовка с флагом -l (сжимаем не тут а ниже):
        //  compressed        uncompressed  ratio uncompressed_name
        //       30758               32206   4.6% algo.odt
    }
    return true;
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
                        if (!TryFlags(flags, entryFileName)) continue;
                        std::cout << entryFileName << std::endl;

                        // TODO: сжать все файлы внутри, если они не директории
                    
                    }
                }
                continue;
            }
            // if (ec) {
            //     throw Dialog::Exception(progName + ": " + ec.message());
            // }
            if (!TryFlags(flags, fileName)) continue;


            // TODO: архивируем здесь


            // std::ifstream input(fileName);
            // std::ofstream output(fileName + ".slzw");
            // char c;
            // int iter = 0;
            // while (input >> std::noskipws >> c) {
            //     // std::cout << c << std::endl;
            //     iter++;
            //     output << c;
            // }
            // std::cout << iter << std::endl;
            // input.close();
            // output.close();    
            std::cout << fileName << "\n";
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
