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

#ifndef CARD_COUNTER_LABEL_HPP
#define CARD_COUNTER_LABEL_HPP

#include <QLabel>

/**
 * @brief Custom QLabel with default Card-Counter style
 */
class CCLabel : public QLabel {
Q_OBJECT

public:
    /**
     * @brief Constructor
     * @param text The text of the label
     * @param parent The parent widget
     */
    explicit CCLabel(const QString &text, QWidget *parent = nullptr);
};

#endif //CARD_COUNTER_LABEL_HPP

