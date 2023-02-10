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

#ifndef YACARDCOUNTER_MAINWINDOW_HPP
#define YACARDCOUNTER_MAINWINDOW_HPP

// KF
#include <KXmlGuiWindow>
// Qt
#include <QPointer>
#include <QLabel>
#include <QSet>

class KGameClock;

class KToggleAction;

class QGridLayout;

class QSvgRenderer;

class TableSlot;

class MainWindow : public KXmlGuiWindow {
Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

signals:

    void gamePaused(bool paused);

    void tableSlotResized(QSize newFixedSize);

private Q_SLOTS:

    void onScoreChanged(int count);

    void newGame();

    void onGameOver(bool);

    void advanceTime(const QString &);

    void showHighScores();

    void configureSettings();

    void pauseGame(bool paused);

    void loadSettings();

    void onTableSlotActivated();

    void onTableSlotFinished();

    void onTableSlotRemoved();

    void onTableSlotReshuffled();

    void onUserQuizzed();

    void onUserAnswered(bool correct);

    void onSwapTargetSelected();

    void pickUpCards();

protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    void setupActions();

    void addNewTableSlot();

    void setRenderer(QString cardTheme);


    KGameClock *m_gameClock = nullptr;
    KToggleAction *m_actionPause = nullptr;

    QPointer<QLabel> scoreLabel = new QLabel;
    QPointer<QLabel> timeLabel = new QLabel;

    QSvgRenderer *renderer;
    QGridLayout *layout;
    QWidget *table;

    QVector<int> swapTarget;
    QRectF bounds;
    QVector<TableSlot *> items;

    quint32 columnCount = -1;
    qreal scale = -1;

    bool launching;
    QTimer *countdown;

    QSet<quint32> jokers;

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
    void reorganizeTable(int newColumnCount, double newScale);

};


#endif //YACARDCOUNTER_MAINWINDOW_HPP
