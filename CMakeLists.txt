cmake_minimum_required(VERSION 3.16)

project(BJGUI VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets LinguistTools Multimedia)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets LinguistTools Multimedia)

set(TS_FILES
    i18n/BJGUI_uk_UA.ts
    i18n/BJGUI_en_US.ts
)

set(PROJECT_SOURCES
        src/main.cpp
        src/presentation/widgets/MainWindow/mainwindow.cpp
        src/presentation/widgets/MainWindow/mainwindow.h
        src/presentation/widgets/MainWindow/mainwindow.ui

        src/presentation/CustomWidgets/DraggableQFrame/draggableframe.cpp
        src/presentation/CustomWidgets/DraggableQFrame/draggableframe.h
        src/presentation/CustomWidgets/SlidingStackedWidget/slidingstackedwidget.cpp
        src/presentation/CustomWidgets/SlidingStackedWidget/slidingstackedwidget.h
        ${TS_FILES}
)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/presentation/CustomWidgets)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/presentation/widgets)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(BJGUI
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        res.qrc
        src/presentation/widgets/BettingPanel/bettingpanel.h src/presentation/widgets/BettingPanel/bettingpanel.cpp src/presentation/widgets/BettingPanel/bettingpanel.ui
        src/presentation/widgets/ClickableChipStack/clickablechipstack.h src/presentation/widgets/ClickableChipStack/clickablechipstack.cpp
        src/presentation/Styles/styles.h
        src/presentation/Icons/icons.h
        src/Core/deck.h src/Core/deck.cpp
        src/Core/hand.h src/Core/hand.cpp

        src/Infrastructure/StateMachine/IGameState.h
        src/Infrastructure/StateMachine/StateMachine.h
        src/Infrastructure/StateMachine/States/introstate.h src/Infrastructure/StateMachine/States/introstate.cpp
        src/Infrastructure/StateMachine/States/bettingstate.h src/Infrastructure/StateMachine/States/bettingstate.cpp
        src/Infrastructure/Service/skinmanager.h src/Infrastructure/Service/skinmanager.cpp
        src/presentation/widgets/BetStackWidget/betstackwidget.h src/presentation/widgets/BetStackWidget/betstackwidget.cpp
        src/Core/carddata.h
        src/presentation/widgets/Card/cardwidget.h src/presentation/widgets/Card/cardwidget.cpp
        src/presentation/widgets/Hand/handwidget.h src/presentation/widgets/Hand/handwidget.cpp
        src/Infrastructure/Service/balancemanager.h src/Infrastructure/Service/balancemanager.cpp
        src/presentation/widgets/Settings/settingswindow.h src/presentation/widgets/Settings/settingswindow.cpp src/presentation/widgets/Settings/settingswindow.ui
        src/Infrastructure/Helpers/settingshelper.h src/Infrastructure/Helpers/settingshelper.cpp
        src/Infrastructure/Service/audiomanager.h src/Infrastructure/Service/audiomanager.cpp
        src/Core/gamesession.h src/Core/gamesession.cpp
        src/Core/blackjackgame.h src/Core/blackjackgame.cpp

        src/presentation/Outcomes/outcome.h src/presentation/Outcomes/outcome.cpp src/presentation/Outcomes/outcome.ui
        src/presentation/Dimmer/overlaywidget.h src/presentation/Dimmer/overlaywidget.cpp
        src/Infrastructure/Helpers/languagehelper.h
        src/Core/chipcalculator.h src/Core/chipcalculator.cpp

    )
    qt_add_translations(BJGUI
        TS_FILES ${TS_FILES}
        RESOURCE_PREFIX "/i18n"
        LUPDATE_OPTIONS "-no-obsolete"
    )
endif()

target_link_libraries(BJGUI PRIVATE
  Qt${QT_VERSION_MAJOR}::Widgets
  Qt${QT_VERSION_MAJOR}::Multimedia
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.BJGUI)
endif()
set_target_properties(BJGUI PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS BJGUI
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(BJGUI)
endif()
