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
#include <QComboBox>
#include <QCheckBox>
// KF
#include <KLocalizedString>
// own
#include "tableslot.hpp"
#include "src/widgets/cards.hpp"
#include "src/strategy/strategy.hpp"
#include "src/strategy/strategyinfo.hpp"
// own widgets
#include "src/widgets/base/label.hpp"
#include "src/widgets/base/frame.hpp"

TableSlot::TableSlot(StrategyInfo *strategies, QSvgRenderer *renderer, bool isActive, QWidget *parent)
        : Cards(renderer, parent), _strategies(strategies) {

    // QLabels:
    messageLabel = new CCLabel(i18n("TableSlot Weight: 0"));
    indexLabel = new CCLabel("0/0");
    weightLabel = new CCLabel("weight: 0");
    strategyHintLabel = new CCLabel("");
    onStrategyChanged(0);

    // QComboBoxes:
    strategyBox = new QComboBox();
    onNewStrategy();
    connect(strategies, &StrategyInfo::newStrategy, this, &TableSlot::onNewStrategy);
    connect(strategyBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &TableSlot::onStrategyChanged);
    auto *indexing = new QCheckBox();
    connect(indexing, &QCheckBox::stateChanged, indexLabel, &CCLabel::setVisible);
    auto *strategyHint = new QCheckBox();
    connect(strategyHint, &QCheckBox::stateChanged, strategyHintLabel, &CCLabel::setVisible);
    auto *training = new QCheckBox();
    connect(training, &QCheckBox::stateChanged, weightLabel, &CCLabel::setVisible);

    // QFrames:
    answerFrame = new CCFrame();
    settingsFrame = new CCFrame();
    settingsFrame->show();
    controlFrame = new CCFrame();

    // QSpinBoxes:
    weightBox = new QSpinBox();
    weightBox->setRange(-100, 100);

    deckCount = new QSpinBox();
    connect(deckCount, QOverload<int>::of(&QSpinBox::valueChanged), this, &TableSlot::activate);
    deckCount->setRange(isActive, 10);

    // QPushButtons:
    auto *submitButton = new QPushButton(QIcon::fromTheme("answer"), i18n("&Submit"));
    submitButton->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    connect(submitButton, &QPushButton::clicked, this, &TableSlot::userChecking);

    auto *skipButton = new QPushButton(QIcon::fromTheme("media-skip-forward"), i18n("&Skip"));
    skipButton->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
// todo:    connect(skipButton, &QPushButton::clicked, this, &TableSlot::skipping);

    auto *strategyInfoButton = new QPushButton(QIcon::fromTheme("kt-info-widget"), i18n("&Info"));
    strategyInfoButton->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    connect(strategyInfoButton, &QPushButton::clicked, this, &TableSlot::strategyInfoAssist);

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
    auto *settings = new QFormLayout(settingsFrame);
    settings->setFormAlignment(Qt::AlignCenter);
    auto *answer = new QFormLayout(answerFrame);
    answer->setFormAlignment(Qt::AlignCenter);

    // Other Layouts:
    auto *boxLayout = new QVBoxLayout(this);
    auto *infoLayout = new QHBoxLayout();
    auto *controlLayout = new QHBoxLayout(controlFrame);
    auto *strategyLayout = new QHBoxLayout();

    strategyLayout->addWidget(strategyBox);
    strategyLayout->addWidget(strategyInfoButton);

    infoLayout->addWidget(weightLabel);
    infoLayout->addStretch();
    infoLayout->addWidget(indexLabel);

    answer->addRow(tr("&Weight:"), weightBox);
    answer->addRow(submitButton, skipButton);

    settings->addRow(tr("&Number of Card Decks:"), deckCount);
    settings->addRow(tr("Type of Strategy:"), strategyLayout);
    settings->addRow(indexing, new QLabel("Use card indexing"));
    settings->addRow(strategyHint, new QLabel("Add name of strategy"));
    settings->addRow(training, new QLabel("Is training"));

    controlLayout->addWidget(closeButton);
    controlLayout->addWidget(refreshButton);
    controlLayout->addWidget(swapButton);

    boxLayout->addWidget(strategyHintLabel);
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
        cards = shuffleCards(deckCount->value());
        refreshButton->show();
//        swapButton->hide();
        setId(-1);
        settingsFrame->hide();
    }
    if (paused) {
        answerFrame->hide();
        setName("blue_back");
        controlFrame->show();
    } else {
        controlFrame->hide();
        if (isJoker()) {
            setName(getCardNameByCurrentId());
            userQuizzing();
        }
    }
    update();
}

bool TableSlot::isFake() const {
    return fake;
}

void TableSlot::pickUpCard() {
    if (cards.empty()) {
        setName("back");
        emit tableSlotFinished();
        settingsFrame->show();
        controlFrame->show();
        update();
        return;
    }
//    if (isJoker()){
//        messageLabel->hide();
//    }
    setId(cards.front());
    setName(getCardNameByCurrentId());
    cards.pop_front();
    if (!messageLabel->isHidden()) {
        messageLabel->hide();
    }
    update();
    indexLabel->setText(i18n("%1/%2", deckCount->value() * 54 - cards.size(), deckCount->value() * 54));
    if (isJoker()) {
        userQuizzing();
    } else {
        currentWeight = _strategy->updateWeight(currentWeight, getCurrentRank());
        weightLabel->setText(i18n("weight: %1", currentWeight));
    }
    // add highlighting
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
    cards = shuffleCards(deckCount->value());
    settingsFrame->hide();
    // hide controlFrame if not paused
}

void TableSlot::onCanRemove(bool canRemove) {
    closeButton->setVisible(canRemove);
}

void TableSlot::activate(int value) {
    if (value > 0 && fake) {
        fake = false;
        controlFrame->show();
        setName("green_back");
        currentWeight = 0;
        deckCount->setMinimum(1);
        emit tableSlotActivated();
    }
}

void TableSlot::onNewStrategy() {
    QStringList items;
    for (auto *item: _strategies->getStrategies()) {
        items.push_back(item->getName());
    }
    items.pop_back(); // last is fake
    strategyBox->clear();
    strategyBox->addItems(items);
}

void TableSlot::onStrategyChanged(int index) {
    if (index >= 0) {
        _strategy = _strategies->getStrategyById(index);
        strategyHintLabel->setText(_strategy->getName());
    }
}
