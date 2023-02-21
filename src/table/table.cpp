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
#include <QVBoxLayout>
#include <QSvgRenderer>
#include <QStandardPaths>
#include <QtMath>
#include <QTimer>
#include <QRandomGenerator>
// own
#include "table.hpp"
#include "tableslot.hpp"
#include "src/strategy/strategyinfo.hpp"

Table::Table(QWidget *parent) : QWidget(parent) {
    countdown = new QTimer(this);
    connect(countdown, &QTimer::timeout, this, &Table::pickUpCards);

    setRenderer("tigullio-international");
    strategyInfo = new StrategyInfo(renderer);

    layout = new QGridLayout();
    setLayout(layout);
}

void Table::onTableSlotActivated() {
    auto *tableSlot = qobject_cast<TableSlot *>(sender());
    available.insert(layout->indexOf(tableSlot));
    addNewTableSlot();
    calculateNewColumnCount(size(), bounds.size(), items.count());
    emit canRemove(tableSlotCountLimit < available.size());
}

void Table::addNewTableSlot(bool isActive) {
    auto *tableSlot = new TableSlot(strategyInfo->getStrategies(), renderer, isActive, this);
    if (isActive) {
        available.insert(items.size());
    }
    connect(tableSlot, &TableSlot::tableSlotActivated, this, &Table::onTableSlotActivated);
    connect(tableSlot, &TableSlot::tableSlotFinished, this, &Table::onTableSlotFinished);
    connect(tableSlot, &TableSlot::tableSlotRemoved, this, &Table::onTableSlotRemoved);
    connect(tableSlot, &TableSlot::tableSlotReshuffled, this, &Table::onTableSlotReshuffled);
    connect(tableSlot, &TableSlot::userQuizzed, this, &Table::onUserQuizzed);
    connect(tableSlot, &TableSlot::userAnswered, this, &Table::onUserAnswered);
    connect(tableSlot, &TableSlot::swapTargetSelected, this, &Table::onSwapTargetSelected);
    connect(tableSlot, &TableSlot::strategyInfoAssist, this, &Table::onStrategyInfoAssist);
    connect(this, &Table::gamePaused, tableSlot, &TableSlot::onGamePaused);
    connect(this, &Table::tableSlotResized, tableSlot,
            [tableSlot](QSize newFixedSize) { tableSlot->setFixedSize(newFixedSize); });
    connect(this, &Table::canRemove, tableSlot, &TableSlot::onCanRemove);
    items.push_back(tableSlot);
}

void Table::onTableSlotFinished() {
    auto *tableSlot = qobject_cast<TableSlot *>(sender());
    available.remove(layout->indexOf(tableSlot));
//    qDebug() << available;
}

void Table::onTableSlotRemoved() {
    auto *tableSlot = qobject_cast<TableSlot *>(sender());
    items.remove(layout->indexOf(tableSlot));
    available.remove(layout->indexOf(tableSlot));
    jokers.remove(layout->indexOf(tableSlot));
    calculateNewColumnCount(size(), bounds.size(), items.count());
    emit canRemove(available.size() > tableSlotCountLimit);
}

void Table::onTableSlotReshuffled() {
    auto *tableSlot = qobject_cast<TableSlot *>(sender());
    available.insert(layout->indexOf(tableSlot));
}

void Table::onUserQuizzed() {
    countdown->stop();
    auto *tableSlot = qobject_cast<TableSlot *>(sender());
    jokers.insert(layout->indexOf(tableSlot));
    available.remove(layout->indexOf(tableSlot));
}

void Table::onUserAnswered(bool correct) {
    auto *tableSlot = qobject_cast<TableSlot *>(sender());
    jokers.remove(layout->indexOf(tableSlot));
    available.insert(layout->indexOf(tableSlot));
    if (jokers.empty()) {
        countdown->stop();
        countdown->start(300);
    }
    emit scoreUpdate(correct);
}

void Table::calculateNewColumnCount(const QSizeF &tableSize, const QSizeF &aspectRatio, int itemCount) {
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

void Table::reorganizeTable(qint32 newColumnCount, double newScale) {
    // Remove all items from the layout
    while (layout->count()) {
        QLayoutItem *item = layout->takeAt(0);
        item->widget()->hide();
        delete item;
    }

    // Calculate the new size for each item
    QSizeF newFixedSize(bounds.width() * newScale, bounds.height() * newScale);
    emit tableSlotResized(newFixedSize.toSize());

    // Add items back to the layout with the new size and new number of columns
    qint32 itemsCount = items.count();
    for (qint32 i = 0; i < itemsCount; i++) {
        TableSlot *item = items[i];
        layout->addWidget(item, i / newColumnCount, i % newColumnCount);
        item->show();
    }
    columnCount = newColumnCount;
    scale = newScale;
}

void Table::onSwapTargetSelected() {
    swapTarget.push_back(layout->indexOf(qobject_cast<TableSlot *>(sender())));
    if (swapTarget.size() == 2) {
        items.swapItemsAt(swapTarget[0], swapTarget[1]);
        swapTarget.clear();
    }
    reorganizeTable(columnCount, scale);
}

void Table::pickUpCards() {
//    qDebug() << available;
    if (available.empty()) {
        countdown->stop();
        emit gameOver();
        return;
    }
    QSet<qint32> picked;
    while (!available.empty() &&
           (picked.size() < tableSlotCountLimit || Kg::difficultyLevel() == KgDifficultyLevel::Custom)) {
        auto it = available.begin();
        qint32 idx = QRandomGenerator::global()->bounded(available.size());
        std::advance(it, idx);
        if (!picked.contains(*it)) {
            picked.insert(*it);
            items[*it]->pickUpCard();
            available.remove(*it);
        }
    }
    available.unite(picked);
    // emit deHighlighting
    picked.clear();
}

void Table::setRenderer(const QString &cardTheme) {
    renderer = new QSvgRenderer(
            QStandardPaths::locate(QStandardPaths::GenericDataLocation,
                                   QString("carddecks/svg-%1/%1.svgz").arg(cardTheme)));
    bounds = renderer->boundsOnElement("back");
}

void Table::createNewGame(KgDifficultyLevel::StandardLevel level) {
    countdown->stop();
    launching = true;
    while (!items.empty()) {
        TableSlot *last = items.last();
        last->hide();
        items.pop_back();
        delete last;
    }
    available.clear();
    jokers.clear();
    switch (level) {
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
            break;
    }
    while (items.count() < tableSlotCountLimit) {
        addNewTableSlot(true);
    }
    addNewTableSlot();
    calculateNewColumnCount(size(), bounds.size(), items.count());
}

void Table::pause(bool paused) {
    if (launching) {
        launching = false;
        TableSlot *last = items.last();
        if (last->isFake()) {
            items.pop_back();
            delete last;
            calculateNewColumnCount(size(), bounds.size(), layout->count());
        }
    }
    emit gamePaused(paused);
    if (paused) {
        countdown->stop();
    } else if (jokers.empty()) {
        countdown->stop();
        countdown->start(300);
    }
}

void Table::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);

    calculateNewColumnCount(size(), bounds.size(), items.count());
}

void Table::onStrategyInfoAssist() {
    strategyInfo->show();
}
