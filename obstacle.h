#ifndef OBSTACLE_H
#define OBSTACLE_H

#include "entity.h"

class Obstacle : public Entity
{
public:
    Obstacle(float x = 0, float y = -110);

    void setLane(int laneIndex);
    int getLane() const;

    void setSpeed(int speed);
    int getSpeed() const;

    void updatePosition(int dy) override;

private:
    int m_lane;
    int m_speed;
};

#endif 