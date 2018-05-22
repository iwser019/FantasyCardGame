#include "player.h"

/*!
 * \brief Конструктор объекта игрока
 * \param parent Родительский объект класса QObject
 */
Player::Player(QObject *parent):QObject(parent)
{
	actionID = 0;
	actionPick = {};
	currentState = new PlayerStateInitial(dynamic_cast<Player*>(this));
	assignedThread = new QThread(this);
	currentCardAction = -1;
	connect(assignedThread, SIGNAL(started()), this, SLOT(makeTurn()));
	pickStrategy = nullptr;
}

/*!
 * \brief Конструктор объекта игрока
 * \param isCPU Является ли игрок компьютером
 * \param parent Родительский объект класса QObject
 */\
Player::Player(bool isCPU, QObject *parent):Player(parent)
{
	cpu = isCPU;
	if (isCPU)
	{
		if (Random::probability(0.5))
			pickStrategy = new CPUPickRandom();
		else
			pickStrategy = new CPUPickWeakestFirst();
		if (Random::probability(0.5))
			pickStrategyUsed = new CPUPickRandom();
		else
			pickStrategyUsed = new CPUPickWeakestFirst();
	}
}

/*!
 * \brief Получение имеюющихся карт, не выведенных на поле
 * \return Коллекция объектов карт
 */
QList<ICard *> Player::getCards()
{
	return cards;
}

/*!
 * \brief Получение карт игрока, выведенныз на поле
 * \return Коллекция объектов карт
 */
QList<ICard *> Player::getCardsInUse()
{
	return cardsInUse;
}

/*!
 * \brief Получение выбранных игроком карт
 * \return Коллекция объектов карт
 */
QList<ICard *> Player::getPickedCards()
{
	return pickedCards;
}

/*!
 * \brief Выдать карты игроку для начала
 * \param cardsToGive Коллекция объектов карт
 */
void Player::giveCards(QList<ICard *> cardsToGive)
{
	int sum = cardsToGive.size();
	for (int i = 0; i < sum; i++)
		cards.push_back(cardsToGive[i]);
}

/*!
 * \brief Выбор карт игроком для воздействия
 * \param cardsToPick Коллекция объектов карт
 */
void Player::pickCards(QList<ICard *> cardsToPick)
{
	pickedCards = cardsToPick;
}

/*!
 * \brief Выбор карт игроком для воздействия
 * \param cardsToPick Коллекция UID карт
 */
void Player::pickCards(QVector<QString> cardsToPick)
{
	selection = cardsToPick;
}

/*!
 * \brief Установка невыведенных карт
 * \param cardsToSet Коллекция объектов карт
 */
void Player::setCards(QList<ICard *> cardsToSet)
{
	cards = cardsToSet;
}

/*!
 * \brief Установка карт, выведенных на поле
 * \param cardsUsed Коллекция объектов карт
 */
void Player::setCardsInUse(QList<ICard *> cardsUsed)
{
	cardsInUse = cardsUsed;
}

/*!
 * \brief Совершение хода
 */
void Player::makeTurn()
{
	// ID действий:
	// 0: начальное
	// 1: вывод карты на поле
	// 2: задействование выведенной карты
	// 3: выбор целевой карты
	// 10: окончание хода
	currentState->run();
	if (needToRestartThread)
	{
		needToRestartThread = false;
		emit restart();
	}

}

/*!
 * \brief Выбор эффекта у карты с несколькими эффектами (используется компьютером)
 */
void Player::callActionSelect()
{
	ICard *c = (ICard *)QObject::sender();
	if (cpu)
	{
		int num = c->getEffects().size();
		sendActionSelect(Random::randInt(0, num - 1));
		this->setState(new PlayerStateTargetSelect(dynamic_cast<Player *>(this)), {});
	}
}

/*!
 * \brief Вызов окончания хода
 */
void Player::callTurnEnd()
{
	currentState = new PlayerStateFinal(dynamic_cast<Player *>(this));
	restart();
}

