#include "entity.h"

Entity::Entity() {}#include "entity.h"
// Hàm khởi tạo: Nạp các giá trị x, y, width, height vào các biến thành viên tương ứng
Entity::Entity(float x, float y, float width, float height)
    : m_x(x), m_y(y), m_width(width), m_height(height) {}

// Mỗi lần vòng lặp game (Game Loop) chạy, hàm này sẽ được gọi để cộng dồn tọa độ Y
void Entity::updatePosition(int dy) {
    m_y += dy; // Vì trục Y của Qt tăng dần từ trên xuống, cộng thêm dy nghĩa là vật thể đang rơi xuống
}

// Thằng làm Logic va chạm sẽ xài hàm này để lấy ra cái khung bảo vệ của vật thể
QRectF Entity::getBoundingBox() const {
    // Trả về một đối tượng QRectF chứa đủ 4 thông số: tọa độ góc trên-trái và kích thước rộng-cao
    return QRectF(m_x, m_y, m_width, m_height);
}

// Thằng làm giao diện đồ họa sẽ gọi hàm này để biết thực thể đang đứng ở vị trí nào mà render ảnh
QPointF Entity::getPosition() const {
    // Trả về một điểm tọa độ chứa cặp số (x, y)
    return QPointF(m_x, m_y);
}
