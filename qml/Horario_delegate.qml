import QtQuick 2.0
import QtQuick.Layouts

RowLayout
{    
id:root_box
required property bool can_book

//color:'transparent'
required property bool booked
required property bool selected
required property string hour
required property int index
spacing:4
    Text {
        id:time_
        text: root_box.hour
        color:"white"
        Layout.preferredWidth: 80
        Layout.minimumWidth: 50
        Layout.fillHeight: true

        Layout.maximumWidth: 200
        Layout.alignment: Qt.AlignTop
        horizontalAlignment: Text.AlignRight
    }
    Rectangle
    {
        id:line_
        Layout.preferredWidth: 25
        Layout.preferredHeight:  2
        Layout.maximumHeight:   20
        Layout.fillWidth: true
        Layout.alignment: Qt.AlignTop
        border.color:'white'
        border.width: 1
        height:1
        color:'white'
    }
    Rectangle
    {
        property bool choosen:false
        Layout.preferredWidth: 200
        Layout.fillHeight: true
        Layout.fillWidth: true
        Layout.alignment: Qt.AlignHCenter
        Layout.maximumWidth: 500
        border.color:'white'
        border.width: 1
        height:parent.height
        color: root_box.booked?'#e7001b':((choosen?'0f79af':'transparent'))

        opacity:0.7
        Text
        {
            anchors.centerIn: parent
            text:(parent.choosen)?'-':'+'
            visible:!root_box.booked&&root_box.can_book
        }

        MouseArea {
                anchors.fill: parent
                enabled:!root_box.booked&&root_box.can_book
                onClicked: {
                    parent.choosen = !parent.choosen
                    root_box.ListView.view.model.setProperty(root_box.index,"selected",parent.choosen)
                }
            }
    }



}
