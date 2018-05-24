#include "Config.h"

#include <stdlib.h>
#include "stdio_fl.h"

#define LOG_TAG "XML"
#include "Log.h"
#include "ErrorTools.h"

#include "memory_fl.h"
#include "string_fl.h"

#include "XML.h"

#define FILE_NUM 0x8900

static char* xmlHeader = "<?xml version=\"1.0\" encoding=\"ASCII\"?>";

static char* findProperty(char* data, pXmlProperty* xmlProperty);
static char* findContent(char* data, char* end, pXmlContent* xmlContent);

static char* getStart(char* data) {

#undef FUNC_CODE
#define FUNC_CODE 0x01

	char* ptrS = strstr(data, "<");
	char* ptrT = strstr(data, "</");

	if (ptrS == ptrT) {
		return NULL;
	}
	else {
		return ptrS;
	}
}

static char* getName(char* data) {

#undef FUNC_CODE
#define FUNC_CODE 0x02

    char* pD = data;
    int nameLength = 0;

    while (' ' == *pD || '\n' == *pD || '<' == *pD || '\t' == *pD) {
        pD++;
    }
    char* pS = strstr(pD, " ");
    char* pE = strstr(pD, ">");
    char* pF = strstr(pD, "/");

    char* pNe = NULL;
    if (NULL != pS) 
        pNe = pS;
    if (NULL != pE){
        if(NULL == pNe){
            pNe = pE;
        }
        else{
            pNe = pE < pNe ? pE : pNe;
        }
    }
    if (NULL != pF){
        if(NULL == pNe){
            pNe = pF;
        }
        else{
            pNe = pF < pNe ? pF : pNe;
        }
    }

    if (NULL == pNe) {
        //LOGE("Not an element\n");
        return NULL;
    }

    pNe -= 1;

    nameLength = (int)(pNe - pD + 1);
	if (0 == nameLength) {
		return NULL;
	}
    char* ptr = (char*)malloc(nameLength + 1);
    memset(ptr, 0, nameLength + 1);

    strncpy_fl(ptr, nameLength + 1, pD, nameLength);

    return ptr;
}

static char* getEnd(char* data, char* name) {

#undef FUNC_CODE
#define FUNC_CODE 0x03

    char* End = NULL;
    char eleEndName[2048] = { 0 };
    sprintf_fl(eleEndName, 2048, "</%s>", name);

    End = strstr(data, eleEndName);
    if (NULL == End) {
        End = strstr(data, "/>") + 2;
    }
    else {
        End += strlen(eleEndName);
    }

    return End;
}

static char* findElements(char* data, pXmlElement xmlElement) {
    
#undef FUNC_CODE
#define FUNC_CODE 0x04

    char *eleEnd = NULL, *eleStart = NULL;

    eleStart = getStart(data);
	if (NULL != eleStart) {
		xmlElement->name = getName(eleStart);
		eleEnd = getEnd(data, xmlElement->name);
	}

    if (NULL == eleStart || NULL == eleEnd || NULL == xmlElement->name)
        return data;

    char* ptr = NULL;

    ptr = findProperty(eleStart + strlen(xmlElement->name) + 1, &(xmlElement->propertyList));

    while (' ' == *ptr || '\n' == *ptr || '\t' == *ptr)
        ptr++;

    if(ptr < eleEnd)
        ptr = findContent(ptr, eleEnd, &(xmlElement->content));

    return eleEnd;
}

static char* findPropertyPair(char* data, pXmlProperty xmlProperty) {

#undef FUNC_CODE
#define FUNC_CODE 0x05

    char* nameStart = data;
    while (' ' == *nameStart)
        nameStart++;
    char* nameEnd = strstr(data, "=") - 1;

    char* valueStart = strstr(nameEnd, "\"") + 1;
    char* valueEnd = strstr(valueStart, "\"") - 1;

    int nameLength = (int)(nameEnd - nameStart + 1);
    int valueLength = (int)(valueEnd - valueStart + 1);
    xmlProperty->name = (char*)malloc(nameLength + 1);
    memset(xmlProperty->name, 0, nameLength + 1);
    xmlProperty->value = (char*)malloc(valueLength + 1);
    memset(xmlProperty->value, 0, valueLength + 1);

    strncpy_fl(xmlProperty->name , nameLength + 1 , nameStart , nameLength );
    strncpy_fl(xmlProperty->value, valueLength + 1, valueStart, valueLength);

    xmlProperty->next = NULL;

    return valueEnd + 2;
}

