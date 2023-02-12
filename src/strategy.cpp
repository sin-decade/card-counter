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

// own
#include "strategy.hpp"
#include "carddeck.hpp"

qint32 Strategy::updateWeight(qint32 currentWeight, qint32 cardID, qint32 strategyID) {
    const QVector<QVector<qint32>> weights = {{-1, 1, 1, 1, 1, 1, 0, 0, 0, -1, -1, -1, -1}, // Hi-Lo Count
                                              {0,  0, 1, 1, 1, 1, 0, 0, 0, -1, -1, -1, -1}, // Hi-Opt I Count
                                              {0,  1, 1, 2, 2, 1, 1, 0, 0, -2, -2, -2, -2}, // Hi-Opt II Count
                                              {-1, 1, 1, 1, 1, 1, 1, 0, 0, -1, -1, -1, -1}, // KO Count
    };
    return currentWeight + weights[strategyID][CardDeck::getRank(cardID) - 1];
}
