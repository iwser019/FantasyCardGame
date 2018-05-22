#include "gameform.h"
#include "ui_gameform.h"

/*!
 * \brief Конструктор игровой формы
 * \param parent Родительский объект класса QObject
 */
GameForm::GameForm(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::GameForm)
{
	ui->setupUi(this);
	selectOwnedCards = false;
}

/*!
 * \brief Установка контекста
 * \param game Объект игрового сеанса
 * \param initial Инициальное состояние
 */
void GameForm::setContext(GameSession *game, bool initial)
{
	gs = game;
	if (initial)
	{
		int numCards = QInputDialog::getInt(this, "Новая игра", "По сколько карт?", 20, 1, 100);
		gs->initialize(numCards);
		initUI();
	}
}

/*!
 * \brief Деструктор игровой формы
 */
GameForm::~GameForm()
{
	delete ui;
}

/*!
 * \brief Добавление элемента карты
 * \param item Элемент информации о карте
 * \param dest Поле назначения
 */
void GameForm::addCardEntry(listItem item, int dest)
{
	QListWidgetItem *it = new QListWidgetItem(QIcon(QPixmap::fromImage(item.drawable)), item.name);
	cardMapping[item.uid] = dest;
	cardItems[item.uid] = it;
	switch (dest)
	{
		case 0:
			{
				ui->listPlayerCards->addItem(it);
			}
		case 1:
			{
				ui->listPlayerUsedCards->addItem(it);
			}
		case 2:
			{
				ui->listEnemyCards->addItem(it);
			}
	}
}

/*!
 * \brief Обновление информации о карте
 * \param item Элемент информации о карте
 */
void GameForm::updateCardEntry(listItem item)
{
	QListWidgetItem *it = cardItems[item.uid];
	it->setIcon(QIcon(QPixmap::fromImage(item.drawable)));
}

/*!
 * \brief Удаление информации о карте
 * \param item Элемент информации о карте
 */
void GameForm::removeCardEntry(listItem item)
{
	QListWidgetItem *wi = cardItems[item.uid];
	int place = cardMapping[item.uid];
	switch (place)
	{
		case 0:
			{
				ui->listPlayerCards->removeItemWidget(wi);
				break;
			}
		case 1:
			{
				ui->listPlayerUsedCards->removeItemWidget(wi);
				break;
			}
		case 2:
			{
				ui->listEnemyCards->removeItemWidget(wi);
				break;
			}
	}
	cardMapping.remove(item.uid);
	cardItems.remove(item.uid);
	delete wi;
}

/*!
 * \brief Инициализация интерфейса
 */
void GameForm::initUI()
{
	QList<listItem> litems = gs->getDrawables();
	for (int i = 0; i < litems.size(); i++)
	{
		listItem item = litems[i];
		QListWidgetItem *it = new QListWidgetItem(QIcon(QPixmap::fromImage(item.drawable)), item.name);
		cardItems[item.uid] = it;
		cardMapping[item.uid] = 0;
		ui->listPlayerCards->addItem(it);
	}
	readyToStart();
}

/*!
 * \brief Подготовка к началу
 */
void GameForm::readyToStart()
{
	gs->nextTurn();
}

/*!
 * \brief Отображение информации о карте
 * \param cardInfo Текст информации
 * \param dest Поле назначения
 */
void GameForm::getCardInfo(QString cardInfo, int dest)
{
	switch (dest)
	{
		case 0:
			ui->lblPlayerCard->setText(cardInfo);
			break;
		case 1:
			ui->lblPlayerUsedCard->setText(cardInfo);
			break;
		case 2:
			ui->lblEnemyCard->setText(cardInfo);
			break;
	}
}

/*!
 * \brief Установка режима выбора карт
 * \param enable Включение выбора
 * \param isOwned Выбор своих карт
 */
void GameForm::setCardSelectMode(bool enable, bool isOwned)
{
	selectOwnedCards = isOwned;
	ui->btnTargetCardSelect->setEnabled(enable);
}

/*!
 * \brief Запрос на выбор эффекта
 * \param effs Список элементов информации об эффектах
 */
void GameForm::callForActionChoose(QList<effectInfoItem> effs)
{
	currEffs = effs;
	int actionID = 0;
	if (currEffs.size() > 1)
	{
		actionID = -1;
		QStringList items;
		for (int i = 0; i < currEffs.size(); i++)
			items.push_back(currEffs[i].info);
		items.push_back("(по умолчанию)");
		QString selected = QInputDialog::getItem(this, "Выбор действия", "Выберите:", items);
		for (int i = 0; i < currEffs.size(); i++)
			if (currEffs[i].info == selected)
			{
				actionID = i;
				break;
			}
	}
	currEffID = actionID;
	ui->btnTargetCardSelect->setEnabled(true);
}

/*!
 * \brief Отображение результатов игры
 * \param playerID ID победителя
 */
void GameForm::showGameResult(int playerID)
{
	// подразумевается, что:
	// игрок 1 - компьютер
	// игрок 0 - ТЫ
	QMessageBox(QMessageBox::Information,
				"Конец игры",
				playerID == 0 ?
					"Вы выстояли этот раунд, и вы победили!"
				  : "Вам больше нечем биться, а посему вы проиграли.",
				QMessageBox::Ok,
				this).exec();
	this->close();
	gs->installController(NULL);
}

