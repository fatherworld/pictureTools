#ifndef _XML_H_
#define _XML_H_

#ifdef __cplusplus
extern "C"
{
#endif

#ifdef AlvaPublic
#include "XML_Hide.h"
#endif

    struct _XmlContent_;

    typedef struct _XmlProperty_ {
        char* name;
        char* value;
        struct _XmlProperty_* next;
    }XmlProperty, *pXmlProperty;

    typedef struct _XmlElement_ {
        char* name;
        pXmlProperty propertyList;
        struct _XmlContent_* content;
        struct _XmlElement_* next;
    }XmlElement, *pXmlElement;

    typedef struct _XmlContent_ {
        char* value;
        pXmlElement pelement;
    }XmlContent, *pXmlContent;

    typedef struct _XML_ {
        char* header;
        pXmlElement body;
    }XML, *pXML;

    int initXml(char* data, pXML xml);

    int addElement(pXML xml, char* parentName, pXmlElement xmlElement);

    pXmlElement find(pXmlElement element, char* parentName);

    int fileXml(pXML xml, FILE* data);

    int packageXml(pXML xml, char** data, int* olength);

    int freeElement(pXmlElement ele);

    int freeXml(pXML xml);

#ifdef __cplusplus
}
#endif

#endif