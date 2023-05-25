TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

LIBS += -L"C:\Users\mccp3\Documents\GitHub\Labirynth/SFML-2.5.1/lib"
LIBS += -L"C:\Users\mccp3\Documents\GitHub\Labirynth/SFML-2.5.1/bin"

CONFIG(release, debug|release): LIBS += -lsfml-audio -lsfml-graphics -lsfml-system -lsfml-network -lsfml-window
CONFIG(debug  , debug|release): LIBS += -lsfml-audio -lsfml-graphics -lsfml-system -lsfml-network -lsfml-window

INCLUDEPATH += "C:\Users\mccp3\Documents\GitHub\Labirynth/SFML-2.5.1/include"
DEPENDPATH += "C:\Users\mccp3\Documents\GitHub\Labirynth/SFML-2.5.1/include"

SOURCES += \
        main.cpp
