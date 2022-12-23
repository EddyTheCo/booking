import QtQuick 2.0
import plugins.booking


ListView {
    id:horario
    required property int delegate_height
    required property bool can_book
    required property Hour_model horario_model
    model: horario_model
    delegate: Horario_delegate {
        height:horario.delegate_height
        width:horario.width
        can_book:horario.can_book
    }

}
