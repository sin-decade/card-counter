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
// own
#include "strategyinfo.hpp"
#include "strategy.hpp"

StrategyInfo::StrategyInfo(QSvgRenderer *renderer, QWidget *parent, Qt::WindowFlags flags)
        : KPageDialog(parent, flags), m_renderer(renderer) {
    setWindowTitle("Strategy Info");
    setModal(true);

    const QVector<QVector<qint32>> weights = {{-1, 1, 1, 1, 1, 1, 0, 0, 0, -1, -1, -1, -1}, // Hi-Lo Count
                                              {0,  0, 1, 1, 1, 1, 0, 0, 0, -1, -1, -1, -1}, // Hi-Opt I Count
                                              {0,  1, 1, 2, 2, 1, 1, 0, 0, -2, -2, -2, -2}, // Hi-Opt II Count
                                              {-1, 1, 1, 1, 1, 1, 1, 0, 0, -1, -1, -1, -1}, // KO Count
    };
    const QVector<QString> names = {"Hi-Lo Count", "Hi-Opt I Count", "Hi-Opt II Count", "KO Count"};

    for (int i = 0; i < weights.size(); i++) {
        auto *strategy = new Strategy(m_renderer);
        strategy->setName(names[i]);
        strategy->setDescription(
                "The **Hi-Opt II** (Hi Opt 2) blackjack card counting system, the more advanced of the high optimum card counting systems, is a card counting strategy that is designed for use by much more advanced blackjack players . This system is actually far more complicated than the Hi-Opt I strategy and should only be used by seasoned professionals.\n\nThe actual extra player advantage that you get from using the Hi-Opt II card counting system, as opposed to simpler card counting systems, is very small. We don't recommend trying to learn the Hi-Opt II system until you have completely mastered the Hi-Opt I system and truly feel that you want the small extra edge that this system provides.");
        strategy->setWeights(weights[i]);
        addPage(strategy, names[i]);
        items.push_back(strategy);
    }
}

QVector<QString> StrategyInfo::getStrategiesList() {
    QVector<QString> list;
    for (auto *item: items) {
        list.push_back(item->getName());
    }
    return list;
}

Strategy *StrategyInfo::getStrategyById(qint32 id) {
    return items[id];
}

void StrategyInfo::showStrategyById(qint32 id) {
    setCurrentPage(reinterpret_cast<KPageWidgetItem *>(getStrategyById(id)));
}


