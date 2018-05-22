#ifndef EFFECTSELECTOR_H
#define EFFECTSELECTOR_H

#include <QObject>
#include "defs.h"
#include "effect.h"

/*!
 * \brief Интерфейс IEffectSelector - интерфейс выборщика эффектов
 * \details Используется в том случае, если при количестве эффектов более одного
 * пользователь при задействовании карты не сделал выбор эффекта явно
 */
class IEffectSelector : public QObject
{
		Q_OBJECT
	public:
		explicit IEffectSelector(QObject *parent = 0);
		/*!
		 * \brief Выбор эффекта из предложенных
		 * \param effs Коллекция доступных эффектов
		 * \return Результат выборки
		 */
		virtual QList<IEffect *> selectEffect(QList<IEffect *> effs) = 0;
		/*!
		 * \brief Получение значения типа выборщика
		 * \return Значение типа выборщика
		 */
		virtual int getType() = 0;
};

/*!
 * \brief Класс EffectSelectorRandom - случайный выборщик
 * \details Получает на вход несколько эффектов, отдает случайный эффект из набора
 */
class EffectSelectorRandom : public IEffectSelector
{
		Q_OBJECT
	public:
		explicit EffectSelectorRandom(QObject *parent = 0);
		QList<IEffect *> selectEffect(QList<IEffect *> effs);
		int getType();
};

/*!
 * \brief Класс EffectSelectorAll - выборщик с перетасовкой
 * \details Получает на вход несколько эффектов, расставляет их в случайном порядке
 * и отдает обратно
 */
class EffectSelectorAll : public IEffectSelector
{
		Q_OBJECT
	public:
		explicit EffectSelectorAll(QObject *parent = 0);
		QList<IEffect *> selectEffect(QList<IEffect *> effs);
		int getType();
};

#endif // EFFECTSELECTOR_H
