#include <iostream>

#include "Arifm/Arifm.h"
#include "Lzw/Lzw.h"
#include "Dialog/Dialog.h"

int main(int argc, char const *argv[])
{
    Dialog::Archiver archiver(argc, (char **) argv);
    archiver.Run();
    return 0;
}
