#include "dbTable.h"

DBtable::DBtable(QObject *parent)
    : QAbstractTableModel(parent)
{
}

int DBtable::rowCount(const QModelIndex &parent) const
{
    return m_basicInfoList.size();
}

int DBtable::columnCount(const QModelIndex &parent) const
{
    return 6; // 번호판, 동호수, 연락처, 입출차시간, 타입
}

QVariant DBtable::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || role != Qt::DisplayRole)
        return QVariant();

    int row = index.row();
    int col = index.column();

    switch(col) {
    case 0: return m_basicInfoList[row].get_plate();
    case 1: return m_basicInfoList[row].get_home();
    case 2: return m_basicInfoList[row].get_phone();
    case 3:
        if (row < m_timeInfoList.size())
            return m_timeInfoList[row].get_time().toString("yyyy-MM-dd hh:mm:ss");
        break;
    case 4:
        if (row < m_timeInfoList.size())
            return m_timeInfoList[row].get_type();
        break;
    }
    return QVariant();
}

QVariant DBtable::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal) {
        switch (section) {
        case 0: return "번호판";
        case 1: return "동호수";
        case 2: return "연락처";
        case 3: return "시간";
        case 4: return "타입";
        default: return QVariant();
        }
    }
    return QVariant();
}

void DBtable::updateBasicInfo(const QList<BasicInfo>& basicInfoList)
{
    beginResetModel();
    m_basicInfoList = basicInfoList;
    endResetModel();
}

void DBtable::updateTimeInfo(const QList<TimeInfo>& timeInfoList)
{
    beginResetModel();
    m_timeInfoList = timeInfoList;
    endResetModel();
}
