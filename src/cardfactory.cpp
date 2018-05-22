#include "cardfactory.h"

/*!
 * \brief Конструктор объектов интерфейса фабрики карт
 * \param parent Родительский объект класса QObject
 */
CardFactory::CardFactory(QObject *parent) : QObject(parent)
{
	// заглушка
}

/*!
 * \brief Конструктор объектов фабрики карт-персонажей
 * \param parent Родительскийц объект класса QObject
 */
ActorCardFactory::ActorCardFactory(QObject *parent) : CardFactory(parent)
{
	// заглушка
}

/*!
 * \brief Установка контекста
 * \param gs Объект игрового сеанса
 */
void ActorCardFactory::setContext(GameSession *gs)
{
	context = gs;
}

/*!
 * \brief Установка ссылки на генератор имен
 * \param ng Объект генератора имен
 */
void ActorCardFactory::installNamegen(NameGen *ng)
{
	nameGen = ng;
}

/*!
 * \brief Установка ссылки на фабрику эффектов
 * \param ef Объект фабрики эффектов
 */
void ActorCardFactory::setEffectFactory(EffectFactory *ef)
{
	effFactory = ef;
}

/*!
 * \brief Генерация карты-персонажа
 * \param connection Является ли карта корневой (т. е. не член команды)
 * \return Объект карты-персонажа
 */
ICard *ActorCardFactory::generateCard(bool connection)
{
	ActorCard *card = new ActorCard();
	card->setName(nameGen->randomActorName());
	QList<IEffect *> effects;
	card->setStat("health", 100, false);
	card->setStat("energy", 100, false);
	int numEffects = Random::randInt(1, 3);
	for (int j = 0; j < numEffects; j++)
	{
		IEffect *e = effFactory->generateEffect(connection);
		effects.push_back(e);
	}
	IEffectSelector *selector;
	switch (Random::randIntWeighted({5, 5}))
	{
		case 0:
			selector = new EffectSelectorRandom();
			break;
		case 1:
			selector = new EffectSelectorAll();
			break;
	}
	card->setEffectSelector(selector);
	card->setEffects(effects);
	if (connection)
	{
		connect(card, SIGNAL(sendMsg(QString)), context->getInfoTrans(), SLOT(receiveMsg(QString)));
	}
	return card;
}

/*!
 * \brief Конструктор объекта фабрики карт-действий
 * \param parent Родительский объект класса QObject
 */
ActionCardFactory::ActionCardFactory(QObject *parent) : CardFactory(parent)
{
	// заглушка
}

/*!
 * \brief Установка контекста
 * \param gs Объект игрового сеанса
 */
void ActionCardFactory::setContext(GameSession *gs)
{
	context = gs;
}

/*!
 * \brief Установка ссылки на генератор имен
 * \param ng Объект генератора имен
 */
void ActionCardFactory::installNamegen(NameGen *ng)
{
	nameGen = ng;
}

/*!
 * \brief Установка ссылки на фабрику эффектов
 * \param ef Объект фабрики эффектов
 */
void ActionCardFactory::setEffectFactory(EffectFactory *ef)
{
	effFactory = ef;
}

/*!
 * \brief Генерация карты-действия
 * \param connection Является ли карта корневой (т. е. не член команды)
 * \return Объект карты-действия
 */
ICard *ActionCardFactory::generateCard(bool connection)
{
	ActionCard *card = new ActionCard();
	card->setName(nameGen->randomActionName());
	IEffect *e = effFactory->generateEffect(connection);
	card->setEffects({e});
	if (connection)
	{
		connect(card, SIGNAL(sendMsg(QString)), context->getInfoTrans(), SLOT(receiveMsg(QString)));
	}
	return card;
}

/*!
 * \brief Конструкторп объектов фабрики карт-команд
 * \param parent Родительский объект класса QObject
 */
TeamCardFactory::TeamCardFactory(QObject *parent) : CardFactory(parent)
{
	// заглушка
}

/*!
 * \brief Установка контекста
 * \param gs Объект игрового сеанса
 */
void TeamCardFactory::setContext(GameSession *gs)
{
	context = gs;
}

/*!
 * \brief Установка ссылки на генератор имен
 * \param ng Объект генератора имен
 */
void TeamCardFactory::installNamegen(NameGen *ng)
{
	nameGen = ng;
}

/*!
 * \brief Установка ссылки на фабрику эффектов
 * \param ef Объект фабрики эффектов
 */
void TeamCardFactory::setEffectFactory(EffectFactory *ef)
{
	effFactory = ef;
}

