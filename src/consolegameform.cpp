#include "consolegameform.h"
#include "ui_consolegameform.h"

/*!
 * \brief Конструктор псевдоконсольной формы
 * \param parent Родительский объект класса QObject
 */
ConsoleGameForm::ConsoleGameForm(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::ConsoleGameForm)
{
	ui->setupUi(this);
	selectOwnedCards = false;
	currEffID = -1;
	cmdID = CMD_CARDNUM;
}

/*!
 * \brief Установка контекста
 * \param game Объект игрового сеанса
 * \param initial Инициальное состояние
 */
void ConsoleGameForm::setContext(GameSession *game, bool initial)
{
	gs = game;
	if (initial)
		showMsg("Сколько карт?");
}

/*!
 * \brief Запрос на выбор эффекта
 * \param actions Список элементов информации об эффектах
 */
void ConsoleGameForm::callForActionChoose(QList<effectInfoItem> actions)
{
	currEffs = actions;
	if (currEffs.size() > 1)
	{
		showMsg("Выберите действие:");
		for (int i = 0; i < currEffs.size(); i++)
			showMsg(QString::number(i + 1) + ") " + currEffs[i].info);
		showMsg(QString::number(currEffs.size() + 1) + ") " + "(по умолчанию)");
		cmdID = CMD_SELECT_ACTION;
	}
	else
	{
		currEffID = 0;
		cmdID = CMD_SELECT_TARGET;
		showTargets();
	}
}

/*!
 * \brief Отображение результатов игры
 * \param playerID ID победителя
 */
void ConsoleGameForm::showGameResult(int playerID)
{
	showMsg("Игра подошла к концу...");
	showMsg(playerID == 0 ?
				"Вы выстояли этот раунд, и вы победили!"
			  : "Вам больше нечем биться, а посему вы проиграли.");
	cmdID = CMD_ENDGAME;
	gs->installController(NULL);
}

/*!
 * \brief Отображение количества карт противника
 * \param count Количество карт
 */
void ConsoleGameForm::showEnemyCardCount(int count)
{
	ui->textOutputBox->append("(у противника осталось " + QString::number(count) + " карт)");
}

/*!
 * \brief Отображение информации о карте
 * \param cardInfo Текст информации
 * \param dest Поле назначения
 */
void ConsoleGameForm::getCardInfo(QString cardInfo, int dest)
{
	ui->textOutputBox->append(cardInfo);
}

/*!
 * \brief Установка режима выбора карт
 * \param enable Включение выбора
 * \param isOwned Выбор своих карт
 */
void ConsoleGameForm::setCardSelectMode(bool enable, bool isOwned)
{
	return;
}

/*!
 * \brief Выполнение команды
 * \param cmdText Текст команды
 */
void ConsoleGameForm::submitCommand(QString cmdText)
{
	if (cmdID == CMD_CARDNUM)
	{
		bool ok = false;
		int cardNum = cmdText.toInt(&ok);
		if (!ok || (cardNum < 1) || (cardNum > CARD_POOL_SIZE / 2))
		{
			showMsg("Неверный ввод, попробуйте еще раз!");
			return;
		}
		gs->initialize(cardNum);
		initUI();
		gs->nextTurn();
	}
	else if (cmdID == CMD_SELECT_CMD)
	{
		bool ok = false;
		int cmdNum = cmdText.toInt(&ok);
		if (!ok || cmdNum > 5 || cmdNum < 1)
		{
			showMsg("Неверный ввод, попробуйте еще раз!");
			return;
		}
		execCommand(cmdNum);
	}
	else if (cmdID == CMD_SELECT_CARDTOUSE)
	{
		bool ok = false;
		int cardNum = cmdText.toInt(&ok);
		if (!ok || (cardNum < 1) || (cardNum > currSelectionSet.size()))
		{
			showMsg("Неверный ввод, попробуйте еще раз!");
			return;
		}
		currPick = currSelectionSet[cardNum - 1];
		cmdID = CMD_IDLE;
		PlayerCardUseCommand *pcuc = new PlayerCardUseCommand();
		pcuc->setContext(gs);
		pcuc->setPick(currPick);
		pcuc->execute();
		delete pcuc;
		currPick = "";
	}
	else if (cmdID == CMD_SELECT_CARDTOACTIVATE)
	{
		bool ok = false;
		int cardNum = cmdText.toInt(&ok);
		if (!ok || (cardNum < 1) || (cardNum > currSelectionSet.size()))
		{
			showMsg("Неверный ввод, попробуйте еще раз!");
			return;
		}
		currPick = currSelectionSet[cardNum - 1];
		PlayerCardActivateCommand *pcac = new PlayerCardActivateCommand();
		pcac->setContext(gs);
		pcac->setPick(currPick);
		pcac->execute();
		delete pcac;
	}
	else if (cmdID == CMD_SELECT_ACTION)
	{
		bool ok = false;
		int effNum = cmdText.toInt(&ok);
		if (!ok || (effNum < 1) || (effNum > (currEffs.size() + 1)))
		{
			showMsg("Неверный ввод, попробуйте еще раз!");
			return;
		}
		currEffID = (effNum == (currEffs.size() + 1)) ? -1 : (effNum - 1);
		cmdID = CMD_SELECT_TARGET;
		showTargets();
	}
	else if (cmdID == CMD_SELECT_TARGET)
	{
		bool ok = false;
		int cardNum = cmdText.toInt(&ok);
		if (!ok || (cardNum < 1) || (cardNum > currSelectionSet.size()))
		{
			showMsg("Неверный ввод, попробуйте еще раз!");
			return;
		}
		targetPick = currSelectionSet[cardNum - 1];
		cmdID = CMD_IDLE;
		gs->setCurrentAction(currEffID);
		gs->sendCardSelection({targetPick},
							  ((currEffID == -1) ? effectInfoItem("COMBO", true, true, 0) : currEffs[currEffID]));
		currPick = "";
		targetPick = "";
	}
	else if (cmdID == CMD_ENDGAME)
	{
		this->close();
	}
}

