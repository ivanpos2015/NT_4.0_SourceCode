/*
**
** Copyright (c) 1993, Microsoft Corporation, all rights reserved
**
** Module Name:
**
**   globals.cxx
**
** Abstract:
**
**    This module contains the global data for port configuration of
**    NT RAS Setup.
**
** Author:
**
**    RamC 8/18/93   Original
**
** Revision History:
**
**/

#include "precomp.hxx"

/* Global data
 */

// dlPortInfo is the list of ports currently configured.
// It is global because the main dialog and the sub dialogs
// access and modify it.

DLIST_OF(PORT_INFO) dlPortInfo = NULL;

// this list is used to store information about installed TAPI devices. Since
// the device type and name are tied to the port, we need this information
// stored per port.

DLIST_OF(PORT_INFO) dlTapiProvider = NULL;

// this list is used to store information about installed other devices like
// RasEther, SNARAS, etc. Since the device type and name are tied to the port,
// we need this information stored per port.

DLIST_OF(PORT_INFO) dlOtherMedia = NULL;

// this is the list of deleted ports

DLIST_OF(PORT_INFO) dlDeletedPorts = NULL;

// the dlDeviceInfo list is generated by reading MODEM.INF and PAD.INF files

DLIST_OF(DEVICE_INFO) dlDeviceInfo = NULL;

// dlAddPortList is a list of COM ports available for configuration on the
// local machine -
// it is read in from Registry at HARDWARE\DeviceMap\SerialComm

DLIST_OF(PORT_INFO) dlAddPortList = NULL;

// global list of currently installed ports on the system
DLIST_OF(PORT_INFO) dlInstalledPorts = NULL;

// strDetectModemList is a list of modems that were detected on a particular
// port

STRLIST strDetectModemList = NULL;

// strInstalledSerialPorts is a list of serial ports read in from HARDWARE\DeviceMap\SerialComm

STRLIST strInstalledSerialPorts;

// strAddPadPortList is a list of ports available to associate a serial port with
// FALSE flag indicates that we will free the memory allocated for elements of the list
STRLIST strAddPadPortList(FALSE);


// File name strings

CHAR   SerialIniPath[PATHLEN+1];
CHAR   ModemInfPath[PATHLEN+1];
CHAR   PadInfPath[PATHLEN+1];
TCHAR  WSerialIniPath[sizeof(TCHAR) * PATHLEN];
TCHAR  WSerialIniBakPath[sizeof(TCHAR) * PATHLEN];

// Client, Server or ClientAndServer
TCHAR  GInstalledOption[RAS_SETUP_SMALL_BUF_LEN];

// indicates if we are installing or configuring
BOOL   GfInstallMode = TRUE;

// this is true if a netcard is installed on the system, FALSE otherwise
BOOL   GfNetcardInstalled = FALSE;

// this is set to TRUE if we need to notify the user that the Network access
// has been changed because we discovered that there is no installed netcard.
BOOL   GfNetAccessChangeNotify = FALSE;

// force update of serial.ini or registry
BOOL   GfForceUpdate = FALSE;

// TRUE if device info has been read from modem.inf and pad.inf
BOOL   GfFillDevice = FALSE;

CHAR ReturnTextBuffer[RETURN_BUFFER_SIZE];

// Is the installation in Unattended mode?
BOOL GfGuiUnattended = FALSE;

// Enable/Disable Unimodem usage
BOOL GfEnableUnimodem = TRUE;              // Enable unimodem config by default

