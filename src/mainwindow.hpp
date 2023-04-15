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

#ifndef CARD_COUNTER_MAINWINDOW_HPP
#define CARD_COUNTER_MAINWINDOW_HPP

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
    /**
     * @brief Constructs the main window.
     * @param parent The parent widget.
     */
    explicit MainWindow(QWidget *parent = nullptr);

private Q_SLOTS:

    /**
     * @brief Advances the time on the game clock.
     * @param elapsedTime The time elapsed since the last call to this function.
     */
    void advanceTime(const QString &elapsedTime);

    /**
     * @brief Loads the application settings.
     */
    void loadSettings();

    /**
     * @brief Starts a new game.
     */
    void newGame();

    /**
     * @brief Called when the game is over.
     */
    void onGameOver();

    /**
     * @brief Shows the high scores dialog.
     */
    void showHighScores();

    /**
     * @brief Shows the settings dialog.
     */
    void configureSettings();

    /**
     * @brief Pauses or unpauses the game.
     * @param paused True to pause the game, false to unpause it.
     */
    void pauseGame(bool paused);

    /**
     * @brief Called when the score is updated.
     * @param inc True if the score should be incremented, false if it should be decremented.
     */
    void onScoreUpdate(bool inc);

private:
    /**
     * @brief Sets up the actions for the main window.
     */
    void setupActions();

    Table *table;

    KGameClock *m_gameClock = nullptr;
    KToggleAction *m_actionPause = nullptr;

    QPointer<QLabel> timeLabel = new QLabel;
    QPointer<QLabel> scoreLabel = new QLabel;

    /**
     * @brief The current score.
     */
    QPair<qint32, qint32> score;
};

#endif // CARD_COUNTER_MAINWINDOW_HPP

