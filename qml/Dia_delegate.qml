import QtQuick 2.0
import QtQuick.Layouts
import plugins.booking

ColumnLayout
{

    id:dia_delegate
    spacing:hours_container.delegate_height
    required property  date day
    required property  bool can_book
    required property  Hour_model hour_model
    Rectangle
    {

        id:day_label
        Layout.preferredHeight: 150
        Layout.minimumHeight:  50
        Layout.fillWidth: true

        Layout.alignment: Qt.AlignTop
        color: 'blue'
        ColumnLayout
        {

            anchors.fill: day_label
            Text
            {
                function dayOfWeekAsString(dayIndex) {
                  return ["Sun", "Mon","Tue","Wed","Thu","Fri","Sat"][dayIndex] || '';
                }
                text: dayOfWeekAsString(dia_delegate.day.getDay())
                font.pointSize:50
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.alignment: Qt.AlignCenter
                horizontalAlignment: Text.AlignHCenter
            }
            Text
            {
                text: dia_delegate.day.toLocaleDateString({ year: 'numeric', month: 'long', day: 'numeric' })
                font.pointSize:30
                Layout.minimumHeight:  25
                Layout.minimumWidth:  25
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.alignment: Qt.AlignCenter
                horizontalAlignment: Text.AlignHCenter
            }

        }






    }
    Horario_list_view
    {
        id:hours_container

        Layout.fillHeight: true
        Layout.preferredHeight: 500
        Layout.minimumHeight:  300
        Layout.fillWidth: true
        Layout.alignment: Qt.AlignBottom

        delegate_height:height/5
        can_book:can_book
        horario_model: dia_delegate.hour_model
    }






}


