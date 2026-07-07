#ifndef CAR_H              // Tránh include nhiều lần
#define CAR_H

#include "Entity.h"        // Kế thừa lớp Entity

class Car : public Entity
{
public:
    Car();                                     // Hàm khởi tạo mặc định
    Car(double x, double y, double width, double height);
    // x      : Tọa độ ngang của xe
    // y      : Tọa độ dọc của xe
    // width  : Chiều rộng xe
    // height : Chiều cao xe

    void setLane(int laneIndex);               // Đặt xe vào làn đường
    int getLane() const;                       // Lấy làn đường hiện tại

    void setSpeed(int speed);                  // Thiết lập tốc độ xe
    int getSpeed() const;                      // Lấy tốc độ xe

    void setHealth(int health);                // Thiết lập máu
    int getHealth() const;                     // Lấy máu

    void updatePosition(int dy) override;
    // dy: khoảng cách xe di chuyển theo trục Y

private:
    int m_currentLane; // Làn đường hiện tại (1: trái, 2: giữa, 3: phải)

    int m_speed;       // Tốc độ di chuyển của xe

    int m_health;      // Máu của xe
};

#endif
