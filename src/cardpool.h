#ifndef CARDPOOL_H
#define CARDPOOL_H

#include <QObject>
#include "defs.h"
#include "card.h"

/*!
 * \brief Класс CardPool - пул карт
 * \details Пул карт, используемый для хранения объектов карт, используемых
 * в рамках игрового сеанса. При добавлении и извлечении карт
 * предусматтривается сброс параметров карт. Поддерживается перетасовка карт.
 */
class CardPool : public QObject
{
		Q_OBJECT
	public:
		static CardPool * getInstance();
		// добавление в пул и извлечение из него
		void putCard(ICard *card);
		ICard * getCard();
		// перетасовка "колоды"
		void shuffle();
		void clear();
		bool isEmpty();
	private:
		/*!
		 * \brief Экземпляр класса
		 */
		static CardPool *instance;
		/*!
		 * \brief Коллекция карт
		 */
		QList<ICard *> cards;
		explicit CardPool(QObject *parent = 0);
	signals:

	public slots:
};

#endif // CARDPOOL_H
