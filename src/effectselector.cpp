#include "effectselector.h"

/*!
 * \brief Конструктор интерфейса выборщика эффектов
 * \param parent Родительский объект класса QObject
 */
IEffectSelector::IEffectSelector(QObject *parent) : QObject(parent)
{

}

/*!
 * \brief Конструктор объекта выборщика одного случайного эффекта
 * \param parent Родительский объект класса QObject
 */
EffectSelectorRandom::EffectSelectorRandom(QObject *parent) : IEffectSelector(parent)
{

}

/*!
 * \brief Выбор эффекта из предложенных
 * \param effs Коллекция доступных эффектов
 * \return Результат выборки - один случайный эффект
 */
QList<IEffect *> EffectSelectorRandom::selectEffect(QList<IEffect *> effs)
{
	return {effs[Random::randInt(0, effs.size() - 1)]};
}

/*!
 * \brief Получение значения типа выборщика
 * \return Значение типа выборщика
 */
int EffectSelectorRandom::getType()
{
	return 0;
}

/*!
 * \brief Конструктор объекта выборщика, перемешивающего исходную коллекцию
 * \param parent Родительский объект класса QObject
 */
EffectSelectorAll::EffectSelectorAll(QObject *parent) : IEffectSelector(parent)
{

}

/*!
 * \brief Выбор эффекта из предложенных
 * \param effs Коллекция доступных эффектов
 * \return Результат выборки - перетасованная коллекция
 */
QList<IEffect *> EffectSelectorAll::selectEffect(QList<IEffect *> effs)
{
	QList<IEffect *> result, src = effs;
	while (!src.empty())
		result.push_back(src.takeAt(Random::randInt(0, src.size() - 1)));
	return result;
}

/*!
 * \brief Получение значения типа выборщика
 * \return Значение типа выборщика
 */
int EffectSelectorAll::getType()
{
	return 1;
}
