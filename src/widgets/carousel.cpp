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
#include <QDebug>
#include <QLabel>
// own
#include "carousel.hpp"

template<typename T>
Carousel<T>::Carousel(QWidget *parent) : QWidget(parent) {
    auto *boxLayout = new QHBoxLayout(this);
    auto *carouselBox = new QWidget;
    carouselBox->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    qDebug() << carouselBox->size();
    carousel = new QHBoxLayout();
    carousel->addWidget(new QLabel("jhkjsf"));
    carouselBox->setLayout(carousel);

    auto *back = new QPushButton();
    back->setIcon(QIcon::fromTheme("draw-arrow-back"));
    back->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Minimum);
//    connect(back, &QPushButton::clicked, this, &TableSlot::userChecking);

    auto *next = new QPushButton();
    next->setIcon(QIcon::fromTheme("draw-arrow-forward"));
    next->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Minimum);
//    connect(back, &QPushButton::clicked, this, &TableSlot::userChecking);

    boxLayout->addWidget(back);
    boxLayout->addStretch();
    boxLayout->addWidget(carouselBox);
    boxLayout->addStretch();
    boxLayout->addWidget(next);
}

template<typename T>
void Carousel<T>::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);
}

template<typename T>
void Carousel<T>::addWidget(T *widget) {
    carousel->addWidget(widget);
    qDebug() << std::type_info(widget) << widget->size() << carousel->count();
    widget->show();
    carousel->update();
}
