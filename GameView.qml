import QtQuick
import QtQuick.Controls
import QtQuick 2.15
import QtMultimedia

Item {
    id: gameRoot

    width: 400
    height: 380
    focus: true

    property int scrollSpeed: 5
    property var lanePositions: [131, 200, 229]
    property int currentLaneIndex: 1 // LƯU Ý: ĐÂY LÀ 3 LÀN ĐƯỜNG

    signal startGame()
    signal returnToMenu()

    // --- BACKGROUND TỰ CUỘN ---
    Image {
        id: bg1
        width: parent.width; height: parent.height
        y: 0
        source: "assets/main.png"
        fillMode: Image.Stretch
    }

    Image {
        id: bg2
        width: parent.width; height: parent.height
        y: -parent.height
        source: "assets/main.png"
        fillMode: Image.Stretch
    }

    Timer {
        id: gameTimer
        interval: 16
        repeat: true
        running: gameRoot.visible

        onTriggered: {
            bg1.y += gameRoot.scrollSpeed
            bg2.y += gameRoot.scrollSpeed

            if (bg1.y >= gameRoot.height) bg1.y = bg2.y - gameRoot.height
            if (bg2.y >= gameRoot.height) bg2.y = bg1.y - gameRoot.height
        }
    }

    // --- XE NGƯỜI CHƠI ---
    Image {
        id: playerCar
        source: "assets/Car.png"
        width: 90; height: 110
        fillMode: Image.PreserveAspectFit
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 30
        x: gameRoot.lanePositions[gameRoot.currentLaneIndex]

        onXChanged: gameEngine.updatePlayerPosition(x, y)
        onYChanged: gameEngine.updatePlayerPosition(x, y)

        Behavior on x {
            NumberAnimation { duration: 120; easing.type: Easing.OutQuad }
        }

        // 🔴 MẮT THẦN TEST VA CHẠM (XE PLAYER)
        Rectangle {
            anchors.fill: parent
            anchors.margins: 10 // Số lượng pixel bị gọt (Sửa số này để test)
            color: "transparent"
            border.color: "red"
            border.width: 2
        }
    }

    // --- VẬT CẢN ---
    Repeater {
            // 1. 🌟 ĐỔI PROPERTY: Từ 'gameEngine.obstacles' thành 'gameEngine.obstacleModel'
            model: gameEngine.obstacleModel

            Image {
                // 2. 🌟 THAY 'modelData' THÀNH 'obstacle' (Tên role bạn định nghĩa trong C++)
                x: obstacle.x
                y: obstacle.y
                width: obstacle.getWidth()
                height: obstacle.getHeight()
                source: obstacle.imagePath()
                fillMode: Image.PreserveAspectFit
                z: 5

                Behavior on y {
                    NumberAnimation {
                        duration: 16
                        easing.type: Easing.Linear
                    }
                }

                // Mắt thần hitbox vật cản (đã gọt 15px như xe player)
                Rectangle {
                    anchors.fill: parent
                    anchors.margins: 15
                    color: "transparent"
                    border.color: "red"
                    border.width: 2
                }
            }
        }

// Đã thêm dấu } đóng cho thẻ Repeater
    Connections {
        target: gameEngine

        function onGameOverChanged() {
            gameOverOverlay.visible = true
        }
    }

    // --- MÀN HÌNH GAME OVER ---
    Item {
            id: gameOverOverlay
            anchors.fill: parent
            visible: false
            z: 10

            MouseArea { anchors.fill: parent }

            // Ảnh chữ GAME OVER - Ép đầy khung để giữ nguyên vị trí vẽ gốc
            Image {
                id: gameOverTitle
                source: "assets/over.png"
                anchors.fill: parent
                fillMode: Image.PreserveAspectFit
            }

            // Ảnh nút RESTART - Ép đầy khung để giữ nguyên vị trí vẽ gốc
            Image {
                id: restartBtn
                source: "assets/replay.png"
                anchors.fill: parent
                fillMode: Image.PreserveAspectFit

                MouseArea {
                    anchors.fill: parent
                    cursorShape: Qt.PointingHandCursor
                    onClicked: {
                        gameOverOverlay.visible = false
                        gameEngine.startGame()
                        bgMusic.play()
                    }
                }
            }

            // Ảnh nút EXIT - Ép đầy khung để giữ nguyên vị trí vẽ gốc
            Image {
                id: exitBtn
                source: "assets/exit.png"
                anchors.fill: parent
                fillMode: Image.PreserveAspectFit

                MouseArea {
                    anchors.fill: parent
                    cursorShape: Qt.PointingHandCursor
                    onClicked: {
                        // 🌟 THAY VÌ THOÁT GAME (Qt.quit()), BẠN GỌI CÁC LỆNH SAU:
                        bgMusic.stop()            // 1. Tắt nhạc game
                        gameOverOverlay.visible = false // 2. Ẩn màn hình Game Over đi
                        gameRoot.returnToMenu()   // 3. Phát tín hiệu đòi về Menu
                    }
                }
            }
        }
    // --- ĐIỀU KHIỂN BÀN PHÍM ---
    Keys.onPressed: (event) => {
        if (event.key === Qt.Key_Left) {
            if (gameRoot.currentLaneIndex > 0) gameRoot.currentLaneIndex--;
        }
        else if (event.key === Qt.Key_Right) {
            // Lưu ý: lanePositions đang có 3 phần tử (index 0, 1, 2)
            if (gameRoot.currentLaneIndex < 2) gameRoot.currentLaneIndex++;
        }
        else if (event.key === Qt.Key_Up) {
            if (gameRoot.scrollSpeed < 12) gameRoot.scrollSpeed += 1;
        }
        else if (event.key === Qt.Key_Down) {
            if (gameRoot.scrollSpeed > 2) gameRoot.scrollSpeed -= 1;
        }
    }
}
