import QtQuick
import QtQuick.Controls

Item {
    id: gameRoot

    width: 400
    height: 380

    focus: true
    property int scrollSpeed: 5

        property var lanePositions: [120, 165, 240]
        property int currentLaneIndex: 0

        signal startGame()


        Image {
            id: bg1
            width: parent.width
            height: parent.height
            y: 0
            source: "assets/main.png"
            fillMode: Image.Stretch
        }

        Image {
            id: bg2
            width: parent.width
            height: parent.height
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

                if (bg1.y >= gameRoot.height) {
                    bg1.y = bg2.y - gameRoot.height
                }
                if (bg2.y >= gameRoot.height) {
                    bg2.y = bg1.y - gameRoot.height
                }
            }
        }

        Image {
            id: playerCar
            source: "assets/Car.png"

            width: 90
            height: 110
            fillMode: Image.PreserveAspectFit

            anchors.bottom: parent.bottom
            anchors.bottomMargin: 30

            // Tọa độ X tự động đồng bộ theo mảng vị trí cố định
            x: gameRoot.lanePositions[gameRoot.currentLaneIndex]

            // Hiệu ứng giúp xe lướt qua lướt lại mượt mà thay vì nhảy giật cục
            Behavior on x {
                NumberAnimation {
                    duration: 120
                    easing.type: Easing.OutQuad
                }
            }
        }


        Keys.onPressed: (event) => {
            if (event.key === Qt.Key_Left) {
                if (gameRoot.currentLaneIndex > 0) {
                    gameRoot.currentLaneIndex--;
                }
            }
            else if (event.key === Qt.Key_Right) {
                if (gameRoot.currentLaneIndex < 2) {
                    gameRoot.currentLaneIndex++;
                }
            }
            else if (event.key === Qt.Key_Up) {
                if (gameRoot.scrollSpeed < 12) {
                    gameRoot.scrollSpeed += 1;
                }
            }
            else if (event.key === Qt.Key_Down) {
                if (gameRoot.scrollSpeed > 2) {
                    gameRoot.scrollSpeed -= 1;
                }
            }
        }
    }
