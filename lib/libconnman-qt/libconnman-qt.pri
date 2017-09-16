INCLUDEPATH += $$PWD

DBUS_INTERFACES = \
    $$PWD/connman_clock.xml \
    $$PWD/connman_session.xml \
    $$PWD/connman_technology.xml \

PUBLIC_HEADERS += \
    $$PWD/networkmanager.h \
    $$PWD/networktechnology.h \
    $$PWD/networkservice.h \
    $$PWD/commondbustypes.h \
    $$PWD/connmannetworkproxyfactory.h \
    $$PWD/clockmodel.h \
    $$PWD/useragent.h \
    $$PWD/sessionagent.h \
    $$PWD/networksession.h \
    $$PWD/counter.h

HEADERS += \
    $$PUBLIC_HEADERS \
    $$PWD/libconnman_p.h \

SOURCES += \
    $$PWD/networkmanager.cpp \
    $$PWD/networktechnology.cpp \
    $$PWD/networkservice.cpp \
    $$PWD/clockmodel.cpp \
    $$PWD/commondbustypes.cpp \
    $$PWD/connmannetworkproxyfactory.cpp \
    $$PWD/useragent.cpp \
    $$PWD/sessionagent.cpp \
    $$PWD/networksession.cpp \
    $$PWD/counter.cpp

OTHER_FILES = $$PWD/connman_service.xml \
    $$PWD/connman_technology.xml \
    $$PWD/connman_clock.xml \
    $$PWD/connman_session.xml \
    $$PWD/connman_notification.xml
