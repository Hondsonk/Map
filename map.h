#ifndef MAP_H
#define MAP_H

#include <QObject>
#include <QQmlComponent>
#include <QDebug>
#include <QQuickView>
#include <QTimer>
#include "map_adaptor.h"


class Map : public QObject
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.hondson.demo.Map")
    Q_ENUMS(Directions)
    Q_ENUMS(FocusObjects)

public:
    explicit Map(QObject *parent = nullptr);
    ~Map();
    enum Directions {up, down, left, right};
    enum FocusObjects {searchButtonBackground, searchBarBackground, viewBackground };
    void setWindow(QObject*);
    QObject* getWindow();

public slots:
    void onClicked();
    void onDoubleClicked();
    void pan(int, int);
    void swipe(QString, int);

private slots:
    void onWindowSizeChanged(int, int);
    void removeFocus();

signals:
    void windowSizeChanged(int, int);

private:
    QString getCurrentFocus();
    void setFocus(QString);
    QObject *m_window;
    QTimer* focusTimer;
    QString currentFocusObject;
    int glowPeriod;
    bool panEnabled;
};

#endif // MAP_H
