#ifndef _XML_H_
#define _XML_H_

/*


students{display: block;font-size: 24pt;font-weight: bold;}
first.xml

<?xml version="1.0" encoding="utf-8" standalone = "yes" ?>
<?xml-stylesheet type="text/css" href="hello.css"?>
<students>

<student gender="male" isHandsome="true">
<id>100</id>
<name>lisi</name>
<address>beijing</address>
</student>

<student gender="female">
<id>100</id>
<name>lisi</name>
<address>beijing</address>
</student>

</students>


*******
xml文件分析:
<>都是表示元素，<>里面的<>表示的是父元素的子元素
<>父元素的内容可能是content内容值value，也可能是一些子元素<>
<>元素里面的内容可能是一些属性列表 也可能是一段文本
*/

#ifdef __cplusplus
extern "C"
{
#endif

#ifdef AlvaPublic
#include "XML_Hide.h"
#endif

    //    struct _XmlContent_;
    typedef struct _XmlProperty_ {
        char* name;
        char* value;
        struct _XmlProperty_* next;
    }XmlProperty, *pXmlProperty;

    typedef struct _XmlElement_ {
        //
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

    typedef struct _SignalXmlElement {
        char* name;
        pXmlProperty propertyList;
        struct _XmlContent_* content;
    }*pSignalXmlElement;

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