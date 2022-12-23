import QtQuick 2.0
import QtQuick.Controls
import plugins.booking

SwipeView {
    id: swipe_view
    required property Day_model day_model
    required property bool can_book
    currentIndex: 0

    Repeater {
            model: swipe_view.day_model
            delegate: Dia_delegate {
can_book:can_book
}
            }


}



