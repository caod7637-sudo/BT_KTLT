#include "obstacle.h"
#include <QPointF>
#include "entity.h"

Obstacle::Obstacle(float x, float y, float width, float height, Type type, QObject *parent)
    : QObject(parent), Entity(x, y, width, height), m_type(type),
    m_lane(0),
    m_speed(0)
{
}

float Obstacle::getWidth() const { return m_width; }
float Obstacle::getHeight() const { return m_height; }

void Obstacle::setLane(int laneIndex)
{
    m_lane = laneIndex;
}

int Obstacle::getLane() const
{
    const float LANE_LEFT_X  = 121;
    const float LANE_RIGHT_X = 219;
    return m_lane;
}

void Obstacle::setSpeed(int speed)
{
    m_speed = speed;
}

int Obstacle::getSpeed() const
{
    return m_speed;
}

Obstacle::Type Obstacle::getType() const
{
    return m_type;
}

QString Obstacle::imagePath() const
{
    switch (m_type)
    {
    case car1:
        return "assets/car1.png";

    case Car2:
        return "assets/Car2.png";

    case Car3:
        return "assets/Car3.png";

    case Car4:
        return "assets/Car4.png";

    case Barrier:
        return "assets/Obstacle.png";
    }

    return "";
}

void Obstacle::updatePosition(int speed) {
    m_y += speed;       // Cộng tọa độ Y cho xe chạy xuống
    emit yChanged();    // 🌟 BẮN TÍN HIỆU Ở ĐÂY: Báo riêng cho QML biết xe này vừa nhích xuống
}

QPointF Obstacle::getPosition() const {
    // Gọi thẳng hàm của "cha" Entity là xong
    return Entity::getPosition();
}
