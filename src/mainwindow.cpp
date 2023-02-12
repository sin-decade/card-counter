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
#include <QApplication>
#include <QStatusBar>
#include <QGraphicsView>
#include <QQuickWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QSvgRenderer>
#include <QStandardPaths>
#include <QLineEdit>
#include <QSpinBox>
#include <QGraphicsSvgItem>
#include <QtMath>
#include <QTimer>
#include <QRandomGenerator>
#include <QStack>
// KDEGames
#include <KGameClock>
#include <KgDifficulty>
#include <KStandardGameAction>
// KF
#include <KLocalizedString>
#include <KActionCollection>
#include <KStandardAction>
#include <KMessageBox>
// own
#include "mainwindow.hpp"
#include "tableslot.hpp"

MainWindow::MainWindow(QWidget *parent) : KXmlGuiWindow(parent) {
    m_gameClock = new KGameClock(this, KGameClock::FlexibleHourMinSec);
    connect(m_gameClock, &KGameClock::timeChanged, this, &MainWindow::advanceTime);

    scoreLabel->setText(i18n("Score: 0/0"));
    timeLabel->setText(i18n("Time: 00:00"));

    statusBar()->insertPermanentWidget(0, scoreLabel);
    statusBar()->insertPermanentWidget(1, timeLabel);

    countdown = new QTimer(this);
    connect(countdown, &QTimer::timeout, this, &MainWindow::pickUpCards);

    setRenderer("tigullio-international");

    table = new QWidget;

    layout = new QGridLayout();


    table->setLayout(layout);
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
    launching = true;

    m_gameClock->restart();
    m_gameClock->pause();
    while (!items.empty()) {
        TableSlot *last = items.last();
        last->hide();
        items.pop_back();
        delete last;
    }
    available.clear();
    jokers.clear();

    if (!m_actionPause->isChecked()) {
        m_actionPause->setChecked(true);
    }

    Kg::difficulty()->setGameRunning(false);
    switch (Kg::difficultyLevel()) {
        case KgDifficultyLevel::Easy:
            // tableSlotsCount: 1+
            // cardPickUpsAtTime: 1
            tableSlotCountLimit = 1;
            break;
        case KgDifficultyLevel::Medium:
            // tableSlotsCount: 2+
            // cardPickUpsAtTime: 2
            tableSlotCountLimit = 2;
            break;
        case KgDifficultyLevel::Hard:
            // tableSlotsCount: 4+
            // cardPickUpsAtTime: 4
            tableSlotCountLimit = 4;
            break;
        case KgDifficultyLevel::Custom: // Nightmare
            // tableSlotsCount: 6+
            // cardPickUpsAtTime: all
            tableSlotCountLimit = 6;
            break;
        default:
            qDebug() << Kg::difficultyLevel();
            break;
    }
    while (items.count() < tableSlotCountLimit) {
        addNewTableSlot(true);
    }

    timeLabel->setText(i18n("Time: 00:00"));
    addNewTableSlot();
    calculateNewColumnCount(table->size(), bounds.size(), items.count());
}

void MainWindow::onGameOver(bool won) {

}

void MainWindow::advanceTime(const QString &timeStr) {
    timeLabel->setText(i18n("Time: %1", timeStr));
}

void MainWindow::showHighScores() {

}

void MainWindow::configureSettings() {

}

void MainWindow::pauseGame(bool paused) {
    if (launching) {
        launching = false;
        TableSlot *last = items.last();
        if (last->isFake()) {
            items.pop_back();
            delete last;
            calculateNewColumnCount(table->size(), bounds.size(), layout->count());
        }
        m_gameClock->resume();
    }
    emit gamePaused(paused);
    if (paused) {
        countdown->stop();
    } else if (jokers.empty()) {
        countdown->start(300);
    }
}

void MainWindow::loadSettings() {

}

void MainWindow::onTableSlotActivated() {
    TableSlot *tableSlot = qobject_cast<TableSlot *>(sender());
    available.insert(layout->indexOf(tableSlot));
    addNewTableSlot();
    calculateNewColumnCount(table->size(), bounds.size(), items.count());
    emit canRemove(available.size() > tableSlotCountLimit);
}

void MainWindow::addNewTableSlot(bool isActive) {
//    qDebug() << items.count() << isActive;
    TableSlot *tableSlot = new TableSlot(renderer, isActive, this);
    if (isActive) {
        available.insert(items.size());
    }
    connect(tableSlot, &TableSlot::tableSlotActivated, this, &MainWindow::onTableSlotActivated);
    connect(tableSlot, &TableSlot::tableSlotFinished, this, &MainWindow::onTableSlotFinished);
    connect(tableSlot, &TableSlot::tableSlotRemoved, this, &MainWindow::onTableSlotRemoved);
    connect(tableSlot, &TableSlot::tableSlotReshuffled, this, &MainWindow::onTableSlotReshuffled);
    connect(tableSlot, &TableSlot::userQuizzed, this, &MainWindow::onUserQuizzed);
    connect(tableSlot, &TableSlot::userAnswered, this, &MainWindow::onUserAnswered);
    connect(tableSlot, &TableSlot::swapTargetSelected, this, &MainWindow::onSwapTargetSelected);
    connect(this, &MainWindow::gamePaused, tableSlot, &TableSlot::onGamePaused);
    connect(this, &MainWindow::tableSlotResized, tableSlot, &TableSlot::onTableSlotResized);
    connect(this, &MainWindow::canRemove, tableSlot, &TableSlot::onCanRemove);
    items.push_back(tableSlot);
}

