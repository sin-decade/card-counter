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
#include <QFormLayout>
#include <QSpinBox>
#include <QPainter>
#include <QPushButton>
#include <QDebug>
#include <QComboBox>
#include <QCheckBox>
// KF
#include <KLocalizedString>
// own
#include "tableslot.hpp"
#include "carddeck.hpp"
#include "strategy.hpp"
// own widgets
#include "widgets/label.hpp"
#include "widgets/frame.hpp"

void TableSlot::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);

    if (m_renderer->isValid() && m_renderer->elementExists(svgName)) {
        QPainter painter(this);
        m_renderer->render(&painter, svgName);
    }
}

TableSlot::TableSlot(QSvgRenderer *renderer, bool isActive, QWidget *parent)
        : QWidget(parent), m_renderer(renderer) {
    QStringList items = {"Hi-Lo Count", "Hi-Opt I Count", "Hi-Opt II Count", "KO Count"};

    // YaLabels:
    messageLabel = new YaLabel(i18n("TableSlot Weight: 0"));
    indexLabel = new YaLabel("0/0");
    weightLabel = new YaLabel("weight: 0");

    // QComboBoxes:
    QComboBox *strategy = new QComboBox();
    connect(strategy, QOverload<int>::of(&QComboBox::currentIndexChanged), [=](int index) {
        strategyID = index;
    });
    QCheckBox *indexing = new QCheckBox();
    connect(indexing, &QCheckBox::stateChanged, indexLabel, &YaLabel::setVisible);
    QCheckBox *training = new QCheckBox();
    connect(training, &QCheckBox::stateChanged, weightLabel, &YaLabel::setVisible);

    // QFrames:
    answerFrame = new YaFrame();
    settingsFrame = new YaFrame();
    settingsFrame->show();
    controlFrame = new YaFrame();

    // QSpinBoxes:
    weightBox = new QSpinBox();
    weightBox->setRange(-100, 100);

    deckCount = new QSpinBox();
    connect(deckCount, QOverload<int>::of(&QSpinBox::valueChanged), this, &TableSlot::activate);
    deckCount->setRange(isActive, 10);

    // QPushButtons:
    QPushButton *submitButton = new QPushButton(QIcon::fromTheme("answer"), i18n("&Submit"));
    submitButton->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    connect(submitButton, &QPushButton::clicked, this, &TableSlot::userChecking);

    QPushButton *skipButton = new QPushButton(QIcon::fromTheme("media-skip-forward"), i18n("&Skip"));
    skipButton->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
//    connect(skipButton, &QPushButton::clicked, this, &TableSlot::skipping);

    closeButton = new QPushButton(QIcon::fromTheme("delete"), i18n("&Remove"));
    closeButton->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    connect(closeButton, &QPushButton::clicked, this, &TableSlot::tableSlotRemoved);
    closeButton->hide();

    refreshButton = new QPushButton(QIcon::fromTheme("view-refresh"), i18n("&Reshuffle"));
    refreshButton->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    connect(refreshButton, &QPushButton::clicked, this, &TableSlot::reshuffleDeck);
    refreshButton->hide();

    swapButton = new QPushButton(QIcon::fromTheme("exchange-positions"), i18n("&Swap"));
    swapButton->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    connect(swapButton, &QPushButton::clicked, this, &TableSlot::swapTargetSelected);

    // QFormLayouts:
    QFormLayout *settings = new QFormLayout(settingsFrame);
    settings->setFormAlignment(Qt::AlignCenter);
    QFormLayout *answer = new QFormLayout(answerFrame);
    answer->setFormAlignment(Qt::AlignCenter);

    // Other Layouts:
    QVBoxLayout *boxLayout = new QVBoxLayout(this);
    QHBoxLayout *infoLayout = new QHBoxLayout();
    QHBoxLayout *controlLayout = new QHBoxLayout(controlFrame);

    infoLayout->addWidget(weightLabel);
    infoLayout->addStretch();
    infoLayout->addWidget(indexLabel);

    answer->addRow(tr("&Weight:"), weightBox);
    answer->addRow(submitButton, skipButton);

    settings->addRow(tr("&Number of Card Decks:"), deckCount);
    strategy->addItems(items);
    settings->addRow(tr("&Type of Strategy:"), strategy);
    settings->addRow(indexing, new QLabel("Use card indexing"));
    settings->addRow(training, new QLabel("Is training"));

    controlLayout->addWidget(closeButton);
    controlLayout->addWidget(refreshButton);
    controlLayout->addWidget(swapButton);

    boxLayout->addStretch();
    boxLayout->addWidget(messageLabel);
    boxLayout->addWidget(answerFrame);
    boxLayout->addWidget(settingsFrame);
    boxLayout->addWidget(controlFrame);
    boxLayout->addStretch();
    boxLayout->addLayout(infoLayout);
}

void TableSlot::onGamePaused(bool paused) {
    if (!settingsFrame->isHidden()) {
        cards = CardDeck::shuffleCards(deckCount->value());
        refreshButton->show();
//        swapButton->hide();
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

void TableSlot::pickUpCard() {
    if (cards.empty()) {
        svgName = "back";
        emit tableSlotFinished();
        settingsFrame->show();
        controlFrame->show();
        update();
        return;
    }
//    if (CardDeck::isJoker(currentCardID)){
//        messageLabel->hide();
//    }
    currentCardID = cards.front();
    svgName = CardDeck::cardName(currentCardID);
    cards.pop_front();
    if (!messageLabel->isHidden()) {
        messageLabel->hide();
    }
    update();
    indexLabel->setText(i18n("%1/%2", deckCount->value() * 54 - cards.size(), deckCount->value() * 54));
    if (CardDeck::isJoker(currentCardID)) {
        userQuizzing();
    } else {
        currentWeight = Strategy::updateWeight(currentWeight, currentCardID, strategyID);
        weightLabel->setText(i18n("weight: %1", currentWeight));
    }
}

void TableSlot::userQuizzing() {
    answerFrame->show();
    emit userQuizzed();
}

void TableSlot::userChecking() {
    messageLabel->setText(i18n("TableSlot Weight: %1", currentWeight));
    answerFrame->hide();
    bool isCorrect = weightBox->value() == currentWeight;
    messageLabel->setPalette(QPalette(isCorrect ? Qt::green : Qt::red));
    messageLabel->show();
    emit userAnswered(isCorrect);
}

void TableSlot::reshuffleDeck() {
    cards = CardDeck::shuffleCards(deckCount->value());
}

void TableSlot::onCanRemove(bool canRemove) {
    closeButton->setVisible(canRemove);
}

void TableSlot::activate(int value) {
    if (value > 0 && fake) {
        fake = false;
        controlFrame->show();
        svgName = "green_back";
        currentWeight = 0;
        deckCount->setMinimum(1);
        emit tableSlotActivated();
    }
}
