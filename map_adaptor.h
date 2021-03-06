/*
 * This file was generated by qdbusxml2cpp version 0.8
 * Command line was: qdbusxml2cpp -a map_adaptor org.hondson.demo.Map.xml
 *
 * qdbusxml2cpp is Copyright (C) 2015 The Qt Company Ltd.
 *
 * This is an auto-generated file.
 * This file may have been hand-edited. Look for HAND-EDIT comments
 * before re-generating it.
 */

#ifndef MAP_ADAPTOR_H
#define MAP_ADAPTOR_H

#include <QtCore/QObject>
#include <QtDBus/QtDBus>
QT_BEGIN_NAMESPACE
class QByteArray;
template<class T> class QList;
template<class Key, class Value> class QMap;
class QString;
class QStringList;
class QVariant;
QT_END_NAMESPACE

/*
 * Adaptor class for interface org.hondson.demo.Map
 */
class MapAdaptor: public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.hondson.demo.Map")
    Q_CLASSINFO("D-Bus Introspection", ""
"  <interface name=\"org.hondson.demo.Map\">\n"
"    <signal name=\"windowSizeChanged\">\n"
"      <arg direction=\"out\" type=\"i\"/>\n"
"      <arg direction=\"out\" type=\"i\"/>\n"
"    </signal>\n"
"    <method name=\"onClicked\"/>\n"
"    <method name=\"onDoubleClicked\"/>\n"
"    <method name=\"pan\">\n"
"      <arg direction=\"in\" type=\"i\"/>\n"
"      <arg direction=\"in\" type=\"i\"/>\n"
"    </method>\n"
"    <method name=\"swipe\">\n"
"      <arg direction=\"in\" type=\"s\"/>\n"
"      <arg direction=\"in\" type=\"i\"/>\n"
"    </method>\n"
"  </interface>\n"
        "")
public:
    MapAdaptor(QObject *parent);
    virtual ~MapAdaptor();

public: // PROPERTIES
public Q_SLOTS: // METHODS
    void onClicked();
    void onDoubleClicked();
    void pan(int in0, int in1);
    void swipe(const QString &in0, int in1);
Q_SIGNALS: // SIGNALS
    void windowSizeChanged(int in0, int in1);
};

#endif
