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

#include <QRandomGenerator>
#include "carddeck.hpp"

QList<quint32> CardDeck::shuffleCards(quint32 deckCount, quint32 shuffleCoefficient) {
    QList<quint32> deck = generateDeck(deckCount);
    quint32 threshold = deck.size() / (deckCount * shuffleCoefficient);

    while (true) {
        std::shuffle(deck.begin(), deck.end(), *QRandomGenerator::global());
        quint32 lastJokerIndex = -1;
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

QString CardDeck::cardName(quint32 id, quint32 standard) {
    quint32 rank = getRank(id);
    quint32 suit = getSuit(id);
    QString name = getRankName(rank, standard & 1);
    if (isJoker(id)) {
        name = getColourName(suit) + name;
    } else {
        name += getSuitName(suit);
    }

    return name;
}

QList<quint32> CardDeck::generateDeck(quint32 deckCount) {
    QList<quint32> deck;
    for (quint32 i = 0; i < deckCount; i++) {
        for (quint32 rank = Rank::Ace; rank <= Rank::King; rank++) {
            for (quint32 suit = Suit::Clubs; suit <= Suit::Spades; suit++) {
                deck.append(((suit & 0xff) << 8) | (rank & 0xff));
            }
        }
        for (int colour = Colour::Black; colour <= Colour::Red; colour++) {
            deck.append((colour & 0xff) << 8);
        }
    }
    return deck;
}

QString CardDeck::getColourName(quint32 colour) {
    switch (colour) {
        case Black:
            return QStringLiteral("black_");
        case Red:
            return QStringLiteral("red_");
        default:
            return QStringLiteral();
    }
}

QString CardDeck::getSuitName(quint32 suit) {
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
            return QStringLiteral();
    }
}

QString CardDeck::getRankName(quint32 rank, bool standard) {
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

bool CardDeck::isJoker(quint32 id) {
    return !getRank(id);
}

quint32 CardDeck::getRank(quint32 id) {
    return Rank(id & 0xff);
}

quint32 CardDeck::getSuit(quint32 id) {
    return (id >> 8) & 0xff;
}