/*!
 * \brief Установка ссылок на объекты фабрик карт-персонажей и карт-действий
 * для генерации членов команды
 * \param forActor Объект фабрики карт-персонажей
 * \param forAction Объект фабрики карт-действий
 */
void TeamCardFactory::setSubfactories(CardFactory *forActor, CardFactory *forAction)
{
	actorFact = forActor;
	actionFact = forAction;
}

/*!
 * \brief Генерация карт-команд
 * \param connection Является ли карта корневой (т. е. не членом команды)
 * \return Объект карты-команды
 */
ICard *TeamCardFactory::generateCard(bool connection)
{
	int numMembers = Random::randInt(2, 5);
	TeamCard *card = new TeamCard();
	card->setName(nameGen->randomTeamName());
	QList<ICard *> members;
	for (int i = 0; i < numMembers; i++)
	{
		ICard *member;
		switch (Random::randIntWeighted({25, 15, 5}))
		{
			case 0:
				member = actorFact->generateCard(false);
				break;
			case 1:
				member = actionFact->generateCard(false);
				break;
			case 2:
				member = this->generateCard(false);
				break;
		}
		members.push_back(member);
	}
	card->setMembers(members);
	connect(card, SIGNAL(sendMsg(QString)), context->getInfoTrans(), SLOT(receiveMsg(QString)));
	return card;
}

/*!
 * \brief CardFactoryTiered::CardFactoryTiered-
 * Конструктор фабрики карт с уровнем
 * \param parent Родительский объект класса QObject
 */
CardFactoryTiered::CardFactoryTiered(QObject *parent) : QObject(parent)
{

}

/*!
 * \brief Конструктор фабрики карт второго уровня
 * \param parent Родительский объект класса QObject
 */
CardFactoryTier2::CardFactoryTier2(QObject *parent) : CardFactoryTiered(parent)
{

}

/*!
 * \brief Установка ссылки на фабрику эффектов
 * \param ef Объект фабрики эффектов
 */
void CardFactoryTier2::setEffectFactory(EffectFactory *ef)
{
	effFactory = ef;
}

/*!
 * \brief Генерация карты-персонажа с характеристиками второго уровня
 * \param connection Является ли карта корневой
 * \return Объект карты-персонажа с характеристиками второго уровня.
 */
ICard *CardFactoryTier2::generateActorCard(bool connection)
{
	ActorCard *card = new ActorCard();
	card->setName(nameGen->randomActorName());
	QList<IEffect *> effects;
	card->setStat("health", 200, false);
	card->setStat("energy", 200, false);
	card->setStat("healthInit", 200, false);
	card->setStat("energyInit", 200, false);
	int numEffects = Random::randInt(1, 3);
	for (int j = 0; j < numEffects; j++)
	{
		IEffect *e = effFactory->generateEffect(connection);
		e->setStat("power", e->getStat("power") * 2);
		e->setStat("amount", e->getStat("amount") * 2);
		effects.push_back(e);
	}
	IEffectSelector *selector;
	switch (Random::randIntWeighted({5, 5}))
	{
		case 0:
			selector = new EffectSelectorRandom();
			break;
		case 1:
			selector = new EffectSelectorAll();
			break;
	}
	card->setEffectSelector(selector);
	card->setEffects(effects);
	if (connection)
	{
		connect(card, SIGNAL(sendMsg(QString)), context->getInfoTrans(), SLOT(receiveMsg(QString)));
	}
	return card;
}

/*!
 * \brief Генерация карты-действия с характеристиками второго уровня
 * \param connection Является ли карта корневой
 * \return Карта-действие второго уровня
 */
ICard *CardFactoryTier2::generateActionCard(bool connection)
{
	ActionCard *card = new ActionCard();
	card->setName(nameGen->randomActionName());
	IEffect *e = effFactory->generateEffect(connection);
	card->setEffects({e});
	e->setStat("power", e->getStat("power") * 2);
	e->setStat("amount", e->getStat("amount") * 2);
	if (connection)
	{
		connect(card, SIGNAL(sendMsg(QString)), context->getInfoTrans(), SLOT(receiveMsg(QString)));
	}
	return card;
}

/*!
 * \brief Генерация карты-команды с характеристиками второго уровня
 * \param connection Является ли карта корневой
 * \return Карта-команда второго уровня
 */
ICard *CardFactoryTier2::generateTeamCard(bool connection)
{
	int numMembers = Random::randInt(5, 8);
	TeamCard *card = new TeamCard();
	card->setName(nameGen->randomTeamName());
	QList<ICard *> members;
	for (int i = 0; i < numMembers; i++)
	{
		ICard *member;
		switch (Random::randIntWeighted({25, 15, 5}))
		{
			case 0:
				member = this->generateActorCard(false);
				break;
			case 1:
				member = this->generateActionCard(false);
				break;
			case 2:
				member = this->generateTeamCard(false);
				break;
		}
		members.push_back(member);
	}
	card->setMembers(members);
	connect(card, SIGNAL(sendMsg(QString)), context->getInfoTrans(), SLOT(receiveMsg(QString)));
	return card;
}

