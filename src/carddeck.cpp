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
#include <QRandomGenerator>
// own
#include "carddeck.hpp"

QList<qint32> CardDeck::shuffleCards(qint32 deckCount, qint32 shuffleCoefficient) {
    QList<qint32> deck = generateDeck(deckCount);
    qint32 threshold = deck.size() / (deckCount * shuffleCoefficient);

    while (true) {
        std::shuffle(deck.begin(), deck.end(), *QRandomGenerator::global());
        qint32 lastJokerIndex = -1;
        for (int i = 0; i < deck.size(); i++) {
            if (!getRank(deck[i])) {
                if (i - lastJokerIndex < threshold)
                    continue;
                lastJokerIndex = i;
            }
        }
        break;
    }

    return deck;
}

QString CardDeck::cardName(qint32 id, qint32 standard) {
    qint32 rank = getRank(id);
    qint32 suit = getSuit(id);
    QString name = getRankName(rank, standard & 1);
    if (isJoker(id)) {
        name = getColourName(suit) + name;
    } else {
        name += getSuitName(suit);
    }

    return name;
}

QList<qint32> CardDeck::generateDeck(qint32 deckCount) {
    QList<qint32> deck;
    for (qint32 i = 0; i < deckCount; i++) {
        for (qint32 rank = Rank::Ace; rank <= Rank::King; rank++) {
            for (qint32 suit = Suit::Clubs; suit <= Suit::Spades; suit++) {
                deck.append(((suit & 0xff) << 8) | (rank & 0xff));
            }
        }
        for (int colour = Colour::Black; colour <= Colour::Red; colour++) {
            deck.append((colour & 0xff) << 8);
        }
    }
    return deck;
}

QString CardDeck::getColourName(qint32 colour) {
    switch (colour) {
        case Black:
            return QStringLiteral("black_");
        case Red:
            return QStringLiteral("red_");
        default:
            return "";
    }
}

QString CardDeck::getSuitName(qint32 suit) {
    switch (suit) {
        case Clubs:
            return QStringLiteral("_club");
        case Diamonds:
            return QStringLiteral("_diamond");
        case Hearts:
            return QStringLiteral("_heart");
        case Spades:
            return QStringLiteral("_spade");
        default:
            return "";
    }
}

QString CardDeck::getRankName(qint32 rank, bool standard) {
    switch (rank) {
        case King:
            return QStringLiteral("king");
        case Queen:
            return QStringLiteral("queen");
        case Jack:
            return QStringLiteral("jack");
        case Joker:
            if (standard) {
                return QStringLiteral("joker");
            } else {
                return QStringLiteral("jocker");
            }
        case Ace:
            if (standard) {
                return QStringLiteral("ace");
            }
        default:
            return QString::number(rank);
    }
}

bool CardDeck::isJoker(qint32 id) {
    return !getRank(id);
}

qint32 CardDeck::getRank(qint32 id) {
    return Rank(id & 0xff);
}

qint32 CardDeck::getSuit(qint32 id) {
    return (id >> 8) & 0xff;
}