/*!
 * \brief Отображение количества карт противника
 * \param count Количество карт
 */
void GameForm::showEnemyCardCount(int count)
{
	ui->lcdEnemyNumCards->display(count);
}

/*!
 * \brief Отображение текстового сообщения
 * \param msg Сообщение
 */
void GameForm::showMsg(QString msg)
{
	ui->listLog->addItem(msg);
}

/*!
 * \brief Начальная подготовка
 */
void GameForm::makeReady()
{
	ui->btnPlayerCardUse->setEnabled(true);
	ui->btnUsedCardAct->setEnabled(true);
}

/*!
 * \brief Очистка всех полей с картами
 */
void GameForm::clearAllEntries()
{
	currEffs.clear();
	currEffID = -1;
	QList<QString> keys = cardMapping.keys();
	ui->listPlayerCards->clear();
	ui->listPlayerUsedCards->clear();
	ui->listEnemyCards->clear();
	while (!keys.empty())
	{
		QString key = keys.takeFirst();
		cardMapping.remove(key);
		//delete cardItems[key];
		cardItems.remove(key);
	}
	ui->listLog->clear();
}

/*!
 * \brief Обработчик нажатия кнопки вывода карты на поле
 */
void GameForm::on_btnUsedCardAct_clicked()
{
	QListWidgetItem *item = ui->listPlayerUsedCards->selectedItems()[0];
	QString uid = cardItems.key(item);
	deselectAll();
	ui->btnUsedCardAct->setEnabled(false);
	ui->btnPlayerCardUse->setEnabled(false);
	PlayerCardActivateCommand *pcac = new PlayerCardActivateCommand();
	pcac->setContext(gs);
	pcac->setPick(uid);
	pcac->execute();
	delete pcac;
}

/*!
 * \brief Обработчик нажатия кнопки задействования карты
 */
void GameForm::on_btnPlayerCardUse_clicked()
{
	QListWidgetItem *item = ui->listPlayerCards->selectedItems()[0];
	QString uid = cardItems.key(item);
	deselectAll();
	PlayerCardUseCommand *pcuc = new PlayerCardUseCommand();
	pcuc->setContext(gs);
	pcuc->setPick(uid);
	pcuc->execute();
	delete pcuc;
}

/*!
 * \brief Обработчик нажатия кнопки выбора карты
 */
void GameForm::on_btnTargetCardSelect_clicked()
{
	QList<QListWidgetItem *> sel;
	if (currEffID != -1)
	{
		if (currEffs[currEffID].canAffectPlayer)
		{
			sel.append(ui->listPlayerUsedCards->selectedItems());
		}
		if (currEffs[currEffID].canAffectEnemy)
		{
			sel.append(ui->listEnemyCards->selectedItems());
		}
	}
	else
	{
		sel.append(ui->listPlayerUsedCards->selectedItems());
		sel.append(ui->listEnemyCards->selectedItems());
	}
	if (!sel.empty())
	{
		QVector<QString> ids;
		for (int i = 0; i < sel.size(); i++)
		{
			ids.push_back(cardItems.key(sel[i]));
		}
		gs->setCurrentAction(currEffID);
		if (currEffID != -1)
			gs->sendCardSelection(ids, currEffs[currEffID]);
		else
			gs->sendCardSelection(ids, effectInfoItem("COMBO", true, true, 0));
		ui->btnTargetCardSelect->setEnabled(false);
		ui->btnUsedCardAct->setEnabled(true);
		ui->btnPlayerCardUse->setEnabled(true);
		deselectAll();
	}
}

/*!
 * \brief Снятие выделения с карт
 */
void GameForm::deselectAll()
{
	ui->listPlayerCards->clearSelection();
	ui->listPlayerUsedCards->clearSelection();
	ui->listEnemyCards->clearSelection();
}

/*!
 * \brief Обработчик изменения выделения карт в поле карт противника
 */
void GameForm::on_listEnemyCards_itemSelectionChanged()
{
	if (!ui->listEnemyCards->selectedItems().empty())
		this->getCardInfo(gs->getCardFullInfo(cardItems.key(ui->listEnemyCards->selectedItems()[0])), 2);
	else
		ui->lblEnemyCard->setText("(нет)");
}

/*!
 * \brief Обрабботчик изменения выделения карт в поле выведенных карт
 */
void GameForm::on_listPlayerUsedCards_itemSelectionChanged()
{
	if (!ui->listPlayerUsedCards->selectedItems().empty())
		this->getCardInfo(gs->getCardFullInfo(cardItems.key(ui->listPlayerUsedCards->selectedItems()[0])), 1);
	else
		ui->lblPlayerUsedCard->setText("(нет)");
}

/*!
 * \brief Обрабботчик изменения выделения карт в поле доступных карт
 */
void GameForm::on_listPlayerCards_itemSelectionChanged()
{
	if (!ui->listPlayerCards->selectedItems().empty())
		this->getCardInfo(gs->getCardFullInfo(cardItems.key(ui->listPlayerCards->selectedItems()[0])), 0);
	else
		ui->lblPlayerCard->setText("(нет)");
}