static char* findProperty(char* data, pXmlProperty* xmlProperty) {

#undef FUNC_CODE
#define FUNC_CODE 0x06

    char* endptr = strstr(data, ">");

    char* ptr = data;

    while (*ptr == ' ' || *ptr == '\n' || *ptr == '\t')
        ptr++;

    pXmlProperty mproperty = NULL, tproperty = NULL;

    while (ptr < endptr) {
        tproperty = (pXmlProperty)malloc(sizeof(XmlProperty));
        memset(tproperty, 0, sizeof(XmlProperty));

        if(NULL == *xmlProperty)
            *xmlProperty = tproperty;

        if (NULL == mproperty) {
            mproperty = tproperty;
        }
        else {
            mproperty->next = tproperty;
            mproperty = mproperty->next;
        }

        ptr = findPropertyPair(ptr, mproperty);

        while (' ' == *ptr || '/' == *ptr || '\n' == *ptr)
            ptr++;
    }

    return endptr + 1;
}

static char* findContent(char* data, char* end, pXmlContent* xmlContent) {

#undef FUNC_CODE
#define FUNC_CODE 0x07

    char* ptr = data;

    while (*ptr == ' ' || *ptr == '\n' || *ptr == '\t')
        ptr++;

	if ('<' == (*ptr) && '/' == *(ptr + 1))
		return ptr;

    pXmlElement mE = NULL, pE = NULL;

    while (ptr < end) {
        if (*xmlContent == NULL) {
            *xmlContent = (pXmlContent)malloc(sizeof(XmlContent));
            memset(*xmlContent, 0, sizeof(XmlContent));
        }

        pE = (pXmlElement)malloc(sizeof(XmlElement));
        memset(pE, 0, sizeof(XmlElement));
        
        if ((*xmlContent)->pelement == NULL) {
            (*xmlContent)->pelement = pE;
        }

        if(NULL == mE)
            mE = pE;
        else {
            mE->next = pE;
            mE = mE->next;
        }

        ptr = findElements(ptr, pE);
        pE = pE->next;

        while (' ' == *ptr || '\n' == *ptr || '\t' == *ptr)
            ptr++;

        if ('<' == (*ptr) && '/' == *(ptr + 1))
            break;
    }

    return ptr;
}

int initXml(char* data, pXML xml) {

#undef FUNC_CODE
#define FUNC_CODE 0x08

    char* ptr = NULL;

    pXmlElement pE = NULL, tE = NULL;
    
    if (NULL != data) {
        char* header = strstr(data, xmlHeader);
        if (NULL == header) {
            LOGE("Wrong XML\n");
            return ERROR_CODE(0x001 | ERROR_OWNER);
        }
    }

    xml->header = (char*)malloc(strlen(xmlHeader) + 1);
    strcpy_fl(xml->header, strlen(xmlHeader) + 1, xmlHeader);
    xml->body = NULL;

    if (NULL != data) {
        ptr = data + strlen(xmlHeader);

        while ((*ptr) != '\0') {
            
            tE = (pXmlElement)malloc(sizeof(XmlElement));
            memset(tE, 0, sizeof(XmlElement));

            if (NULL == xml->body)
                xml->body = tE;

            if (NULL == pE)
                pE = tE;
            else {
                pE->next = tE;
                pE = pE->next;
            }

            ptr = findElements(ptr, tE);
            while ((*ptr) == ' ' || (*ptr) == '\n' || *ptr == '\t')
                ptr++;
        }
    }

    return 0;
}

