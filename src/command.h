#ifndef COMMAND_H
#define COMMAND_H

#include "defs.h"
#include "gamesession.h"
#include "player.h"

class GameSession;

/*!
 * \brief Класс Command - команда
 * \details Используется для передаче игровому сеансу команд для выполнения
 */
class Command
{
	protected:
		/*!
		 * \brief Контекст - игровой сеанс
		 */
		GameSession *context;
	public:
		Command();
		void setContext(GameSession *gs);
		/*!
		 * \brief Выполнение
		 */
		virtual void execute() = 0;
};

/*!
 * \brief Класс PlayerCardUseCommand - команда на вывод карты на поле
 */
class PlayerCardUseCommand : public Command
{
	protected:
		/*!
		 * \brief Текущий выбор
		 */
		QString pickID;
	public:
		PlayerCardUseCommand();
		void execute();
		void setPick(QString id);
};

/*!
 * \brief Класс PlayerCardActivateCommand - команда на задействование карты
 */
class PlayerCardActivateCommand : public Command
{
	protected:
		/*!
		 * \brief Текущий выбор
		 */
		QString pickID;
	public:
		PlayerCardActivateCommand();
		void execute();
		void setPick(QString id);
};

#endif // COMMAND_H
