#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>
#include "defs.h"
#include "card.h"

class Player;
class PlayerMediator;

/*!
 * \brief Интерфейс IPlayerState - состояние игрока
 * \details Определяет деятельность игрока в определенный момент
 */
class IPlayerState : public QObject
{
	Q_OBJECT
	public:
		IPlayerState(QObject *parent = 0);
		/*!
		 * \brief Конструктор объекта состояния игрока
		 * \param plr Объект игрока
		 * \param parent Родительский объект класса QObject
		 */
		IPlayerState(Player *plr, QObject *parent = 0);
		/*!
		 * \brief Установка контекста
		 * \param plr Объект игрока
		 */
		virtual void setContext(Player *plr) = 0;
		/*!
		 * \brief Получение контекста
		 * \return Объект игрока
		 */
		virtual Player * getContext() = 0;
		/*!
		 * \brief Задействование логики состояния
		 */
		virtual void run() = 0;
};

/*!
 * \brief Класс PlayerStateInitial - начальное состояние
 * \details Изначальное состояние, при котором компьютер выбирает дальнейшее
 * действие, а человеку предлагается выбор
 */
class PlayerStateInitial : public IPlayerState
{
	Q_OBJECT
	protected:
		/*!
		 * \brief Контекст
		 */
		Player *context;
	public:
		PlayerStateInitial(QObject *parent = 0);
		PlayerStateInitial(Player *plr, QObject *parent = 0);
		void setContext(Player *plr);
		Player * getContext();
		void run();
};

/*!
 * \brief Класс PlayerStateCardSelect - состояние выбора карты
 */
class PlayerStateCardSelect : public IPlayerState
{
	Q_OBJECT
	protected:
		/*!
		 * \brief Контекст
		 */
		Player *context;
	public:
		PlayerStateCardSelect(QObject *parent = 0);
		PlayerStateCardSelect(Player *plr, QObject *parent = 0);
		void setContext(Player *plr);
		Player * getContext();
		void run();
};

/*!
 * \brief Класс PlayerStateCardActivation - состояние задействования карты
 */
class PlayerStateCardActivation : public IPlayerState
{
	Q_OBJECT
	protected:
		/*!
		 * \brief Контекст
		 */
		Player *context;
	public:
		PlayerStateCardActivation(QObject *parent = 0);
		PlayerStateCardActivation(Player *plr, QObject *parent = 0);
		void setContext(Player *plr);
		Player * getContext();
		void run();
};

/*!
 * \brief Класс PlayerStateTargetSelect - состояние выбора цели
 */
class PlayerStateTargetSelect : public IPlayerState
{
	Q_OBJECT
	protected:
		/*!
		 * \brief Контекст
		 */
		Player *context;
	public:
		PlayerStateTargetSelect(QObject *parent = 0);
		PlayerStateTargetSelect(Player *plr, QObject *parent = 0);
		void setContext(Player *plr);
		Player * getContext();
		void run();
};

/*!
 * \brief Класс PlayerStateFinal - финальное состояние
 * \details Игрок сбрасывает текущий выбор и передает выбор лругому игроку
 */
class PlayerStateFinal : public IPlayerState
{
	Q_OBJECT
	protected:
		/*!
		 * \brief Контекст
		 */
		Player *context;
	public:
		PlayerStateFinal(QObject *parent = 0);
		PlayerStateFinal(Player *plr, QObject *parent = 0);
		void setContext(Player *plr);
		Player * getContext();
		void run();
};

/*!
 * \brief Интерфейс CPUCardPickStrategy - стратегия выбора карты
 */
class CPUCardPickStrategy
{
	public:
		CPUCardPickStrategy();
		/*!
		 * \brief Выбор карты из исходного набора
		 * \param cards Исходный набор карты
		 * \return Выбранная карта
		 */
		virtual int pickCard(QList<ICard *> cards) = 0;
};

/*!
 * \brief Класс CPUPickRandom - стратегия выбора случайной карты
 * \details Используется компьютером при автоматическом выборе кары для дальнейших
 * действий, в данном случае выбирается случайная карта
 */
class CPUPickRandom : public CPUCardPickStrategy
{
	public:
		CPUPickRandom();
		int pickCard(QList<ICard *> cards);
};

/*!
 * \brief Класс CPUPickWeakestFirst - стратегия выбора слабой карты
 * \details Испоользуется компьютером при автоматическом выборе карт для дальнейших
 * действий, в данном случае выбирается самая слабая карта из набора
 */
class CPUPickWeakestFirst : public CPUCardPickStrategy
{
	public:
		CPUPickWeakestFirst();
		int pickCard(QList<ICard *> cards);
};

/*!
 * \brief Класс Player - игрок
 * \details Представляет собой игрока, который держит у себя карты, выводит их на поле
 * и ими атакует карты, которые другой игрок вывел на поле
 */
