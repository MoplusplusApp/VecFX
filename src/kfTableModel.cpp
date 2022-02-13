#include "kfTableModel.h"

kfTableModel::kfTableModel(QObject *parent)
    : QAbstractTableModel(parent)
{
}

int kfTableModel::rowCount(const QModelIndex & /*parent*/) const
{
   return 10;
}

int kfTableModel::columnCount(const QModelIndex & /*parent*/) const
{
    return 4;
}

QVariant kfTableModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole)
       return QString("Row%1, Column%2")
                   .arg(index.row() + 1)
                   .arg(index.column() +1);

    return QVariant();
}
