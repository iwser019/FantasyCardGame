#ifndef GAMESESSION_H
#define GAMESESSION_H

#include <QObject>
#include "gamecontroller.h"
#include "defs.h"
#include "card.h"
#include "effect.h"
#include "namegen.h"
#include "player.h"
#include "effectfactory.h"
#include "cardfactory.h"
#include "cardpool.h"
#include "gamestatemanager.h"

class EffectFactory;
class CardFactory;
class CardFactoryTiered;
class CardInfoObserver;
class Player;
class PlayerMediator;
class GameStateManager;

/*!
 * \brief Класс GameSession - игровой сеанс
 * \details Представляет собой основную игровую модель, взаимодействует с большинством
 * основных объектов приложения. Допускается существование только одного
 * объекта во время выполнения приложения (синглтон)
 */
class GameSession : public QObject
{
	Q_OBJECT
	private:
		/*!
		 * \brief Экземпляр
		 */
		static GameSession *instance;
		/*!
		 * \brief Контроллер для взаимодействия
		 */
		GameController *controller;
		/*!
		 * \brief Игроки
		 */
		QList<Player *> players;
		/*!
		 * \brief "Битые" карты - карты с нулевым здоровьем
		 */
		QList<ICard *> beatenCards;
		/*!
		 * \brief Выборка карт по UID
		 */
		QVector<QString> selection;
		/*!
		 * \brief Информация по тому или иному действию
		 */
		effectInfoItem eInfo;
		/*!
		 * \brief Текущий ход
		 */
		int currentTurn;
		/*!
		 * \brief Победитель
		 */
		int winner;
		/*!
		 * \brief Текущее действие карты
		 */
		int curCardAction;
		/*!
		 * \brief Готовность
		 */
		bool ready;
		/*!
		 * \brief Передатчик сообщений
		 */
		InfoTransmitter *infoTrans;
		/*!
		 * \brief Коллекция объектов фабрик карт
		 */
		QList<CardFactory *> cardFactories;
		/*!
		 * \brief Коллекция объектов фабрик карт с уровнем
		 */
		QList<CardFactoryTiered *> cardFactoriesTiered;
		/*!
		 * \brief Фабрика эффектов
		 */
		EffectFactory *effFactory;
		/*!
		 * \brief Наблюдатель информации о состоянии карт
		 */
		CardInfoObserver *cio;
		/*!
		 * \brief Менеджер состояний для загрузки и сохранения
		 */
		GameStateManager *gsm;
		/*!
		 * \brief Посредник между игроками
		 */
		PlayerMediator *pMed;
		// приватный конструктор
		GameSession(QObject *parent = 0);
		// генерация карт
		QList<ICard *> generateCards(int num, int ctype = -1, bool connections = true);
	public:
		static GameSession * getInstance(bool reset = false);
		void reset();
		void installController(GameController *gc);
		void initialize(int numCards);
		// получение всех картинок
		QList<listItem> getDrawables();
		// готовность
		bool isReady();
		InfoTransmitter * getInfoTrans();
		QString getCardFullInfo(QString uid);
		int saveState();
		int loadState();
		Player * getCurrentPlayer();
		Player * getNextPlayer();
	public slots:
		// когда карта бита
		void disposeCard();
		void disposeCard(ICard *card);
		// переход хода
		void nextTurn();
		void cardInfoRequest(int dest, QString id);
		// вызов выбора карты
		void cardSelectCall();
		// повторители для работы со списками
		// и отображением данных
		// используют контроллер
		void listItemRemoveRepeater(listItem item);
		void listItemAddRepeater(listItem item, int dest);
		void listItemUpdateRepeater(listItem item);
		void selectionModeRepeater(bool enable, bool owned);
		void cardCountRepeater(int count);
		void effectsInfoRepeater(QList<effectInfoItem> effs);
		// сброс треда игрока
		void restartPlayerThread();
		// отправка выборки карт
		void sendCardSelection(QVector<QString> ids, effectInfoItem eff);
		void setCurEffInfo(effectInfoItem eff);
		void setCurrentAction(int id);
		void msgRepeater(QString msg);
	signals:
		/*!
		 * \brief Отображение количества карт противника
		 * \param count Количество карт
		 */
		showCardCount(int count);
		/*!
		 * \brief Отправка информации о карте
		 * \param cardInfo Текст информации о карте
		 * \param dest Поле назначения
		 */
		getCardInfo(QString cardInfo, int dest);
		/*!
		 * \brief Установка режима выбора карт
		 * \param enable Включение выбора
		 * \param isOwned Выбор своих карт
		 */
		setCardSelectionMode(bool enable, bool isOwned);
		/*!
		 * \brief Отображение информации об эффектах выбранной карты
		 * \param effs Список элементов информации об эффектах
		 */
		void showPickedCardInfo(QList<effectInfoItem> effs);
		/*!
		 * \brief Окончание игры
		 * \param winner ID победителя
		 */
		endGame(int winner);
		/*!
		 * \brief Запрос на инициализацию интерфейса
		 */
		initUI();
		/*!
		 * \brief Отправка текстового сообщения
		 * \param Сообщение
		 */
		sendMsg(QString msg);
};

#endif // GAMESESSION_H
