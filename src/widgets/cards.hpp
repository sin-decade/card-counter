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

#ifndef YACARDCOUNTER_CARDS_HPP
#define YACARDCOUNTER_CARDS_HPP

// Qt
#include <QWidget>

class QSvgRenderer;

/**
 * @brief The Cards class represents a playing card with a given ID.
 *
 * This class allows the user to get information about a specific playing card, including its name, rank, suit, and
 * colour. It can also generate a shuffled deck of cards or generate a single card name by ID.
 */
class Cards : public QWidget {
Q_OBJECT
public:
    /**
     * @brief Creates a new Cards object with the given SVG renderer.
     * @param renderer The SVG renderer to use for rendering the card.
     * @param parent The parent widget (default: nullptr).
     */
    explicit Cards(QSvgRenderer *renderer, QWidget *parent = nullptr);

    void paintEvent(QPaintEvent *event) override;

    /**
     * @brief Sets the ID of the current card.
     * @param id The ID of the card.
     */
    void setId(qint32 id);

    /**
     * @brief Sets the name of the current card.
     * @param name The name of the card.
     */
    void setName(QString name);

    /**
     * @brief An enumeration representing the colours of the card (black or red).
     */
    enum Colour {
        Black = 0, /**< Black colour. */
        Red /**< Red colour. */
    };

    /**
     * @brief An enumeration representing the suits of the card (clubs, diamonds, hearts, or spades).
     */
    enum Suit {
        Clubs = 0, /**< Clubs suit. */
        Diamonds, /**< Diamonds suit. */
        Hearts, /**< Hearts suit. */
        Spades /**< Spades suit. */
    };

    /**
     * @brief An enumeration representing the ranks of the card (joker, ace, two, three, ..., king).
     */
    enum Rank {
        Joker = 0, /**< Joker rank. */
        Ace, /**< Ace rank. */
        Two, /**< Two rank. */
        Three, /**< Three rank. */
        Four, /**< Four rank. */
        Five, /**< Five rank. */
        Six, /**< Six rank. */
        Seven, /**< Seven rank. */
        Eight, /**< Eight rank. */
        Nine, /**< Nine rank. */
        Ten, /**< Ten rank. */
        Jack, /**< Jack rank. */
        Queen, /**< Queen rank. */
        King /**< King rank. */
    };

    /**
     * @brief Generates a shuffled deck of cards.
     * @param deckCount The number of decks to use in the shuffle (default: 1).
     * @param shuffleCoefficient The number of times to shuffle the deck (default: 2).
     * @return A QList containing the IDs of the shuffled cards.
     */
    static QList<qint32> shuffleCards(qint32 deckCount, qint32 shuffleCoefficient = 2);

    static QList<qint32> generateDeck(qint32 deckCount);

    /**
     * @brief Generates a single card name by ID.
     * @param id The ID of the card.
     * @param standard Whether to use standard card names (default: true).
     * @return The name of the card.
     */
    static QString cardName(qint32 id, qint32 standard = 0);

    /**
     * @brief Gets the name of the current card.
     * @param standard Whether to use standard card names (default: true).
     * @return The name of the card.
     */
    QString getCardNameByCurrentId(qint32 standard = 0) const;

    /**
     * Check if a card id represents a joker.
     *
     * @param id The id of the card.
     * @return True if the card is a joker, false otherwise.
     */
    static bool isJoker(qint32 id);

    /**
     * @brief Checks if the current card is a joker.
     *
     * @return True if the current card is a joker, false otherwise.
     */
    bool isJoker() const;

    /**
     * @brief Returns the name of the colour corresponding to the given index.
     *
     * @param colour The index of the colour to get the name for.
     * @return The name of the colour.
     */
    static QString getColourName(qint32 colour);

    /**
     * @brief Returns the name of the suit corresponding to the given index.
     *
     * @param suit The index of the suit to get the name for.
     * @return The name of the suit.
     */
    static QString getSuitName(qint32 suit);

    /**
     * @brief Returns the name of the rank corresponding to the given index.
     *
     * @param rank The index of the rank to get the name for.
     * @param standard Whether to return the standard name or not (default: true).
     * @return The name of the rank.
     */
    static QString getRankName(qint32 rank, bool standard = true);

    /**
     * @brief Returns the rank of the card corresponding to the given index.
     *
     * @param id The index of the card to get the rank for.
     * @return The rank of the card.
     */
    static qint32 getRank(qint32 id);

    /**
     * @brief Returns the rank of the current card.
     *
     * @return The rank of the current card.
     */
    qint32 getCurrentRank() const;

    /**
     * @brief Returns the suit of the card corresponding to the given index.
     *
     * @param id The index of the card to get the suit for.
     * @return The suit of the card.
     */
    static qint32 getSuit(qint32 id);


private:
    QString svgName; ///< The name of the SVG file used to render the cards.
    qint32 currentCardID; ///< The ID of the current card being displayed.

    QSvgRenderer *m_renderer; ///< The renderer used to draw the cards.
};


#endif //YACARDCOUNTER_CARDS_HPP
