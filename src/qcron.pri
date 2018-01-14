QT += core
CONFIG += c++1

HEADERS += $$PWD/holiday.hpp \
           $$PWD/qcron.hpp \
           $$PWD/qcronfield.hpp \
           $$PWD/qcronnode.hpp
SOURCES += $$PWD/holiday.cpp \
           $$PWD/qcron.cpp \
           $$PWD/qcronfield.cpp \
           $$PWD/qcronnode.cpp

INCLUDEPATH += $$PWD

DISTFILES += $$PWD/../README.md \
             $$PWD/../LICENSE
