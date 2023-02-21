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

#ifndef YACARDCOUNTER_STRATEGY_HPP
#define YACARDCOUNTER_STRATEGY_HPP

// Qt
#include <QWidget>

class QLabel;

class QSvgRenderer;

class QSpinBox;

/**
 * @brief Represents a card counting strategy in blackjack.
 *
 * @details The `Strategy` class provides a means of implementing various card counting strategies for use in blackjack.
 * Each strategy is defined by a set of weights, one for each rank in the deck, which indicate the value of each card
 * for the purposes of counting. The `Strategy` class also allows for the name and description of each strategy to
 * be set and retrieved, and provides a function for updating the weight of a deck based on the last card drawn,
 * according to the current strategy. The `Strategy` class inherits from `QWidget`, allowing it to be easily integrated
 * into a GUI application.
 */
class Strategy : public QWidget {
Q_OBJECT
public:
    /**
    * @brief Constructs a new Strategy object.
    *
    * @details This constructor creates a new Strategy object that uses the specified `renderer` to
     * display playing cards. The `custom` parameter indicates whether the strategy is a custom strategy,
     * and the `parent` parameter is the parent widget. After creating the object, you can set the strategy's name
     * and description using the `setName` and `setDescription` functions, and you can set the weights for the strategy
     * using the `setWeights` function.
    *
    * @param renderer The renderer to use for displaying playing cards.
    * @param custom Whether the strategy is a custom strategy.
    * @param parent The parent widget.
    */
    explicit Strategy(QSvgRenderer *renderer, bool custom = false, QWidget *parent = nullptr);

    /**
     * @brief Sets the name of the strategy.
     *
     * @details This function sets the name of the strategy to the specified `name`.
     *
     * @param name The name of the strategy.
     */
    void setName(QString name);

    /**
     * @brief Gets the name of the strategy.
     *
     * @details This function returns the name of the strategy as a QString.
     *
     * @return The name of the strategy.
     */
    QString getName();

    /**
     * @brief Sets the description of the strategy.
     *
     * @details This function sets the description of the strategy to the specified `description`.
     * The description should include any relevant historical or background information about the strategy.
     *
     * @param description The description of the strategy.
     */
    void setDescription(const QString &description);

    /**
     * @brief Updates the weight of the deck using this strategy based on the rank of the last card that was dealt and
     * the current weight of the deck.
     *
     * @details This function updates the weight of the deck using this strategy to the last card opened.
     * The weight of individual ranks may change depending on the current weight of the deck for this strategy,
     * which is passed as the `currentWeight` parameter. The rank of the last card that was dealt, which is passed as
     * the `rank` parameter, is used to update the weights for the strategy and the weight of the deck using
     * this strategy to the last card opened. The updated weight of the deck for this strategy is returned,
     * which should be used to update the running count of the deck.
     *
     * @param currentWeight The current weight of the deck for this strategy.
     * @param rank The rank of the last card that was dealt.
     *
     * @return The updated weight of the deck for this strategy.
     */
    qint32 updateWeight(qint32 currentWeight, qint32 rank);

    /**
     * @brief Sets the weights for the strategy.
     *
     * @details This function sets the weights for the strategy to the specified `weights` vector.
     * The weights vector should contain one weight value for each rank in the deck, in order from lowest to highest
     * rank. The weight value for each rank is the number by which the weight of the deck should be adjusted when
     * a card of that rank is drawn.
     *
     * @param weights A vector containing the weights for each rank in the deck.
     */
    void setWeights(QVector<qint32> weights);

private:
    QVector<QSpinBox *> _weights;
    QString _name;
    QLabel *browser;
};


#endif //YACARDCOUNTER_STRATEGY_HPP
