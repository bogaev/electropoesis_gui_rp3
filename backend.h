#ifndef BACKEND_H
#define BACKEND_H

#include <QObject>
#include <QString>
#include <QCheckBox>

#include "qserial_port.h"

class BackEnd : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString userName READ userName WRITE setUserName NOTIFY userNameChanged)
    Q_PROPERTY(Qt::CheckState qCheckboxState0 READ getCheckboxStatus0 WRITE setCheckboxStatus NOTIFY checkboxChanged)
    Q_PROPERTY(Qt::CheckState qCheckboxState1 READ getCheckboxStatus1 WRITE setCheckboxStatus NOTIFY checkboxChanged)
    Q_PROPERTY(Qt::CheckState qCheckboxState2 READ getCheckboxStatus2 WRITE setCheckboxStatus NOTIFY checkboxChanged)
    Q_PROPERTY(Qt::CheckState qCheckboxState3 READ getCheckboxStatus3 WRITE setCheckboxStatus NOTIFY checkboxChanged)
    Q_PROPERTY(Qt::CheckState qCheckboxState4 READ getCheckboxStatus4 WRITE setCheckboxStatus NOTIFY checkboxChanged)
    Q_PROPERTY(Qt::CheckState qCheckboxState5 READ getCheckboxStatus5 WRITE setCheckboxStatus NOTIFY checkboxChanged)
    Q_PROPERTY(Qt::CheckState qCheckboxState6 READ getCheckboxStatus6 WRITE setCheckboxStatus NOTIFY checkboxChanged)
    Q_PROPERTY(Qt::CheckState qCheckboxState7 READ getCheckboxStatus7 WRITE setCheckboxStatus NOTIFY checkboxChanged)
    Q_PROPERTY(int qCheckboxIndex WRITE setCheckboxIndex)
    Q_PROPERTY(bool qSingleSelect WRITE setSingleSelect)
    Q_PROPERTY(bool qSingleSelect WRITE setSingleSelect)
    Q_PROPERTY(int qUartTest WRITE UartTest)

public:
    explicit BackEnd(QObject *parent = nullptr);

    QString userName();
    void setUserName(const QString &userName);

    Qt::CheckState getCheckboxStatus0(void);
    Qt::CheckState getCheckboxStatus1(void);
    Qt::CheckState getCheckboxStatus2(void);
    Qt::CheckState getCheckboxStatus3(void);
    Qt::CheckState getCheckboxStatus4(void);
    Qt::CheckState getCheckboxStatus5(void);
    Qt::CheckState getCheckboxStatus6(void);
    Qt::CheckState getCheckboxStatus7(void);

    void setCheckboxStatus(Qt::CheckState status);
    void setCheckboxIndex(int qCheckboxIndex);
    void setSingleSelect(bool single);
    void UartTest(int qUartTest);

signals:
    void userNameChanged();
    void checkboxChanged();
    void uartStatusChanged(const QString &newStatus);

//private:
public:
    bool singleSelect;
    Qt::CheckState checkbox0;
    Qt::CheckState checkbox1;
    Qt::CheckState checkbox2;
    Qt::CheckState checkbox3;
    Qt::CheckState checkbox4;
    Qt::CheckState checkbox5;
    Qt::CheckState checkbox6;
    Qt::CheckState checkbox7;
    int checkboxIndex;
    Qt::CheckState* checkboxes[8];
    QString m_userName;
    QUart uart;
};

#endif // BACKEND_H
