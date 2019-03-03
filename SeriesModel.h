#pragma once

#include <memory>
#include <vector>

#include <QtCore/QPointer>
#include <QtCore/QAbstractListModel>

#include <QtCharts/QXYSeries>

QT_CHARTS_USE_NAMESPACE

class PointHandle;

class SeriesModel : public QAbstractListModel
{
    Q_OBJECT

    enum Roles
    {
        CoordX, CoordY, Point, Handle
    };

    using PointHandlePtr = std::unique_ptr<PointHandle>;

    QPointer<QXYSeries> m_series;
    std::vector<PointHandlePtr> m_handles;

    QMetaObject::Connection m_addConnection;
    QMetaObject::Connection m_removeConnection;
    QMetaObject::Connection m_removeAllConnection;
    QMetaObject::Connection m_replaceConnection;

public:
    explicit SeriesModel(QObject* parent = nullptr);
    ~SeriesModel() override;

public slots:
    void setSeries(QXYSeries* value);

    void insert(int index, const QPointF& value);
    void update(int index, const QPointF& value);
    void append(const QPointF& value);
    void remove(int index);
    void clear();

public: // QAbstractItemModel
    QHash<int, QByteArray> roleNames() const override;
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;

private:
    PointHandlePtr makeHandle(int index) const;
    const QPointF& point(int index) const;
    void reloadHandles();
    void connectSeries();

};
