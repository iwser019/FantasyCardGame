#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTime>
#include "gameform.h"
#include "consolegameform.h"
#include "gamecontroller.h"
#include "gamesession.h"

namespace Ui {
	class MainWindow;
}

/*!
 * \brief Класс MainWindow - главная форма
 */
class MainWindow : public QMainWindow
{
		Q_OBJECT

	public:
		explicit MainWindow(QWidget *parent = 0);
		~MainWindow();

	private slots:
		void on_actionNewGame_triggered();
		void on_actionNewGameConsole_triggered();
		void on_actionExit_triggered();

		void on_actionLoad_triggered();

		void on_actionSave_triggered();

	private:
		/*!
		 * \brief ui
		 * Интерфейс
		 */
		Ui::MainWindow *ui;
		/*!
		 * \brief gf
		 * Игроваая форма
		 */
		QWidget *gf;
		/*!
		 * \brief gs
		 * Игроовой сеанс
		 */
		GameSession *gs;
};

#endif // MAINWINDOW_H
