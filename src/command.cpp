#include "command.h"

/*!
 * \brief Конструктор объекта команды
 */
Command::Command()
{
	context = nullptr;
}

/*!
 * \brief Установка ссылки на игровой сеанс
 * \param gs Объект игрового сеанса
 */
void Command::setContext(GameSession *gs)
{
	context = gs;
}

/*!
 * \brief Конструктор объекта команды вывода карты на поле
 */
PlayerCardUseCommand::PlayerCardUseCommand() : Command()
{
	pickID = "";
}

/*!
 * \brief Выполнение команды вывода карты на поле
 */
void PlayerCardUseCommand::execute()
{
	Player *plr = context->getCurrentPlayer();
	if (plr != nullptr)
	{
		plr->setState(new PlayerStateCardSelect(dynamic_cast<Player*>(plr)), {pickID});
		plr->getThread()->terminate();
		plr->getThread()->start();
	}
}

/*!
 * \brief Установка текущего выбора карты для вывода на поле
 * \param id UID карты
 */
void PlayerCardUseCommand::setPick(QString id)
{
	pickID = id;
}

/*!
 * \brief Конструктор объекта команды задействования карты
 */
PlayerCardActivateCommand::PlayerCardActivateCommand() : Command()
{
	pickID = "";
}

/*!
 * \brief Выполнение команды задействования карты
 */
void PlayerCardActivateCommand::execute()
{
	Player *plr = context->getCurrentPlayer();
	if (plr != nullptr)
	{
		plr->setState(new PlayerStateCardActivation(dynamic_cast<Player*>(plr)), {pickID});
		plr->getThread()->terminate();
		plr->getThread()->start();
	}
}

/*!
 * \brief Установка текущего выбора карты для задействования
 * \param id UID карты
 */
void PlayerCardActivateCommand::setPick(QString id)
{
	pickID = id;
}
