#include "map.h"

Map::Map(QObject *parent)
    : QObject(parent),
      m_window(NULL),
      focusTimer(NULL),
      currentFocusObject(""),
      glowPeriod(7500),
      panEnabled(true)
{
    focusTimer = new QTimer(this);
    connect(focusTimer, SIGNAL(timeout()), this, SLOT(removeFocus()));
    new MapAdaptor(this);
    QDBusConnection dbus = QDBusConnection::sessionBus();
    dbus.registerObject("/Map", this);
    dbus.registerService("org.hondson.demo.Map");
}

Map::~Map()
{
    focusTimer->stop();
    delete focusTimer;
}

void Map::setWindow(QObject *window)
{
    m_window = window;
}

QObject* Map::getWindow()
{
    return m_window;
}

void Map::onWindowSizeChanged(int width, int height)
{
    if( width >=0 && height >=0 )
    {
        emit windowSizeChanged(width,height);
    }
    else
    {
        qDebug() << "Invalid Window Size";
    }
}

void Map::onClicked()
{
    QMetaObject MetaObject = this->staticMetaObject;
    QMetaEnum focusObjects = MetaObject.enumerator(MetaObject.indexOfEnumerator("FocusObjects"));

    QObject* searchButton = m_window->findChild<QObject *>("searchButton");
    QObject* searchBar = m_window->findChild<QObject *>("searchBar");
    QObject* view = m_window->findChild<QObject *>("view");

    QVariant currentIndex;

    switch(focusObjects.keysToValue(currentFocusObject.toLatin1()))
    {
    case searchBarBackground:
        QMetaObject::invokeMethod(searchBar, "focus");
        break;
    case searchButtonBackground:
        QMetaObject::invokeMethod(searchButton, "search");
        setFocus("viewBackground");
        break;
    case viewBackground:
        QMetaObject::invokeMethod(view, "removeFocus");
        QMetaObject::invokeMethod(view, "getCurrentIndex", Qt::AutoConnection, Q_RETURN_ARG(QVariant, currentIndex));
        removeFocus();
        break;
    default:
        currentFocusObject = "";
        return;
    }
}

void Map::onDoubleClicked()
{
    QMetaObject::invokeMethod(m_window, "toggleOverlay");
    panEnabled = m_window->property("panEnabled").toBool();
}

void Map::pan(int dx, int dy)
{
    QVariant returnedValue;
    // Inverted controls observed in the native map and on google maps website
    QMetaObject::invokeMethod(m_window, "pan", Qt::AutoConnection, Q_RETURN_ARG(QVariant, returnedValue), Q_ARG(QVariant, -dx), Q_ARG(QVariant, -dy));
}

// Swipe-pan algorithm needs to be refined for big swipes.
void Map::swipe(QString dir, int dist)
{
    if(panEnabled)
    {
        if(dist >= 300) dist = qSqrt(dist);
        m_window->setProperty("swipeDir", dir);
        QPropertyAnimation *animation = new QPropertyAnimation(m_window, "swipeDist");
        animation->setDuration(1000);
        animation->setEasingCurve(QEasingCurve::OutQuint);
        animation->setStartValue(dist);
        animation->setEndValue(0);
        animation->start();
    }
    else
    {
        QMetaObject MetaObject = this->staticMetaObject;
        QMetaEnum dirs = MetaObject.enumerator(MetaObject.indexOfEnumerator("Directions"));

        QObject* searchBar = m_window->findChild<QObject *>("searchBar");
        QObject* view = m_window->findChild<QObject *>("view");

        switch(dirs.keysToValue(dir.toLatin1()))
        {
        case up:
            if(getCurrentFocus() != "viewBackground")
                setFocus("searchBarBackground");
            break;
        case down:
            if(getCurrentFocus() != "viewBackground")
            {
                setFocus("viewBackground");
                QMetaObject::invokeMethod(view, "focus");
            }
            break;
        case left:
            setFocus("searchBarBackground");
            QMetaObject::invokeMethod(searchBar, "focus");
            break;
        case right:
            setFocus("searchButtonBackground");
            // Click after focus to confirm search
            //QMetaObject::invokeMethod(searchButton, "search");
            break;
        }

        if(getCurrentFocus() == "viewBackground")
        {
            focusTimer->stop();
            switch(dirs.keysToValue(dir.toLatin1()))
            {
            case up:
                QMetaObject::invokeMethod(view, "decrementCurrentIndex");
                break;
            case down:
                QMetaObject::invokeMethod(view, "incrementCurrentIndex");
                break;
            }
        }
    }
}

void Map::setFocus(QString objectName)
{
    QMetaObject MetaObject = this->staticMetaObject;
    QMetaEnum focusObjects = MetaObject.enumerator(MetaObject.indexOfEnumerator("FocusObjects"));

    QObject *focusObject = m_window->findChild<QObject *>(objectName);

    if(focusObject == NULL)
    {
        qDebug() << "focusObject couldn't be found, check object names";
        return;
    }

    bool success = false;
    removeFocus();

    switch(focusObjects.keysToValue(objectName.toLatin1()))
    {
    case searchBarBackground:
        success = focusObject->setProperty("focusActive", true);
        break;
    case searchButtonBackground:
        success = focusObject->setProperty("focusActive", true);
        break;
    case viewBackground:
        success = focusObject->setProperty("focusActive", true);
        break;
    default:
        currentFocusObject = "";
        return;
    }

    if (success)
    {
        currentFocusObject = objectName;
    }

    focusTimer->start(glowPeriod);
}

// Focus objects would be better served as a QList of QObjects but faster this way
// Only three focus objects so far so name changes aren't too bad
QString Map::getCurrentFocus()
{
    if(currentFocusObject == "")
    {
        qDebug() << "There is no current focus object";
    }
    return currentFocusObject;
}

void Map::removeFocus()
{
    QObject *searchBar = m_window->findChild<QObject *>("searchBarBackground");
    searchBar->setProperty("focusActive", false);
    QObject *searchButton = m_window->findChild<QObject *>("searchButtonBackground");
    searchButton->setProperty("focusActive", false);
    QObject *view = m_window->findChild<QObject *>("viewBackground");
    view->setProperty("focusActive", false);
    currentFocusObject = "";
}
