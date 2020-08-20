import QtQuick 2.0

Item {
    id: root
    signal changeText(string input_text)
    onChangeText: {
        print ("change text signal")
        progress_text.text = input_text;
    }
    Text{
        id: progress_text
        anchors.fill: parent
    }
}
