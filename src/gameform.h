#ifndef GAMEFORM_H
#define GAMEFORM_H

#include <QWidget>
#include <QMap>
#include <QMessageBox>
#include <QInputDialog>
#include <QListWidget>
#include "gamesession.h"
#include "command.h"

namespace Ui {
	class GameForm;
}

/*!
 * \brief Класс GameForm - графическая форма интерфейса
 */
class GameForm : public QWidget, public GameController
{
		Q_OBJECT

	public:
		explicit GameForm(QWidget *parent = 0);
		void setContext(GameSession *game, bool initial = true);
		void addCardEntry(listItem item, int dest);
		void updateCardEntry(listItem item);
		void removeCardEntry(listItem item);
		void getCardInfo(QString cardInfo, int dest);
		void setCardSelectMode(bool enable, bool isOwned);
		void callForActionChoose(QList<effectInfoItem> effs);
		void showGameResult(int playerID);
		void showEnemyCardCount(int count);
		void showMsg(QString msg);
		void makeReady();
		void clearAllEntries();
		~GameForm();
	public slots:

		void initUI();
		void readyToStart();

	signals:
		/*!
		 * \brief Выведение карты на поле
		 * \param id ID карты
		 */
		void useCard(QString id);
		/*!
		 * \brief Задействование карты
		 * \param id ID карты
		 */
		void activateCard(QString id);
		/*!
		 * \brief Отправка выборки карт
		 * \param ids Список ID выбранных карт
		 * \param eff Список элементов информации об эффектах
		 */
		void sendCardSelection(QVector<QString> ids, effectInfoItem eff);
		/*!
		 * \brief Запрос об информации о карте
		 * \param dest Поле назначения
		 * \param id ID карты
		 */
		void cardInfoRequest(int dest, QString id);

	private slots:
		void on_btnUsedCardAct_clicked();
		void on_btnPlayerCardUse_clicked();
		void on_btnTargetCardSelect_clicked();
		void deselectAll();
		void on_listEnemyCards_itemSelectionChanged();
		void on_listPlayerUsedCards_itemSelectionChanged();
		void on_listPlayerCards_itemSelectionChanged();

	private:
		/*!
		 * \brief Форма
		 */
		Ui::GameForm *ui;
		/*!
		 * \brief Объект игрового сеанса
		 */
		GameSession *gs;
		/*!
		 * \brief Флаг выбора своих карт
		 */
		bool selectOwnedCards;
		/*!
		 * \brief Распределение карт по полям
		 */
		QMap<QString, int> cardMapping;
		/*!
		 * \brief Графические элементы карт
		 */
		QMap<QString, QListWidgetItem *> cardItems;
		/*!
		 * \brief Элементы информации об эффектах текущей карты
		 */
		QList<effectInfoItem> currEffs;
		/*!
		 * \brief Номер выбранного эффекта
		 */
		int currEffID;
};

#endif // GAMEFORM_H