pXmlElement find(pXmlElement element, char* parentName) {

#undef FUNC_CODE
#define FUNC_CODE 0x09

    pXmlElement mE = NULL, fE = NULL;

    if (!strcmp(element->name, parentName)) {
        return element;
    }
    else {
        if (element->content) {
            if (element->content->pelement) {
                mE = element->content->pelement;
                while (mE) {
                    fE = find(element->content->pelement, parentName);
                    if (NULL == fE) {
                        mE = mE->next;
                    }
                    else {
                        return fE;
                    }
                }
                return NULL;
            }
            else {
                return NULL;
            }
        }
        else {
            return NULL;
        }
    }
}

int addElement(pXML xml, char* parentName, pXmlElement xmlElement) {

#undef FUNC_CODE
#define FUNC_CODE 0x0A

    pXmlElement pEle = NULL;

    if (NULL == parentName) {
        xml->body = xmlElement;
        return 0;
    }

    pEle = find(xml->body, parentName);

    if (NULL != pEle) {
        if (NULL == pEle->content) {
            pEle->content = (pXmlContent)malloc(sizeof(XmlContent));
            memset(pEle->content, 0, sizeof(XmlContent));
        }
        if (NULL == pEle->content->pelement) {
            pEle->content->pelement = xmlElement;
        }
        else{
            pEle = pEle->content->pelement;
            while (pEle->next != NULL) {
                pEle = pEle->next;
            }
            pEle->next = xmlElement;
        }
    }

    return 0;
}

static int outElement(pXmlElement ele, FILE* data, int tabCount) {

#undef FUNC_CODE
#define FUNC_CODE 0x0B

    pXmlProperty pro;
    pXmlContent cont;
    pXmlElement next;

    int i = 0;
    for (i = 0; i < tabCount; i++)
        fprintf(data, "\t");

    fprintf(data, "<%s", ele->name);
    
    pro = ele->propertyList;

    while (pro) {
        fprintf(data, " %s=\"%s\"", pro->name, pro->value);
        pro = pro->next;
    }

    cont = ele->content;

    if (NULL == cont)
        fprintf(data, "/>\n");
    else{
        fprintf(data, ">\n");

        next = cont->pelement;
        while(next){
            outElement(next, data, tabCount + 1);
            next = next->next;
        }

        for (i = 0; i < tabCount; i++)
            fprintf(data, "\t");
        fprintf(data, "</%s>\n", ele->name);
    }

    return 0;
}

int fileXml(pXML xml, FILE* data) {

#undef FUNC_CODE
#define FUNC_CODE 0x0C

    fprintf(data, "%s\n", xml->header);

    if (NULL != xml->body)
        outElement(xml->body, data, 0);

    return 0;
}

static int countElement(pXmlElement ele, int tabCount) {

#undef FUNC_CODE
#define FUNC_CODE 0x0D

    pXmlProperty pro;
    pXmlContent cont;
    pXmlElement next;

    int length = 0;

    int i = 0;
    for (i = 0; i < tabCount; i++)
        length += (int)strlen("\t");

    length += (int)strlen("<") + (int)strlen(ele->name);

    pro = ele->propertyList;

    while (pro) {
        length += (int)strlen(" ") + (int)strlen(pro->name) + (int)strlen("=\"") + (int)strlen(pro->value) + (int)strlen("\"");
        pro = pro->next;
    }

    cont = ele->content;

    if (NULL == cont)
        length += (int)strlen("></") + (int)strlen(ele->name) + (int)strlen(">\n");
    else {
        length += (int)strlen(">\n");

        next = cont->pelement;
        while (next) {
            length += countElement(next, tabCount + 1);
            next = next->next;
        }

        for (i = 0; i < tabCount; i++)
            length += (int)strlen("\t");
        length += (int)strlen("</") + (int)strlen(ele->name) + (int)strlen(">\n");
    }
    return length;
}

static int countXml(pXML xml, int tabCount) {

#undef FUNC_CODE
#define FUNC_CODE 0x0E

    int length = 0, i = 0;
    //package header
    length += (int)strlen(xml->header) + (int)strlen("\n");

    if(NULL != xml->body)
        length += countElement(xml->body, 0);

    return length;
}