/*!
 * \brief Проверка, является ли игрок компьютером
 * \return Результат проверки
 */
bool Player::isCPU()
{
	return cpu;
}

/*!
 * \brief Получение выполняемого потока игрока
 * \return Объект потока
 */
QThread *Player::getThread()
{
	return assignedThread;
}

/*!
 * \brief Установка данных для совершения хода с задействованием карты
 * \param id Номер эффекта задействуемой карты
 * \param pick UID карт, на которые предполакается воздействие
 */
void Player::setAction(int id, QList<QString> pick)
{
	actionPick = pick;
	actionID = id;
}

/*!
 * \brief Установка состояния игрока
 * \param state Объект состояния
 */
void Player::setState(IPlayerState *state)
{
	currentState = state;
}

/*!
 * \brief Установка состояния игрока
 * \param state Объпект состояния
 * \param pick Коллекция UID карт для выбора
 */
void Player::setState(IPlayerState *state, QList<QString> pick)
{
	actionPick = pick;
	currentState = state;
}

/*!
 * \brief \return
 */
QList<QString> Player::getActionPick()
{
	return actionPick;
}

/*!
 * \brief \return
 */
QString Player::getCurrentPick()
{
	return currentPick;
}

/*!
 * \brief \return
 */
QVector<QString> Player::getSelection()
{
	return selection;
}

/*!
 * \brief \param pick
 */
void Player::setCurrentPick(QString pick)
{
	currentPick = pick;
}

/*!
 * \brief Выбор эффекта задействуемой карты
 * \param id Номер эффекта
 */
void Player::chooseCardAction(int id)
{
	currentCardAction = id;
}

/*!
 * \brief Получение текущего выбранного эффекта задействуемой карты
 * \return Номер текущего эффекта
 */
int Player::getCardAction()
{
	return currentCardAction;
}

/*!
 * \brief Получение текстовой информации о карте у игрока
 * \param uid UID карты
 * \return Текстовая информация
 */
QString Player::getCardFullInfo(QString uid)
{
	int sizeInt;
	sizeInt = cards.size();
	for (int i = 0; i < sizeInt; i++)
		if (cards[i]->getUID() == uid)
			return cards[i]->getFullInfo();
	sizeInt = cardsInUse.size();
	for (int i = 0; i < sizeInt; i++)
		if (cardsInUse[i]->getUID() == uid)
			return cardsInUse[i]->getFullInfo();
	return "";
}

/*!
 * \brief Установить флаг перезагрузки потока
 * \param ok Значение флага перезагрузки потока
 */
void Player::setRestartTask(bool ok)
{
	needToRestartThread = ok;
}

/*!
 * \brief Получение стратегии выбора карты компьютером
 * \param forActivation Для задействования (true) или для вывода на поле
 * \return Объект стратегии выбора карты компьютером
 */
CPUCardPickStrategy *Player::getStrategy(bool forActivation)
{
	return (forActivation ? pickStrategyUsed : pickStrategy);
}

/*!
 * \brief Установить ссылку на посредника
 * \param mediator Объект посредника между игроками
 */
void Player::attachMediator(PlayerMediator *mediator)
{
	if (this->mediator != mediator)
		this->mediator = mediator;
}

/*!
 * \brief Убрать ссылку на посредника между игроками
 * \param mediator Объект посредника между игроками (для проверки)
 */
void Player::detachMediator(PlayerMediator *mediator)
{
	if (this->mediator == mediator)
		this->mediator = nullptr;
}

/*!
 * \brief Получить карты, выведенные противником на поле
 * \return Коллекция объектов карт
 */
QList<ICard *> Player::lookForNextPlayerUsedCards()
{
	QList<ICard *> result;
	if (mediator != nullptr)
		result = mediator->showNextPlayerCards();
	return result;
}

/*!
 * \brief Получить информацию о текущем выбранном эффекте
 * \return Элемент информации об эффекте
 */
effectInfoItem Player::getSelectedEffectInfo()
{
	return eInfo;
}

