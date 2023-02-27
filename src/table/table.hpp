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

#ifndef YACARDCOUNTER_TABLE_HPP
#define YACARDCOUNTER_TABLE_HPP

// Qt
#include <QWidget>
#include <QSet>
#include <KgDifficulty>

class QGridLayout;

class QSvgRenderer;

class TableSlot;

class StrategyInfo;

class Table : public QWidget {
Q_OBJECT
public:
    /**
     * @brief Constructs a new Table object.
     *
     * @param parent The parent widget of the table.
     */
    explicit Table(QWidget *parent = nullptr);

    /**
     * @brief Creates a new game on the table with the given difficulty level.
     *
     * @param level The difficulty level of the game.
     */
    void createNewGame(KgDifficultyLevel::StandardLevel level);

    /**
     * @brief Pauses or resumes the game on the table.
     *
     * @param paused A boolean indicating whether to pause or resume the game.
     */
    void pause(bool paused);

signals:

    /**
     * @brief Emitted when the game on the table is paused or resumed.
     *
     * @param paused A boolean indicating whether the game is now paused or resumed.
     */
    void gamePaused(bool paused);

    /**
     * @brief Emitted when a table slot is resized.
     *
     * @param newFixedSize The new fixed size of the table slot.
     */
    void tableSlotResized(QSize newFixedSize);

    /**
    * @brief CanRemove - Signal emitted when the minimum number of required TableSlots is reached or exceeded,
    * indicating whether it is now possible to remove some of the TableSlots.
    *
    * @param canRemove A boolean value indicating whether it is now possible to remove some of the TableSlots.
    */
    void canRemove(bool canRemove);

    /**
     * @brief Emitted when the score is updated.
     *
     * @param inc A boolean indicating whether the score is being incremented or decremented.
     */
    void scoreUpdate(bool inc);

    /**
     * @brief Emitted when the game is over.
     */
    void gameOver();

private Q_SLOTS:

    void onTableSlotActivated();

    void onTableSlotFinished();

    void onTableSlotRemoved();

    void onTableSlotReshuffled();

    void onUserQuizzed();

    /**
     * @brief onUserAnswered - Slot for handling a user's answer to a quiz question.
     * @param correct A boolean value indicating whether the answer was correct.
     */
    void onUserAnswered(bool correct);

    void onSwapTargetSelected();

    void pickUpCards();

    void onStrategyInfoAssist();

protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    /**
     * @brief addNewTableSlot - Adds a new TableSlot to the table.
     * @param isActive A boolean value indicating whether the new TableSlot should be active.
     */
    void addNewTableSlot(bool isActive = false);

    /**
     * @brief setRenderer - Sets the renderer used to render the table slots.
     * @param cardTheme The name of the card theme to use.
     */
    void setRenderer(const QString &cardTheme);

    /**
    * @brief The purpose of this function is to find the optimal number of columns for the table,
     * based on the given size of the table and aspect ratio of each item. This is done in order to
     * fill the maximum percent of the table and maintain the aspect ratio of the items. The function
     * balances the height and width of the items to ensure they do not become too tall or too wide.
    * @param tableSize The size of the area in which the grid of table slots should fit.
    * @param aspectRatio The aspect ratio (height to width) of the cards.
    * @param itemCount The number of table slots on the table.
    */
    void calculateNewColumnCount(const QSizeF &tableSize, const QSizeF &aspectRatio, int itemCount);

    /**
     * @brief Reorganizes the table by adjusting the number of columns and the scale of table slots (items).
     *
     * This function is used to rearrange the items in the grid layout so that they fill the maximum amount of
     * the parent widget's space. The new number of columns and the scale of the items are specified as arguments.
     * This ensures that the height and width of the items are properly balanced and optimized for display.
     *
     * @param newColumnCount The new number of columns to use in the grid layout.
     * @param newScale The new scale to use for the size of the table slots (items).
     */
    void reorganizeTable(qint32 newColumnCount, double newScale);

    QGridLayout *layout; ///< The grid layout used to organize the table slots.
    QSvgRenderer *renderer{}; ///< The SVG renderer used to draw the cards.
    QRectF bounds; ///< The bounding rectangle of the SVG image used to draw the cards.
    QTimer *countdown; ///< The timer used for the countdown feature.
    StrategyInfo *strategyInfo; ///< The strategy info dialog.

    bool launching{}; ///< A boolean indicating whether the game is launching.
    qint32 columnCount = -1; ///< The number of columns in the table grid.
    qint32 tableSlotCountLimit{}; ///< The maximum number of table slots allowed on the table.
    qreal scale = -1; ///< The scale of the table slots.

    QVector<int> swapTarget; ///< The current swap target.
    QVector<TableSlot *> items; ///< The list of table slots on the table.
    QSet<qint32> jokers; ///< The set of jokers on the table.
    QSet<qint32> available; ///< The set of available table slots.

};


#endif //YACARDCOUNTER_TABLE_HPP
