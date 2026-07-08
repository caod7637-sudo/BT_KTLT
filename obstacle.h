#ifndef OBSTACLE_H
#define OBSTACLE_H

#include <QObject>
#include <QString>
#include <QAbstractListModel>
#include <QList>
#include <QPointF>
#include "entity.h"

class Obstacle : public QObject, public Entity {
    Q_OBJECT

    Q_PROPERTY(float x READ getX CONSTANT)
    Q_PROPERTY(float y READ getY NOTIFY yChanged)

public:
    enum Type { car1, Car2, Car3, Car4, Barrier };

    Obstacle(float x, float y, float width, float height, Type type, QObject *parent = nullptr);

    float getX() const { return getPosition().x(); }
    float getY() const { return getPosition().y(); }

    Q_INVOKABLE float getWidth() const;
    Q_INVOKABLE float getHeight() const;
    Q_INVOKABLE QString imagePath() const;
    Q_INVOKABLE QPointF getPosition() const;

    void setLane(int laneIndex);
    int getLane() const;
    void setSpeed(int speed);
    int getSpeed() const;
    Type getType() const;

    void updatePosition(int speed) override;

signals:
    void yChanged();

private:
    int m_lane;
    int m_speed;
    Type m_type;
};

class ObstacleModel : public QAbstractListModel {
    Q_OBJECT
public:
    enum Roles { ObstacleRole = Qt::UserRole + 1 };

    explicit ObstacleModel(QObject *parent = nullptr) : QAbstractListModel(parent) {}

    int rowCount(const QModelIndex & = QModelIndex()) const override
    {
        return m_list.size();
    }

    QVariant data(const QModelIndex &index, int role) const override
    {
        if (!index.isValid() || index.row() >= m_list.size())
            return {};
        if (role == ObstacleRole)
            return QVariant::fromValue<QObject*>(m_list.at(index.row()));
        return {};
    }

    QHash<int, QByteArray> roleNames() const override
    {
        return { { ObstacleRole, "obstacle" } };
    }

    void addObstacle(Obstacle *obs)
    {
        beginInsertRows(QModelIndex(), m_list.size(), m_list.size());
        m_list.append(obs);
        endInsertRows();
    }

    void removeAt(int row)
    {
        if (row < 0 || row >= m_list.size())
            return;
        beginRemoveRows(QModelIndex(), row, row);
        Obstacle *obs = m_list.takeAt(row);
        delete obs;
        endRemoveRows();
    }

    void clearAll()
    {
        if (m_list.isEmpty())
            return;
        beginResetModel();
        qDeleteAll(m_list);
        m_list.clear();
        endResetModel();
    }

    Obstacle* at(int row) const { return m_list.value(row); }
    int count() const { return m_list.size(); }

private:
    QList<Obstacle*> m_list;
};

#endif // OBSTACLEMODEL_H