/*!
 * \brief Записать информацию о текущем эффекте
 * \param e Элемент информации об эффекте
 */
void Player::tellSelectedEffectInfo(effectInfoItem e)
{
	eInfo = e;
}

/*!
 * \brief Удалить карту
 */
void Player::disposeCard()
{
	QObject *o = QObject::sender();
	ICard *card = dynamic_cast<ICard *>(o);
	int cardID = cardsInUse.indexOf(card);
	if (cardID != -1)
	{
		cardsInUse.removeAt(cardID);
	}
}

/*!
 * \brief Конструктор объекта интерфейса состояния игрока
 * \param parent Родительский объект класса QObject
 */
IPlayerState::IPlayerState(QObject *parent) : QObject(parent)
{

}

/*!
 * \brief Конструктор объекта интерфейса состояния игрока (с явным указанием игрока)
 * \param plr Объект игрока
 * \param parent Родительский объект класса QObject
 */
IPlayerState::IPlayerState(Player *plr, QObject *parent) : IPlayerState(parent)
{

}

/*!
 * \brief Конструктор объекта начального состояния игрока
 * \param parent Родительский объект класса QObject
 */
PlayerStateInitial::PlayerStateInitial(QObject *parent) : IPlayerState(parent)
{

}

/*!
 * \brief Конструктор объекта начального состояния игрока
 * \param plr Объект игрока
 * \param parent Родительский объект класса QObject
 */
PlayerStateInitial::PlayerStateInitial(Player *plr, QObject *parent) : PlayerStateInitial(parent)
{
	context = plr;
}

/*!
 * \brief Установка контекста для состояния
 * \param plr Объект игрока
 */
void PlayerStateInitial::setContext(Player *plr)
{
	context = plr;
}

/*!
 * \brief Получение контекста состояния игрока
 * \return Объект игрока
 */
Player *PlayerStateInitial::getContext()
{
	return context;
}

/*!
 * \brief Запуск деятельности состояния игрока
 */
void PlayerStateInitial::run()
{
	bool isCPU = context->isCPU();
	if (isCPU)
	{
		int actionID = Random::randIntWeighted({context->getCards().empty() ? 0 : 5, context->getCardsInUse().empty() ? 0 : 5});
		if (actionID == 0)
			context->setState(new PlayerStateCardSelect(dynamic_cast<Player*>(context)));
		else if (actionID == 1)
			context->setState(new PlayerStateCardActivation(dynamic_cast<Player*>(context)));
		context->setRestartTask(true);
	}
	else
	{
		return;
	}
}

/*!
 * \brief Конструктор состояния выбора карты
 * \param parent Родительский объект класса QObject
 */
PlayerStateCardSelect::PlayerStateCardSelect(QObject *parent) : IPlayerState(parent)
{

}

/*!
 * \brief Конструктор состояния выбора карты
 * \param plr Объект игрока
 * \param parent Родительский объект класса QObject
 */
PlayerStateCardSelect::PlayerStateCardSelect(Player *plr, QObject *parent) : PlayerStateCardSelect(parent)
{
	context = plr;
}

/*!
 * \brief Установка контекста для состояния игрока
 * \param plr Объект игрока
 */
void PlayerStateCardSelect::setContext(Player *plr)
{
	context = plr;
}

/*!
 * \brief Получение контекста состояния игрока
 * \return Объект игрока
 */
Player *PlayerStateCardSelect::getContext()
{
	return context;
}

/*!
 * \brief Запуск деятельности состояния игрока
 */