void MainWindow::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);

    calculateNewColumnCount(centralWidget()->size(), bounds.size(), items.count());
}

void MainWindow::onTableSlotFinished() {
    TableSlot *tableSlot = qobject_cast<TableSlot *>(sender());
    available.remove(layout->indexOf(tableSlot));
}

void MainWindow::onTableSlotRemoved() {
    TableSlot *tableSlot = qobject_cast<TableSlot *>(sender());
    items.remove(layout->indexOf(tableSlot));
    available.remove(layout->indexOf(tableSlot));
    jokers.remove(layout->indexOf(tableSlot));
    calculateNewColumnCount(table->size(), bounds.size(), items.count());
    emit canRemove(available.size() > tableSlotCountLimit);
}

void MainWindow::onTableSlotReshuffled() {
    TableSlot *tableSlot = qobject_cast<TableSlot *>(sender());
    available.insert(layout->indexOf(tableSlot));
}

void MainWindow::onUserQuizzed() {
    countdown->stop();
    TableSlot *tableSlot = qobject_cast<TableSlot *>(sender());
    jokers.insert(layout->indexOf(tableSlot));
    available.remove(layout->indexOf(tableSlot));
}

void MainWindow::onUserAnswered(bool correct) {
    TableSlot *tableSlot = qobject_cast<TableSlot *>(sender());
    jokers.remove(layout->indexOf(tableSlot));
    available.insert(layout->indexOf(tableSlot));
    score.second++;
    score.first += correct;
    if (jokers.empty()) {
        countdown->start(300);
    }
    scoreLabel->setText(i18n("Score: %1/%2", score.first, score.second));
}

void MainWindow::calculateNewColumnCount(const QSizeF &tableSize, const QSizeF &aspectRatio, int itemCount) {
    int newColumnCount = 1;
    double newScale = 0;
    for (int testColumnCount = 1; testColumnCount <= itemCount; testColumnCount++) {
        double testScale = 0.9 * qMin(tableSize.width() / (testColumnCount * aspectRatio.width()),
                                      tableSize.height() /
                                      (qCeil(itemCount * 1.0 / testColumnCount) * aspectRatio.height()));
        if (testScale > newScale) {
            newScale = testScale;
            newColumnCount = testColumnCount;
        }
    }
    reorganizeTable(newColumnCount, newScale);
}

void MainWindow::reorganizeTable(int newColumnCount, double newScale) {
    // Remove all items from the layout
    while (layout->count()) {
        QLayoutItem *item = layout->takeAt(0);
        item->widget()->hide();
        delete item;
    }

    // Calculate the new size for each item
    QSize newFixedSize(bounds.width() * newScale, bounds.height() * newScale);
    emit tableSlotResized(newFixedSize);

    // Add items back to the layout with the new size and new number of columns
    int itemsCount = items.count();
    for (int i = 0; i < itemsCount; i++) {
        TableSlot *item = items[i];
        layout->addWidget(item, i / newColumnCount, i % newColumnCount);
        item->show();
    }
    columnCount = newColumnCount;
    scale = newScale;
}

void MainWindow::onSwapTargetSelected() {
    swapTarget.push_back(layout->indexOf(qobject_cast<TableSlot *>(sender())));
    if (swapTarget.size() == 2) {
        items.swapItemsAt(swapTarget[0], swapTarget[1]);
        swapTarget.clear();
    }
    reorganizeTable(columnCount, scale);
}

void MainWindow::pickUpCards() {
    if (Kg::difficultyLevel() == KgDifficultyLevel::Custom || available.size() <= tableSlotCountLimit) {
        for (auto idx: available) {
            items[idx]->pickUpCard();
        }
    } else {
        QStack<quint32> picked;
        while (picked.size() < tableSlotCountLimit) {
            auto it = available.begin();
            picked.push(QRandomGenerator::global()->bounded(available.size()));
            std::advance(it, picked.top());
            items[*it]->pickUpCard();
            available.remove(picked.top());
        }
        while (!picked.empty()) {
            available.insert(picked.pop());
        }
    }
}

void MainWindow::setRenderer(QString cardTheme) {
    renderer = new QSvgRenderer(
            QStandardPaths::locate(QStandardPaths::GenericDataLocation,
                                   QString("carddecks/svg-%1/%1.svgz").arg(cardTheme)));
    bounds = renderer->boundsOnElement("back");
}
