#include "PointHandle.h"

#include <QtCharts/QChart>

PointHandle::PointHandle(QXYSeries* series, int index)
    : QObject(series)
    , m_index(index)
    , m_series(series)
{
    QObject::connect(series, &QXYSeries::pointReplaced, this, [this](int index)
    {
        if (m_index == index)
            reload();
    });

    QObject::connect(series->chart(), &QChart::plotAreaChanged, this, [this]()
    {
        reload();
    });

    reload();
}

void PointHandle::setX(qreal value)
{
    if (qFuzzyCompare(m_point.x(), value))
        return;

    m_point.setX(value);
    emit xChanged();

    update();
}

void PointHandle::setY(qreal value)
{
    if (qFuzzyCompare(m_point.y(), value))
        return;

    m_point.setY(value);
    emit yChanged();

    update();
}

void PointHandle::setActive(bool value)
{
    if (m_active != value)
    {
        m_active = value;
        emit activeChanged();
    }
}

void PointHandle::changeIndex(int value)
{
    m_index = value;
    reload();
}

void PointHandle::reload()
{
    m_point = toPhysical(source());

    emit xChanged();
    emit yChanged();
}

void PointHandle::update()
{
    m_series->replace(m_index, toLogical(m_point));
}

const QPointF& PointHandle::source() const
{
    return m_series->at(m_index);
}

QPointF PointHandle::toLogical(const QPointF& value) const
{
    return m_series->chart()->mapToValue(value, m_series);
}

QPointF PointHandle::toPhysical(const QPointF& value) const
{
    return m_series->chart()->mapToPosition(value, m_series);
}