void PlayerStateCardSelect::run()
{
	GameSession *gs = GameSession::getInstance();
	bool isCPU = context->isCPU();
	if (isCPU)
	{
		QList<ICard *> cards = context->getCards(),
				cardsInUse = context->getCardsInUse();
		int randNum = context->getStrategy()->pickCard(cards);
		cardsInUse.push_back(cards.takeAt(randNum));
		context->setCards(cards);
		context->setCardsInUse(cardsInUse);
		context->addID(listItem(cardsInUse.back()->getDrawable(),
					   cardsInUse.back()->getPseudoDrawable(),
					   cardsInUse.back()->getName(),
					   cardsInUse.back()->getUID()), 2);
		context->showCardCount(cards.size());
		gs->msgRepeater("Игрок вывел на поле " + cardsInUse.back()->getName());
		context->setState(new PlayerStateInitial(dynamic_cast<Player*>(context)), QList<QString>());
		context->getThread()->msleep(250);
		context->turnEnd();
	}
	else
	{
		int pick = 0;
		QList<ICard *> cards = context->getCards(),
				cardsInUse = context->getCardsInUse();
		QList<QString> actionPick = context->getActionPick();
		for (int i = 0; i < cards.size(); i++)
			if (cards[i]->getUID() == actionPick[0])
			{
				pick = i;
				break;
			}
		context->removeID(listItem(cards[pick]->getDrawable(), cards[pick]->getPseudoDrawable(), cards[pick]->getName(), cards[pick]->getUID()));
		cardsInUse.push_back(cards.takeAt(pick));
		context->setCards(cards);
		context->setCardsInUse(cardsInUse);
		context->addID(listItem(cardsInUse.back()->getDrawable(), cardsInUse.back()->getPseudoDrawable(), cardsInUse.back()->getName(), cardsInUse.back()->getUID()), 1);
		gs->msgRepeater("Игрок вывел на поле " + cardsInUse.back()->getName());
		// вывели карту, можно переводить ход
		context->setState(new PlayerStateInitial(dynamic_cast<Player*>(context)), QList<QString>());
		context->getThread()->msleep(250);
		context->turnEnd();
	}
}

/*!
 * \brief Конструктор объекта состояния задействования карты
 * \param parent Родительский объект класса QObject
 */
PlayerStateCardActivation::PlayerStateCardActivation(QObject *parent) : IPlayerState(parent)
{

}

/*!
 * \brief Конструктор объекта состояния задействования карты
 * \param plr Объект игрока
 * \param parent Родительский объект класса QObject
 */
PlayerStateCardActivation::PlayerStateCardActivation(Player *plr, QObject *parent) : PlayerStateCardActivation(parent)
{
	context = plr;
}

/*!
 * \brief Установка контекста состояния игрока
 * \param plr Объект игрока
 */
void PlayerStateCardActivation::setContext(Player *plr)
{
	context = plr;
}

/*!
 * \brief Получение контекста состояния игрока
 * \return Объект игрока
 */
Player *PlayerStateCardActivation::getContext()
{
	return context;
}

/*!
 * \brief Запуск деятельности состояния игрока
 */
void PlayerStateCardActivation::run()
{
	GameSession *gs = GameSession::getInstance();
	bool isCPU = context->isCPU();
	if (isCPU)
	{
		QList<ICard *> cardsInUse = context->getCardsInUse();
		int randNum = (cardsInUse.size() != 1) ? context->getStrategy(true)->pickCard(cardsInUse) : 0;
		//actionPick = randNum;
		context->setState(new PlayerStateTargetSelect(dynamic_cast<Player*>(context)));
		context->setCurrentPick(cardsInUse[randNum]->getUID());
		gs->msgRepeater("Игрок выбрал " + cardsInUse[randNum]->getName());
		QList<effectInfoItem> effs = cardsInUse[randNum]->getEffectsInfo();
		int effID = Random::randInt(0, effs.size() - 1);
		context->chooseCardAction(effID);
		emit context->showPickedCardInfoSingle(effs[effID]);
		QList<ICard *> pick;
		if (effs[effID].canAffectEnemy)
		{
			QList<ICard *> playerCards = context->lookForNextPlayerUsedCards();
			if (!playerCards.empty())
			{
				int cardId = Random::randInt(0, playerCards.size() - 1);
				pick.push_back(playerCards[cardId]);
			}
		}
		if (effs[effID].canAffectPlayer)
		{
			int cardId = Random::randInt(0, cardsInUse.size() - 1);
			pick.push_back(cardsInUse[cardId]);
		}
		context->pickCards(pick);
		context->restart();
	}
	else
	{
		QList<ICard *> cardsInUse = context->getCardsInUse();
		QList<QString> actionPick = context->getActionPick();
		int pick = 0;
		for (int i = 0; i < cardsInUse.size(); i++)
			if (cardsInUse[i]->getUID() == actionPick[0])
			{
				pick = i;
				break;
			}
		emit context->showPickedCardInfo(cardsInUse[pick]->getEffectsInfo());
		gs->msgRepeater("Игрок выбрал " + cardsInUse[pick]->getName());
		context->setState(new PlayerStateTargetSelect(dynamic_cast<Player*>(context)));
	}
}

