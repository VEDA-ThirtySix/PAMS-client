#ifndef DBTABLE_H
#define DBTABLE_H

#include <QTableView>
#include <QAbstractTableModel>
#include <QList>
#include "metadata.h"

class DBtable : public QAbstractTableModel {
    Q_OBJECT
public:
    explicit DBtable(QObject *parent = nullptr);

    // QAbstractTableModel 필수 오버라이드 함수
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    // 데이터 업데이트 함수
    void updateBasicInfo(const QList<BasicInfo>& basicInfoList);
    void updateTimeInfo(const QList<TimeInfo>& timeInfoList);

private:
    QList<BasicInfo> m_basicInfoList;
    QList<TimeInfo> m_timeInfoList;
};


#endif// DBTABLE_H
