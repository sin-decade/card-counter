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

class Cards : public QWidget {
Q_OBJECT
public:
    explicit Cards(QSvgRenderer *renderer, QWidget *parent = nullptr);

    void paintEvent(QPaintEvent *event) override;

    void setId(qint32 id);

    void setName(QString name);

    enum Colour {
        Black = 0, Red
    };

    enum Suit {
        Clubs = 0, Diamonds, Hearts, Spades
    };

    enum Rank {
        Joker = 0, Ace, Two, Three, Four, Five, Six, Seven, Eight, Nine, Ten, Jack, Queen, King
    };

    static QList<qint32> shuffleCards(qint32 deckCount, qint32 shuffleCoefficient = 2);

    static QList<qint32> generateDeck(qint32 deckCount);

    static QString cardName(qint32 id, qint32 standard = 0);

    QString getCardNameByCurrentId(qint32 standard = 0) const;

    static bool isJoker(qint32 id);

    bool isJoker() const;

    static QString getColourName(qint32 colour);

    static QString getSuitName(qint32 suit);

    static QString getRankName(qint32 rank, bool standard = true);

    static qint32 getRank(qint32 id);

    qint32 getCurrentRank() const;

    static qint32 getSuit(qint32 id);

//public Q_SLOTS:
//    void onResized(QSize newFixedSize);

private:
    QString svgName;
    qint32 currentCardID;

    QSvgRenderer *m_renderer;
};


#endif //YACARDCOUNTER_CARDS_HPP
