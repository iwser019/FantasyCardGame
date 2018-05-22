#include "transmitters.h"

/*!
 * \brief Конструктор объекта передатчика параметров карт
 * \param parent Родительский объект класса QObject
 */
StatsTransmitter::StatsTransmitter(QObject *parent) : QObject(parent)
{

}

/*!
 * \brief Передача именованных параметров для карты
 * \param statName Наименование параметра
 * \param statVal Численное значение параметра
 * \param isInc Флаг, определяющий, заменять данные или добавлять
 */
void StatsTransmitter::setStat(QString statName, int statVal, bool isInc)
{
	emit sendStat(statName, statVal, isInc);
}

/*!
 * \brief Конструктор передатчика информационных сообщений
 * \param parent Родительский объект QObject
 */
InfoTransmitter::InfoTransmitter(QObject *parent) : QObject(parent)
{

}

/*!
 * \brief Отправка информационного сообщения
 * \param msg Текст сообщения
 */
void InfoTransmitter::receiveMsg(QString msg)
{
	sendMsg(msg);
}
