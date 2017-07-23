import QtQuick 2.2

Item
{
    id: mapOverlay
    anchors.fill: parent
    anchors.margins: 25
    visible: false

    Rectangle
    {
        id: overlay
        objectName: "overlay"
        color: "black"
        border.width: 2
        border.color: "#34B1DF"
        radius: 5
        width: parent.width
        height: parent.height

        opacity: 0.75
        OpacityAnimator
        {
            target: overlay;
            from: 0;
            to: 0.75;
            duration: 250
            running: true
        }
    }
}
