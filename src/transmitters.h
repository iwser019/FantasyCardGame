#ifndef TRANSMITTERS_H
#define TRANSMITTERS_H

#include <QObject>
#include "defs.h"

/*!
 * \brief Класс StatsTransmitter - передатчик параметров
 * \details Передает объекту карты определенные параметры
 * Призван спасти код от циклической зависимости
 */
class StatsTransmitter : public QObject
{
		Q_OBJECT
	public:
		explicit StatsTransmitter(QObject *parent = 0);
	signals:
		/*!
		 * \brief Передача значения параметра карте
		 * \param statName Имя параметра
		 * \param statVal Целочисленное значение параметра
		 * \param isInc Флаг замены/добавления
		 */
		void sendStat(QString statName, int statVal, bool isInc);
	public slots:
		void setStat(QString statName, int statVal, bool isInc);

};

/*!
 * \brief Класс InfoTransmitter - передатчик сообщений
 * \details Используется для переачи текстовых сообщений, которые можно использовать,
 * к примеру, в журнале
 */
class InfoTransmitter : public QObject
{
		Q_OBJECT
	public:
		explicit InfoTransmitter(QObject *parent = 0);
	signals:
		/*!
		 * \brief Передача текстового сообщения
		 * \param msg Текст сообщения
		 */
		void sendMsg(QString msg);
	public slots:
		void receiveMsg(QString msg);
};

#endif // STATSTRANSMITTER_H
