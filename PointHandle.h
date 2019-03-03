#pragma once

#include <QtCore/QObject>

#include <QtCharts/QXYSeries>

QT_CHARTS_USE_NAMESPACE

class PointHandle : public QObject
{
    Q_OBJECT

    int m_index = -1;
    bool m_updating = false;

    QPointF m_point;
    bool m_active = false;

    QXYSeries* const m_series = nullptr;

    Q_PROPERTY(qreal x READ x WRITE setX NOTIFY xChanged)
    Q_PROPERTY(qreal y READ y WRITE setY NOTIFY yChanged)
    Q_PROPERTY(bool active READ active WRITE setActive NOTIFY activeChanged)

public:
    PointHandle(QXYSeries* series, int index);

    qreal x() const { return m_point.x(); }
    void setX(qreal value);

    qreal y() const { return m_point.y(); }
    void setY(qreal value);

    bool active() const { return m_active; }
    void setActive(bool value);

    void changeIndex(int value);

private:
    void reload();
    void update();

    const QPointF& source() const;
    QPointF toLogical(const QPointF& value) const;
    QPointF toPhysical(const QPointF& value) const;

signals:
    void xChanged();
    void yChanged();
    void activeChanged();

};
