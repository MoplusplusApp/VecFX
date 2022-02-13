#ifndef KFTABLEMODEL_H
#define KFTABLEMODEL_H
#include <QAbstractTableModel>
#include <pugixml.hpp>
class kfTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    kfTableModel(QObject *parent = nullptr);
    pugi::xml_document kfTableXml;
    QStringList headers={"","","",""};
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
};
#endif // KFTABLEMODEL_H
