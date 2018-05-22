#include "effectfactory.h"

/*!
 * \brief Конструктор объекта фабрики эффектов
 * \param parent Родительский объект класса QObject
 */
EffectFactory::EffectFactory(QObject *parent) : QObject(parent)
{
	builder = new EffectBuilder(this);
	director = new EffectBuildDirector(builder, this);
}

/*!
 * \brief Установка ссылки на игровой сеанс
 * \param gs Объект игрового сеанса
 */
void EffectFactory::setContext(GameSession *gs)
{
	context = gs;
}

/*!
 * \brief Установка ссылки на генератор имен
 * \param ng Объект генератора имен
 */
void EffectFactory::installNamegen(NameGen *ng)
{
	nameGen = ng;
}

/*!
 * \brief Генерация эффекта
 * \param connection Должен ли эффект принадлежать корневой карте
 * \return Объект эффекта
 */
IEffect *EffectFactory::generateEffect(bool connection)
{
	IEffect *e;
	director->construct();
	e = builder->getEffect();
	if (connection)
		connect(e, SIGNAL(callCardSelect()), context, SLOT(cardSelectCall()));
	connect(e, SIGNAL(sendMsg(QString)), context->getInfoTrans(), SLOT(receiveMsg(QString)));
	return e;
}

/*!
 * \brief Конструктор объекта строителя эффектов
 * \param parent Родительский объект класса QObject
 */
EffectBuilder::EffectBuilder(QObject *parent)
{

}

/*!
 * \brief Генерация эффекта определенного типа
 * \param eType Тип генерируемого эффекта (0 - атака, 1 - помощь)
 */
void EffectBuilder::generateEffect(int eType)
{
	NameGen *nameGen = new NameGen();
	switch (eType)
	{
		case 0:
			{
				result = new AttackEffect();
				result->setName(nameGen->randomEffectName());
				result->setStat("cost", Random::randInt(1, 5));
				result->setStat("power", Random::randInt(10, 40));
				break;
			}
		case 1:
			{
				result = new HelpEffect();
				result->setName(nameGen->randomEffectName());
				result->setStat("amount", Random::randInt(10, 50));
				result->setStat("limit", 1);
				result->setStat("helptype", Random::randIntWeighted({10, 5}));
				break;
			}
	}
}

/*!
 * \brief Применение модификатора к генерируемому эффекту
 */
void EffectBuilder::applyModifier()
{
	EffectDestabilizer *destab = new EffectDestabilizer(result);
	destab->setName("разброс");
	destab->setStat("distAmount", Random::randInt(5, 10));
	result = destab;
}

/*!
 * \brief Получение сгенерированного эффекта
 * \return Объект эффекта
 */
IEffect *EffectBuilder::getEffect()
{
	return result;
}

/*!
 * \brief Конструктор объекта директора строителя эффектов
 * \param parent Родительский объект класса QObject
 */
EffectBuildDirector::EffectBuildDirector(QObject *parent) : QObject(parent)
{

}

/*!
 * \brief Конструктор объекта директора строителя эффектов с явным определением строителя
 * \param bldr Объект строителя
 * \param parent Родительский объект класса QObject
 */
EffectBuildDirector::EffectBuildDirector(EffectBuilder *bldr, QObject *parent) : EffectBuildDirector(parent)
{
	builder = bldr;
}

/*!
 * \brief Построение эффекта при помощи указания команд строителю
 */
void EffectBuildDirector::construct()
{
	builder->generateEffect(Random::randIntWeighted({75, 25}));
	if (Random::probability(0.5))
		builder->applyModifier();
}
