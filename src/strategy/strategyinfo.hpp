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

#ifndef YACARDCOUNTER_STRATEGYINFO_HPP
#define YACARDCOUNTER_STRATEGYINFO_HPP

// Qt
#include <QDialog>

class Strategy;

class QSvgRenderer;

class QLabel;

class QSpinBox;

class StrategyInfo : public QDialog {
Q_OBJECT
public:
    explicit StrategyInfo(QSvgRenderer *renderer, QWidget *parent = nullptr, Qt::WindowFlags flags = Qt::WindowFlags());

    Strategy *getStrategyById(qint32 id);

    void showStrategyByName(const QString &name);

    QVector<Strategy *> getStrategies();

private:
    QVector<Strategy *> items;
    QSvgRenderer *m_renderer;

    qint32 _id;
    QLabel *title;
    QLabel *browser;
    QVector<QSpinBox *> weights;

    void initStrategies();
};


#endif //YACARDCOUNTER_STRATEGYINFO_HPP
