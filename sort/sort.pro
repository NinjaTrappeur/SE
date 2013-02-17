TEMPLATE = app
TARGET = ../Sort
DEPENDPATH += . includes src ui
INCLUDEPATH += . includes

# Input
HEADERS += includes/SortInterface.h includes/SortEngine.h
FORMS += ui/Process.ui
SOURCES += src/SortInterface.cpp src/SortEngine.cpp src/main.cpp
