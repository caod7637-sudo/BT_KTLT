import QtQuick
import QtQuick.Controls

ApplicationWindow {
    id: window
    visible: true

    width: 450
    height: 700

    minimumWidth: 450
    minimumHeight: 700
    maximumWidth: 450
    maximumHeight: 700

    title: qsTr("Car Game Racing")

    MenuView {
        id: menuView
        anchors.fill: parent
        visible: true

        onStartGame: {
            menuView.visible = false
            gameView.visible = true
            gameEngine.startGame()
        }
    }

    GameView {
        id: gameView
        anchors.fill: parent
        visible: false
    }
        // Mẹo nhỏ: Nếu sau này trong GameView mắm làm thêm nút "Quay lại Menu"
        // và phát ra một cái signal tên là backToMenu(), thì mắm viết thêm đoạn này:
        /*
        onBackToMenu: {
            gameView.visible = false
            menuView.visible = true
        }
        */
}