/*!
 * \brief Конструктор объекта состояния выбора цели
 * \param parent Родительский объект класса QObject
 */
PlayerStateTargetSelect::PlayerStateTargetSelect(QObject *parent) : IPlayerState(parent)
{

}

/*!
 * \brief Конструктор объекта состояния выбора цели
 * \param plr Объект игрока
 * \param parent Родительский объект класса QObject
 */
PlayerStateTargetSelect::PlayerStateTargetSelect(Player *plr, QObject *parent) : PlayerStateTargetSelect(parent)
{
	context = plr;
}

/*!
 * \brief Установка контекста состояния игрока
 * \param plr Объект игрока
 */
void PlayerStateTargetSelect::setContext(Player *plr)
{
	context = plr;
}

/*!
 * \brief Получение контекста состояния игрока
 * \return Объхект игрока
 */
Player *PlayerStateTargetSelect::getContext()
{
	return context;
}

/*!
 * \brief Запуск деятельности состояния игрока
 */
void PlayerStateTargetSelect::run()
{
	GameSession *gs = GameSession::getInstance();
	bool isCPU = context->isCPU();
	if (isCPU)
	{
		QList<ICard *> cardsInUse = context->getCardsInUse();
		QList<ICard *> pickedCards = context->getPickedCards();
		QString currentPick = context->getCurrentPick();
		int pick = 0;
		if (pickedCards.empty())
		{
			gs->msgRepeater("Игрок ничего не выбрал и пропускает ход.");
			context->callTurnEnd();
			return;
		}
		for (int i = 0; i < cardsInUse.size(); i++)
			if (cardsInUse[i]->getUID() == currentPick)
			{
				pick = i;
				break;
			}
		cardsInUse[pick]->makeActionSelection(context->getCardAction());
		cardsInUse[pick]->act(context->getPickedCards());
		context->callTurnEnd();
	}
	else
	{
		QList<ICard *> cardsInUse = context->getCardsInUse();
		QList<QString> actionPick = context->getActionPick();
		int pick = 0;
		for (int i = 0; i < cardsInUse.size(); i++)
			if (cardsInUse[i]->getUID() == actionPick[0])
			{
				pick = i;
				break;
			}
		cardsInUse[pick]->makeActionSelection(context->getCardAction());
		QList<ICard *> tCards, tPick;
		QVector<QString> selection = context->getSelection();
		effectInfoItem eInfo = context->getSelectedEffectInfo();
		if (eInfo.canAffectEnemy)
			tCards.append(context->lookForNextPlayerUsedCards());
		if (eInfo.canAffectPlayer)
			tCards.append(cardsInUse);
		for (int i = 0; i < selection.size(); i++)
		{
			for (int j = 0; j < tCards.size(); j++)
			{
				if (selection[i] == tCards[j]->getUID())
				{
					tPick.push_back(tCards[j]);
				}
			}
		}
		context->pickCards(tPick);
		cardsInUse[pick]->act(context->getPickedCards());
		context->callTurnEnd();
	}
}

/*!
 * \brief Конструктор объекта конечного состояния
 * \param parent Родительский объект класса QObject
 */
