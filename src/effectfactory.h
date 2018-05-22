#ifndef EFFECTFACTORY_H
#define EFFECTFACTORY_H

#include <QObject>
#include "defs.h"
#include "namegen.h"
#include "gamesession.h"
#include "effect.h"

class GameSession;
class AttackEffect;
class HelpEffect;

/*!
 * \brief Объект EffectBuilder - строитель эффектов
 * \details Генерирует объекты эффектов по частям
 */
class EffectBuilder : public QObject
{
		Q_OBJECT
	public:
		EffectBuilder(QObject *parent = 0);
		void generateEffect(int eType);
		void applyModifier();
		IEffect *getEffect();
	private:
		/*!
		 * \brief Результат генерации
		 */
		IEffect *result;
};

/*!
 * \brief EffectBuildDirector - директр строителя эффектов
 * \details Управляет процессом построения эффекта
 */
class EffectBuildDirector : public QObject
{
		Q_OBJECT
	public:
		EffectBuildDirector(QObject *parent = 0);
		EffectBuildDirector(EffectBuilder *bldr, QObject *parent = 0);
		void construct();
	private:
		/*!
		 * \brief Объект строителя
		 */
		EffectBuilder *builder;
};

/*!
 * \brief Класс EffectFactory - фабрика эффектов
 * \details Генерирует объекты эффектов со случайными характеристиками
 * на основе некоторых параметров. Действует на основе "строителя"
 */
class EffectFactory : public QObject
{
		Q_OBJECT
	public:
		explicit EffectFactory(QObject *parent = 0);
		void setContext(GameSession *gs);
		void installNamegen(NameGen *ng);
		IEffect * generateEffect(bool connection = true);
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
		 * \brief Объект строителя
		 */
		EffectBuilder *builder;
		/*!
		 * \brief Объект директора
		 */
		EffectBuildDirector *director;

	signals:

	public slots:
};

#endif // EFFECTFACTORY_H
