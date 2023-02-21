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
#include <QSvgRenderer>
#include <QPainter>
// own
#include "cards.hpp"

QList<qint32> Cards::shuffleCards(qint32 deckCount, qint32 shuffleCoefficient) {
    QList<qint32> deck = generateDeck(deckCount);
    qint32 threshold = deck.size() / (deckCount * shuffleCoefficient);
    bool flag;

    do {
        flag = false;
        std::shuffle(deck.begin(), deck.end(), *QRandomGenerator::global());
        qint32 lastJokerIndex = -1;
        for (int i = 0; i < deck.size(); i++) {
            if (isJoker(deck[i])) {
                if (i - lastJokerIndex < threshold) {
                    flag = true;
                    break;
                }
                lastJokerIndex = i;
            }
        }
    } while (flag);

    return deck;
}

QString Cards::cardName(qint32 id, qint32 standard) {
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

QList<qint32> Cards::generateDeck(qint32 deckCount) {
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

QString Cards::getColourName(qint32 colour) {
    switch (colour) {
        case Black:
            return QStringLiteral("black_");
        case Red:
            return QStringLiteral("red_");
        default:
            return "";
    }
}

QString Cards::getSuitName(qint32 suit) {
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

QString Cards::getRankName(qint32 rank, bool standard) {
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

bool Cards::isJoker(qint32 id) {
    return !getRank(id);
}

qint32 Cards::getRank(qint32 id) {
    return Rank(id & 0xff);
}

qint32 Cards::getSuit(qint32 id) {
    return (id >> 8) & 0xff;
}

void Cards::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event)

    if (m_renderer->isValid() && m_renderer->elementExists(svgName)) {
        QPainter painter(this);
        m_renderer->render(&painter, svgName);
//        qDebug()<<m_renderer->aspectRatioMode();
//        if (false) {
//            painter.setPen(QPen(Qt::red, 5));
//            painter.drawRoundedRect(rect(), 19, 19);
//        }
    }
}

Cards::Cards(QSvgRenderer *renderer, QWidget *parent)
        : QWidget(parent), svgName("back"), currentCardID(-1), m_renderer(renderer) {
    setFixedSize(renderer->boundsOnElement("back").size().toSize());
}

void Cards::setId(qint32 id) {
    currentCardID = id;
    setName(cardName(currentCardID));
}

void Cards::setName(QString name) {
    svgName = std::move(name);
}

QString Cards::getCardNameByCurrentId(qint32 standard) const {
    return cardName(currentCardID, standard);
}

bool Cards::isJoker() const {
    return isJoker(currentCardID);
}

qint32 Cards::getCurrentRank() const {
    return getRank(currentCardID);
}
//
//void Cards::onResized(QSize newFixedSize) {
//    if (size() != newFixedSize) {
//        setFixedSize(newFixedSize);
//    }
//}
