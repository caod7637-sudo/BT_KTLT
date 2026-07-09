import QtQuick
import QtQuick.Controls
import QtMultimedia

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
            visible: true // Mới mở app lên thì Menu hiện

            onStartGame: {
                menuView.visible = false // Ẩn Menu
                gameView.visible = true  // Hiện Game
                gameEngine.startGame() // Báo C++ reset điểm, sinh xe
                bgMusic.play()
            }
        }

        // --- MÀN HÌNH GAME ---
    GameView {
        id: gameView
        anchors.fill: parent
        visible: false // Mới mở app lên thì Game ẩn

        // 🌟 BẮT TÍN HIỆU ĐƯỢC PHÁT RA TỪ NÚT EXIT Ở BƯỚC TRƯỚC
        onReturnToMenu: {
            gameView.visible = false // Ẩn Game
            menuView.visible = true  // Hiện Menu lại
        }
    }
    MediaPlayer {
            id: bgMusic
            source: "sounds/game.wav" // Nhớ kiểm tra đúng đường dẫn file nhạc

            audioOutput: AudioOutput {
                volume: 0.7
            }
            loops: MediaPlayer.Infinite
            // (Không có Component.onCompleted ở đây nhé)
        }
    SoundEffect {
            id: coinSound
            source: "sounds/coin.mp3" // 🌟 Nhớ chuẩn bị file và thay đúng đường dẫn nhé
            volume: 1.0
        }

        SoundEffect {
            id: crashSound
            source: "sounds/crash.wav" // 🌟 Nhớ chuẩn bị file và thay đúng đường dẫn nhé
            volume: 1.0
        }

        // 2. BẮT TÍN HIỆU TỪ C++ ĐỂ PHÁT NHẠC
        Connections {
            target: gameEngine

            function onCoinCollected() {
                coinSound.play()
            }

            function onCrashed() {
                crashSound.play()
                bgMusic.stop() // Tùy chọn: Tắt nhạc nền xập xình khi game over cho tăng độ bi thảm
            }
        }
    }
