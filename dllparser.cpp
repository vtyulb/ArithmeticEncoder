#include "dllparser.h"
#include <stdio.h>
#include <string.h>

struct IMAGE_FILE_HEADER {
    unsigned short machine;
    unsigned short numberOfSections;
    unsigned int timeDateStamp;
    unsigned int pointerToSymbolTable;
    unsigned int numberOfSymbols;
    unsigned short sizeOfOptionalHeader;
    unsigned short characteristics;
};

struct IMAGE_NT_HEADER {
    char signature[4];
    IMAGE_FILE_HEADER header;
//    int optionalHeader; // not int!!!
};

struct SECTION {
    char name[8];
    unsigned int miscSize;
    unsigned int virtAdress;
    unsigned int sizeOfRawData;
    unsigned int pointerToRawData;
    unsigned int pointerToRelocations;
    unsigned int pointerToLineNumbers;
    unsigned short numberOfRelocations;
    unsigned short numberOfLineNumbers;
    unsigned int characteristics;
};

std::vector<int> getSections(std::vector<unsigned char> dataVect) {
    char *data = (char*) dataVect.data();

    std::vector<int> res;
    if (data[0] != 'M' || data[1] != 'Z')
        return res;

    int offset = *(int*)(data + 0x3C);
    if (offset > dataVect.size())
        return res;

    IMAGE_NT_HEADER h;
    h = *(IMAGE_NT_HEADER*)(data + offset);

    if (strncmp(h.signature, "PE", 2))
        return res;

    offset += h.header.sizeOfOptionalHeader + sizeof(IMAGE_NT_HEADER);
    for (int i = 0; i < h.header.numberOfSections; i++) {
        SECTION s;
        s = *(SECTION*)(data + offset + i * sizeof(SECTION));
        if (s.pointerToRawData)
            res.push_back(s.pointerToRawData);
    }

    return res;
}
