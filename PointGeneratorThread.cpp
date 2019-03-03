#include "PointGeneratorThread.h"

PointGeneratorThread::PointGeneratorThread(QObject* parent)
    : QThread(parent)
{
}

PointGeneratorThread::~PointGeneratorThread()
{
    pause();
}

void PointGeneratorThread::updateBound(QRectF bound)
{
    m_bound = bound;
}

void PointGeneratorThread::resume()
{
    if (!m_timerId)
        m_timerId = startTimer(1000);
}

void PointGeneratorThread::pause()
{
    if (m_timerId)
    {
        killTimer(m_timerId);
        m_timerId = 0;
    }
}

void PointGeneratorThread::timerEvent(QTimerEvent* event)
{
    Q_UNUSED(event)

    const double x = m_generator.bounded(m_bound.width()) + m_bound.x();
    const double y = m_generator.bounded(m_bound.height()) + m_bound.y();

    emit generated(QPointF(x, y));
}
