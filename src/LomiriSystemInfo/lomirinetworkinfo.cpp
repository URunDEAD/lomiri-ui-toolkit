/****************************************************************************
**
** Copyright (C) 2018 The Qt Company Ltd and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the QtSystems module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl-3.0.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 or (at your option) the GNU General
** Public license version 3 or any later version approved by the KDE Free
** Qt Foundation. The licenses are as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL2 and LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-2.0.html and
** https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "lomirinetworkinfo.h"

#include "linux/lomirinetworkinfo_linux_p.h"

#include <QtCore/qmetaobject.h>

QT_BEGIN_NAMESPACE

/*!
    \class LomiriNetworkInfo
    \inmodule QtSystemInfo
    \brief The LomiriNetworkInfo class provides various information about the network status.

    \ingroup systeminfo

    To support the cases where one has multiple interfaces / modems for the same network mode, you
    can specify which interface you refer to. For those cases, the 'interface' parameter is the index
    of the interface, starting from 0.
*/

/*!
    \enum LomiriNetworkInfo::CellDataTechnology
    This enum describes the type of cellular technology.

    \value UnknownDataTechnology   The cellular technology is unknown or an error occured.
    \value GprsDataTechnology      General Packet Radio Service (GPRS) data service.
    \value EdgeDataTechnology      Enhanced Data Rates for GSM Evolution (EDGE) data service.
    \value UmtsDataTechnology      Universal Mobile Telecommunications System (UMTS) data service.
    \value HspaDataTechnology      High Speed Packet Access (HSPA) data service.
*/

/*!
    \enum LomiriNetworkInfo::NetworkMode
    This enumeration describes the type of the network.

    \value UnknownMode     The network is unknown or an error occured.
    \value GsmMode         Global System for Mobile (GSM) network.
    \value CdmaMode        Code Division Multiple Access (CDMA) network.
    \value WcdmaMode       Wideband Code Division Multiple Access (WCDMA) network.
    \value WlanMode        Wireless local area network (WLAN) network.
    \value EthernetMode    Local area network (LAN), or Ethernet network.
    \value BluetoothMode   Bluetooth network.
    \value WimaxMode       Worldwide Interoperability for Microwave Access (WiMAX) network.
    \value LteMode         3GPP Long Term Evolution (LTE) network.
    \value TdscdmaMode     Time Division Synchronous Code Division Multiple Access (TD-SCDMA) network.
*/

/*!
    \enum LomiriNetworkInfo::NetworkStatus
    This enumeration describes the status of the network.

    \value UnknownStatus        The status is unknown or an error occured.
    \value NoNetworkAvailable   There is no network available.
    \value EmergencyOnly        The network only allows emergency calls.
    \value Searching            The device is searching or connecting to the network.
    \value Busy                 The network is too busy to be connected.
    \value Denied               The connection to the network has been denied.
    \value HomeNetwork          The device is connected to the home network.
    \value Roaming              The device is connected to some roaming network.
*/

/*!
    \fn void LomiriNetworkInfo::cellIdChanged(int interface, const QString &id)

    This signal is Q_EMITted whenever the cell ID for \a interface has changed to \a id.
*/

/*!
    \fn void LomiriNetworkInfo::currentCellDataTechnologyChanged(int interface, LomiriNetworkInfo::CellDataTechnology tech)

    This signal is Q_EMITted whenever the current cell data technology for \a interface has changed to \a tech.
*/

/*!
    \fn void LomiriNetworkInfo::currentMobileCountryCodeChanged(int interface, const QString &mcc)

    This signal is Q_EMITted whenever the current Mobile Country Code (MCC) for \a interface has changed
    to \a mcc.
*/

/*!
    \fn void LomiriNetworkInfo::currentMobileNetworkCodeChanged(int interface, const QString &mnc)

    This signal is Q_EMITted whenever the current Mobile Network Code (MNC) for \a interface has changed
    to \a mnc.
*/

