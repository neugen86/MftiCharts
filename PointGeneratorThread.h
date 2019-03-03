#include <QtCore/QRectF>
#include <QtCore/QThread>
#include <QtCore/QRandomGenerator>

class QTimerEvent;

class PointGeneratorThread : public QThread
{
    Q_OBJECT

    int m_timerId = 0;

    QRectF m_bound;
    QRandomGenerator m_generator;

public:
    explicit PointGeneratorThread(QObject* parent = nullptr);
    ~PointGeneratorThread() override;

public slots:
    void updateBound(QRectF bound);

    void resume();
    void pause();

protected: // QObject
    void timerEvent(QTimerEvent* event) override;

signals:
    void generated(QPointF point);

};
