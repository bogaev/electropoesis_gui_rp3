#include "backend.h"
#include <QMessageBox>

BackEnd::BackEnd(QObject *parent) :
      QObject(parent)
      ,singleSelect(Qt::Unchecked)
      ,checkbox0(Qt::Checked)
      ,checkbox1(Qt::Checked)
      ,checkbox2(Qt::Unchecked)
      ,checkbox3(Qt::Unchecked)
      ,checkbox4(Qt::Unchecked)
      ,checkbox5(Qt::Unchecked)
      ,checkbox6(Qt::Unchecked)
      ,checkbox7(Qt::Unchecked)
      ,checkboxIndex(0)
      ,uart(parent)
{
    checkboxes[0] = &checkbox0;
    checkboxes[1] = &checkbox1;
    checkboxes[2] = &checkbox2;
    checkboxes[3] = &checkbox3;
    checkboxes[4] = &checkbox4;
    checkboxes[5] = &checkbox5;
    checkboxes[6] = &checkbox6;
    checkboxes[7] = &checkbox7;
}

void BackEnd::UartTest(int qUartTest) {
    QString debugMessage;
    auto error = uart.openSerialPort();
    debugMessage += error;
    debugMessage += uart.writeData("Test");
    debugMessage += uart.closeSerialPort();
    debugMessage += "Uart Test End\n";
    emit uartStatusChanged(debugMessage);
}

QString BackEnd::userName()
{
    return m_userName;
}

void BackEnd::setUserName(const QString &userName)
{
    if (userName == m_userName)
        return;

    m_userName = userName;
    emit userNameChanged();
}

Qt::CheckState BackEnd::getCheckboxStatus0(void)
{
    return *(checkboxes[0]);
}

Qt::CheckState BackEnd::getCheckboxStatus1(void)
{
    return *(checkboxes[1]);
}

Qt::CheckState BackEnd::getCheckboxStatus2(void)
{
    return *(checkboxes[2]);
}

Qt::CheckState BackEnd::getCheckboxStatus3(void)
{
    return *(checkboxes[3]);
}

Qt::CheckState BackEnd::getCheckboxStatus4(void)
{
    return *(checkboxes[4]);
}

Qt::CheckState BackEnd::getCheckboxStatus5(void)
{
    return *(checkboxes[5]);
}

Qt::CheckState BackEnd::getCheckboxStatus6(void)
{
    return *(checkboxes[6]);
}

Qt::CheckState BackEnd::getCheckboxStatus7(void)
{
    return *(checkboxes[7]);
}

void BackEnd::setCheckboxStatus(Qt::CheckState status)
{
    *(checkboxes[checkboxIndex]) = status;
    if(singleSelect)
    {
        for(int i=0; i<8; i++)
        {
            if(i != checkboxIndex)
            {
                *(checkboxes[i]) = Qt::Unchecked;
            }
        }
    }
    emit checkboxChanged();
}

void BackEnd::setCheckboxIndex(int qCheckboxIndex)
{
    checkboxIndex = qCheckboxIndex;
}

void BackEnd::setSingleSelect(bool qSingleSelect)
{
    singleSelect = qSingleSelect;
}
