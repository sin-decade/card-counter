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

// Qt
#include <QWidget>

class QTextEdit;

class QLabel;

class QSvgRenderer;

class Strategy : public QWidget {
Q_OBJECT
public:
    explicit Strategy(QSvgRenderer *renderer, bool isNew = false, QWidget *parent = nullptr);

    void setName(QString name);

    QString getName();

    void setDescription(QString description);

    static qint32 updateWeight(qint32 currentWeight, qint32 rank, qint32 strategyID);

    void setWeights(QVector<qint32> weights);

private:
    QVector<qint32> _weights;
    QString _name;
    QString _description;

    QLabel *browser;
};


#endif //YACARDCOUNTER_STRATEGY_HPP
