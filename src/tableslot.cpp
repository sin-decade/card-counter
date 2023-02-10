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
#include <QSvgRenderer>
#include <QFrame>
#include <QFormLayout>
#include <QSpinBox>
#include <QPainter>
#include <QPushButton>
#include <QToolButton>
#include <QDebug>
#include <QComboBox>
#include <QCheckBox>
// KF
#include <KLocalizedString>
// own
#include "tableslot.hpp"
#include "carddeck.hpp"

void TableSlot::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);

    if (m_renderer->isValid() && m_renderer->elementExists(svgName)) {
        QPainter painter(this);
        m_renderer->render(&painter, svgName);
    }
}

TableSlot::TableSlot(QSvgRenderer *renderer, QWidget *parent)
        : QWidget(parent), m_renderer(renderer) {
    fake = true;
    svgName = "back";

    label->setText(i18n("TableSlot Weight: 0"));
//    label->setFrameStyle(QFrame::HLine);
    label->setAutoFillBackground(true);
    label->setPalette(QPalette(Qt::green));
    label->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Maximum);
    label->setAlignment(Qt::AlignCenter);


    answerFrame = new QFrame();
//    answerFrame->setFrameStyle(QFrame::HLine);
    answerFrame->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Maximum);
    answerFrame->setAutoFillBackground(true);
    answerFrame->setPalette(QPalette(Qt::gray));
    QFormLayout *answer = new QFormLayout(answerFrame);
    weightBox = new QSpinBox();
    weightBox->setRange(-100, 100);
    answer->addRow(tr("&Weight:"), weightBox);
    answer->setFormAlignment(Qt::AlignCenter);
    QPushButton *submit = new QPushButton(i18n("&Submit"));
    submit->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    connect(submit, &QPushButton::clicked, this, &TableSlot::userChecking);
    answer->addRow(submit);


    settingsFrame = new QFrame();
    settingsFrame->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Maximum);
    settingsFrame->setAutoFillBackground(true);
    settingsFrame->setPalette(QPalette(Qt::gray));
    QFormLayout *settings = new QFormLayout(settingsFrame);
    deckCount = new QSpinBox();
    deckCount->setRange(0, 10);
    deckCount->setValue(0);
    connect(deckCount, QOverload<int>::of(&QSpinBox::valueChanged), [=](int value) {
        // Check if the previous value was 0 and the new value is greater than 0
        if (value > 0 && fake) {
            deckCount->setMinimum(1);
            fake = false;
            controlFrame->show();
            svgName = "green_back";
            emit tableSlotActivated();
        }
    });
    settings->addRow(tr("&Number of Card Decks:"), deckCount);
    settings->setFormAlignment(Qt::AlignCenter);
    QComboBox *strategy = new QComboBox();
    QStringList items = {"Hi-Lo", "Hi-Opt I", "Hi-Opt II", "Zen Count"};
    strategy->addItems(items);
    settings->addRow(tr("&Type of Strategy:"), strategy);
    QCheckBox *indexing = new QCheckBox();
    settings->addRow(indexing, new QLabel("Use card indexing"));
    QCheckBox *training = new QCheckBox();
    settings->addRow(training, new QLabel("Is training"));


    controlFrame = new QFrame();
//    controlFrame->setFrameStyle(QFrame::HLine);
    controlFrame->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Maximum);
    controlFrame->setAutoFillBackground(true);
    controlFrame->setPalette(QPalette(Qt::gray));
    QHBoxLayout *controlLayout = new QHBoxLayout(controlFrame);
    QPushButton *closeButton = new QPushButton();
    closeButton->setIcon(QIcon::fromTheme("delete"));
    closeButton->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    connect(closeButton, &QPushButton::clicked, this, &TableSlot::tableSlotRemoved);
    //    connect(infoButton, &QPushButton::clicked, this, &TableSlot::showInfo);
    QPushButton *refreshButton = new QPushButton();
    refreshButton->setIcon(QIcon::fromTheme("view-refresh"));
    refreshButton->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    QPushButton *swapButton = new QPushButton();
    swapButton->setIcon(QIcon::fromTheme("exchange-positions"));
    swapButton->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    connect(swapButton, &QPushButton::clicked, this, &TableSlot::swapTargetSelected);


    controlLayout->addWidget(closeButton);
    controlLayout->addWidget(refreshButton);
    refreshButton->hide();
    controlLayout->addWidget(swapButton);


    boxLayout = new QVBoxLayout(this);
    boxLayout->addWidget(label);
    boxLayout->addWidget(answerFrame);
    boxLayout->addWidget(settingsFrame);
    boxLayout->addWidget(controlFrame);
    label->hide();
    answerFrame->hide();
    controlFrame->hide();
}

void TableSlot::onGamePaused(bool paused) {
    if (!settingsFrame->isHidden()) {
        cards = CardDeck::shuffleCards(deckCount->value());
        currentCardID = -1;
        settingsFrame->hide();
    }
    if (paused) {
        answerFrame->hide();
        svgName = "blue_back";
        controlFrame->show();
    } else {
        controlFrame->hide();
        if (CardDeck::isJoker(currentCardID)) {
            svgName = CardDeck::cardName(currentCardID);
            userQuizzing();
        }
    }
    update();
}

void TableSlot::onTableSlotResized(QSize newFixedSize) {
    if (size() != newFixedSize) {
        setFixedSize(newFixedSize);
    }
}

bool TableSlot::isFake() {
    return fake;
}

bool TableSlot::pickUpCard() {
    if (cards.empty()) {
        throw std::runtime_error("Deck is empty, cannot pick up another card");
    }
    currentCardID = cards.front();
    svgName = CardDeck::cardName(currentCardID);
    cards.pop_front();
    if (!label->isHidden()) {
        label->hide();
    }
    update();
    if (CardDeck::isJoker(currentCardID)) {
        userQuizzing();
    }
    return cards.empty();
}

void TableSlot::userQuizzing() {
    answerFrame->show();
    emit userQuizzed();
}

void TableSlot::userChecking() {
    label->show();
    answerFrame->hide();
    emit userAnswered(weightBox->value() == 0);
}
