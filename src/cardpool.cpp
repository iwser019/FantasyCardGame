#include "cardpool.h"

CardPool* CardPool::instance = NULL;

/*!
 * \brief Получение объекта пула карт
 * \return Объект пула карт
 */
CardPool *CardPool::getInstance()
{
	if (instance == NULL)
		instance = new CardPool();
	return instance;
}

/*!
 * \brief Помещение карты в пул
 * \param card Карта
 */
void CardPool::putCard(ICard *card)
{
	ICard *cardToPut = card;
	card->reset();
	cards.push_back(cardToPut);
}

/*!
 * \brief Получение карты из пула. Если пул пуст, возвращается 0
 * \return Карта
 */
ICard *CardPool::getCard()
{
	if (cards.empty())
		return NULL;
	ICard *cardToGet = cards.back();
	cards.pop_back();
	return cardToGet;
}

/*!
 * \brief Перемешивание карт
 */
void CardPool::shuffle()
{
	QList<ICard *> srcCards = cards;
	cards.clear();
	while (!srcCards.empty())
		cards.push_back(srcCards.takeAt(Random::randInt(0, srcCards.size() - 1)));
}

/*!
 * \brief Очистка пула
 */
void CardPool::clear()
{
	while (!cards.empty())
		delete cards.takeFirst();
}

/*!
 * \brief Проверка на то, пустой ли пул
 * \return Значение пустоты пула
 */
bool CardPool::isEmpty()
{
	return (cards.size() == 0);
}

/*!
 * \brief Конструктор пула карт
 * \param parent Родительский объект класса QObject
 */
CardPool::CardPool(QObject *parent) : QObject(parent)
{

}
