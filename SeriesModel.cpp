#include "SeriesModel.h"

#include <iterator>

#include "PointHandle.h"

SeriesModel::SeriesModel(QObject* parent)
    : QAbstractListModel(parent)
{
}

SeriesModel::~SeriesModel()
{
}

void SeriesModel::setSeries(QXYSeries* value)
{
    beginResetModel();

    m_series = value;
    reloadHandles();
    connectSeries();

    endResetModel();
}

void SeriesModel::insert(int index, const QPointF& value)
{
    m_series->insert(index, value);
}

void SeriesModel::update(int index, const QPointF& value)
{
    m_series->replace(index, value);
}

void SeriesModel::append(const QPointF& value)
{
    m_series->append(value);
}

void SeriesModel::remove(int index)
{
    m_series->remove(index);
}

void SeriesModel::clear()
{
    m_series->clear();
}

QHash<int, QByteArray> SeriesModel::roleNames() const
{
    return
    {
        { Roles::CoordX, "x" },
        { Roles::CoordY, "y" },
        { Roles::Point, "point" },
        { Roles::Handle, "handle" }
    };
}

int SeriesModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent)

    if (!m_series)
        return 0;

    return m_series->count();
}

QVariant SeriesModel::data(const QModelIndex& index, int role) const
{
    if (!m_series)
        return QVariant();

    const int row = index.row();

    switch (role)
    {
    case Roles::CoordX:
        return point(row).x();

    case Roles::CoordY:
        return point(row).y();

    case Roles::Point:
        return QVariant::fromValue(point(row));

    case Roles::Handle:
        return QVariant::fromValue(m_handles.at(size_t(row)).get());

    default:
        return QVariant();
    }
}

bool SeriesModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!m_series)
        return false;

    const int row = index.row();

    const QPointF oldPoint = point(row);
    QPointF newPoint = oldPoint;

    switch (role)
    {
    case Roles::CoordX:
        newPoint.setX(value.toDouble());
        break;

    case Roles::CoordY:
        newPoint.setY(value.toDouble());
        break;

    case Roles::Point:
        newPoint = value.value<QPointF>();
        break;

    default:
        return false;
    }

    if (oldPoint != newPoint)
    {
        m_series->replace(row, newPoint);
        emit dataChanged(index, index, { role });

        return true;
    }

    return false;
}

SeriesModel::PointHandlePtr SeriesModel::makeHandle(int index) const
{
    return std::make_unique<PointHandle>(m_series, index);
}

const QPointF& SeriesModel::point(int index) const
{
    return m_series->at(index);
}

void SeriesModel::reloadHandles()
{
    m_handles.clear();

    const int count = m_series->count();

    for (int index = 0; index < count; ++index)
    {
        m_handles.push_back(makeHandle(index));
    }
}

void SeriesModel::connectSeries()
{
    auto updateHandles = [this]()
    {
        int index = 0;

        for (auto& item : m_handles)
        {
            item->changeIndex(index);
            ++index;
        }
    };

    m_addConnection = QObject::connect(m_series, &QXYSeries::pointAdded,
                                       this, [this, updateHandles](int index)
    {
        beginInsertRows(QModelIndex(), index, index);

        m_handles.insert(m_handles.begin() + index, makeHandle(index));
        updateHandles();

        endInsertRows();
    });

    m_removeConnection = QObject::connect(m_series, &QXYSeries::pointRemoved,
                                          this, [this, updateHandles](int index)
    {
        beginRemoveRows(QModelIndex(), index, index);

        m_handles.erase(m_handles.begin() + index);
        updateHandles();

        endRemoveRows();
    });

    m_removeAllConnection = QObject::connect(m_series, &QXYSeries::pointsRemoved,
                                             this, [this, updateHandles](int index, int count)
    {
        beginRemoveRows(QModelIndex(), index, index + count - 1);

        auto from = m_handles.begin() + index;

        m_handles.erase(from, from + count);
        updateHandles();

        endRemoveRows();
    });

    m_replaceConnection = QObject::connect(m_series, &QXYSeries::pointReplaced,
                                           this, [this](int index)
    {
        QModelIndex modelIndex = QAbstractListModel::index(index);
        emit dataChanged(modelIndex, modelIndex);
    });
}