/*!
 * \brief Установка ссылки на генератор имен
 * \param ng Объект генератора имен
 */
void CardFactoryTier2::installNamegen(NameGen *ng)
{
	nameGen = ng;
}

/*!
 * \brief Установка ссылки на игровой сеанс
 * \param gs Объект игрового сеанса
 */
void CardFactoryTier2::setContext(GameSession *gs)
{
	context = gs;
}

/*!
 * \brief Конструктор фабрики карт третьего уровня
 * \param parent Родительский объект класса QObject
 */
CardFactoryTier3::CardFactoryTier3(QObject *parent) : CardFactoryTiered(parent)
{

}

/*!
 * \brief Установка ссылки на фабрику эффектов
 * \param ef Объект фабрики эффектов
 */
void CardFactoryTier3::setEffectFactory(EffectFactory *ef)
{
	effFactory = ef;
}

/*!
 * \brief Генерация карты-персонажа с характеристиками третьего уровня
 * \param connection Является ли карта корневой
 * \return Карта-персонаж третьего уровня
 */
ICard *CardFactoryTier3::generateActorCard(bool connection)
{
	ActorCard *card = new ActorCard();
	card->setName(nameGen->randomActorName());
	QList<IEffect *> effects;
	card->setStat("health", 300, false);
	card->setStat("energy", 300, false);
	card->setStat("healthInit", 300, false);
	card->setStat("energyInit", 300, false);
	int numEffects = Random::randInt(1, 3);
	for (int j = 0; j < numEffects; j++)
	{
		IEffect *e = effFactory->generateEffect(connection);
		e->setStat("power", e->getStat("power") * 3);
		e->setStat("amount", e->getStat("amount") * 3);
		effects.push_back(e);
	}
	IEffectSelector *selector;
	switch (Random::randIntWeighted({5, 5}))
	{
		case 0:
			selector = new EffectSelectorRandom();
			break;
		case 1:
			selector = new EffectSelectorAll();
			break;
	}
	card->setEffectSelector(selector);
	card->setEffects(effects);
	if (connection)
	{
		connect(card, SIGNAL(sendMsg(QString)), context->getInfoTrans(), SLOT(receiveMsg(QString)));
	}
	return card;
}

/*!
 * \brief Генерация карты-действия с характеристиками третьего уровня
 * \param connection Является ли карта корневой
 * \return Объект карты-действия третьего уровня
 */
ICard *CardFactoryTier3::generateActionCard(bool connection)
{
	ActionCard *card = new ActionCard();
	card->setName(nameGen->randomActionName());
	IEffect *e = effFactory->generateEffect(connection);
	card->setEffects({e});
	e->setStat("power", e->getStat("power") * 3);
	e->setStat("amount", e->getStat("amount") * 3);
	if (connection)
	{
		connect(card, SIGNAL(sendMsg(QString)), context->getInfoTrans(), SLOT(receiveMsg(QString)));
	}
	return card;
}

/*!
 * \brief Генерация карты-команды с характеристиками третьего уровня
 * \param connection Является ли карта корневой
 * \return Объект карты-команды третьего уровня
 */
ICard *CardFactoryTier3::generateTeamCard(bool connection)
{
	int numMembers = Random::randInt(5, 8);
	TeamCard *card = new TeamCard();
	card->setName(nameGen->randomTeamName());
	QList<ICard *> members;
	for (int i = 0; i < numMembers; i++)
	{
		ICard *member;
		switch (Random::randIntWeighted({25, 15, 5}))
		{
			case 0:
				member = this->generateActorCard(false);
				break;
			case 1:
				member = this->generateActionCard(false);
				break;
			case 2:
				member = this->generateTeamCard(false);
				break;
		}
		members.push_back(member);
	}
	card->setMembers(members);
	connect(card, SIGNAL(sendMsg(QString)), context->getInfoTrans(), SLOT(receiveMsg(QString)));
	return card;
}

/*!
 * \brief Установка ссылки на генератор имен
 * \param ng Объект генератора имен
 */
void CardFactoryTier3::installNamegen(NameGen *ng)
{
	nameGen = ng;
}

/*!
 * \brief Установка ссылки на игровой сеанс
 * \param gs Объект игрового сеанса
 */
void CardFactoryTier3::setContext(GameSession *gs)
{
	context = gs;
}
