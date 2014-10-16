#-------------------------------------------------
#
# Project created by QtCreator 2014-10-11T22:42:21
#
#-------------------------------------------------

QT       -= core

QT       -= gui

TARGET = ArithmeticEncoder
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

LIBS += -lboost_program_options

SOURCES += main.cpp \
    ar_encoder.cpp \
    ar_decoder.cpp \
    ar_ppm_model.cpp \
    ar_normal_model.cpp

HEADERS += \
    ar_model.h \
    ar_encoder.h \
    ar_decoder.h \
    ar_ppm_model.h \
    ar_normal_model.h
