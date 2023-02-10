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

#ifndef YACARDCOUNTER_CARDDECK_HPP
#define YACARDCOUNTER_CARDDECK_HPP
// Qt
#include <QObject>

class CardDeck {
public:
    enum Colour {
        Black = 0, Red
    };

    enum Suit {
        Clubs = 0, Diamonds, Hearts, Spades
    };

    enum Rank {
        Joker = 0, Ace, Two, Three, Four, Five, Six, Seven, Eight, Nine, Ten, Jack, Queen, King
    };

    static QList<quint32> shuffleCards(quint32 deckCount, quint32 shuffleCoefficient = 2);

    static QList<quint32> generateDeck(quint32 deckCount);

    static QString cardName(quint32 id, quint32 standard = 0);

    static bool isJoker(quint32 id);

    static QString getColour(quint32 colour);

    static QString getSuit(quint32 suit);

    static QString getRank(quint32 rank, bool standard = true);
};


#endif //YACARDCOUNTER_CARDDECK_HPP
