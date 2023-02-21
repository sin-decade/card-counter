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
#include <QSpinBox>
#include <QFormLayout>
// own
#include "strategy.hpp"
#include "src/widgets/carousel.hpp"
#include "src/widgets/cards.hpp"

qint32 Strategy::updateWeight(qint32 currentWeight, qint32 rank) {
    return currentWeight + _weights[rank - 1]->value();
}

Strategy::Strategy(QSvgRenderer *renderer, bool isNew, QWidget *parent) : QWidget(parent) {
    auto *strategy = new QVBoxLayout(this);
    browser = new QLabel();
    browser->setWordWrap(true);
    browser->setTextFormat(Qt::TextFormat::MarkdownText);
    browser->setOpenExternalLinks(true);

    auto *carousel = new Carousel(QSizeF(3, 4));
    for (int i = Cards::Rank::Ace; i <= Cards::Rank::King; i++) {
        auto *card = new Cards(renderer);
        card->setId(i);
        auto *form = new QFormLayout(card);
        _weights.push_back(new QSpinBox());
        _weights.last()->setRange(-5, 5);
        _weights.last()->setReadOnly(!isNew);
        _weights.last()->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
        form->setFormAlignment(Qt::AlignCenter);
        form->addRow(_weights.last());
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
    Q_ASSERT(weights.size() <= _weights.size());

    for (int i = 0; i < weights.size(); i++) {
        _weights[i]->setValue(weights[i]);
    }
}
