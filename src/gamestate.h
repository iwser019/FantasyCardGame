#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "defs.h"
#include "card.h"

/*!
 * \brief Структура GameStateCardsSet - тип набора карт
 */
enum GameStateCardsSet
{
	GS_POOL = 0,
	GS_PLAYER,
	GS_PLAYERUSED,
	GS_ENEMY,
	GS_ENEMYUSED,
	GS_BEATEN
};

/*!
 * \brief Класс GameState - состояние игры
 * \details Хранит в себе данные об игроках и картах, участвующих в игре на момент
 * состояния
 */
class GameState
{
	private:
		/*!
		 * \brief Наборы карт
		 */
		QList <ICard *> pool,
		player,
		playerUsed,
		enemy,
		enemyUsed,
		beaten;
		/*!
		 * \brief Флаг наличия игроков
		 */
		bool playersFlag;
	public:
		GameState();
		void setCardSet(QList<ICard *> cards, int cardsType);
		QList<ICard *> getCardSet(int cardsType);
		void setPlayersFlag(bool flag);
		bool getPlayersFlag();
};

#endif // GAMESTATE_H
