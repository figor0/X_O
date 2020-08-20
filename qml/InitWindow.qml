import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.12

Item
{
    id: root
    property int player1_type: -1
    property int player2_type: -1
    property string player1_name: "Player1"
    property string player2_name: "Player2"
    signal initRequest(int type1, int type2)
    function refresh(){
        player1_type = -1;
        player2_type = -1;
        player1_menu.title = player1_name
        player2_menu.title = player2_name
    }
    MenuBar{
        id: players
        width: parent.width*0.6
        height: parent.height
        anchors{
            left: parent.left
            top: parent.top
            bottom: parent.bottom
        }
        implicitWidth: width*0.5
        implicitHeight: height
        Menu{
            id: player1_menu
            title: qsTr(player1_name)
            Action {
                text: qsTr("Player")
                onTriggered: {
                    player1_type = 0;
                    player1_menu.title = text
                }
            }
            Action {
                text: qsTr("SimpleAI")
                enabled: player2_menu.title != "SimpleAI" && player2_menu.title != "SmartAI"
                onTriggered: {
                   player1_type = 1;
                   player1_menu.title = text
                }
            }
            Action {
                text: qsTr("SmartAI")
                enabled: player2_menu.title != "SimpleAI" && player2_menu.title != "SmartAI"
                onTriggered: {
                   player1_type = 2;
                   player1_menu.title = text
                }
            }
        }
        Menu{
            id: player2_menu
            title: qsTr(player2_name)
            Action {
                text: qsTr("Player")
                onTriggered: {
                    player2_type = 0;
                    player2_menu.title = text
                }
            }
            Action {
                text: qsTr("SimpleAI")
                enabled: player1_menu.title != "SimpleAI" && player1_menu.title != "SmartAI"
                onTriggered: {
                   player2_type = 1;
                   player2_menu.title = text
                }
            }
            Action {
                text: qsTr("SmartAI")
                enabled: player1_menu.title != "SimpleAI" && player1_menu.title != "SmartAI"
                onTriggered: {
                   player2_type = 2;
                   player2_menu.title = text
                }
            }
        }

    }
    Button{
        id: commiter
        height: parent.height
        width: parent.width*0.3
        text: qsTr("Start")
        anchors{
            left: players.right
            leftMargin: parent.width*0.05
            top: parent.top
        }
        onClicked: {
            if (player1_type != -1 && player2_type != -1){
                initRequest(player1_type, player2_type)
                refresh()
            }
        }
    }
}
