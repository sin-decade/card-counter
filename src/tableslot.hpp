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

#ifndef YACARDCOUNTER_TABLESLOT_HPP
#define YACARDCOUNTER_TABLESLOT_HPP


// Qt
#include <QWidget>

class QSvgRenderer;

class QSpinBox;

class QVBoxLayout;

class QPushButton;

class YaFrame;

class YaLabel;

/*!
 * \brief The TableSlot class represents the slot on a table that can contain
 * one or multiple shuffled deck of playing cards. The slot can be fake (not contain any deck)
 * or activated (contain at least one deck).
 */
class TableSlot : public QWidget {
Q_OBJECT

public:
    explicit TableSlot(QSvgRenderer *renderer, bool isActive = false, QWidget *parent = nullptr);

    void paintEvent(QPaintEvent *event) override;

    bool isFake();

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

public Q_SLOTS:

    void onGamePaused(bool paused);

    void onTableSlotResized(QSize newFixedSize);

    void onCanRemove(bool canRemove);

    void userChecking();

    void reshuffleDeck();

    void activate(int value);

private:
    void userQuizzing();

    QList<quint32> cards;
    QString svgName = "back";
    qint32 currentCardID;
    quint32 strategyID = 0;
    qint32 currentWeight;
    bool fake = true;

    QSvgRenderer *m_renderer;

    YaFrame *answerFrame;
    YaFrame *settingsFrame;
    YaFrame *controlFrame;
    /**
     * @brief deckCount - Spin box for setting the number of standard decks in the table slot
     */
    QSpinBox *deckCount;
    QSpinBox *weightBox;

    YaLabel *messageLabel;
    YaLabel *indexLabel;
    YaLabel *weightLabel;

    QPushButton *refreshButton;
    QPushButton *swapButton;
    QPushButton *closeButton;
};

#endif //YACARDCOUNTER_TABLESLOT_HPP