/*!
    \fn void LomiriNetworkInfo::currentNetworkModeChanged(LomiriNetworkInfo::NetworkMode mode)

    This signal is Q_EMITted whenever the current network has changed to \a mode.
*/

/*!
    \fn void LomiriNetworkInfo::locationAreaCodeChanged(int interface, const QString &lac)

    This signal is Q_EMITted whenever the location area code for \a interface has changed to \a lac.
*/

/*!
    \fn void LomiriNetworkInfo::networkInterfaceCountChanged(LomiriNetworkInfo::NetworkMode mode, int count)

    This signal is Q_EMITted whenever the number of interfaces for the \a mode has changed to \a count.
*/

/*!
    \fn void LomiriNetworkInfo::networkNameChanged(LomiriNetworkInfo::NetworkMode mode, int interface, const QString &name)

    This signal is Q_EMITted whenever the name for the \a interface of \a mode has changed to \a name.
*/

/*!
    \fn void LomiriNetworkInfo::networkSignalStrengthChanged(LomiriNetworkInfo::NetworkMode mode, int interface, int strength)

    This signal is Q_EMITted whenever the signal strength for the \a interface of \a mode has changed
    to \a strength.
*/

/*!
    \fn void LomiriNetworkInfo::networkStatusChanged(LomiriNetworkInfo::NetworkMode mode, int interface, LomiriNetworkInfo::NetworkStatus status)

    This signal is Q_EMITted whenever the status for the \a interface of \a mode has changed to \a status.
*/

/*!
    Constructs a LomiriNetworkInfo object with the given \a parent.
*/
LomiriNetworkInfo::LomiriNetworkInfo(QObject *parent)
    : QObject(parent)
    , d_ptr(new LomiriNetworkInfoPrivate(this))
{
}

/*!
    Destroys the object
*/
LomiriNetworkInfo::~LomiriNetworkInfo()
{
    delete d_ptr;
}

/*!
    Returns the number of interfaces for the \a mode. If the information is not available, or error
    occurs, -1 is returned.
*/
int LomiriNetworkInfo::networkInterfaceCount(LomiriNetworkInfo::NetworkMode mode) const
{
    return d_ptr->networkInterfaceCount(mode);
}

/*!
    Returns the signal strength for \a interfaceDevice of \a mode, in 0 - 100 scale. If the information
    is not available, or error occurs, -1 is returned.
*/
int LomiriNetworkInfo::networkSignalStrength(LomiriNetworkInfo::NetworkMode mode, int interfaceDevice) const
{
    return d_ptr->networkSignalStrength(mode, interfaceDevice);
}

/*!
    Returns the current cell data technology used for \a interfaceDevice.
*/
LomiriNetworkInfo::CellDataTechnology LomiriNetworkInfo::currentCellDataTechnology(int interfaceDevice) const
{
    return d_ptr->currentCellDataTechnology(interfaceDevice);
}

/*!
    Returns the current active network mode. If there are more than one modes activated, the preferred
    one is returned.
*/
LomiriNetworkInfo::NetworkMode LomiriNetworkInfo::currentNetworkMode() const
{
    return d_ptr->currentNetworkMode();
}

/*!
    Returns the current status for \a interfaceDevice of \a mode.
*/
LomiriNetworkInfo::NetworkStatus LomiriNetworkInfo::networkStatus(LomiriNetworkInfo::NetworkMode mode, int interfaceDevice) const
{
    return d_ptr->networkStatus(mode, interfaceDevice);
}

#ifndef QT_NO_NETWORKINTERFACE
/*!
    Returns the first found interface for \a interfaceDevice of \a mode. If none is found, or it can't be
    represented by QNetworkInterface (e.g. Bluetooth), and empty object is returned.
*/
QNetworkInterface LomiriNetworkInfo::interfaceForMode(LomiriNetworkInfo::NetworkMode mode, int interfaceDevice) const
{
    return d_ptr->interfaceForMode(mode, interfaceDevice);
}
#endif // QT_NO_NETWORKINTERFACE

