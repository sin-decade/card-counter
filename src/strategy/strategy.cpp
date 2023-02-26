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
#include <QBoxLayout>
#include <QTextEdit>
#include <QSpinBox>
#include <utility>
// own
#include "strategy.hpp"

qint32 Strategy::updateWeight(qint32 currentWeight, qint32 rank) {
    return currentWeight + _weights[rank - 1];
}

Strategy::Strategy(QString name, QString description, QVector<qint32> weights, bool custom)
        : _custom(custom), _weights(std::move(weights)), _name(std::move(name)), _description(std::move(description)) {

}

QString Strategy::getName() {
    return _name;
}

QString Strategy::getDescription() {
    return _description;
}

bool Strategy::isCustom() const {
    return _custom;
}

qint32 Strategy::getWeights(qint32 id) {
    return _weights[id];
}
