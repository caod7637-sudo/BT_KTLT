#include "obstacle.h"

Obstacle::Obstacle(float x, float y)
    : Entity(x, y, 60, 110),
    m_lane(2),
    m_speed(8)
{
}

void Obstacle::setLane(int laneIndex)
{
    m_lane = laneIndex;
}

int Obstacle::getLane() const
{
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

void Obstacle::updatePosition(int dy)
{
    // Xe đi ngược chiều sẽ chạy từ trên xuống
    Entity::updatePosition(dy + m_speed);
}