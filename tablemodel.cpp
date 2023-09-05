#include "tablemodel.h"

TableModel::TableModel(QObject *parent) : QAbstractTableModel(parent)
{
    table.append({"Канал", "Несущая", "Ампл. мод.", "Част. мод."});
    table.append({"1", "-", "-", "-"});
    table.append({"2", "-", "-", "-"});
    table.append({"3", "-", "-", "-"});
    table.append({"4", "-", "-", "-"});
    table.append({"5", "-", "-", "-"});
    table.append({"6", "-", "-", "-"});
    table.append({"7", "-", "-", "-"});
    table.append({"8", "-", "-", "-"});
}

int TableModel::rowCount(const QModelIndex &) const
{
    return table.size();
}

int TableModel::columnCount(const QModelIndex &) const
{
    return table.at(0).size();
}

QVariant TableModel::data(const QModelIndex &index, int role) const
{
    switch (role) {
    case TableDataRole: {
        return table.at(index.row()).at(index.column());
    }
    case HeadingRole:
        if(index.row()==0) {
            return true;
        }
        else {
            return false;
        }
    default:
        break;
    }

    return QVariant();
}

QHash<int, QByteArray> TableModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[TableDataRole] = "tabledata";
    roles[HeadingRole] = "heading";
    return roles;
}

void TableModel::setCellText(int channel, int signal, QString value)
{
    table[channel+1][signal+1] = value;

    QModelIndex startOfRow = this->index(channel+1, 0);
    QModelIndex endOfRow   = this->index(channel+1, signal+1);
    emit QAbstractItemModel::dataChanged(startOfRow, endOfRow);
}
