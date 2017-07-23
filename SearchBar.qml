import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.2

Rectangle
{
    property string query
    property string focusActiveColor: "orange"
    property int focusGlowSize: 5
    property int searchButtonSize: 35

    id: root
    objectName: "searchBackground"
    color: "white"
    height: 35
    anchors
    {
        top: parent.top
        topMargin: 25
        right: parent.right
        rightMargin: 25
        left: parent.left
        leftMargin: 25
    }

    Rectangle
    {
        property bool focusActive: false

        id: searchBarBackground
        objectName: "searchBarBackground"
        width: root.width - searchButtonSize
        anchors
        {
            top: parent.top
            topMargin: -focusGlowSize
            left: parent.left
            leftMargin: -focusGlowSize
            bottom: parent.bottom
            bottomMargin: -focusGlowSize
        }
        color: "transparent"
        border.color: focusActive ? focusActiveColor : "transparent"
        border.width: focusGlowSize

        TextField
        {
            function focus()
            {
                forceActiveFocus();
            }

            id: input
            objectName: "searchBar"
            placeholderText: "Search destination or POI"
            font.pointSize: 14

            style:

                TextFieldStyle
                {
                    background:
                        Rectangle
                        {
                            width: input.width
                            height: input.height
                            color: "white"
                            radius: 0
                        }
                }

            anchors.fill: parent
            anchors.margins: focusGlowSize
        }
    }

    Rectangle
    {
        property bool focusActive: false

        id: searchButtonBackground
        objectName: "searchButtonBackground"
        color: "transparent"
        border.color: focusActive ? focusActiveColor : "transparent"
        border.width: focusGlowSize

        width: searchButtonSize + 2*focusGlowSize

        anchors
        {
            right: root.right
            leftMargin: -focusGlowSize
            top: root.top
            topMargin: -focusGlowSize
            bottom: root.bottom
            bottomMargin: -focusGlowSize
        }

        MouseArea
        {
            function search()
            {
                onClicked(Qt.MouseEvent);
            }

            property bool focusActive: false

            id: searchButton
            objectName: "searchButton"
            width: root.height
            height: root.height

            onClicked:
            {
                query = input.displayText;
            }

            Image
            {
                id: magnifyingGlass
                source: "assets/search.png"
                anchors.centerIn: parent
            }

            anchors.fill: parent
            anchors.margins: focusGlowSize
        }
    }
}
