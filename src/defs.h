#ifndef DEFS_H
#define DEFS_H

#define CARD_POOL_SIZE 200

#include <QList>
#include <QString>
#include <QThread>
#include <QImage>
#include <QPainter>
#include <QColor>
#include <QMap>
#include <QFile>
#include <QStringList>
#include <QDataStream>
#include "randomizer.h"

/*!
 * \brief Структура listItem - элемент информации о карте
 */
struct listItem
{
		/*!
		 * \brief Графическое изображение карты
		 */
		QImage drawable;
		/*!
		 * \brief Псевдографическое представление карты
		 */
		QString pseudoDrawable;
		/*!
		 * \brief Имя карты
		 */
		QString name;
		/*!
		 * \brief ID карты
		 */
		QString uid;
		listItem()
		{

		}
		listItem(QImage _drawable, QString _name, QString _uid)
		{
			drawable = _drawable;
			name = _name;
			uid = _uid;
		}
		listItem(QImage _drawable, QString _pDrawable, QString _name, QString _uid)
		{
			drawable = _drawable;
			pseudoDrawable = _pDrawable;
			name = _name;
			uid = _uid;
		}
};

/*!
 * \brief Структура effectInfoItem - элемент информации об эффекте
 */
struct effectInfoItem
{
		/*!
		 * \brief Текст информации
		 */
		QString info;
		/*!
		 * \brief Может действовать на карты противника
		 */
		bool canAffectEnemy;
		/*!
		 * \brief Может действовать на карты игрока
		 */
		bool canAffectPlayer;
		/*!
		 * \brief Тип эффекта
		 */
		int effectType;
		effectInfoItem()
		{

		}
		effectInfoItem(QString _info, bool affectEnemy, bool affectPlayer, int _type)
		{
			info = _info;
			canAffectEnemy = affectEnemy;
			canAffectPlayer = affectPlayer;
			effectType = _type;
		}
};


#endif // DEFS_H
