#include "gameengine.h"
#include "Model/obstacle.h"
#include "Model/car.h"
#include "Model/entity.h"
#include <QRandomGenerator>

const float LANE_LEFT_X  = 131;
const float LANE_RIGHT_X = 229; // Tọa độ X của làn bên PHẢI
const float SPAWN_Y_START = -180; // Xuất hiện âm hẳn lên trên để ẩn giấu lúc mới sinh

GameEngine::GameEngine(QObject *parent)
    : QObject(parent),
    m_gameTimer(new QTimer(this)),
    m_playerCar(nullptr),
    m_obstacleModel(new ObstacleModel(this)), // 🌟 ĐÃ SỬA: Khởi tạo model mới quản lý vật cản
    m_playerScore(0),
    m_isGameOver(false),
    m_spawnCounter(0)
{
    m_gameTimer->setTimerType(Qt::PreciseTimer);
    connect(m_gameTimer, &QTimer::timeout, this, &GameEngine::updateGameTick);
}

GameEngine::~GameEngine()
{
    delete m_playerCar;
    m_obstacleModel->clearAll(); // 🌟 ĐÃ SỬA: Giải phóng bộ nhớ thông qua Model
}

void GameEngine::startGame()
{
    if (m_playerCar) delete m_playerCar;
    m_playerCar = new Car(200, 550, 90, 110);

    m_obstacleModel->clearAll();

    m_playerScore = 0;
    m_isGameOver = false;
    m_spawnCounter = 0;
    m_scrollSpeed = 3; // Vận tốc nền ban đầu nhẹ nhàng
    m_dynamicSpawnLimit = 150;

    emit scoreChanged(m_playerScore);
    m_gameTimer->start(16);
}

void GameEngine::pauseGame()
{
    if (m_gameTimer->isActive()) {
        m_gameTimer->stop();
    }
}

void GameEngine::updateGameTick()
{
    if (m_isGameOver) return;

    // Di chuyển vật cản xuống bằng cách duyệt qua Model mới
    for (int i = m_obstacleModel->count() - 1; i >= 0; --i)
    {
        Obstacle *obj = m_obstacleModel->at(i); // 🌟 ĐÃ SỬA: Lấy từ Model
        int currentSpeed = (obj->getType() != Obstacle::Barrier) ? m_scrollSpeed + 1 : m_scrollSpeed;

        obj->updatePosition(currentSpeed);

        if (obj->getPosition().y() > 700)
        {
            m_obstacleModel->removeAt(i);
            m_playerScore += 10;
            emit scoreChanged(m_playerScore);
        }
    }

    // Kiểm tra nhịp đếm để gọi sinh xe
    m_dynamicSpawnLimit = 150 - (m_playerScore / 4);

    if (m_dynamicSpawnLimit < 70) {
        m_dynamicSpawnLimit = 70;
    }

    m_spawnCounter++;

    if (m_spawnCounter >= m_dynamicSpawnLimit)
    {
        spawnEnemy();
        m_spawnCounter = 0;
    }

    checkCollisions();
}

void GameEngine::spawnEnemy()
{
    const float SPAWN_Y_NORMAL = -180;
    const float SPAWN_Y_BACK   = -340;

    int rate = QRandomGenerator::global()->bounded(100);

    if (rate < 30)
    {
        // Xe trái
        Obstacle* obsLeft = new Obstacle(LANE_LEFT_X, SPAWN_Y_NORMAL, 60, 110, Obstacle::car1, this);
        obsLeft->setLane(1);

        // Xe phải 1
        Obstacle::Type typeRight1 = static_cast<Obstacle::Type>(QRandomGenerator::global()->bounded(0, 4));
        Obstacle* obsRight1 = new Obstacle(LANE_RIGHT_X, SPAWN_Y_NORMAL, 90, 110, typeRight1, this);
        obsRight1->setLane(2);

        // Xe phải 2
        Obstacle::Type typeRight2 = static_cast<Obstacle::Type>(QRandomGenerator::global()->bounded(0, 4));
        Obstacle* obsRight2 = new Obstacle(LANE_RIGHT_X, SPAWN_Y_BACK, 90, 110, typeRight2, this);
        obsRight2->setLane(2);

        m_obstacleModel->addObstacle(obsLeft);
        m_obstacleModel->addObstacle(obsRight1);
        m_obstacleModel->addObstacle(obsRight2);
    }
    else
    {
        int lane = QRandomGenerator::global()->bounded(1, 3);
        float x = (lane == 1) ? LANE_LEFT_X : LANE_RIGHT_X;

        Obstacle::Type type = static_cast<Obstacle::Type>(QRandomGenerator::global()->bounded(0, 5));
        float width = 90;
        float height = 110;

        if (type == Obstacle::Barrier)
        {
            width = 100;
            height = 150;
        }

        Obstacle* obs = new Obstacle(x, SPAWN_Y_NORMAL, width, height, type, this);
        obs->setLane(lane);
        m_obstacleModel->addObstacle(obs); // 🌟 ĐÃ SỬA: Thêm vào Model

        if (QRandomGenerator::global()->bounded(100) < 35)
        {
            int secondLane = (lane == 1) ? 2 : 1;
            float x2 = (secondLane == 1) ? LANE_LEFT_X : LANE_RIGHT_X;

            Obstacle* obs2 = new Obstacle(x2, SPAWN_Y_NORMAL, 60, 110, Obstacle::car1, this);
            obs2->setLane(secondLane);

            m_obstacleModel->addObstacle(obs2); // 🌟 ĐÃ SỬA: Thêm vào Model
        }
    }
}

void GameEngine::checkCollisions()
{
    if (!m_playerCar || m_isGameOver) return;

    QRectF playerBox = m_playerCar->getBoundingBox();
    QRectF playerHitbox = playerBox.adjusted(10, 10, -10, -10);

    for (int i = 0; i < m_obstacleModel->count(); ++i)
    {
        Obstacle* obs = m_obstacleModel->at(i);
        QRectF obsBox = obs->getBoundingBox();
        QRectF obsHitbox = obsBox.adjusted(10, 10, -10, -10);

        if (playerHitbox.intersects(obsHitbox))
        {
            m_isGameOver = true;
            m_gameTimer->stop();
            emit gameOverChanged();
            break;
        }
    }
}