/*!
 * \brief Выполнение команды
 * \param cmdNum Тип команды
 */
void ConsoleGameForm::execCommand(int cmdNum)
{
	int sizeInt = 0;
	if (cmdNum == 1)
	{
		QList<QString> ids, keys = cardMapping.keys();
		sizeInt = keys.size();
		for (int i = 0; i < sizeInt; i++)
			if (cardMapping[keys[i]] == 0)
				ids.push_back(keys[i]);
		sizeInt = ids.size();
		if (!ids.empty())
			for (int i = 0; i < sizeInt; i++)
			{
				ui->textOutputBox->append(QString::number(i + 1) + " " + cardItems[ids[i]].pseudoDrawable + " " + gs->getCardFullInfo(ids[i]));
			}
		else
			ui->textOutputBox->append("(нет)");
		makeReady();
	}
	else if (cmdNum == 2)
	{
		QList<QString> ids, keys = cardMapping.keys();
		sizeInt = keys.size();
		for (int i = 0; i < sizeInt; i++)
			if (cardMapping[keys[i]] == 1)
				ids.push_back(keys[i]);
		sizeInt = ids.size();
		if (!ids.empty())
			for (int i = 0; i < sizeInt; i++)
			{
				ui->textOutputBox->append(QString::number(i + 1) + " " + cardItems[ids[i]].pseudoDrawable + " " + gs->getCardFullInfo(ids[i]));
			}
		else
			ui->textOutputBox->append("(нет)");
		makeReady();
	}
	else if (cmdNum == 3)
	{
		QList<QString> ids, keys = cardMapping.keys();
		sizeInt = keys.size();
		for (int i = 0; i < sizeInt; i++)
			if (cardMapping[keys[i]] == 2)
				ids.push_back(keys[i]);
		sizeInt = ids.size();
		if (!ids.empty())
			for (int i = 0; i < sizeInt; i++)
			{
				ui->textOutputBox->append(QString::number(i + 1) + " " + cardItems[ids[i]].pseudoDrawable + " " + gs->getCardFullInfo(ids[i]));
			}
		else
			ui->textOutputBox->append("(нет)");
		makeReady();
	}
	else if (cmdNum == 4)
	{
		QList<QString> ids, keys = cardMapping.keys();
		sizeInt = keys.size();
		for (int i = 0; i < sizeInt; i++)
			if (cardMapping[keys[i]] == 0)
				ids.push_back(keys[i]);
		sizeInt = ids.size();
		if (!ids.empty())
			for (int i = 0; i < sizeInt; i++)
			{
				ui->textOutputBox->append(QString::number(i + 1) + " " + cardItems[ids[i]].pseudoDrawable + " " + gs->getCardFullInfo(ids[i]));
			}
		else
		{
			ui->textOutputBox->append("Нечего выводить!");
			makeReady();
			return;
		}
		currSelectionSet = ids;
		ui->textOutputBox->append("Выбор?");
		cmdID = CMD_SELECT_CARDTOUSE;
	}
	else if (cmdNum == 5)
	{
		QList<QString> ids, keys = cardMapping.keys();
		sizeInt = keys.size();
		for (int i = 0; i < sizeInt; i++)
			if (cardMapping[keys[i]] == 1)
				ids.push_back(keys[i]);
		sizeInt = ids.size();
		if (!ids.empty())
			for (int i = 0; i < sizeInt; i++)
			{
				ui->textOutputBox->append(QString::number(i + 1) + " " + cardItems[ids[i]].pseudoDrawable + " " + gs->getCardFullInfo(ids[i]));
			}
		else
		{
			ui->textOutputBox->append("Нечего выбирать!");
			makeReady();
			return;
		}
		currSelectionSet = ids;
		ui->textOutputBox->append("Выбор?");
		cmdID = CMD_SELECT_CARDTOACTIVATE;
	}
}

