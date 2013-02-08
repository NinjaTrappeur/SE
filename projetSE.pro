TEMPLATE = app
TARGET = MultiProcessSort
DEPENDPATH += . includes src ui
INCLUDEPATH += . includes

# Input
HEADERS += includes/MainWindow.h includes/Random.h
FORMS += ui/MainWindow.ui ui/Process.ui
SOURCES += src/main.cpp src/MainWindow.cpp src/Random.cpp
