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

#ifndef YACARDCOUNTER_CAROUSEL_HPP
#define YACARDCOUNTER_CAROUSEL_HPP

// Qt
#include <QWidget>

class QHBoxLayout;

/**
 * @brief The Carousel class is a widget for displaying a carousel of items.
 * The items are added with addWidget() and the aspect ratio of each item
 * is set with the constructor's `aspectRatio` parameter. The size of the carousel
 * widget is automatically adjusted to fit the aspect ratio and number of items.
 * When the size of the carousel changes, the items are automatically resized
 * to fit the new size.
 */
class Carousel : public QWidget {
Q_OBJECT
public:
    /**
     * @brief Constructs a carousel widget with the given `aspectRatio`.
     *
     * @param aspectRatio The aspect ratio of each item in the carousel.
     * @param parent The parent widget.
     */
    explicit Carousel(QSizeF aspectRatio, QWidget *parent = nullptr);

    /**
     * @brief Adds a widget to the carousel.
     *
     * @param widget The widget to add.
     */
    void addWidget(QWidget *widget);

protected:
    /**
     * @brief Handles resize events for the carousel widget.
     *
     * @param event The resize event.
     */
    void resizeEvent(QResizeEvent *event) override;

signals:

    /**
     * @brief Emitted when the size of an item in the carousel is changed.
     *
     * @param newFixedSize The new fixed size of the item.
     */
    void itemResized(QSize newFixedSize);

private:
    /**
     * @brief Updates the properties of the carousel widget based on its size.
     *
     * @param size The new size of the carousel widget.
     */
    void updateProps(QSize size);

    /**
     * @brief Updates the layout of the carousel widget based on its size and the aspect ratio of its items.
     *
     * @param newScale The new scale to apply to the items, or -1 to use the current scale.
     */
    void updateLayout(double newScale = -1);

    QHBoxLayout *carousel; ///< The layout for the carousel.
    QVector<QWidget *> widgets; ///< The list of widgets in the carousel.
    QSizeF ratio; ///< The aspect ratio of the items in the carousel.
    qint32 columnCount = 0; ///< The number of columns in the carousel.
    qint32 idx = 0; ///< The index of the current item.
};


#endif //YACARDCOUNTER_CAROUSEL_HPP
