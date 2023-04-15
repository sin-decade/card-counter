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

#ifndef CARD_COUNTER_STRATEGYINFO_HPP
#define CARD_COUNTER_STRATEGYINFO_HPP

// Qt
#include <QDialog>

class Strategy;

class QSvgRenderer;

class QLabel;

class QSpinBox;

class QPushButton;

class QLineEdit;

class QTextEdit;

class QListWidget;

class KConfigGroup;

/**
 * @brief The StrategyInfo class provides a dialog for managing card-counting strategies.
 */
class StrategyInfo : public QDialog {
Q_OBJECT
public:
    /**
     * @brief Constructs a new StrategyInfo dialog with the given SVG renderer, parent widget, and window flags.
     *
     * @param renderer The SVG renderer to use for rendering card images.
     * @param parent The parent widget.
     * @param flags The window flags.
     */
    explicit StrategyInfo(QSvgRenderer *renderer, QWidget *parent = nullptr, Qt::WindowFlags flags = Qt::WindowFlags());

    /**
     * @brief Returns the strategy with the given ID.
     *
     * @param id The ID of the strategy to retrieve.
     * @return The strategy with the given ID, or nullptr if no such strategy exists.
     */
    Strategy *getStrategyById(qint32 id);

    /**
     * @brief Shows the strategy with the given name.
     *
     * @param name The name of the strategy to show.
     */
    void showStrategyByName(const QString &name);

    /**
     * @brief Returns a vector containing all strategies.
     *
     * @return A vector containing all strategies.
     */
    QVector<Strategy *> getStrategies();

signals:

    /**
     * @brief This signal is emitted when a new strategy is created.
     */
    void newStrategy();

private:
    QVector<Strategy *> items; ///< The strategies currently being displayed.
    QSvgRenderer *m_renderer; ///< The SVG renderer to use for rendering card images.
    qint32 _id; ///< The ID of the currently selected strategy.
    QLabel *_name; ///< The label displaying the name of the currently selected strategy.
    QLabel *_description; ///< The label displaying the description of the currently selected strategy.
    QLineEdit *_nameInput; ///< The input field for editing the name of the currently selected strategy.
    QTextEdit *_descriptionInput; ///< The input field for editing the description of the currently selected strategy.
    QPushButton *saveButton; ///< The button for saving changes to the currently selected strategy.
    QListWidget *listWidget; ///< The list of available strategies.
    QVector<QSpinBox *> weights; ///< The list of spin boxes for editing strategy weights.
    KConfigGroup *strategiesGroup; ///< The configuration group containing the list of strategies.

    /**
     * @brief Initializes the list of built-in strategies.
     */
    void initStrategies();

    /**
     * @brief Adds a fake strategy to the list of strategies.
     */
    void addFakeStrategy();

    /**
     * @brief Fills the list of available strategies with the current set of strategies.
     */
    void fillList();
};

#endif //CARD_COUNTER_STRATEGYINFO_HPP
