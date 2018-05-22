#pragma once
#ifndef ICARD_H
#define ICARD_H

#include <QObject>
#include "defs.h"
#include "effect.h"
#include "effectselector.h"
#include "namegen.h"
#include "transmitters.h"
#include "util.h"
#include "gamesession.h"

class CardInfoObserver;
class CardVisitor;

/*!
 * \brief Интерфейс карты
 * \details Интерфейс для объектов, представляющих собой карту
 */
class ICard : public QObject
{
	Q_OBJECT
	public:
		
		ICard(QObject *parent = 0);
		/*!
		 * \brief Задействование карты
		 * \param cardsToAffect Карты, на которые предполагается воздействие
		 */
		virtual void act(QList<ICard *> cardsToAffect) = 0;
		/*!
		 * \brief Получение имени
		 * \return Имя карты
		 */
		virtual QString getName() = 0;
		/*!
		 * \brief Установка имени
		 * \param Новое имя
		 */
		virtual void setName(QString cardName) = 0;
		/*!
		 * \brief Получение ID карты
		 * \return ID карты
		 */
		virtual QString getUID() = 0;
		/*!
		 * \brief Установка ID нарты
		 * \param id ID карты
		 */
		virtual void setUID(QString id) = 0;
		/*!
		 * \brief Получение цвета фона карты
		 * \return Цвет фона карты
		 */
		virtual QColor getBG() = 0;
		/*!
		 * \brief Установка цвета фона карты
		 * \param bg Цвет фона карты
		 */
		virtual void setBG(QColor bg) = 0;
		/*!
		 * \brief Получение значение параметра
		 * \param statName Имя параметра
		 * \return Целочисленное значение параметра
		 */
		virtual int getStat(QString statName) = 0;
		/*!
		 * \brief Установка значения параметра
		 * \param statName Имя параметра
		 * \param statVal Целочисленное значение параметра
		 * \param isInc Флаг замены/добавления
		 */
		virtual void setStat(QString statName, int statVal, bool isInc = false) = 0;
		/*!
		 * \brief Установка эффектов
		 * \param effects Список объектов эффектов
		 */
		virtual void setEffects(QList<IEffect *> effects) = 0;
		/*!
		 * \brief Получение эффектов
		 * \return Список объектов эффектов
		 */
		virtual QList<IEffect *> getEffects() = 0;
		/*!
		 * \brief Обновление статистики
		 */
		virtual void updateStats() = 0;
		/*!
		 * \brief Получение графического изображения карты
		 * \param height Высота изображения
		 * \return Графическое изображение
		 */
		virtual QImage getDrawable(int height = 128) = 0;
		/*!
		 * \brief Получение псевдографического изображения карты
		 * \return Псевдографическое представление карты
		 */
		virtual QString getPseudoDrawable() = 0;
		/*!
		 * \brief Получение текстовой информации о карте
		 * \return Текстовая информация
		 */
		virtual QString getFullInfo() = 0;
		/*!
		 * \brief Получение информации об эффектах
		 * \return Список элементов информации об эффектах
		 */
		virtual QList<effectInfoItem> getEffectsInfo() = 0;
		/*!
		 * \brief Сброс парамеров карты
		 */
		virtual void reset() = 0;
		/*!
		 * \brief Принятие наблюдателя
		 * \param observer Объект наблюдателя
		 */
		virtual void attachObserver(CardInfoObserver *observer) = 0;
		/*!
		 * \brief Отсоединение наблюдателя
		 * \param observer Объект наблюдателя (для проверки)
		 */
		virtual void detachObserver(CardInfoObserver *observer) = 0;
		/*!
		 * \brief Загрузка из потока данных
		 * \param stream Объект потока данных
		 */
		virtual void loadFromStream(QDataStream *stream) = 0;
		/*!
		 * \brief Запись в поток данных
		 * \param stream Объект потока данных
		 */
		virtual void saveToStream(QDataStream *stream) = 0;
		/*!
		 * \brief Принятие посетителя карт
		 * \param visitor объект посетителя карт
		 */
		virtual void acceptVisitor(CardVisitor *visitor) = 0;
	public slots:
		/*!
		 * \brief Выбор эффекта для дальнейшего действия
		 * \param id Номер эффекта
		 */
		virtual void makeActionSelection(int id) = 0;
		/*!
		 * \brief Принятие значения параметра (перенаправление)
		 * \param statName Имя параметра
		 * \param statVal Целочисленное значение параметра
		 * \param isInc Флаг замены/добавления
		 */
		virtual void acceptStat(QString statName, int statVal, bool isInc = false) = 0;
	signals:
		/*!
		 * \brief Сигнал о переносе карты в "битые"
		 */
		void disposeMe();
		/*!
		 * \brief Сигнал об обновлении элемента с информацией
		 */
		void updateDrawable(listItem);
		/*!
		 * \brief Запрос на выбор эффекта
		 */
		void callActionSelect();
		/*!
		 * \brief ОТправка информационного сообщения
		 * \param msg Текст сообщения
		 */
		void sendMsg(QString msg);
};