PlayerStateFinal::PlayerStateFinal(QObject *parent) : IPlayerState(parent)
{

}

/*!
 * \brief Конструктор объекта конечного состояния
 * \param plr Объект игрока
 * \param parent Родительский объект класса QObject
 */
PlayerStateFinal::PlayerStateFinal(Player *plr, QObject *parent) : PlayerStateFinal(parent)
{
	context = plr;
}

/*!
 * \brief Установка контекста состояния игрока
 * \param plr Объект игрока
 */
void PlayerStateFinal::setContext(Player *plr)
{
	context = plr;
}

/*!
 * \brief Получение контекста состояния игрока
 * \return Объект игрока
 */
Player *PlayerStateFinal::getContext()
{
	return context;
}

/*!
 * \brief Запуск деятельности состояния игрока
 */
void PlayerStateFinal::run()
{
	bool isCPU = context->isCPU();
	if (isCPU)
	{
		context->setState(new PlayerStateInitial(dynamic_cast<Player *>(context)), {});
		context->setCurrentPick("");
		context->chooseCardAction(-1);
		context->getThread()->msleep(250);
		context->turnEnd();
	}
	else
	{
		context->setState(new PlayerStateInitial(dynamic_cast<Player *>(context)), {});
		context->setCurrentPick("");
		context->chooseCardAction(-1);
		context->getThread()->msleep(250);
		context->turnEnd();
	}
}

/*!
 * \brief Конструктор объекта стратегии выбора карт
 */
CPUCardPickStrategy::CPUCardPickStrategy()
{

}

/*!
 * \brief Конструктор объекта стратегии случайного выбора карты
 */
CPUPickRandom::CPUPickRandom() : CPUCardPickStrategy()
{

}

/*!
 * \brief Выбор карты с использованием обозначенной стратегии
 * \param cards Исходный набор карт
 * \return Выбранная карта
 */
int CPUPickRandom::pickCard(QList<ICard *> cards)
{
	return Random::randInt(0, cards.size() - 1);
}

/*!
 * \brief Контруктор объекта стратегии выбора слабой карты
 */
CPUPickWeakestFirst::CPUPickWeakestFirst()
{

}

/*!
 * \brief Выбор карты с использованием обозначенной стратегии
 * \param cards Исходный набор карт
 * \return Выбранная карта
 */
int CPUPickWeakestFirst::pickCard(QList<ICard *> cards)
{
	QList<int> actors, actions;
	int sizeInt = cards.size();
	int result = 0;
	for (int i = 0; i < sizeInt; i++)
	{
		// если это действие (а оно всега имеет 999 HP)
		if (cards[i]->getStat("health") == 999)
			actions.push_back(i);
		else
			actors.push_back(i);
	}
	bool selectActions = ((!actors.empty() && !actions.empty() && Random::probability(0.5))
						  || (actors.empty() && !actions.empty()));
	if (!selectActions)
	{
		// выбираем из актеров и команд
		sizeInt = actors.size();
		int curHealth = cards[actors[0]]->getStat("health");
		result = actors[0];
		for (int i = 1; i < sizeInt; i++)
		{
			int nextHealth = cards[actors[i]]->getStat("health");
			if (nextHealth < curHealth)
			{
				result = actors[i];
				curHealth = nextHealth;
			}
		}
	}
	else
	{
		// выбираем из действий
		result = actions[Random::randInt(0, actions.size() - 1)];
	}
	return result;
}

/*!
 * \brief Конструктор объекта посредника между игроками
 * \param parent Родительский объект класса QObject
 */
PlayerMediator::PlayerMediator(QObject *parent) : QObject(parent)
{

}

/*!
 * \brief Просмотр карт следующего игрока на поле
 * \return Набор объектов карт
 */
QList<ICard *> PlayerMediator::showNextPlayerCards()
{
	QList<ICard *> result;
	Player *nextPlayer = GameSession::getInstance()->getNextPlayer();
	if (nextPlayer != nullptr)
		result = nextPlayer->getCardsInUse();
	return result;
}