static int packageElement(pXmlElement ele, char* ptr, int size, int tabCount) {

#undef FUNC_CODE
#define FUNC_CODE 0x0F

    pXmlProperty pro;
    pXmlContent cont;
    pXmlElement next;

    int length = 0, offset = 0, i = 0, tl = 0, eSize = 0;
    
    for (i = 0; i < tabCount; i++) {
        sprintf_fl(ptr, size - offset, "\t");
        tl = (int)strlen("\t");
        ptr += tl;
        offset += tl;
    }

    sprintf_fl(ptr, size - offset, "<%s", ele->name);
    tl = (int)strlen("<") + (int)strlen(ele->name);
    ptr += tl;
    offset += tl;

    pro = ele->propertyList;

    while (pro) {
        sprintf_fl(ptr, size - offset, " %s=\"%s\"", pro->name, pro->value);
        tl = (int)strlen(" ") + (int)strlen(pro->name) + (int)strlen("=\"") + (int)strlen(pro->value) + (int)strlen("\"");
        ptr += tl;
        offset += tl;
        pro = pro->next;
    }

    cont = ele->content;

    if (NULL == cont) {
        sprintf_fl(ptr, size - offset, "></%s>\n", ele->name);
        tl = (int)strlen("></") + (int)strlen(ele->name) + (int)strlen(">\n");
        ptr += tl;
        offset += tl;
    }
    else {
        sprintf_fl(ptr, size - offset, ">\n");
        tl = (int)strlen(">\n");
        ptr += tl;
        offset += tl;

        next = cont->pelement;
        while (next) {
            eSize = packageElement(next, ptr, size - offset, tabCount + 1);
            offset += eSize;
            ptr += eSize;
            next = next->next;
        }

        for (i = 0; i < tabCount; i++) {
            sprintf_fl(ptr, size - offset, "\t");
            tl = (int)strlen("\t");
            ptr += tl;
            offset += tl;
        }
        sprintf_fl(ptr, size - offset, "</%s>\n", ele->name);
        tl = (int)strlen("</") + (int)strlen(ele->name) + (int)strlen(">\n");
        ptr += tl;
        offset += tl;
    }

    return offset;
}

int packageXml(pXML xml, char** data, int* olength) {

#undef FUNC_CODE
#define FUNC_CODE 0x10

    int length = 0, i = 0, offset = 0;

    char* ptr = NULL;

    length = countXml(xml, 0) + 1;
    *data = (char*)malloc(length);
    memset(*data, 0, length);
    ptr = *data;

    //package header
    sprintf_fl(ptr, length, "%s\n", xml->header);
    ptr += (int)strlen(xml->header) + 1;
    offset += (int)strlen(xml->header) + 1;

    if (NULL != xml->body)
        packageElement(xml->body, ptr, length - offset, 0);

    *olength = length;

    return 0;
}

int freeElement(pXmlElement ele) {

#undef FUNC_CODE
#define FUNC_CODE 0x11

    pXmlProperty ppro, ppronext;
    pXmlContent pcon;
    pXmlElement pE, pEnext;

    if (NULL != ele->name) {
        free(ele->name);
    }

    ppro = ele->propertyList;
    while (ppro) {
        ppronext = ppro->next;
        if (NULL != ppro->name) {
            free(ppro->name);
        }
        if (NULL != ppro->value) {
            free(ppro->value);
        }
        free(ppro);
        ppro = ppronext;
    }

    pcon = ele->content;
    if (NULL != pcon) {
        pE = pcon->pelement;
        while (pE) {
            pEnext = pE->next;
            freeElement(pE);
            pE = pEnext;
        }
    }
    free(pcon);

    free(ele);

    return 0;
}

int freeXml(pXML xml) {

#undef FUNC_CODE
#define FUNC_CODE 0x12

    pXmlElement pE, pEnext;

    if (NULL != xml->header) {
        free(xml->header);
    }

    pE = xml->body;
    while(pE){
        pEnext = pE->next;
        freeElement(pE);
        pE = pEnext;
    }

    return 0;
}