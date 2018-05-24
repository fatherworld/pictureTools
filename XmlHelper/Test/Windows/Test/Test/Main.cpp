#include "Config.h"

#include "string_fl.h"

#define LOG_TAG "Main"
#include "Log.h"

#include "Xml.h"

int main(int argc, char** argv) {

    pXML xml = (pXML)malloc(sizeof(XML));
    memset(xml, 0, sizeof(XML));
    initXml(NULL, xml);

    {
        pXmlElement ele = (pXmlElement)malloc(sizeof(XmlElement));
        memset(ele, 0, sizeof(XmlElement));

        ele->name = (char*)malloc(strlen("Key") + 1);
        strcpy_fl(ele->name, strlen("Key") + 1, "Key");
        addElement(xml, NULL, ele);
    }
    
    {
        pXmlElement ele = (pXmlElement)malloc(sizeof(XmlElement));
        memset(ele, 0, sizeof(XmlElement));

        ele->name = (char*)malloc(strlen("Store") + 1);
        strcpy_fl(ele->name, strlen("Store") + 1, "Store");
        addElement(xml, "Key", ele);
    }

    {
        pXmlElement ele = (pXmlElement)malloc(sizeof(XmlElement));
        memset(ele, 0, sizeof(XmlElement));

        ele->name = (char*)malloc(12);
        strcpy_fl(ele->name, 12, "MyKey");

        pXmlProperty proTitle = (pXmlProperty)malloc(sizeof(XmlProperty));
        memset(proTitle, 0, sizeof(XmlProperty));

        pXmlProperty proSubtitle = (pXmlProperty)malloc(sizeof(XmlProperty));
        memset(proSubtitle, 0, sizeof(XmlProperty));

        proTitle->name = (char*)malloc(6);
        strcpy_fl(proTitle->name, 6, "title");
        proTitle->value = (char*)malloc(strlen("fenglang") + 1);
        strcpy_fl(proTitle->value, strlen("fenglang") + 1, "fenglang");

        proSubtitle->name = (char*)malloc(9);
        strcpy_fl(proSubtitle->name, 9, "subtitle");
        proSubtitle->value = (char*)malloc(strlen("fenglang") + 1);
        strcpy_fl(proSubtitle->value, strlen("fenglang") + 1, "fenglang");

        ele->propertyList = proTitle;
        proTitle->next = proSubtitle;

        addElement(xml, "Store", ele);
    }

    char* package = NULL;
    int length = 0;

    packageXml(xml, &package, &length);

    FILE* file;
    fopen_fl(&file, "key.xml", "wb");
    fileXml(xml, file);
    fclose(file);
    fopen_fl(&file, "key_package.xml", "wb");
    fwrite(package, 1, length, file);
    fclose(file);

    return 0;
}