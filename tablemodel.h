#ifndef TABLEMODEL_H
#define TABLEMODEL_H

#include <QObject>
#include <QAbstractTableModel>
#include <QString>

class TableModel : public QAbstractTableModel
{
    Q_OBJECT
    enum TableRoles {
        TableDataRole = Qt::UserRole + 1,
        HeadingRole
    };

public:
    explicit TableModel(QObject *parent = nullptr);
    int rowCount(const QModelIndex & = QModelIndex()) const override;
    int columnCount(const QModelIndex & = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

    Q_INVOKABLE void setCellText(int channel, int signal, QString value);

signals:
    void tableChanged();

public slots:
private:
    QVector<QVector<QString>> table;
};

#endif // TABLEMODEL_H
