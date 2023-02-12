/*
 *   The GNU General Public License v3.0
 *
 *   Copyright (C) 2023 Yaroslav Riabtsev <yaroslav.riabtsev@rwth-aachen.de>
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
*/

#ifndef YACARDCOUNTER_MAINWINDOW_HPP
#define YACARDCOUNTER_MAINWINDOW_HPP

// KF
#include <KXmlGuiWindow>
// Qt
#include <QPointer>
#include <QLabel>

class KGameClock;

class KToggleAction;

class Table;

class MainWindow : public KXmlGuiWindow {
Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

private Q_SLOTS:

    void advanceTime(const QString &);

    void loadSettings();

    void newGame();

    void onGameOver();

    void showHighScores();

    void configureSettings();

    void pauseGame(bool paused);

    void onScoreUpdate(bool inc);

private:
    void setupActions();

    Table *table;

    KGameClock *m_gameClock = nullptr;
    KToggleAction *m_actionPause = nullptr;

    QPointer<QLabel> timeLabel = new QLabel;
    QPointer<QLabel> scoreLabel = new QLabel;
    QPair<qint32, qint32> score;
};


#endif //YACARDCOUNTER_MAINWINDOW_HPP
