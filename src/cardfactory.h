#ifndef CARDFACTORY_H
#define CARDFACTORY_H

#include <QObject>
#include "defs.h"
#include "gamesession.h"
#include "card.h"
#include "namegen.h"
#include "effect.h"
#include "effectselector.h"
#include "effectfactory.h"

class GameSession;
class EffectFactory;

/*!
 * \brief Класс CardFactoryTiered - интерфейс карт с уровнем
 * \details Интерфейс фабрики карт определенного уровня, отличающиеся определенными
 * параметрами
 */
class CardFactoryTiered : public QObject
{
		Q_OBJECT
	public:
		CardFactoryTiered(QObject *parent = 0);
		/*!
		 * \brief Установка контекста
		 * \param gs Объект игрового сеанса
		 */
		virtual void setContext(GameSession *gs) = 0;
		/*!
		 * \brief Установка генератора имен
		 * \param ng Объект генератора имен
		 */
		virtual void installNamegen(NameGen *ng) = 0;
		/*!
		 * \brief Установка фабрики эффектов
		 * \param ef Объект фабрики эффектов
		 */
		virtual void setEffectFactory(EffectFactory *ef) = 0;
		/*!
		 * \brief Генерация карты-персонажа
		 * \param connection Флаг корневой карты
		 * \return Объект карты-персонажа
		 */
		virtual ICard * generateActorCard(bool connection = true) = 0;
		/*!
		 * \brief Генерация карты-действия
		 * \param connection Флаг корневой карты
		 * \return Объект карты-действия
		 */
		virtual ICard * generateActionCard(bool connection = true) = 0;
		/*!
		 * \brief Генерация карты-команды
		 * \param connection Флаг корневой карты
		 * \return Объект карты-команды
		 */
		virtual ICard * generateTeamCard(bool connection = true) = 0;
};

/*!
 * \brief Класс CardFactoryTier2 - Фабрика карт 2-го уровня
 */
class CardFactoryTier2 : public CardFactoryTiered
{
		Q_OBJECT
	public:
		CardFactoryTier2(QObject *parent = 0);
		void setContext(GameSession *gs);
		void installNamegen(NameGen *ng);
		void setEffectFactory(EffectFactory *ef);
		ICard * generateActorCard(bool connection = true);
		ICard * generateActionCard(bool connection = true);
		ICard * generateTeamCard(bool connection = true);
	private:
		GameSession *context;
		NameGen *nameGen;
		EffectFactory *effFactory;
};

/*!
 * \brief Класс CardFactoryTier3 - Фабрика карт 3-го уровня
 */\
class CardFactoryTier3 : public CardFactoryTiered
{
		Q_OBJECT
	public:
		CardFactoryTier3(QObject *parent = 0);
		void setContext(GameSession *gs);
		void installNamegen(NameGen *ng);
		void setEffectFactory(EffectFactory *ef);
		ICard * generateActorCard(bool connection = true);
		ICard * generateActionCard(bool connection = true);
		ICard * generateTeamCard(bool connection = true);
	private:
		GameSession *context;
		NameGen *nameGen;
		EffectFactory *effFactory;
};

/*!
 * \brief Класс CardFactory - интерфейс фабрики карт
 * \details Интерфейс фабрики карт, создает объекты карт различных типов.
 */
class CardFactory : public QObject
{
		Q_OBJECT
	public:
		explicit CardFactory(QObject *parent = 0);
		/*!
		 * \brief Установка контекста
		 * \param gs Объект игрового сеанса
		 */
		virtual void setContext(GameSession *gs) = 0;
		/*!
		 * \brief Установка генератора имен
		 * \param ng Объект генератора имен
		 */
		virtual void installNamegen(NameGen *ng) = 0;
		/*!
		 * \brief Установка фабрики эффектов
		 * \param ef Объект фабрики эффектов
		 */
		virtual void setEffectFactory(EffectFactory *ef) = 0;
		/*!
		 * \brief Генерация карты
		 * \param connect Флаг корневой карты
		 * \return Объект карты
		 */
		virtual ICard * generateCard(bool connect = true) = 0;

	private:

	signals:

	public slots:
};

/*!
 * \brief Класс ActorCardFactory - фабрика карт-персонажей
 * \details Класс фабрики, генерирующей объекты карт-персонажей
 */
class ActorCardFactory : public CardFactory
{
		Q_OBJECT
	public:
		ActorCardFactory(QObject *parent = 0);
		void setContext(GameSession *gs);
		void installNamegen(NameGen *ng);
		void setEffectFactory(EffectFactory *ef);
		ICard * generateCard(bool connection = true);
	private:
		/*!
		 * \brief Объект игрового сеанса
		 */
		GameSession *context;
		/*!
		 * \brief Объект генератора имен
		 */
		NameGen *nameGen;
		/*!
		 * \brief Объект фабрики эффектов
		 */
		EffectFactory *effFactory;
};

/*!
 * \brief Класс ActionCardFactory - фабрика карт-действий
 * \details Класс фабрики, генерирующий объекты карт-действий
 */
class ActionCardFactory : public CardFactory
{
		Q_OBJECT
	public:
		ActionCardFactory(QObject *parent = 0);
		void setContext(GameSession *gs);
		void installNamegen(NameGen *ng);
		void setEffectFactory(EffectFactory *ef);
		ICard * generateCard(bool connection = true);
	private:
		/*!
		 * \brief Объект игрового сеанса
		 */
		GameSession *context;
		/*!
		 * \brief Объект генератора имен
		 */
		NameGen *nameGen;
		/*!
		 * \brief Объект фабрики эффектов
		 */
		EffectFactory *effFactory;
};

/*!
 * \brief Класс TeamCardFactory - фабрика карт-команд
 * \details Класс фабрики, генерирующей объекты карт-команд.
 * Использует для генерации членов фабрики карт-персонажей и карт-действий
 */
class TeamCardFactory : public CardFactory
{
		Q_OBJECT
	public:
		TeamCardFactory(QObject *parent = 0);
		void setContext(GameSession *gs);
		void installNamegen(NameGen *ng);
		void setEffectFactory(EffectFactory *ef);
		void setSubfactories(CardFactory *forActor, CardFactory *forAction);
		ICard * generateCard(bool connection = true);
	private:
		/*!
		 * \brief Объект игрового сеанса
		 */
		GameSession *context;
		/*!
		 * \brief Объект генератора имен
		 */
		NameGen *nameGen;
		/*!
		 * \brief Объект фабрики эффектов
		 */
		EffectFactory *effFactory;
		/*!
		 * \brief Объект фабрики карт-персонажей
		 */
		CardFactory *actorFact;
		/*!
		 * \brief Объект фабрики карт-дейстий
		 */
		CardFactory *actionFact;
};

#endif // CARDFACTORY_H
