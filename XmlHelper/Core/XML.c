#include "Config.h"

#define LOG_TAG "XML"
#include "Log.h"
#include "ErrorTools.h"

#include "memory_fl.h"
#include "string_fl.h"

#include "XML.h"

#define FILE_NUM 0x0100

static char* xmlHeader = "<?xml version=\"1.0\" encoding=\"ASCII\"?>";

static char* findProperty(char* data, pXmlProperty* xmlProperty);
static char* findContent(char* data, char* end, pXmlContent* xmlContent);
static int content_is_value(char* data, char*end);

static char* getStart(char* data) {

#undef FUNC_CODE
#define FUNC_CODE 0x01
    return strstr(data, "<");
}

static char* getName(char* data, char** elebegin) {

#undef FUNC_CODE
#define FUNC_CODE 0x02

    char* pD = data;
    char* tempValue = data;
    char* hasContValue1 = NULL;
    char* hasContValue2 = NULL;
    while (1)
    {
        hasContValue1 = strstr(tempValue, "<");
        hasContValue2 = strstr(tempValue, "/");
        if (hasContValue1)
        {
            if (hasContValue2)
                if (hasContValue2 - hasContValue1 == 1)
                {
                    pD = strstr(tempValue, ">") + 1;
                    tempValue = pD;
                }
                else
                    break;
            else
                break;
        }
        else
            break;
    }

    while (' ' == *pD || '\n' == *pD || '<' == *pD || '\t' == *pD) {
        pD++;
    }
    char* pS = strstr(pD, " ");
    char* pE = strstr(pD, ">");
    char* pF = strstr(pD, "/");

    char* pNe = NULL;

    if (NULL != pS)
    {
        pNe = pS;
        if (NULL != pE)
        {
            pNe = pNe < pE ? pNe : pE;
            if (pF != NULL)
                pNe = pNe < pF ? pNe : pF;
        }
        else
        {
            if (NULL != pF)
                pNe = pNe < pF ? pNe : pF;
        }
    }
    else
    {
        if (NULL == pE)
            pNe = pF;
        if (NULL == pF)
            pNe = pE;
        if (NULL != pE && pF != NULL)
            pNe = pE < pF ? pE : pF;
    }


    if (NULL == pNe) {
        LOGE("Not an element\n");
        return NULL;
    }

    pNe -= 1;

    char* ptr = (char*)malloc(pNe - pD + 1 + 1);
    *elebegin = tempValue;
    strncpy_fl(ptr, pNe - pD + 1 + 1, pD, pNe - pD + 1);

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
    char temp_elename[100] = { 0 };
    char* elebegin;
    xmlElement->name = getName(eleStart, &elebegin);
    char* nameEnd = strstr(eleStart, ">");
    memcpy(temp_elename, eleStart, 100);
   //strcpy_fl(temp_elename,100, eleStart);
   // strncpy(temp_elename, eleStart, 99);
    if (!strstr(temp_elename, xmlElement->name))
    {
        eleStart = nameEnd + 1;
        while (' ' == *eleStart || '\n' == *eleStart || '\t' == *eleStart)
            eleStart++;

        for (char* cur = eleStart; cur <elebegin; cur++)
        {
            if (*cur == '<' && *(cur + 1) == '/')
            {
                eleStart = elebegin + 1;
                break;
            }

        }
        return eleStart;
    }

    eleEnd = getEnd(data, xmlElement->name);

    if (NULL == eleStart || NULL == eleEnd || NULL == xmlElement->name)
        return data;

    char* ptr = NULL;

    ptr = findProperty(eleStart + strlen(xmlElement->name) + 1, &(xmlElement->propertyList));

    while (' ' == *ptr || '\n' == *ptr || '\t' == *ptr)
        ptr++;

    if (ptr < eleEnd)
        ptr = findContent(ptr, eleEnd, &(xmlElement->content));

    if (ptr > eleEnd)
        return ptr;
    else
        return eleEnd;
}


//问题可能就是在这，即标签必须要有属性。
static char* findPropertyPair(char* data, pXmlProperty xmlProperty) {
    char* nameStart = data;
    while (' ' == *nameStart)
        nameStart++;
    char* nameEnd = strstr(data, "=") - 1;

    char* valueStart = strstr(nameEnd, "\"") + 1;
    char* valueEnd = strstr(valueStart, "\"") - 1;

    int nameLength = nameEnd - nameStart + 1;
    int valueLength = valueEnd - valueStart + 1;
    xmlProperty->name = (char*)malloc(nameLength + 1);
    xmlProperty->value = (char*)malloc(valueLength + 1);

    strncpy_fl(xmlProperty->name, nameLength + 1, nameStart, nameLength);
    strncpy_fl(xmlProperty->value, valueLength + 1, valueStart, valueLength);

    xmlProperty->next = NULL;

    return valueEnd + 2;
}

