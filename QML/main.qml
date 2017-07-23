import QtQuick 2.3
import QtQuick.Window 2.2
import QtPositioning 5.5
import QtLocation 5.5

Window
{
    // San Francisco
    property real defaultLatitude: 37.77
    property real defaultLongitude: -122.42
    property int mapResolution: width*height

    property string swipeDir: ""
    property int swipeDist: 0
    property int padding: 25
    property int searchBarHeight: 35
    property bool panEnabled: true
    property string focusActiveColor: "orange"
    property int focusGlowSize: 5

    signal clicked
    signal doubleClicked
    signal windowSizeChanged(int width, int height)

    function toggleOverlay()
    {
        overlay.visible = !overlay.visible;
        panEnabled = !overlay.visible
    }

    function pan(dx, dy)
    {
        if(panEnabled)
        {
            map.pan(dx, dy);
        }
    }

    id: window
    visible: true
    width: 640
    height: 480

    onMapResolutionChanged: windowSizeChanged(width, height)

    onSwipeDistChanged:
    {
        //console.log("SwipeDir: ", swipeDir, "SwipeDist: ", swipeDist);
        if(swipeDir == "left")
            pan(swipeDist, 0);
        else if(swipeDir == "right")
            pan(-swipeDist, 0);
        else if(swipeDir == "up")
            pan(0, -swipeDist);
        else if(swipeDir =="down")
            pan(0, swipeDist);
    }

    Map
    {
        id: map
        objectName: "map"
        anchors.fill: parent
        // Google Maps plugin built from : https://github.com/vladest/googlemaps
        plugin: Plugin { name: "googlemaps" }
        zoomLevel: 16    // Max: 21    Min: 1.3
        center: QtPositioning.coordinate(defaultLatitude, defaultLongitude)

        MapQuickItem
        {
            id: marker
            coordinate: QtPositioning.coordinate(defaultLatitude, defaultLongitude)
            onCoordinateChanged: console.log("Coordinate Changed");
            sourceItem: Image { id: image; source: "assets/mapMarker.png" }
        }

        GeocodeModel
        {
            id: geoModel
            plugin: map.plugin
            query: searchBar.query
            onQueryChanged:
            {
                update();
            }

            onLocationsChanged:
            {
                console.log("Locations Changed");
                dumpGeoModelData();
                if(count)
                    marker.coordinate = get(0).coordinate
            }

            function dumpGeoModelData()
            {
                for(var i=0; i<geoModel.count; i++)
                {
                    var city, country, countryCode, county, district, postalCode, state, street, text, coordinate;
                    if(get(i).address)
                    {

                        if(get(i).address.city) city = " city: " + get(i).address.city; else city = "";
                        if(get(i).address.country) country = " country: " + get(i).address.country; else country = "";
                        if(get(i).address.countryCode) countryCode = " countryCode: " + get(i).address.countryCode; else countryCode = "";
                        if(get(i).address.county) county = " county: " + get(i).address.county; else county = "";
                        if(get(i).address.district) district = " district: " + get(i).address.district; else district = "";
                        if(get(i).address.postalCode) postalCode = " postalCode: " + get(i).address.postalCode; else postalCode = "";
                        if(get(i).address.state) state = " state: " + get(i).address.state; else state = "";
                        if(get(i).address.street) street = " street: " + get(i).address.street; else street = "";
                        if(get(i).address.text) text = " text: " + get(i).address.text; else text = "";
                    }
                    if(get(i).coordinate)
                    {
                        coordinate = " coordinate: " + get(i).coordinate;
                    }

                    console.log(i, ".", city, country, countryCode, county, district, postalCode, state, street, text, coordinate);
                }
            }
        }

        MouseArea
        {
            anchors.fill: parent
            onClicked:
            {
                window.clicked();
            }
            onDoubleClicked:
            {
                window.doubleClicked();
            }
        }
    }

    MapOverlay
    {
        id: overlay
        width: window.width - 2*padding
        height: window.height - 2*padding

        SearchBar
        {
            id: searchBar
        }

        Rectangle
        {
            property bool focusActive: false

            id: viewBackground
            objectName: "viewBackground"
            color: focusActive ? "black" : "transparent"
            border.color: focusActive ? focusActiveColor: "transparent"
            border.width: focusActive ? focusGlowSize: 0
            opacity: 0.5
            width: parent.width - 2*padding
            height: parent.height - searchBarHeight - padding
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: searchBar.bottom
            anchors.bottom: parent.bottom
            anchors.topMargin: padding
            anchors.bottomMargin: padding

            ListView
            {
                function focus()           { forceActiveFocus();  }
                function removeFocus()     { currentIndex = -1;   }
                function getCurrentIndex() { console.log(currentIndex); return currentIndex; }

                function decrementCurrentIndex()
                {
                    //if      (currentIndex == -1)   { currentIndex = 0; }
                    if (currentIndex >= 0)     { currentIndex--;   }
                    else                           { removeFocus();    }
                }

                function incrementCurrentIndex()
                {
                    //if      (currentIndex == -1)   { currentIndex = 0; }
                    if (currentIndex < count) { currentIndex++; }
                    else                           { removeFocus(); }
                }

                id: view
                objectName: "view"
                clip: true
                focus: false
                highlight: Rectangle { color: "lightsteelblue"; }
                highlightFollowsCurrentItem: true
                anchors.fill: parent
                model: geoModel
                delegate:
                    Component
                    {
                        id: searchResult
                        Item
                        {
                            width: view.width
                            height: 40
                            Rectangle
                            {
                                anchors
                                {
                                    fill: parent
                                    leftMargin: 1
                                    rightMargin: 1
                                }

                                color: "transparent"
                                Text
                                {
                                    verticalAlignment: Text.AlignVCenter
                                    anchors.fill: parent
                                    anchors.margins: 5
                                    text: geoModel.get(index).address.county
                                    color: "white"
                                    font.pointSize: 12
                                }
                            }
                        }
                    }
            }
        }
    }
}