/*!
 * \brief Отображение целей
 */
void ConsoleGameForm::showTargets()
{
	QList<QString> sel;
	int sizeInt = 0;
	showMsg("Выберите цель:");
	if (currEffID != -1)
	{
		sizeInt = cardMapping.size();
		QList<QString> keys = cardMapping.keys();
		for (int i = 0; i < sizeInt; i++)
		{
			if (currEffs[currEffID].canAffectPlayer && cardMapping[keys[i]] == 1)
			{
				ui->textOutputBox->append(QString::number(sel.size() + 1) + " (свое) " + cardItems[keys[i]].pseudoDrawable + " " + gs->getCardFullInfo(keys[i]));
				sel.append(keys[i]);
			}
			if (currEffs[currEffID].canAffectEnemy && cardMapping[keys[i]] == 2)
			{
				ui->textOutputBox->append(QString::number(sel.size() + 1) + " " + cardItems[keys[i]].pseudoDrawable + " " + gs->getCardFullInfo(keys[i]));
				sel.append(keys[i]);
			}
		}
	}
	else
	{
		sizeInt = cardMapping.size();
		QList<QString> keys = cardMapping.keys();
		for (int i = 0; i < sizeInt; i++)
		{
			if (cardMapping[keys[i]] == 1)
			{
				ui->textOutputBox->append(QString::number(sel.size() + 1) + " (свое) " + cardItems[keys[i]].pseudoDrawable + " " + gs->getCardFullInfo(keys[i]));
				sel.append(keys[i]);
			}
			else if (cardMapping[keys[i]] == 2)
			{
				ui->textOutputBox->append(QString::number(sel.size() + 1) + " " + cardItems[keys[i]].pseudoDrawable + " " + gs->getCardFullInfo(keys[i]));
				sel.append(keys[i]);
			}
		}
	}
	currSelectionSet = sel;
}

/*!
 * \brief Деструктор формы
 */
ConsoleGameForm::~ConsoleGameForm()
{
	delete ui;
}

/*!
 * \brief Инициализация интерфейса
 */
void ConsoleGameForm::initUI()
{
	QList<listItem> litems = gs->getDrawables();
	for (int i = 0; i < litems.size(); i++)
	{
		listItem item = litems[i];
		cardItems[item.uid] = item;
		cardMapping[item.uid] = 0;
	}
}

/*!
 * \brief Отображение текстового сообщения
 * \param msg Сообщение
 */
void ConsoleGameForm::showMsg(QString msg)
{
	ui->textOutputBox->append(msg);
}

/*!
 * \brief Добавление элемента карты
 * \param item Элемент информации о карте
 * \param dest Поле назначения
 */
void ConsoleGameForm::addCardEntry(listItem item, int dest)
{
	cardMapping[item.uid] = dest;
	cardItems[item.uid] = item;
}

/*!
 * \brief Обновление информации о карте
 * \param item Элемент информации о карте
 */
void ConsoleGameForm::updateCardEntry(listItem item)
{
	cardItems[item.uid] = item;
}

/*!
 * \brief Удаление информации о карте
 * \param item Элемент информации о карте
 */
void ConsoleGameForm::removeCardEntry(listItem item)
{
	cardMapping.remove(item.uid);
	cardItems.remove(item.uid);
}

/*!
 * \brief Начальная подготовка
 */
void ConsoleGameForm::makeReady()
{
	cmdID = CMD_SELECT_CMD;
	showMsg(QString("Что будем делать?\n")
			+ QString("1) Посмотреть свои карты\n")
			+ QString("2) Посмотреть свои карты на поле\n")
			+ QString("3) Посмотреть карты противника на поле\n")
			+ QString("4) Вывести карту на поле\n")
			+ QString("5) Задействовать карту\n"));
}

/*!
 * \brief Очистка всех полей с картами
 */
void ConsoleGameForm::clearAllEntries()
{
	currEffs.clear();
	currSelectionSet.clear();
	currEffID = -1;
	currPick = "";
	QList<QString> keys = cardMapping.keys();
	while (!keys.empty())
	{
		QString key = keys.takeFirst();
		cardMapping.remove(key);
		cardItems.remove(key);
	}
	ui->textOutputBox->clear();
}

/*!
 * \brief Обработчик нажатия кнопки выполнения команды
 */
void ConsoleGameForm::on_btnSubmit_clicked()
{
	QString text = ui->textInputBox->text();
	ui->textInputBox->clear();
	submitCommand(text);
}
