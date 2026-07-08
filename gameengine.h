#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include <QObject>
#include <QTimer>
#include <QList>
#include "Model/car.h"
#include "Model/obstacle.h"

class GameEngine : public QObject {
    Q_OBJECT

public:
    explicit GameEngine(QObject *parent = nullptr);
    ~GameEngine();

    Q_INVOKABLE void updatePlayerPosition(float x, float y) {
        if (m_playerCar) {
            m_playerCar->setX(x);
            m_playerCar->setY(y);
        }
    }

    Q_INVOKABLE void startGame();
    Q_INVOKABLE void pauseGame();

    Car* getPlayerCar() const { return m_playerCar; }
    int getPlayerScore() const { return m_playerScore; }
    bool isGameOver() const { return m_isGameOver; }

    QObject* getObstacleModel() const { return m_obstacleModel; }

signals:
    void scoreChanged(int newScore);
    void gameOverChanged();

private slots:
    void updateGameTick();

private:
    void spawnEnemy();
    void checkCollisions();

    QTimer *m_gameTimer;
    Car *m_playerCar;
    ObstacleModel *m_obstacleModel;
    int m_scrollSpeed = 6;
    int m_playerScore;
    bool m_isGameOver;
    int m_spawnCounter;
    int m_dynamicSpawnLimit;
};

#endif // GAMEENGINE_H
