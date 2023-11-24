import QtQuick 2.0
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.15

Rectangle {
    width: 800
    height: 600

    Dialog {
        id: showDayInfoDialog
        title: "Edit day: "+forecast_date
        property int id: 0
        property string forecast_date: ""
        standardButtons: Dialog.Ok | Dialog.Cancel
        implicitWidth: parent.width
        implicitHeight: parent.height
        Grid {
            columns: 2
            spacing: 2
            Label {
                text: "Condition"
            }
            TextField {
                id: editCondition
                readOnly: true
            }
            Label {
                text: "Average Temperature ºC"
            }
            TextField {
                id: editAvgTemp
                readOnly: true
            }
            Label {
                text: "Minimal Temperature ºC"
            }
            TextField {
                id: editMinTemp
                readOnly: true
            }
            Label {
                text: "Maximum Temperature ºC"
            }
            TextField {
                id: editMaxTemp
                readOnly: true
            }
            Label {
                text: "Maximum Wind (kph)"
            }
            TextField {
                id: editMaxWind
                readOnly: true
            }
            Label {
                text: "Chance of rain (%)"
            }
            TextField {
                id: editChance
                readOnly: true
            }
            Label {
                text: "Chance of Rain (%)"
            }
            TextField {
                id: editChanceOfRain
                readOnly: true
            }
            Label {
                text: "Precipitation (kph)"
            }
            TextField {
                id: editPrecip
                readOnly: true
            }
            Label {
                text: "Comment"
            }
            TextField {
                id: editComment
                readOnly: false
            }

        }
        onAccepted: forecastModel.appendComment(id, editComment.text);
    }

    Column {
        anchors.margins: 10
        anchors.fill: parent
        spacing: 10
        Rectangle {
            width: parent.width
            height: 30
            color: "lightgrey"
            Text {
                anchors.centerIn: parent
                renderType: Text.NativeRendering
                text: "Saint-Petersburg weather forcast"
            }
        }

        ListView {
            id: view

            width: parent.width
            height: parent.height
            spacing: 10
            model: forecastModel
            clip: true

            delegate: Rectangle {
                width: view.width
                height: 40

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        editCondition.text = condition;
                        editAvgTemp.text = avg_temp_c;
                        editMinTemp.text = min_temp_c;
                        editMaxTemp.text = max_temp_c;
                        editMaxWind.text = maxwind_kph;
                        editChanceOfRain.text = chance_of_rain;
                        editPrecip.text = totalprecip_mm;
                        editComment.text = comment;
                        showDayInfoDialog.id = id
                        showDayInfoDialog.forecast_date = forecast_date
                        showDayInfoDialog.open();
                    }

                }

                Rectangle {
                    id: rectangle0
                    width: 100
                    height: parent.height
                    color: "skyblue"
                    Text {
                        anchors.centerIn: parent
                        renderType: Text.NativeRendering
                        text: forecast_date
                    }
                }
                Rectangle {
                    id: rectangle1
                    anchors.left: rectangle0.right
                    width: 250
                    height: parent.height
                    Text {
                        anchors.centerIn: parent
                        renderType: Text.NativeRendering
                        text: condition
                    }
                }
                Rectangle {
                    id:rectangle2
                    anchors.left: rectangle1.right
                    width:70
                    height: parent.height
                    Text {
                        anchors.centerIn: parent
                        renderType: Text.NativeRendering
                        text: avg_temp_c+"ºC"
                    }
                }
                Rectangle {
                    id:rectangle3
                    anchors.left: rectangle2.right
                    width:70
                    height: parent.height
                    Text {
                        anchors.centerIn: parent
                        renderType: Text.NativeRendering
                        text: maxwind_kph+" kph"
                    }
                }
                Rectangle {
                    id:rectangle4
                    anchors.left: rectangle3.right
                    width:70
                    height: parent.height
                    Text {
                        anchors.centerIn: parent
                        renderType: Text.NativeRendering
                        text: chance_of_rain+"%"
                    }
                }
                Rectangle {
                    id:rectangle5
                    anchors.left: rectangle4.right
                    width:150
                    height: parent.height
                    Text {
                        anchors.centerIn: parent
                        renderType: Text.NativeRendering
                        text: comment
                    }
                }

            }
        }
    }
}
