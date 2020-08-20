import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.12

ApplicationWindow {
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")
    Connections{
        target: xo_proxy
        function onStartNotify()
        {
            progress_view.changeText("Начало игры")
        }
        function onEndNotify(winner_name)
        {
            print("Конец игры")
            if (winner_name === ""){
              progress_view.changeText("Ничья")
            } else{
                progress_view.changeText("Конец игры. Победитель " + winner_name)
            }
            refresh_timer.start()
        }
        function onPlayerChanged(player_name)
        {
            progress_view.changeText("Ход игрока: " + player_name)
        }
    }
    Timer{
        id: refresh_timer
        interval: 2000
        repeat: false
        running: false
        onTriggered: {
            init_window.refresh()
            progress_view.visible = false
            init_window.visible = true
        }
    }

    InitWindow{
        id: init_window
        width: parent.width
        height: parent.height*0.1
        anchors.top: parent.top
        anchors.left: parent.left
        visible: true
        onInitRequest: {
            if (xo_registrator.registration(type1, type2) === true){
                print("success registration")
                init_window.visible = false
                progress_view.visible = true
            } else {
                print("failed registration")
            }
        }
    }
    ProgressView{
        id: progress_view
        width: parent.width
        height: parent.height*0.25
        anchors.top: parent.top
        anchors.left: parent.left
        visible: false
    }
    GridView{
        id: xo_view
        property  int margin: 8
        anchors{
            top: init_window.bottom
            left: parent.left
            right: parent.right
            bottom: parent.bottom
        }
        model: xo_model
        cellHeight: height/3
        cellWidth: width/3
        delegate: Item{
            implicitHeight: xo_view.cellHeight - xo_view.margin
            implicitWidth: xo_view.cellWidth - xo_view.margin
            Rectangle{
                color: "yellow"
                anchors.fill: parent
                Text{
                    id: field_image
                    anchors.fill: parent
                    text: model.state
                    fontSizeMode: Text.Fit
                }
                MouseArea{
                    id: field_mouse
                    anchors.fill: parent
                    acceptedButtons: Qt.LeftButton
                    onClicked: {
                        if (xo_proxy.clickRequest(index) === true){
                            print("success input")
                        } else {
                            print("failed input")
                        }
                    }
                }
            }
        }
    }
   }