/*!
 * \brief Используется для последовательной обработки объектов карт
 */
class Iterator : public IIterator<ICard *>
{
	private:
		/*!
		 * \brief Коллекция объектов карт
		 */
		ICollection<ICard *> *data;
		/*!
		 * \brief Текущая позиция в коллекции
		 */
		int currID;
	public:
		Iterator();
		Iterator(ICollection<ICard *> *coll);
		ICard * getNext();
		void removeCurrentElement();
		bool hasNext();
};

/*!
 * \brief Класс коллекции, используется для хранения объектов карт
 */
class Collection : public ICollection<ICard *>
{
	private:
		/*!
		 * \brief Список объектов карт
		 */
		QList<ICard *> data;
	public:
		friend class Iterator;
		Collection();
		Collection(QList<ICard *> _data);
		ICard * at(int id);
		void removeAt(int id);
		void replaceAt(int id, ICard *src);
		int count();
		Iterator * getIterator();
};

/*!
 * \brief Информационный наблюдатель карт.
 * \details Позволяет оповещать об изменениях
 * параметров карт и получать запросы от карт о переносе их в группу битых
 */
class CardInfoObserver : public QObject
{
	Q_OBJECT
	private:
	public:
		CardInfoObserver(QObject *parent = 0);
		void notify(listItem litem);
		void dispose(ICard *me);
};

/*!
 * \brief Класс ActorCard - карта-персонаж
 * \details Класс ActorCard, преставляющий собой карту-персонаж. Пока значения
 * "здоровья" и "энергии" находятся в пределах допустимых, карта может
 * атаковать другие неограниченное число раз. Может иметь несколько
 * доступных действий, которые можно выбрать.
 */
class ActorCard : public ICard
{
	Q_OBJECT
	private:
		/*!
		 * \brief Название карты
		 */
		QString name;
		/*!
		 * \brief Уникальный id карты
		 */
		QString uid;
		/*!
		 * \brief Показатель здоровья
		 */
		int health;
		/*!
		 * \brief Инициальнный показатель здоровья
		 */
		int healthInit;
		/*!
		 * \brief Показатель энергии
		 */
		int energy;
		/*!
		 * \brief Инициальный показатель энергии
		 */
		int energyInit;
		/*!
		 * \brief Текущий выбор действия
		 */
		int selectedAction;
		/*!
		 * \brief Цвет карты для отрисовки
		 */
		QColor bg;
		/*!
		 * \brief Коллекция эффектов
		 */
		QList<IEffect *> actionEffect;
		/*!
		 * \brief Выборщик действия по умолчанию
		 */
		IEffectSelector *defaultSelector;
		/*!
		 * \brief Наблюдательь информаци о карте
		 */
		CardInfoObserver *cio;
	public:
		ActorCard(QObject *parent = 0);
		void act(QList<ICard *> cardsToAffect);
		QString getName();
		void setName(QString cardName);
		QString getUID();
		void setUID(QString id);
		QColor getBG();
		void setBG(QColor bg);
		int getStat(QString statName);
		void setStat(QString statName, int statVal, bool isInc = false);
		void setEffects(QList<IEffect *> effects);
		QList<IEffect *> getEffects();
		void setEffectSelector(IEffectSelector *selector);
		IEffectSelector * getEffectSelector();
		void updateStats();
		QImage getDrawable(int height = 128);
		QString getPseudoDrawable();
		QString getFullInfo();
		QList<effectInfoItem> getEffectsInfo();
		void reset();
		void attachObserver(CardInfoObserver *observer);
		void detachObserver(CardInfoObserver *observer);
		void loadFromStream(QDataStream *stream);
		void saveToStream(QDataStream *stream);
		void acceptVisitor(CardVisitor *visitor);
	public slots:
		void makeActionSelection(int id);
		void acceptStat(QString statName, int statVal, bool isInc = false);
};

/*!
 * \brief Класс ActionCard - карта-действие
 * \details Класс ActionCard, представляющий собой карту-действие. Имеет при себе
 * только одно допустимое действие. Сбособна быть задействованной только
 * один раз, однако пока она не задействована, имеет неуязвимость.
 */
class ActionCard : public ICard
{
	Q_OBJECT
	private:
		/*!
		 * \brief Имя карты
		 */
		QString name;
		/*!
		 * \brief Уникальный ID карты
		 */
		QString uid;
		/*!
		 * \brief Цвет карты для отрисовки
		 */
		QColor bg;
		/*!
		 * \brief Эффект дейсттвия
		 */
		IEffect *actionEffect;
		/*!
		 * \brief Наблюдатель информации о карте
		 */
		CardInfoObserver *cio;
	public:
		ActionCard(QObject *parent = 0);
		void act(QList<ICard *> cardsToAffect);
		QString getName();
		void setName(QString cardName);
		QString getUID();
		void setUID(QString id);
		QColor getBG();
		void setBG(QColor bg);
		int getStat(QString statName);
		void setStat(QString statName, int statVal, bool isInc = false);
		void setEffects(QList<IEffect *> effects);
		QList<IEffect *> getEffects();
		void updateStats();
		QImage getDrawable(int height = 128);
		QString getPseudoDrawable();
		QString getFullInfo();
		QList<effectInfoItem> getEffectsInfo();
		void reset();
		void attachObserver(CardInfoObserver *observer);
		void detachObserver(CardInfoObserver *observer);
		void loadFromStream(QDataStream *stream);
		void saveToStream(QDataStream *stream);
		void acceptVisitor(CardVisitor *visitor);
	public slots:
		void makeActionSelection(int id);
		void acceptStat(QString statName, int statVal, bool isInc = false);
};