/*!
    Returns the cell ID of the connected tower or based station for \a interfaceDevice. If this information
    is not available or error occurs, an empty string is returned.
*/
QString LomiriNetworkInfo::cellId(int interfaceDevice) const
{
    return d_ptr->cellId(interfaceDevice);
}

/*!
    Returns the current Mobile Country Code (MCC) for \a interfaceDevice. An empty string is returned if the
    information is not available or an error occurs.
*/
QString LomiriNetworkInfo::currentMobileCountryCode(int interfaceDevice) const
{
    return d_ptr->currentMobileCountryCode(interfaceDevice);
}

/*!
    Returns the current Mobile Network Code (MNC) for \a interfaceDevice. An empty string is returned if the
    information is not available or an error occurs.
*/
QString LomiriNetworkInfo::currentMobileNetworkCode(int interfaceDevice) const
{
    return d_ptr->currentMobileNetworkCode(interfaceDevice);
}

/*!
    Returns the home Mobile Country Code (MCC) for \a interfaceDevice. An empty string is returned if the
    information is not available or an error occurs.
*/
QString LomiriNetworkInfo::homeMobileCountryCode(int interfaceDevice) const
{
    return d_ptr->homeMobileCountryCode(interfaceDevice);
}

/*!
    Returns the home Mobile Network Code (MNC) for \a interfaceDevice. An empty string is returned if the
    information is not available or an error occurs.
*/
QString LomiriNetworkInfo::homeMobileNetworkCode(int interfaceDevice) const
{
    return d_ptr->homeMobileNetworkCode(interfaceDevice);
}

/*!
    Returns the International Mobile Subscriber Identity (IMSI) for \a interfaceDevice. If this information is
    not available, or error occurs, an empty string is returned.
*/
QString LomiriNetworkInfo::imsi(int interfaceDevice) const
{
    return d_ptr->imsi(interfaceDevice);
}

/*!
    Returns the location area code of the current cellular radio network for \a interfaceDevice. If this information
    is not available or error occurs, an empty string is returned.
*/
QString LomiriNetworkInfo::locationAreaCode(int interfaceDevice) const
{
    return d_ptr->locationAreaCode(interfaceDevice);
}

/*!
    Returns the MAC address for \a interfaceDevice of \a mode. If the MAC address is not available or error
    occurs, an empty string is returned.
*/
QString LomiriNetworkInfo::macAddress(LomiriNetworkInfo::NetworkMode mode, int interfaceDevice) const
{
    return d_ptr->macAddress(mode, interfaceDevice);
}

/*!
    Returns the name of the operator for \a interfaceDevice of \a mode. If the information is not available,
    or an error occurs, an empty string is returned.

    In case of WLAN, the SSID is returned; for Ethernet, the domain name is returned if available.
*/
QString LomiriNetworkInfo::networkName(LomiriNetworkInfo::NetworkMode mode, int interfaceDevice) const
{
    return d_ptr->networkName(mode, interfaceDevice);
}

extern QMetaMethod proxyToSourceSignal(const QMetaMethod &, QObject *);

/*!
    \internal
*/
void LomiriNetworkInfo::connectNotify(const QMetaMethod &signal)
{
#if defined(Q_OS_LINUX) || defined(Q_OS_WIN) || defined(Q_OS_MAC)
    QMetaMethod sourceSignal = proxyToSourceSignal(signal, d_ptr);
    connect(d_ptr, sourceSignal, this, signal, Qt::UniqueConnection);
#else
    Q_UNUSED(signal)
#endif
}

/*!
    \internal
*/
void LomiriNetworkInfo::disconnectNotify(const QMetaMethod &signal)
{
#if defined(Q_OS_LINUX) || defined(Q_OS_WIN)  || defined(Q_OS_MAC)
    // We can only disconnect with the private implementation, when there is no receivers for the signal.
    if (isSignalConnected(signal))
        return;

    QMetaMethod sourceSignal = proxyToSourceSignal(signal, d_ptr);
    disconnect(d_ptr, sourceSignal, this, signal);
#else
    Q_UNUSED(signal)
#endif
}

QT_END_NAMESPACE
