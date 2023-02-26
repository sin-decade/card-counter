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
#include <QPushButton>
// own
#include "carousel.hpp"

Carousel::Carousel(QSizeF aspectRatio, QWidget *parent) : QWidget(parent), ratio(aspectRatio) {

    auto *boxLayout = new QHBoxLayout(this);
    auto *carouselBox = new QWidget;
    carouselBox->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    carousel = new QHBoxLayout();
    carouselBox->setLayout(carousel);

    auto *back = new QPushButton();
    back->setIcon(QIcon::fromTheme("draw-arrow-back"));
    back->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Minimum);
    connect(back, &QPushButton::clicked, [=]() {
        idx = (idx + widgets.size() - 1) % widgets.size();
        updateLayout();
    });

    auto *next = new QPushButton();
    next->setIcon(QIcon::fromTheme("draw-arrow-forward"));
    next->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Minimum);
    connect(next, &QPushButton::clicked, [=]() {
        idx = (idx + 1) % widgets.size();
        updateLayout();
    });

    boxLayout->addWidget(back);
    boxLayout->addStretch();
    boxLayout->addWidget(carouselBox);
    boxLayout->addStretch();
    boxLayout->addWidget(next);

    updateProps(size());
}

void Carousel::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);

    updateProps(size());

}

void Carousel::addWidget(QWidget *widget) {
    connect(this, &Carousel::itemResized, widget, [widget](QSize newFixedSize) {
        widget->setFixedSize(newFixedSize);
        widget->update();
    });
    widgets.push_back(widget);
    updateProps(size());
}

void Carousel::updateProps(QSize size) {
    QSizeF itemSize = QSizeF(size.height() * ratio.width() / ratio.height(), size.height());
    columnCount = qMin(widgets.size(), qint32(0.95 * size.width() / itemSize.width()));
    updateLayout(0.9 * size.height() / ratio.height());
}

void Carousel::updateLayout(double newScale) {
    while (carousel->count()) {
        QLayoutItem *item = carousel->takeAt(0);
        item->widget()->hide();
        delete item;
    }

    if (newScale > 0) {
        QSizeF newFixedSize(ratio.width() * newScale, ratio.height() * newScale);
        emit itemResized(newFixedSize.toSize());
    }

    for (qint32 i = 0; i < columnCount; i++) {
        QWidget *item = widgets[(idx + i) % widgets.size()];
        carousel->addWidget(item);
        item->show();
    }
}

