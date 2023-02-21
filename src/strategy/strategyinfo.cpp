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

    items.push_back(new Strategy(m_renderer));
    items.last()->setName("Hi-Lo Count");
    items.last()->setDescription(
            "The Hi-Lo blackjack card counting system was first introduced by Harvey Dubner in 1963. Dubner's goal was "
            "to create a simple yet effective system that could be used by anyone to increase their odds of winning "
            "at blackjack.");
    items.last()->setWeights({-1, 1, 1, 1, 1, 1, 0, 0, 0, -1, -1, -1, -1});

    items.push_back(new Strategy(m_renderer));
    items.last()->setName("Hi-Opt I Count");
    items.last()->setDescription(
            "The Hi-Opt I blackjack card counting system was developed by Charles Einstein and introduced in his book "
            "\"The World's Greatest Blackjack Book\" in 1980. The Hi-Opt I system assigns point values to each card in "
            "the deck and is a more complex system than the Hi-Lo system, with additional point values for some cards. "
            "It is considered a more powerful system than the Hi-Lo, but also more difficult to learn "
            "and use effectively.");
    items.last()->setWeights({0, 0, 1, 1, 1, 1, 0, 0, 0, -1, -1, -1, -1});

    items.push_back(new Strategy(m_renderer));
    items.last()->setName("Hi-Opt II Count");
    items.last()->setDescription(
            "The Hi-Opt II blackjack card counting system is a more advanced version of the Hi-Opt I system, "
            "developed by Lance Humble and Carl Cooper in their book \"The World's Greatest Blackjack Book\" in 1980. "
            "The Hi-Opt II system assigns point values to each card in the deck, with additional point values "
            "for some cards, and is considered one of the most powerful card counting systems. It is also one of "
            "the most difficult to learn and use effectively.");
    items.last()->setWeights({0, 1, 1, 2, 2, 1, 1, 0, 0, -2, -2, -2, -2});

    items.push_back(new Strategy(m_renderer));
    items.last()->setName("KO Count");
    items.last()->setDescription(
            "The Knock-Out (KO) blackjack card counting system was developed by Olaf Vancura and Ken Fuchs in their "
            "book \"Knock-Out Blackjack\" in 1998. The KO system assigns point values to each card in the deck, with "
            "the additional advantage that it does not require a true count conversion for betting, making it easier "
            "to use than some other systems.");
    items.last()->setWeights({-1, 1, 1, 1, 1, 1, 1, 0, 0, -1, -1, -1, -1});

    items.push_back(new Strategy(m_renderer));
    items.last()->setName("Omega II Count");
    items.last()->setDescription(
            "The Omega II blackjack card counting system was developed by Bryce Carlson and introduced in his book "
            "\"Blackjack for Blood\" in 2001. The Omega II system assigns point values to each card in the deck, with "
            "additional point values for some cards, and is considered one of the most powerful card counting systems, "
            "especially for multi-deck games.");
    items.last()->setWeights({0, 1, 1, 2, 2, 2, 1, 0, -1, -2, -2, -2, -2});

    items.push_back(new Strategy(m_renderer));
    items.last()->setName("Zen Count");
    items.last()->setDescription(
            "The Zen Count blackjack card counting system was developed by Arnold Snyder and introduced in his book "
            "\"Blackbelt in Blackjack\" in 1983. The Zen Count system assigns point values to each card in the deck, "
            "with additional point values for some cards, and is considered a powerful system for both single "
            "and multi-deck games.");
    items.last()->setWeights({-1, 1, 1, 2, 2, 2, 1, 0, 0, -2, -2, -2, -2});

    items.push_back(new Strategy(m_renderer));
    items.last()->setName("10 Count");
    items.last()->setDescription(
            "The 10 Count blackjack card counting system was developed by Edward O. Thorp, a mathematician and author "
            "of the classic book \"Beat the Dealer\" in 1962. The 10 Count system assigns point values to each card in "
            "the deck, with a focus on the 10-value cards, and is considered one of the earliest "
            "and most basic card counting systems.");
    items.last()->setWeights({1, 1, 1, 1, 1, 1, 1, 1, 1, -2, -2, -2, -2});

    for (auto *item: items) {
        addPage(item, item->getName());
    }
}

Strategy *StrategyInfo::getStrategyById(qint32 id) {
    return items[id];
}

void StrategyInfo::showStrategyById(qint32 id) {
    setCurrentPage(reinterpret_cast<KPageWidgetItem *>(getStrategyById(id)));
}

QVector<Strategy *> StrategyInfo::getStrategies() {
    return items;
}


