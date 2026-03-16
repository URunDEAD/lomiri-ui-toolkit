TEMPLATE = subdirs

SUBDIRS += testparser

#######################################
# these tests are failing in armhf/qemu
# can not create QQuickView
# tested with Qt 5 beta 1 and beta 2
#
# https://bugs.launchpad.net/qemu-linaro/+bug/1084148
#######################################
#!contains(QMAKE_HOST.arch,armv7l) {
    SUBDIRS += components13 \
        components_benchmark
#}

SUBDIRS += \
    alarms \
    animator13 \
    argument \
    arguments \
    asyncloader13 \
    bottomedge13 \
    custom_qpa \
    deprecated_theme_engine13 \
#   i18n13 \ FIXME: breaks xenial
    iconprovider \
    inversemousearea13 \
    layouts13 \
    lomiri_shape11 \
    mainview11 \
    mainview13 \
    mainwindow13 \
    mousefilters13 \
    orientation11 \
    page11 \
    performance13 \
    qquick_image_extension13 \
    quickutils \
    recreateview11 \
    scaling_image_provider13 \
    serviceproperties11 \
    statesaver11 \
    subtheming13 \
    swipearea13 \
    test10 \
    theme11 \
    touchregistry \
    tree \
    units \
    visual13
