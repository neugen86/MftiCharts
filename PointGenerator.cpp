#include "PointGenerator.h"

PointGenerator::PointGenerator(QObject* parent)
    : QObject(parent)
{
    QObject::connect(this, &PointGenerator::updateGeneratorBound,
                     &m_thread, &PointGeneratorThread::updateBound);

    QObject::connect(this, &PointGenerator::resumeGenerator,
                     &m_thread, &PointGeneratorThread::resume);

    QObject::connect(this, &PointGenerator::pauseGenerator,
                     &m_thread, &PointGeneratorThread::pause);

    QObject::connect(&m_thread, &PointGeneratorThread::generated,
                     this, &PointGenerator::generated);

    updateBound();
}

PointGenerator::~PointGenerator()
{
    stop();
}

void PointGenerator::setMinimumX(qreal value)
{
    if (!qFuzzyCompare(m_minimumX, value))
    {
        m_minimumX = value;
        emit minimumXChanged();

        updateBound();
    }
}

void PointGenerator::setMaximumX(qreal value)
{
    if (!qFuzzyCompare(m_maximumX, value))
    {
        m_maximumX = value;
        emit maximumXChanged();

        updateBound();
    }
}

void PointGenerator::setMinimumY(qreal value)
{
    if (!qFuzzyCompare(m_minimumY, value))
    {
        m_minimumY = value;
        emit minimumYChanged();

        updateBound();
    }
}

void PointGenerator::setMaximumY(qreal value)
{
    if (!qFuzzyCompare(m_maximumY, value))
    {
        m_maximumY = value;
        emit maximumYChanged();

        updateBound();
    }
}

void PointGenerator::toggleProcess()
{
    if (paused())
    {
        if (!m_running)
        {
            m_thread.start();

            m_running = true;
            emit runningChanged();
        }

        emit resumeGenerator();
    }
    else
    {
        emit pauseGenerator();
    }

    m_paused = !m_paused;
    emit pausedChanged();
}

void PointGenerator::stop()
{
    emit pauseGenerator();

    m_paused = true;
    emit pausedChanged();

    m_thread.quit();
    m_thread.wait();

    m_running = false;
    emit runningChanged();
}

void PointGenerator::updateBound()
{
    const QPointF topLeft(m_minimumX, m_minimumY);
    const QPointF bottomRight(m_maximumX, m_maximumY);

    emit updateGeneratorBound(QRectF(topLeft, bottomRight));
}
