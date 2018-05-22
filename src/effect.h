#pragma once
#ifndef IEFFECT_H
#define IEFFECT_H

#include <QObject>
#include "defs.h"
#include "transmitters.h"

const QList<QString> effectDict = {"AttackEffect",
								   "HelpEffect"};
class ICard;

/*!
 * \brief Класс IEffect - интерфейс действия карты
 */
class IEffect : public QObject
{
	Q_OBJECT
	public:
		IEffect(QObject *parent = 0);
		/*!
		 * \brief Получение имени
		 * \return Имя эффекта
		 */
		virtual QString getName() = 0;
		/*!
		 * \brief Установка нового имени
		 * \param effectName Новое имя
		 */
		virtual void setName(QString effectName) = 0;
		/*!
		 * \brief Получение именованного параметра
		 * \param statName Наименование параметра
		 * \return Целочисленное значение параметра
		 */
		virtual int getStat(QString statName) = 0;
		/*!
		 * \brief Установка значения параметра
		 * \param statName Наименование параметра
		 * \param statVal Целочисленное значение параметра
		 */
		virtual void setStat(QString statName, int statVal) = 0;
		/*!
		 * \brief Задействование эффекта
		 * \param cardsToAffect Карты, на которые направлено воздействие
		 * \param src Исходная карта
		 */
		virtual void act(QVector<StatsTransmitter *> cardsToAffect, StatsTransmitter *src) = 0;
		/*!
		 * \brief Получение полной информации
		 * \return Текст информации
		 */
		virtual QString getFullInfo() = 0;
		/*!
		 * \brief Получение информации
		 * \return Элемент информации об эффекте
		 */
		virtual effectInfoItem getInfo() = 0;
		/*!
		 * \brief Клонирование эффекта
		 * \return Объект эффекта-копия
		 */
		virtual IEffect * clone() = 0;
		/*!
		 * \brief Загрузка из потока данных
		 * \param stream Объект потока данных
		 */
		virtual void loadFromStream(QDataStream *stream) = 0;
		/*!
		 * \brief Сохранение в поток данных
		 * \param stream Объект потока данных
		 */
		virtual void saveToStream(QDataStream *stream) = 0;
	signals:
		/*!
		 * \brief Запрос на выбор карты
		 */
		void callCardSelect();
		/*!
		 * \brief Сигнал о завершении действия
		 */
		void done();
		/*!
		 * \brief Отправка информационного сообщения
		 * \param msg Текст сообщения
		 */
		void sendMsg(QString msg);
};

/*!
 * \brief Класс EffectChainlink - цепь задействования эффектов
 */
class EffectChainlink
{
	private:
		/*!
		 * \brief Объект эффекта
		 */
		IEffect *effect;
		/*!
		 * \brief Следующее звено
		 */
		EffectChainlink *next;
	public:
		EffectChainlink(IEffect *effect, EffectChainlink *next = nullptr);
		void act(QVector<StatsTransmitter *> cardsToAffect, StatsTransmitter *src);
		~EffectChainlink();
};

/*!
 * \brief Класс AttackEffect - эффект атаки на другую карту
 */
class AttackEffect : public IEffect
{
	Q_OBJECT
	private:
		/*!
		 * \brief Название
		 */
		QString name;
		/*!
		 * \brief Сила
		 */
		int power;
		/*!
		 * \brief Затрата энергии
		 */
		int cost;
	public:
		AttackEffect(QObject *parent = 0);
		QString getName();
		void setName(QString effectName);
		int getStat(QString statName);
		void setStat(QString statName, int statVal);
		void act(QVector<StatsTransmitter *> cardsToAffect, StatsTransmitter *src);
		QString getFullInfo();
		effectInfoItem getInfo();
		IEffect * clone();
		void loadFromStream(QDataStream *stream);
		void saveToStream(QDataStream *stream);
};

/*!
 * \brief Класс HelpEffect - эффект помощи другой карте
 */
class HelpEffect : public IEffect
{
	Q_OBJECT
	private:
		/*!
		 * \brief Название
		 */
		QString name;
		/*!
		 * \brief Мощность
		 */
		int amount;
		/*!
		 * \brief Вид помощи (0 - здоровье, 1 - энергия)
		 */
		int helpType;
		int cardsLimit;
	public:
		HelpEffect(QObject *parent = 0);
		QString getName();
		void setName(QString effectName);
		int getStat(QString statName);
		void setStat(QString statName, int statVal);
		void act(QVector<StatsTransmitter *> cardsToAffect, StatsTransmitter *src);
		QString getFullInfo();
		effectInfoItem getInfo();
		IEffect * clone();
		void loadFromStream(QDataStream *stream);
		void saveToStream(QDataStream *stream);
};

/*!
 * \brief Класс EffectDestabilizer - дестабилизатор эффектов
 * \details Дополнение для эффектов, при задействовании добавляет после
 * срабатывания основного действия случайное количество очков
 * в определенных пределах
 */
class EffectDestabilizer : public IEffect
{
	Q_OBJECT
	private:
		/*!
		 * \brief Основной эффект
		 */
		IEffect *coreEffect;
		/*!
		 * \brief Название
		 */
		QString name;
		/*!
		 * \brief Мощность
		 */
		int amount;
	public:
		EffectDestabilizer(QObject *parent = 0);
		EffectDestabilizer(IEffect *core, QObject *parent = 0);
		QString getName();
		void setName(QString effectName);
		int getStat(QString statName);
		void setStat(QString statName, int statVal);
		void act(QVector<StatsTransmitter *> cardsToAffect, StatsTransmitter *src);
		QString getFullInfo();
		effectInfoItem getInfo();
		IEffect * clone();
		void loadFromStream(QDataStream *stream);
		void saveToStream(QDataStream *stream);
};

#endif // IEFFECT_H
