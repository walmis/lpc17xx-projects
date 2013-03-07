#pragma once

#include <usbapi.h>

#define MAX_PACKET_SIZE	64

#define LE_WORD(x)		((x)&0xFF),((x)>>8)

// CDC definitions
#define CS_INTERFACE			0x24
#define CS_ENDPOINT				0x25

#define INT_IN_EP		0x81
#define BULK_OUT_EP		0x05

#define BULK_IN_EP		0x82
#define BULK_IN_EP_CDC		0x88

static const U8 abDescriptors[] = {

// device descriptor
	0x12,
	DESC_DEVICE,
	LE_WORD(0x0200),			// bcdUSB
	0x00,						// bDeviceClass
	0x00,						// bDeviceSubClass
	0x00,						// bDeviceProtocol
	MAX_PACKET_SIZE0,			// bMaxPacketSize
	LE_WORD(0xFFFF),			// idVendor
	LE_WORD(0xB007),			// idProduct
	LE_WORD(0x0100),			// bcdDevice
	0x01,						// iManufacturer
	0x05,						// iProduct
	0x03,						// iSerialNumber
	0x01,						// bNumConfigurations

// configuration descriptor
	0x09,
	DESC_CONFIGURATION,
	LE_WORD(32),				// wTotalLength
	0x01,						// bNumInterfaces
	0x01,						// bConfigurationValue
	0x00,						// iConfiguration
	0xC0,						// bmAttributes
	0x32,						// bMaxPower

	0x09,
	DESC_INTERFACE,
	0x00,						// bInterfaceNumber
	0x00,						// bAlternateSetting
	0x02,						// bNumEndPoints
	0x00,						// bInterfaceClass = data
	0x00,						// bInterfaceSubClass
	0x00,						// bInterfaceProtocol
	0x04,						// iInterface
// data EP OUT
	0x07,
	DESC_ENDPOINT,
	BULK_OUT_EP,				// bEndpointAddress
	0x02,						// bmAttributes = bulk
	LE_WORD(MAX_PACKET_SIZE),	// wMaxPacketSize
	0x00,						// bInterval
// data EP in
	0x07,
	DESC_ENDPOINT,
	BULK_IN_EP,					// bEndpointAddress
	0x02,						// bmAttributes = bulk
	LE_WORD(MAX_PACKET_SIZE),	// wMaxPacketSize
	0x00,						// bInterval

	// string descriptors
	0x04,
	DESC_STRING,
	LE_WORD(0x0409),

	0x0E,
	DESC_STRING,
	'L', 0, 'P', 0, 'C', 0, 'U', 0, 'S', 0, 'B', 0,

	0x14,
	DESC_STRING,
	'U', 0, 'S', 0, 'B', 0, 'S', 0, 'e', 0, 'r', 0, 'i', 0, 'a', 0, 'l', 0,

	0x12,
	DESC_STRING,
	'1', 0, '0', 0, '1', 0, '5', 0, '8', 0, '0', 0, 'G', 0, 'E', 0,

	26+2,
	DESC_STRING,
	'D',0, 'M',0, 'X',0, ' ',0, 'I',0, 'n',0, 't',0, 'e',0, 'r',0, 'f',0, 'a',0, 'c',0, 'e',0,

	22,
	DESC_STRING,
	'B',0, 'o',0, 'o',0, 't',0, 'l',0, 'o',0, 'a',0, 'd',0, 'e',0, 'r',0,



// terminating zero
	0
};
//static const U8 abDescriptors[] = {
//
//// device descriptor
//	0x12,
//	DESC_DEVICE,
//	LE_WORD(0x0200),			// bcdUSB
//	0x00,						// bDeviceClass
//	0x00,						// bDeviceSubClass
//	0x00,						// bDeviceProtocol
//	MAX_PACKET_SIZE0,			// bMaxPacketSize
//	LE_WORD(0xFFFF),			// idVendor
//	LE_WORD(0x0005),			// idProduct
//	LE_WORD(0x0100),			// bcdDevice
//	0x01,						// iManufacturer
//	0x02,						// iProduct
//	0x03,						// iSerialNumber
//	0x01,						// bNumConfigurations
//
//		// configuration descriptor
//			0x09,
//			DESC_CONFIGURATION,
//			LE_WORD(32),				// wTotalLength
//			0x01,						// bNumInterfaces
//			0x01,						// bConfigurationValue
//			0x00,						// iConfiguration
//			0xC0,						// bmAttributes
//			0x32,						// bMaxPower
//
//
//			// data class interface descriptor
//					0x09,
//					DESC_INTERFACE,
//					0x00,						// bInterfaceNumber
//					0x00,						// bAlternateSetting
//					0x02,						// bNumEndPoints
//					0x0A,						// bInterfaceClass = data
//					0x00,						// bInterfaceSubClass
//					0x00,						// bInterfaceProtocol
//					0x00,						// iInterface
//						// data EP OUT
//							0x07,
//							DESC_ENDPOINT,
//							BULK_OUT_EP,				// bEndpointAddress
//							0x02,						// bmAttributes = bulk
//							LE_WORD(MAX_PACKET_SIZE),	// wMaxPacketSize
//							0x00,						// bInterval
//
//							// data EP in
//							0x07,
//							DESC_ENDPOINT,
//							BULK_IN_EP,					// bEndpointAddress
//							0x02,						// bmAttributes = bulk
//							LE_WORD(MAX_PACKET_SIZE),	// wMaxPacketSize
//							0x00,						// bInterval
//
//	// string descriptors
//	0x04,
//	DESC_STRING,
//	LE_WORD(0x0409),
//
//	0x0E,
//	DESC_STRING,
//	'L', 0, 'P', 0, 'C', 0, 'U', 0, 'S', 0, 'B', 0,
//
//	25,
//	DESC_STRING,
//	'D',0,'M',0,'X',0, ' ',0, 'I',0,'n',0,'t',0,'e',0,'r',0,'f',0,'a',0,'c',0,'e',
//
//	0x12,
//	DESC_STRING,
//	'D', 0, 'E', 0, 'A', 0, 'D', 0, 'C', 0, '0', 0, 'D', 0, 'E', 0,
//
//// terminating zero
//	0
//};
