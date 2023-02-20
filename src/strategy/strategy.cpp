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
#include <QLabel>
#include <QDebug>
// own
#include "strategy.hpp"
#include "src/widgets/carousel.hpp"
#include "src/widgets/cards.hpp"

qint32 Strategy::updateWeight(qint32 currentWeight, qint32 rank, qint32 strategyID) {
    const QVector<QVector<qint32>> weights = {{-1, 1, 1, 1, 1, 1, 0, 0, 0, -1, -1, -1, -1}, // Hi-Lo Count
                                              {0,  0, 1, 1, 1, 1, 0, 0, 0, -1, -1, -1, -1}, // Hi-Opt I Count
                                              {0,  1, 1, 2, 2, 1, 1, 0, 0, -2, -2, -2, -2}, // Hi-Opt II Count
                                              {-1, 1, 1, 1, 1, 1, 1, 0, 0, -1, -1, -1, -1}, // KO Count
    };
    return currentWeight + weights[strategyID][rank - 1];
}

Strategy::Strategy(QSvgRenderer *renderer, bool isNew, QWidget *parent) : QWidget(parent) {
    auto *strategy = new QVBoxLayout(this);
    _weights = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    browser = new QLabel();
    browser->setWordWrap(true);
    browser->setTextFormat(Qt::TextFormat::MarkdownText);
    auto *carousel = new Carousel<QLabel>();
    for (int i = 1; i < 4; i++) {
//        auto *card = new Cards(renderer);
//        card->setId(i);
//        qDebug() << card->size();
        auto * card = new QLabel(QString(i));
        card->setMovie()
        carousel->addWidget(card);
    }
    if (isNew) {
//        auto *form = new QFormLayout();
//        auto *nameInput = new QLineEdit();
//        auto *descriptionArea = new QTextEdit();
//        form->setFormAlignment(Qt::AlignCenter);
//        form->addRow(tr("&Name:"), nameInput);
//        form->addRow(tr("&Description:"), descriptionArea);
    } else {
        strategy->addWidget(browser);
    }
    strategy->addWidget(carousel);
}

void Strategy::setName(QString name) {
    _name = std::move(name);
}

void Strategy::setDescription(QString description) {
    _description = std::move(description);
    browser->setText(_description);
}

QString Strategy::getName() {
    return _name;
}

void Strategy::setWeights(QVector<qint32> weights) {
    _weights = std::move(weights);
//    carousel->setWeights(_weights);
}
