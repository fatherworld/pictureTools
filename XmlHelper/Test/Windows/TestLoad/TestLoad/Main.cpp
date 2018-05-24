#include "Config.h"

#include "string_fl.h"

#include "Format.h"

#define LOG_TAG "Main"
#include "Log.h"

#include "AndroidHelper.h"

#include "Xml.h"

int main(int argc, char** argv) {

    char* data = NULL;
    int length = 0;
    readData(argv[1], Alva_File, &data, &length);

    pXML xml = (pXML)malloc(sizeof(XML));
    memset(xml, 0, sizeof(XML));
    initXml(data, xml);

    FILE* file = NULL;
    fopen_s(&file, "Test.xml", "wb");
    fileXml(xml, file);
    fclose(file);

    return 0;
}