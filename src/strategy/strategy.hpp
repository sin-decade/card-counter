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

#ifndef CARD_COUNTER_STRATEGY_HPP
#define CARD_COUNTER_STRATEGY_HPP

// Qt
#include <QString>
#include <QVector>

/**
 * @brief The Strategy class represents a card counting strategy
 */
class Strategy {
public:
    /**
     * @brief Constructs a new Strategy object
     * @param name The name of the strategy
     * @param description A short description of the strategy
     * @param weights A vector of weights, where the index is the card rank
     * @param custom Whether this strategy is custom or not
     */
    explicit Strategy(QString name, QString description, QVector<qint32> weights, bool custom = false);

    /**
     * @brief isCustom Returns whether this strategy is custom or not
     * @return True if this is a custom strategy, false otherwise
     */
    bool isCustom() const;

    /**
     * @brief getName Returns the name of the strategy
     * @return The name of the strategy
     */
    QString getName();

    /**
     * @brief getDescription Returns a short description of the strategy
     * @return The description of the strategy
     */
    QString getDescription();

    /**
     * @brief getWeights Returns the weight of the given card rank
     * @param id The card rank
     * @return The weight of the given card rank
     */
    qint32 getWeights(qint32 id);

    /**
     * @brief updateWeight Updates the weight of the deck using this strategy to the last card opened
     * @param currentWeight The current weight of the deck
     * @param rank The rank of the last card opened
     * @return The new weight of the deck
     */
    qint32 updateWeight(qint32 currentWeight, qint32 rank);

private:
    bool _custom; /**< Whether this strategy is custom or not */
    QVector<qint32> _weights; /**< A vector of weights, where the index is the card rank */
    QString _name; /**< The name of the strategy */
    QString _description; /**< A short description of the strategy */
};

#endif //CARD_COUNTER_STRATEGY_HPP

