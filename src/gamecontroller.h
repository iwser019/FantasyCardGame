#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include "defs.h"
#include "util.h"

class GameSession;

/*!
 * \brief Интерфейс GameController - контроллер
 * \details Используется для взаимодействия между игровой моделью и пользовательским
 * интерфейсом
 */
class GameController
{
	public:
		/*!
		 * \brief Конструктор
		 */
		explicit GameController();
		/*!
		 * \brief Отображение сообщения
		 * \param msg Сообщение
		 */
		virtual void showMsg(QString msg) = 0;
		/*!
		 * \brief Добавление записи о карте
		 * \param item Элемент списка карт
		 * \param dest Список назначения
		 */
		virtual void addCardEntry(listItem item, int dest) = 0;
		/*!
		 * \brief Обновление записи о карте
		 * \param item Элемент списка карт
		 */
		virtual void updateCardEntry(listItem item) = 0;
		/*!
		 * \brief Удаление записи о карте
		 * \param item Элемент списка карт
		 */
		virtual void removeCardEntry(listItem item) = 0;
		/*!
		 * \brief Вызов выбора эффекта карты
		 * \param actions Список дооступных эффектов
		 */
		virtual void callForActionChoose(QList<effectInfoItem> actions) = 0;
		/*!
		 * \brief Отображение итога игры
		 * \param playerID Игрок-победитель
		 */
		virtual void showGameResult(int playerID) = 0;
		/*!
		 * \brief Отображение количества карт у противника
		 * \param count Количество карт
		 */
		virtual void showEnemyCardCount(int count) = 0;
		/*!
		 * \brief Отображение информации о карте
		 * \param cardInfo Информация о карте
		 * \param dest Номер поля назначения
		 */
		virtual void getCardInfo(QString cardInfo, int dest) = 0;
		/*!
		 * \brief Установление режима выбора карт
		 * \param enable Включен ли режим
		 * \param isOwned Свои (true) карты или противника
		 */
		virtual void setCardSelectMode(bool enable, bool isOwned) = 0;
		/*!
		 * \brief Установка контекста
		 * \param game Объект игрового сеанса
		 * \param initial Инициация
		 */
		virtual void setContext(GameSession *game, bool initial = true) = 0;
		/*!
		 * \brief Подготовка контроллера
		 */
		virtual void makeReady() = 0;
		/*!
		 * \brief Очистка всех записей о картах
		 */
		virtual void clearAllEntries() = 0;
};

#endif // GAMECONTROLLER_H
