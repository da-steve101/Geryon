//*****************************************************************************
//
// usb_device.h - Public function prototypes and globals related to USB device
//                operation in the Quickstart Oscilloscope application.
//
// Copyright (c) 2008-2012 Texas Instruments Incorporated.  All rights reserved.
// Software License Agreement
// 
// Texas Instruments (TI) is supplying this software for use solely and
// exclusively on TI's microcontroller products. The software is owned by
// TI and/or its suppliers, and is protected under applicable copyright
// laws. You may not combine this software with "viral" open-source
// software in order to form a larger program.
// 
// THIS SOFTWARE IS PROVIDED "AS IS" AND WITH ALL FAULTS.
// NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT
// NOT LIMITED TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. TI SHALL NOT, UNDER ANY
// CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR CONSEQUENTIAL
// DAMAGES, FOR ANY REASON WHATSOEVER.
// 
// This is part of revision 9107 of the EK-LM3S3748 Firmware Package.
//
//*****************************************************************************

#ifndef __USB_DEVICE_H__
#define __USB_DEVICE_H__

extern tBoolean ScopeUSBDeviceInit(void);
extern void ScopeUSBDeviceTerm(void);
extern tBoolean ScopeUSBDeviceSendData(tDataAcqCaptureStatus *pCapInfo,
                                       tBoolean bAuto);
extern tBoolean ScopeUSBDeviceSendPacket(unsigned char ucPacketType,
                                         unsigned char ucParam,
                                         unsigned long ulParam);
extern void ScopeUSBDeviceConfigureEndpoints(void);

#endif // __USB_DEVICE_H__