static char* findProperty(char* data, pXmlProperty* xmlProperty) {
    char* endptr = strstr(data, ">");

    char* ptr = data;

    while (*ptr == ' ' || *ptr == '\n')
        ptr++;

    pXmlProperty mproperty = NULL, tproperty = NULL;

    while (ptr < endptr) {
        tproperty = (pXmlProperty)malloc(sizeof(XmlProperty));
        memset(tproperty, 0, sizeof(XmlProperty));

        if (NULL == *xmlProperty)
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

static int content_is_value(char* data, char*end)
{
    int count_left_parent = 0;
    int count_right_parent = 0;
    for (char* begin = data; begin < end; begin++)
    {

        if (*begin == '<')
        {
            count_left_parent++;

        }
        if (*begin == '>')
        {
            count_right_parent++;

        }
        if (count_left_parent  > 1 || count_right_parent > 1)
            return 0;
    }
    return 1;
}

static char* findContent(char* data, char* end, pXmlContent* xmlContent) {
    char* ptr = data;

    while (*ptr == ' ' || *ptr == '\n' || *ptr == '\t')
        ptr++;

    pXmlElement mE = NULL, pE = NULL;


    if (content_is_value(ptr, end))
    {
        if (*xmlContent == NULL) {
            *xmlContent = (pXmlContent)malloc(sizeof(XmlContent));
            memset(*xmlContent, 0, sizeof(XmlContent));
        }
        if ((*xmlContent)->value == NULL)
        {
            ((*xmlContent)->value) = (char*)malloc(sizeof(char)*(strstr(ptr, "</") - ptr + 1));
            memset((*xmlContent)->value, 0, sizeof(char)*(strstr(ptr, "</") - ptr + 1));
            memcpy((*xmlContent)->value, ptr, strstr(ptr, "</") - ptr);
        }

        ptr = strstr(ptr, ">") + 1;
    }
    else {
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

            if (NULL == mE)
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
    }

    return ptr;
}

int initXml(char* data, pXML xml) {

#undef FUNC_CODE
#define FUNC_CODE 0x01

    char* ptr = NULL;

    pXmlElement pE = NULL, tE = NULL;
/*
    if (NULL != data) {
        char* header = strstr(data, xmlHeader);
        if (NULL == header) {
            LOGE("Wrong XML\n");
            return ERROR_CODE(0x001 | ERROR_OWNER);
        }
    }
    */
    xml->header = (char*)malloc(strlen(xmlHeader) + 1);
    strcpy_fl(xml->header, strlen(xmlHeader) + 1, xmlHeader);
    xml->body = NULL;

    if (NULL != data) {

        char* header = strstr(data, xmlHeader);
        if (NULL == header) {
            ptr = data;
        }
        else
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
            while ((*ptr) == ' ' || (*ptr) == '\n')
                ptr++;
        }
    }

    return 0;
}

pXmlElement find(pXmlElement element, char* parentName) {
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
        else {
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

    pXmlProperty pro;
    pXmlContent cont;
    pXmlElement next;

    int i = 0;
    for (; i < tabCount; i++)
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
    //fprintf(data, "/>");
    else {

        if (cont->value)
        {
            fprintf(data, ">");
            fprintf(data, "%s", cont->value);
        }
        else
            fprintf(data, ">\n");
        next = cont->pelement;
        while (next) {
            outElement(next, data, tabCount + 1);
            next = next->next;
        }

        for (; i < tabCount; i++)
            fprintf(data, "\t");
        fprintf(data, "</%s>\n", ele->name);
    }

    return 0;
}

int fileXml(pXML xml, FILE* data) {

    fprintf(data, "%s\n", xml->header);

    if (NULL != xml->body)
        outElement(xml->body, data, 0);

    return 0;
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


static int freeElement(pXmlElement ele) {
    pXmlProperty ppro, ppronext;
    pXmlContent pcon;
    pXmlElement pE, pEnext;

    if (NULL != ele->name) {
        if (NULL != ele->propertyList)
            LOGE("%s %s\n", ele->name, ele->propertyList->value);
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
        if (pcon->value)
        {
            free(pcon->value);
            pcon->value = NULL;
        }
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

    pXmlElement pE, pEnext;

    if (NULL != xml->header) {
        free(xml->header);
    }

    pE = xml->body;
    while (pE) {
        pEnext = pE->next;
        freeElement(pE);
        pE = pEnext;
    }

    return 0;
}