class Player : public QObject
{
	Q_OBJECT
	private:
		/*!
		 * \brief Набор карт
		 */
		QList<ICard *> cards;
		/*!
		 * \brief Набор карт, выведеннх на поле
		 */
		QList<ICard *> cardsInUse;
		/*!
		 * \brief Показатель того, является ли игрок компьютером
		 */
		bool cpu;
		/*!
		 * \brief Номер действия карты
		 */
		int actionID;
		/*!
		 * \brief actionPick
		 */
		QList<QString> actionPick;
		/*!
		 * \brief selection
		 */
		QVector<QString> selection;
		/*!
		 * \brief Выбранные для действия карты
		 */
		QList<ICard *> pickedCards;
		/*!
		 * \brief Номер выбранного действия карты
		 */
		int currentCardAction;
		QString currentPick;
		/*!
		 * \brief Поток
		 */
		QThread *assignedThread;
		/*!
		 * \brief Текущее состояние
		 */
		IPlayerState *currentState;
		/*!
		 * \brief Показатель того, что нужно перезапустить поток
		 */
		bool needToRestartThread;
		/*!
		 * \brief Элемент информации об эффекте
		 */
		effectInfoItem eInfo;
		/*!
		 * \brief Стратегия выбора карт
		 */
		CPUCardPickStrategy *pickStrategy;
		/*!
		 * \brief Стратегия выбора карт на поле
		 */
		CPUCardPickStrategy *pickStrategyUsed;
		/*!
		 * \brief Объект посредника для обращения
		 */
		PlayerMediator *mediator;
	public:
		// конструкторы
		Player(QObject *parent = 0);
		Player(bool isCPU, QObject *parent = 0);
		// получение карт
		QList <ICard *> getCards();
		QList <ICard *> getCardsInUse();
		QList <ICard *> getPickedCards();
		// отдача карт
		void giveCards(QList<ICard *> cardsToGive);
		void pickCards(QList<ICard *> cardsToPick);
		void pickCards(QVector<QString> cardsToPick);
		void setCards(QList<ICard *> cardsToSet);
		void setCardsInUse(QList<ICard *> cardsUsed);
		bool isCPU();
		// получение треда
		QThread* getThread();
		// установка состояния
		void setAction(int id, QList<QString> pick);
		void setState(IPlayerState *state);
		void setState(IPlayerState *state, QList<QString> pick);
		QList<QString> getActionPick();
		QString getCurrentPick();
		QVector<QString> getSelection();
		void setCurrentPick(QString pick);
		void chooseCardAction(int id);
		int getCardAction();
		QString getCardFullInfo(QString uid);
		void setRestartTask(bool ok);
		CPUCardPickStrategy * getStrategy(bool forActivation = false);
		void attachMediator(PlayerMediator *mediator);
		void detachMediator(PlayerMediator *mediator);
		QList <ICard *> lookForNextPlayerUsedCards();
		void tellSelectedEffectInfo(effectInfoItem e);
		effectInfoItem getSelectedEffectInfo();
	public slots:
		// выброс карты
		void disposeCard();
		// ход
		void makeTurn();
		// вызов выбора действия
		// (только для компьютера)
		void callActionSelect();
		void callTurnEnd();
	signals:
		/*!
		 * \brief Удаление элемента карты
		 */
		void removeID(listItem);
		/*!
		 * \brief Добавление элемента карты
		 * \param item Элемент информации о карте
		 * \param dest Поле назначения
		 */
		void addID(listItem item, int dest);
		/*!
		 * \brief Отображение информации об эффектах выбранной карты
		 * \param effs Список элементов информации об эффектах
		 */
		void showPickedCardInfo(QList<effectInfoItem> effs);
		/*!
		 * \brief Отображение информации об ээффекте выбранной карты
		 * \param eff Элемент информации об эффекте
		 */
		void showPickedCardInfoSingle(effectInfoItem eff);
		/*!
		 * \brief Отображение количества карт на поле
		 * \param count Количество карт
		 */
		void showCardCount(int count);
		/*!
		 * \brief Запрос на выбор карты
		 */
		void callCardSelect();
		/*!
		 * \brief Завершение хода
		 */
		void turnEnd();
		/*!
		 * \brief Перезапуск
		 */
		void restart();
		/*!
		 * \brief Отправка информационного сообщения
		 * \param msg Сообщение
		 */
		void sendMsg(QString msg);
		/*!
		 * \brief Выбор действия карты
		 */
		sendActionSelect(int);
};

/*!
 * \brief Класс PlayerMediator - посрредник между игроками
 * \details Используется для действий игрока, предполагающих взаимодействие с другими
 * игроками
 */
class PlayerMediator : public QObject
{
	Q_OBJECT
	private:
		/*!
		 * \brief "Подписчики"
		 */
		QList<Player *> subscribers;
	public:
		PlayerMediator(QObject *parent = 0);
		QList<ICard *> showNextPlayerCards();
};

#endif // PLAYER_H
