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

#ifndef CARD_COUNTER_TABLESLOT_HPP
#define CARD_COUNTER_TABLESLOT_HPP

// own
#include "src/widgets/cards.hpp"

class QSvgRenderer;

class QSpinBox;

class QVBoxLayout;

class QPushButton;

class CCFrame;

class CCLabel;

class StrategyInfo;

class Strategy;

class QComboBox;

/*!
 * \brief The TableSlot class represents the slot on a table that can contain
 * one or multiple shuffled deck of playing cards. The slot can be fake (not contain any deck)
 * or activated (contain at least one deck).
 */
class TableSlot : public Cards {
Q_OBJECT

public:
    /**
     * @brief Constructs a TableSlot object
     * @param strategies The object containing the strategies
     * @param renderer The object used to render the playing cards
     * @param isActive Whether the table slot is initially active
     * @param parent The parent widget
     */
    explicit TableSlot(StrategyInfo *strategies, QSvgRenderer *renderer, bool isActive = false,
                       QWidget *parent = nullptr);

    /**
     * @brief Checks if the table slot is fake, i.e., if it contains no deck of cards
     * @return True if the table slot is fake, false otherwise
     */
    bool isFake() const;

    /**
     * @brief Picks up a card from the table slot
     */
    void pickUpCard();

signals:

    /**
     * @brief TableSlotActivated - Signal emitted when the table slot is activated
     * i.e. when the number of standard decks is set to a value greater than zero
     */
    void tableSlotActivated();

    /**
     * @brief TableSlotRemoved - Signal emitted when the table slot is removed from the table.
     */
    void tableSlotRemoved();

    /**
     * @brief TableSlotFinished - Signal emitted when the table slot is finished,
     * meaning no more cards can be picked
     */
    void tableSlotFinished();

    /**
     * @brief TableSlotReshuffled - Signal emitted when the table slot is reshuffled,
     * meaning the cards have been mixed again
     */
    void tableSlotReshuffled();

    /**
     * @brief UserQuizzed - Signal emitted when the abstract dealer picked up a joker
     * and the user needs to answer a question
     */
    void userQuizzed();

    /**
     * @brief UserAnswered - Signal emitted when the user answered the question
     * about the weight of the table slot
     * @param correct Whether the answer was correct or not
     */
    void userAnswered(bool correct);

    /**
     * @brief SwapTargetSelected - Signal emitted when user selects one of two targets for swapping.
     * @note The receiver of this signal should wait for the second target to be selected.
     */
    void swapTargetSelected();

    /**
     * @brief StrategyInfoAssist - Signal emitted when the user needs assistance with the strategy information
     */
    void strategyInfoAssist();

public Q_SLOTS:

    /**
     * @brief onGamePaused - Slot called when the game is paused or resumed.
     * @param paused Whether the game is paused or not
     */
    void onGamePaused(bool paused);

    /**
     * @brief onCanRemove - Slot called when the table slots can or cannot be removed.
     * @param canRemove Whether the table slots can be removed or not
     */
    void onCanRemove(bool canRemove);

    /**
     * @brief onNewStrategy - Slot called when a new strategy is added.
     */
    void onNewStrategy();

    /**
     * @brief onStrategyChanged - Slot called when the selected strategy is changed.
     * @param index Index of the new strategy in the strategy combo box
     */
    void onStrategyChanged(int index);

    /**
     * @brief userChecking - Slot called when the user is checking the weight of the slot.
     */
    void userChecking();

    /**
     * @brief reshuffleDeck - Slot called when the user wants to reshuffle the deck.
     */
    void reshuffleDeck();

    /**
     * @brief activate - Slot called when the slot is activated, meaning the number of standard decks is set to a value
     * greater than zero.
     * @param value Number of standard decks in the table slot
     */
    void activate(int value);

private:
    /**
     * @brief userQuizzing - Method called when the abstract dealer picked up a joker
     * and the user needs to answer a question.
     */
    void userQuizzing();

    QList<qint32> cards; // List of card indices
    Strategy *_strategy{}; // Pointer to the current strategy
    StrategyInfo *_strategies; // Pointer to the strategies available in the game
    qint32 currentWeight = 0; // The current weight of the slot
    bool fake = true; // Flag indicating whether the slot is fake or not

    // UI elements
    CCFrame *answerFrame; // Frame for displaying the answer input and submit button
    CCFrame *settingsFrame; // Frame for displaying settings related to the table slot
    CCFrame *controlFrame; // Frame for displaying control buttons

    QSpinBox *deckCount; // Spin box for setting the number of standard decks in the table slot
    QSpinBox *weightBox; // Spin box for setting the weight of the table slot

    CCLabel *messageLabel; // Label for displaying messages to the user
    CCLabel *indexLabel; // Label for displaying the index of the table slot
    CCLabel *weightLabel; // Label for displaying the weight of the table slot
    CCLabel *strategyHintLabel; // Label for displaying hints related to the current strategy

    QPushButton *refreshButton; // Button for refreshing the card deck
    QPushButton *swapButton; // Button for swapping the current slot with another
    QPushButton *closeButton; // Button for closing the current slot

    QComboBox *strategyBox; // Combo box for selecting the current strategy
};

#endif //CARD_COUNTER_TABLESLOT_HPP