/*!
 * \brief Класс TeamCard - карта-команда
 * \details Класс TeamCard, представляющий собой карту-команду. Карта представляет
 * собой группу карт различных типов, включая карты-команды. При атаке
 * вызывается случайный член команды, удар на себя принимает случайный же
 * член команды.
 */
class TeamCard : public ICard
{
	Q_OBJECT
	private:
		/*!
		 * \brief Имя карты
		 */
		QString name;
		/*!
		 * \brief Уникальный ID карты
		 */
		QString uid;
		/*!
		 * \brief Цвет карты для отрисовки
		 */
		QColor bg;
		/*!
		 * \brief Коллекция членов команды
		 */
		QList<ICard *> members;
		/*!
		 * \brief Коллекция битых членов команды
		 */
		QList<ICard *> membersBeaten;
		/*!
		 * \brief Наблюдатель информации о карте
		 */
		CardInfoObserver *cio;
	public:
		TeamCard(QObject *parent = 0);
		void act(QList<ICard *> cardsToAffect);
		QString getName();
		void setName(QString cardName);
		QString getUID();
		void setUID(QString id);
		QColor getBG();
		void setBG(QColor bg);
		int getStat(QString statName);
		void setStat(QString statName, int statVal, bool isInc = false);
		void setEffects(QList<IEffect *> effects);
		QList<IEffect *> getEffects();
		void updateStats();
		QImage getDrawable(int height = 128);
		QString getPseudoDrawable();
		QString getFullInfo();
		QList<effectInfoItem> getEffectsInfo();
		void setMembers(QList<ICard *> _members, bool beaten = false);
		QList<ICard *> getMembers(bool beaten = false);
		void reset();
		void attachObserver(CardInfoObserver *observer);
		void detachObserver(CardInfoObserver *observer);
		void loadFromStream(QDataStream *stream);
		void saveToStream(QDataStream *stream);
		void acceptVisitor(CardVisitor *visitor);
	public slots:
		void makeActionSelection(int id);
		void acceptStat(QString statName, int statVal, bool isInc = false);
		void cardDispose();
};

/*!
 * \brief Интерфейс CardVisitor - посетитель карт
 * \details Используется для выполнения внутренних действий для определенных карт.
 * Предусматривает наличие разных действий для разных типов карт.
 */
class CardVisitor
{
	public:
		CardVisitor();
		/*!
		 * \brief Посещение карты-персонажа
		 * \param card Объект карты-персонажа
		 */
		virtual void visitActorCard(ActorCard *card) = 0;
		/*!
		 * \brief Посещение карты-действия
		 * \param card Объект карты-действия
		 */
		virtual void visitActionCard(ActionCard *card) = 0;
		/*!
		 * \brief Посещение карты-команды
		 * \param card Объект карты-команды
		 */
		virtual void visitTeamCard(TeamCard *card) = 0;
};

/*!
 * \brief Сопоставление типа и указателя объекта карты
 * \param cardType Тип карты
 * \return Указатель на объект карты
 */
ICard * matchCardPointer(char cardType);

/*!
 * \brief Класс CardStreamWriter для записи карт в поток
 * \details Используется для записи карты в поток данных, считывая ее свойства.
 * Реализует методы интерфейса CardVisitor
 */
class CardStreamWriter : public CardVisitor
{
	private:
		/*!
		 * \brief Поток данных
		 */
		QDataStream *stream;
	public:
		CardStreamWriter();
		void setupStream(QDataStream *stream);
		void writeCard(ICard *card);
		void visitActorCard(ActorCard *card);
		void visitActionCard(ActionCard *card);
		void visitTeamCard(TeamCard *card);
};

/*!
 * \brief Класс CardStreamReader для чтения карт из потока
 * \details Используется для чтения свойств карты из потока данных.
 * Реализует методы интерфейса CardVisitor
 */
class CardStreamReader : public CardVisitor
{
	private:
		/*!
		 * \brief Поток данных
		 */
		QDataStream *stream;
	public:
		CardStreamReader();
		void setupStream(QDataStream *stream);
		void readCard(ICard *card);
		void visitActorCard(ActorCard *card);
		void visitActionCard(ActionCard *card);
		void visitTeamCard(TeamCard *card);
};

/*!
 * \brief Сбор информации о карте
 * \param card Объект карты
 * \return Элемент информации о карте
 */
listItem getCardListItem(ICard *card);

#endif // ICARD_H
