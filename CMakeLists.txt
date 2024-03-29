cmake_minimum_required(VERSION 3.18)

project(card-counter)

set(QT_MIN_VERSION "5.15.2")
set(KF5_MIN_VERSION "5.78.0")

find_package(ECM 1.0.0 REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings NO_POLICY_SCOPE)
include(FeatureSummary)

find_package(Qt5 ${QT_MIN_VERSION} CONFIG REQUIRED COMPONENTS
        Core    # QCommandLineParser, QStringLiteral
        Widgets # QApplication
        Svg
        )

find_package(KF5 ${KF5_MIN_VERSION} REQUIRED COMPONENTS
        CoreAddons      # KAboutData
        I18n            # KLocalizedString
        XmlGui          # KXmlGuiWindow, KActionCollection
        ConfigWidgets   # KStandardActions
        WidgetsAddons   # KMessageBox
        KIO             # KIO
        )

find_package(KF5KDEGames 7.3.1 REQUIRED)

feature_summary(WHAT ALL INCLUDE_QUIET_PACKAGES FATAL_ON_MISSING_REQUIRED_PACKAGES)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(card-counter_SRCS src/main.cpp src/mainwindow.cpp
        src/table/table.cpp src/table/tableslot.cpp
        src/strategy/strategyinfo.cpp src/strategy/strategy.cpp
        src/widgets/carousel.cpp src/widgets/cards.cpp
        src/widgets/base/label.cpp src/widgets/base/frame.cpp)

add_executable(card-counter ${card-counter_SRCS})

target_link_libraries(card-counter
        Qt5::Widgets
        Qt5::Svg
        KF5::CoreAddons
        KF5::I18n
        KF5::XmlGui
        KF5::ConfigWidgets
        KF5::WidgetsAddons
        KF5::KIOCore
        KF5KDEGames
        )

install(TARGETS card-counter ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})
install(FILES src/card-counterui.rc DESTINATION ${KDE_INSTALL_KXMLGUI5DIR}/card-counter)