TEMPLATE = subdirs

SUBDIRS += \
    FractionCalculator\
    Test_FractionCalculator

FractionCalculator.depends = Test_FractionCalculator

