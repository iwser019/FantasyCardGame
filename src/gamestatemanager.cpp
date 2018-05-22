#include "gamestatemanager.h"

/*!
 * \brief Конструктор объекта диспетчера состояний
 * \param parent Родительский объект класса QObject
 */
GameStateManager::GameStateManager(QObject *parent) : QObject(parent)
{

}

/*!
 * \brief Сохранение состояния игры во внешнюю память
 * \param gs Объект состояния игры
 * \param ok Индикатор успешного завершения
 */
void GameStateManager::saveState(GameState *gs, int *ok)
{
	int result = 0;
	int sizeInt = 0;
	try
	{
		CardStreamWriter *csw = new CardStreamWriter();
		QFile outf("savegame.bin");
		outf.open(QFile::WriteOnly | QFile::Truncate);
		QDataStream outs(&outf);
		outs.setVersion(QDataStream::Qt_4_0);
		csw->setupStream(&outs);
		QList<ICard *> cards;
		cards = gs->getCardSet(GameStateCardsSet::GS_POOL);
		sizeInt = cards.size();
		outs << sizeInt;
		while (!cards.empty())
			csw->writeCard(cards.takeFirst());
		bool players = gs->getPlayersFlag();
		outs << players;
		if (players)
		{
			cards = gs->getCardSet(GameStateCardsSet::GS_PLAYER);
			sizeInt = cards.size();
			outs << sizeInt;
			while (!cards.empty())
				csw->writeCard(cards.takeFirst());
			cards = gs->getCardSet(GameStateCardsSet::GS_PLAYERUSED);
			sizeInt = cards.size();
			outs << sizeInt;
			while (!cards.empty())
				csw->writeCard(cards.takeFirst());
			cards = gs->getCardSet(GameStateCardsSet::GS_ENEMY);
			sizeInt = cards.size();
			outs << sizeInt;
			while (!cards.empty())
				csw->writeCard(cards.takeFirst());
			cards = gs->getCardSet(GameStateCardsSet::GS_ENEMYUSED);
			sizeInt = cards.size();
			outs << sizeInt;
			while (!cards.empty())
				csw->writeCard(cards.takeFirst());
			cards = gs->getCardSet(GameStateCardsSet::GS_BEATEN);
			sizeInt = cards.size();
			outs << sizeInt;
			while (!cards.empty())
				csw->writeCard(cards.takeFirst());
		}
	}
	catch (...)
	{
		result = 1;
	}
	if (ok != NULL)
		*ok = result;
}

/*!
 * \brief Загрузка состояния игры из внешней памяти
 * \param ok Индикатор успешного завершения
 * \return Объект состояния игры
 */
GameState *GameStateManager::loadState(int *ok)
{
	int result = 0;
	int sizeInt = 0;
	GameState *gs = new GameState();
	try
	{
		CardStreamReader *csr = new CardStreamReader();
		QFile inf("savegame.bin");
		inf.open(QFile::ReadOnly);
		QList<ICard *> cards;
		QDataStream ins(&inf);
		ins.setVersion(QDataStream::Qt_4_0);
		csr->setupStream(&ins);
		bool players = false;
		int cardType;
		ICard *card = NULL;
		ins >> sizeInt;
		for (int i = 0; i < sizeInt; i++)
		{
			ins >> cardType;
			card = matchCardPointer(cardType);
			csr->readCard(card);
			cards.push_back(card);
		}
		gs->setCardSet(cards, GameStateCardsSet::GS_POOL);
		ins >> players;
		gs->setPlayersFlag(players);
		if (players)
		{
			ins >> sizeInt;
			cards.clear();
			for (int i = 0; i < sizeInt; i++)
			{
				ins >> cardType;
				card = matchCardPointer(cardType);
				csr->readCard(card);
				cards.push_back(card);
			}
			gs->setCardSet(cards, GameStateCardsSet::GS_PLAYER);
			ins >> sizeInt;
			cards.clear();
			for (int i = 0; i < sizeInt; i++)
			{
				ins >> cardType;
				card = matchCardPointer(cardType);
				csr->readCard(card);
				cards.push_back(card);
			}
			gs->setCardSet(cards, GameStateCardsSet::GS_PLAYERUSED);
			ins >> sizeInt;
			cards.clear();
			for (int i = 0; i < sizeInt; i++)
			{
				ins >> cardType;
				card = matchCardPointer(cardType);
				csr->readCard(card);
				cards.push_back(card);
			}
			gs->setCardSet(cards, GameStateCardsSet::GS_ENEMY);
			ins >> sizeInt;
			cards.clear();
			for (int i = 0; i < sizeInt; i++)
			{
				ins >> cardType;
				card = matchCardPointer(cardType);
				csr->readCard(card);
				cards.push_back(card);
			}
			gs->setCardSet(cards, GameStateCardsSet::GS_ENEMYUSED);
			ins >> sizeInt;
			cards.clear();
			for (int i = 0; i < sizeInt; i++)
			{
				ins >> cardType;
				card = matchCardPointer(cardType);
				csr->readCard(card);
				cards.push_back(card);
			}
			gs->setCardSet(cards, GameStateCardsSet::GS_BEATEN);
		}
	}
	catch (...)
	{
		result = 1;
	}
	if (ok != NULL)
		*ok = result;
	return gs;
}
