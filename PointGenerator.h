#pragma once

#include <QtCore/QObject>

#include "PointGeneratorThread.h"

class PointGenerator : public QObject
{
    Q_OBJECT

    bool m_paused = true;
    bool m_running = false;

    qreal m_minimumX = 0;
    qreal m_maximumX = 600;

    qreal m_minimumY = 0;
    qreal m_maximumY = 400;

    PointGeneratorThread m_thread;

    Q_PROPERTY(bool paused READ paused NOTIFY pausedChanged)
    Q_PROPERTY(bool running READ running NOTIFY runningChanged)

    Q_PROPERTY(qreal minimumX READ minimumX WRITE setMinimumX NOTIFY minimumXChanged)
    Q_PROPERTY(qreal maximumX READ maximumX WRITE setMaximumX NOTIFY maximumXChanged)

    Q_PROPERTY(qreal minimumY READ minimumY WRITE setMinimumY NOTIFY minimumYChanged)
    Q_PROPERTY(qreal maximumY READ maximumY WRITE setMaximumY NOTIFY maximumYChanged)

public:
    explicit PointGenerator(QObject* parent = nullptr);
    ~PointGenerator() override;

    bool paused() const { return m_paused; }
    bool running() const { return m_running; }

    qreal minimumX() const { return m_minimumX; }
    void setMinimumX(qreal value);

    qreal maximumX() const { return m_maximumX; }
    void setMaximumX(qreal value);

    qreal minimumY() const { return m_minimumY; }
    void setMinimumY(qreal value);

    qreal maximumY() const { return m_maximumY; }
    void setMaximumY(qreal value);

public slots:
    void toggleProcess();
    void stop();

private:
    void updateBound();

signals:
    void pausedChanged();
    void runningChanged();

    void minimumXChanged();
    void maximumXChanged();
    void minimumYChanged();
    void maximumYChanged();

    void generated(QPointF point);

signals:
    void updateGeneratorBound(QRectF bound);
    void resumeGenerator();
    void pauseGenerator();

};
