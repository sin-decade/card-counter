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

#ifndef YACARDCOUNTER_STRATEGY_HPP
#define YACARDCOUNTER_STRATEGY_HPP

class Strategy {
public:
    explicit Strategy(QString name, QString description, QVector<qint32> weights, bool custom = false);

    bool isCustom() const;

    QString getName();

    QString getDescription();

    qint32 getWeights(qint32 id);

    qint32 updateWeight(qint32 currentWeight, qint32 rank);

private:
    bool _custom;
    QVector<qint32> _weights;
    QString _name, _description;
};


#endif //YACARDCOUNTER_STRATEGY_HPP
