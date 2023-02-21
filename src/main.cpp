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
#include <QApplication>
#include <QCommandLineParser>
// KF
#include <KAboutData>
#include <KLocalizedString>
// own
#include "mainwindow.hpp"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    KLocalizedString::setApplicationDomain("yacardcounter");

    KAboutData aboutData(
            QStringLiteral("yacardcounter"),
            i18n("YaCardCounter"),
            QStringLiteral("1.0"),
            i18n("A tool for improving arithmetic skills and memory retention "
                 "through counting cards in different table-slots with different strategies. "),
            KAboutLicense::GPL_V3,
            i18n("(c) 2023, Yaroslav Riabtsev"),
            QString(),
            QStringLiteral("https://github.com/YaCodeLab/YaCardCounter"),
            QStringLiteral("yaroslav.riabtsev@rwth-aachen.de"));

    aboutData.addAuthor(i18n("Yaroslav Riabtsev"), i18n("Original author"),
                        QStringLiteral("yaroslav.riabtsev@rwth-aachen.de"),
                        QStringLiteral("https://github.com/YaRiabtsev"),
                        QStringLiteral("yariabtsev"));

    KAboutData::setApplicationData(aboutData);

    QCommandLineParser parser;
    aboutData.setupCommandLine(&parser);
    parser.process(app);
    aboutData.processCommandLine(&parser);

    auto *window = new MainWindow();
    window->show();

    return QApplication::exec();
}
