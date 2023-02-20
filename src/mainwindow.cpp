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

// Qt
#include <QStatusBar>
// KDEGames
#include <KGameClock>
#include <KgDifficulty>
#include <KStandardGameAction>
// KF
#include <KLocalizedString>
#include <KActionCollection>
#include <KScoreDialog>
// own
#include "mainwindow.hpp"
#include "src/table/table.hpp"

MainWindow::MainWindow(QWidget *parent) : KXmlGuiWindow(parent) {
    m_gameClock = new KGameClock(this, KGameClock::FlexibleHourMinSec);
    connect(m_gameClock, &KGameClock::timeChanged, this, &MainWindow::advanceTime);

    scoreLabel->setText(i18n("Score: 0/0"));
    timeLabel->setText(i18n("Time: 00:00"));

    statusBar()->insertPermanentWidget(0, scoreLabel);
    statusBar()->insertPermanentWidget(1, timeLabel);

    table = new Table;
    connect(table, &Table::scoreUpdate, this, &MainWindow::onScoreUpdate);
    connect(table, &Table::gameOver, this, &MainWindow::onGameOver);

    setCentralWidget(table);
    setupActions();
    newGame();
}

void MainWindow::setupActions() {
    KStandardGameAction::gameNew(this, &MainWindow::newGame, actionCollection());
    KStandardGameAction::highscores(this, &MainWindow::showHighScores, actionCollection());

    KStandardGameAction::quit(this, &MainWindow::close, actionCollection());
    KStandardAction::preferences(this, &MainWindow::configureSettings, actionCollection());
    m_actionPause = KStandardGameAction::pause(this, &MainWindow::pauseGame, actionCollection());

    Kg::difficulty()->addStandardLevelRange(
            KgDifficultyLevel::Easy, KgDifficultyLevel::Hard, KgDifficultyLevel::Easy
    );
    Kg::difficulty()->addLevel(new KgDifficultyLevel(1000,
                                                     QByteArray("Nightmare"), i18n("Nightmare")
    ));
    KgDifficultyGUI::init(this);
    connect(Kg::difficulty(), &KgDifficulty::currentLevelChanged, this, &MainWindow::newGame);

    setupGUI(Default, "yacardcounterui.rc");
}

void MainWindow::newGame() {
    m_gameClock->restart();
    m_gameClock->pause();
    table->createNewGame(Kg::difficultyLevel());
    if (!m_actionPause->isChecked()) {
        m_actionPause->setChecked(true);
    }
    Kg::difficulty()->setGameRunning(false);
    timeLabel->setText(i18n("Time: 00:00"));
}

void MainWindow::onGameOver() {
    m_gameClock->pause();
    m_actionPause->setEnabled(false);
    Kg::difficulty()->setGameRunning(false);
    QPointer<KScoreDialog> scoreDialog = new KScoreDialog(KScoreDialog::Name | KScoreDialog::Time, this);
    scoreDialog->initFromDifficulty(Kg::difficulty());

    KScoreDialog::FieldInfo scoreInfo;
    scoreInfo[KScoreDialog::Score] = i18n("%1/%2", score.first, score.second);
    scoreInfo[KScoreDialog::Time] = m_gameClock->timeString();

    if (scoreDialog->addScore(scoreInfo, KScoreDialog::LessIsMore) != 0)
        scoreDialog->exec();

    delete scoreDialog;
}

void MainWindow::advanceTime(const QString &timeStr) {
    timeLabel->setText(i18n("Time: %1", timeStr));
}

void MainWindow::showHighScores() {
    QPointer<KScoreDialog> scoreDialog = new KScoreDialog(KScoreDialog::Name | KScoreDialog::Time, this);
    scoreDialog->initFromDifficulty(Kg::difficulty());
    scoreDialog->exec();
    delete scoreDialog;
}

void MainWindow::configureSettings() {

}

void MainWindow::pauseGame(bool paused) {
    table->pause(paused);
    if (paused) {
        m_gameClock->pause();
    } else {
        m_gameClock->resume();
    }
}

void MainWindow::loadSettings() {

}

void MainWindow::onScoreUpdate(bool inc) {
    score.second++;
    score.first += inc;
    scoreLabel->setText(i18n("Score: %1/%2", score.first, score.second));
}
