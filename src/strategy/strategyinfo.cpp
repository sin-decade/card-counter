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
#include <QLabel>
#include <QFormLayout>
#include <QSpinBox>
#include <QSvgRenderer>
#include <QDialogButtonBox>
#include <QListWidget>
#include <QLineEdit>
#include <QTextEdit>
// KF
#include <KLocalizedString>
// own
#include "strategyinfo.hpp"
#include "strategy.hpp"
#include "src/widgets/carousel.hpp"
#include "src/widgets/cards.hpp"

StrategyInfo::StrategyInfo(QSvgRenderer *renderer, QWidget *parent, Qt::WindowFlags flags)
        : QDialog(parent, flags), m_renderer(renderer), _id(0) {
    setWindowTitle("Strategy Info");
    setModal(true);

    initStrategies();

    auto *dialogButtons = new QDialogButtonBox();
    saveButton = new QPushButton(QIcon::fromTheme("document-save"), i18n("&Save"));
    auto *window = new QHBoxLayout(this);
    auto *leftPanel = new QWidget;
    auto *leftPanelLayout = new QVBoxLayout(leftPanel);
    auto *searchBox = new QLineEdit();
    listWidget = new QListWidget();
    auto *rightPanel = new QWidget;
    auto *body = new QVBoxLayout(rightPanel);
    auto *carousel = new Carousel(renderer->boundsOnElement("back").size());
    _name = new QLabel(items[_id]->getName());
    _description = new QLabel(items[_id]->getDescription());
    _nameInput = new QLineEdit();
    _descriptionInput = new QTextEdit();
    auto *title = new QWidget;
    auto *titleLayout = new QHBoxLayout(title);
    auto *browser = new QWidget;
    auto *browserLayout = new QHBoxLayout(browser);

    leftPanel->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Minimum);
    searchBox->setPlaceholderText(tr("Search"));
    listWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    listWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    listWidget->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    _description->setWordWrap(true);
    _description->setTextFormat(Qt::TextFormat::MarkdownText);
    _description->setOpenExternalLinks(true);
    _name->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    _nameInput->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    _description->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    _descriptionInput->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    dialogButtons->setStandardButtons(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

    dialogButtons->addButton(saveButton, QDialogButtonBox::ActionRole);
    leftPanelLayout->addWidget(searchBox);
    titleLayout->addWidget(_nameInput);
    titleLayout->addWidget(_name);
    browserLayout->addWidget(_descriptionInput);
    browserLayout->addWidget(_description);
    leftPanelLayout->addWidget(listWidget);
    window->addWidget(leftPanel);
    window->addWidget(rightPanel);
    body->addWidget(title);
    body->addWidget(browser);
    for (int i = Cards::Rank::Ace; i <= Cards::Rank::King; i++) {
        auto *card = new Cards(renderer);
        auto *form = new QFormLayout(card);
        auto *spin = new QSpinBox();

        card->setId(i);
        spin->setRange(-5, 5);
        spin->setValue(items[_id]->getWeights(i - Cards::Rank::Ace));
        spin->setReadOnly(!items[_id]->isCustom());
        spin->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
        form->setFormAlignment(Qt::AlignCenter);
        form->addRow(spin);
        carousel->addWidget(card);
        weights.push_back(spin);
    }
    body->addWidget(carousel);
    body->addStretch();
    body->addWidget(dialogButtons);
    fillList();

    listWidget->setCurrentItem(listWidget->item(0));
    connect(saveButton, &QPushButton::clicked, this, [=]() {
        if (_nameInput->text() != _name->text() || _descriptionInput->toMarkdown() != _description->text()) {
            return;
        }
        QVector<qint32> currentWeights;
        for (auto &weight: weights) {
            currentWeights.push_back(weight->value());
        }
        items[_id] = new Strategy(_name->text(), _description->text(),
                                  currentWeights, true);
        listWidget->currentItem()->setText(_name->text());
        addFakeStrategy();
    });
    connect(listWidget, &QListWidget::itemSelectionChanged, this,
            [=]() { showStrategyByName(listWidget->currentItem()->text()); });
    connect(searchBox, &QLineEdit::textChanged, this, [=](const QString &text) {
        for (int i = 0; i < listWidget->count(); i++) {
            auto *item = listWidget->item(i);
            bool matches = item->text().contains(text, Qt::CaseInsensitive);
            item->setHidden(!matches);
        }
    });
    connect(_nameInput, &QLineEdit::textChanged, this,
            [=](const QString &text) { _name->setText(text); });
    connect(_descriptionInput, &QTextEdit::textChanged, this,
            [=]() { _description->setText(_descriptionInput->toMarkdown()); });
    connect(dialogButtons, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(dialogButtons, &QDialogButtonBox::rejected, this, &QDialog::reject);

    saveButton->hide();
    _nameInput->hide();
    _descriptionInput->hide();
}

Strategy *StrategyInfo::getStrategyById(qint32 id) {
    return items[id];
}

void StrategyInfo::showStrategyByName(const QString &name) {
    qint32 id = -1;
    for (int i = 0; i < items.size(); i++) {
        if (items[i]->getName() == name) {
            id = i;
            break;
        }
    }
    if (id != -1 && _id != id) {
        _id = id;
        _name->setText(items[_id]->getName());
        _description->setText(items[_id]->getDescription());
        bool isCustom = items[_id]->isCustom();
        _descriptionInput->setHidden(!isCustom);
        _nameInput->setHidden(!isCustom);
        saveButton->setHidden(!isCustom);
        for (int i = Cards::Rank::Ace; i <= Cards::Rank::King; i++) {
            weights[i - Cards::Rank::Ace]->setValue(items[_id]->getWeights(i - Cards::Rank::Ace));
            weights[i - Cards::Rank::Ace]->setReadOnly(!isCustom);
        }
    }
}

QVector<Strategy *> StrategyInfo::getStrategies() {
    return items;
}

void StrategyInfo::initStrategies() {


    items.push_back(new Strategy(
            "Hi-Opt I Count",
            "The Hi-Opt I blackjack card counting system was developed by Charles Einstein and introduced in his book "
            "\"The World's Greatest Blackjack Book\" in 1980. The Hi-Opt I system assigns point values to each card in "
            "the deck and is a more complex system than the Hi-Lo system, with additional point values for some cards. "
            "It is considered a more powerful system than the Hi-Lo, but also more difficult to learn "
            "and use effectively.",
            {0, 0, 1, 1, 1, 1, 0, 0, 0, -1, -1, -1, -1}));

    items.push_back(new Strategy(
            "Hi-Lo Count",
            "The Hi-Lo blackjack card counting system was first introduced by Harvey Dubner in 1963. Dubner's goal was "
            "to create a simple yet effective system that could be used by anyone to increase their odds of winning "
            "at blackjack.",
            {-1, 1, 1, 1, 1, 1, 0, 0, 0, -1, -1, -1, -1}));

    items.push_back(new Strategy(
            "Hi-Opt II Count",
            "The Hi-Opt II blackjack card counting system is a more advanced version of the Hi-Opt I system, "
            "developed by Lance Humble and Carl Cooper in their book \"The World's Greatest Blackjack Book\" in 1980. "
            "The Hi-Opt II system assigns point values to each card in the deck, with additional point values "
            "for some cards, and is considered one of the most powerful card counting systems. It is also one of "
            "the most difficult to learn and use effectively.",
            {0, 1, 1, 2, 2, 1, 1, 0, 0, -2, -2, -2, -2}));

    items.push_back(new Strategy(
            "KO Count",
            "The Knock-Out (KO) blackjack card counting system was developed by Olaf Vancura and Ken Fuchs in their "
            "book \"Knock-Out Blackjack\" in 1998. The KO system assigns point values to each card in the deck, with "
            "the additional advantage that it does not require a true count conversion for betting, making it easier "
            "to use than some other systems.",
            {-1, 1, 1, 1, 1, 1, 1, 0, 0, -1, -1, -1, -1}));

    items.push_back(new Strategy(
            "Omega II Count",
            "The Omega II blackjack card counting system was developed by Bryce Carlson and introduced in his book "
            "\"Blackjack for Blood\" in 2001. The Omega II system assigns point values to each card in the deck, with "
            "additional point values for some cards, and is considered one of the most powerful card counting systems, "
            "especially for multi-deck games.",
            {0, 1, 1, 2, 2, 2, 1, 0, -1, -2, -2, -2, -2}));

    items.push_back(new Strategy(
            "Zen Count",
            "The Zen Count blackjack card counting system was developed by Arnold Snyder and introduced in his book "
            "\"Blackbelt in Blackjack\" in 1983. The Zen Count system assigns point values to each card in the deck, "
            "with additional point values for some cards, and is considered a powerful system for both single "
            "and multi-deck games.",
            {-1, 1, 1, 2, 2, 2, 1, 0, 0, -2, -2, -2, -2}));

    items.push_back(new Strategy(
            "10 Count",
            "The 10 Count blackjack card counting system was developed by Edward O. Thorp, a mathematician and author "
            "of the classic book \"Beat the Dealer\" in 1962. The 10 Count system assigns point values to each card in "
            "the deck, with a focus on the 10-value cards, and is considered one of the earliest "
            "and most basic card counting systems.",
            {1, 1, 1, 1, 1, 1, 1, 1, 1, -2, -2, -2, -2}));
}

void StrategyInfo::addFakeStrategy() {
    items.push_back(new Strategy(
            "New Strategy",
            "Some Notes (use Markdown)",
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            true));
    listWidget->sortItems();
    auto *widgetItem = new QListWidgetItem(items.last()->getName());
    listWidget->addItem(widgetItem);
}

void StrategyInfo::fillList() {
    for (auto &item: items) {
        auto *widgetItem = new QListWidgetItem(item->getName());
        listWidget->addItem(widgetItem);
    }
    addFakeStrategy();
}


