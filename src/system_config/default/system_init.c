/*******************************************************************************
  System Initialization File

  File Name:
    system_init.c

  Summary:
    This file contains source code necessary to initialize the system.

  Description:
    This file contains source code necessary to initialize the system.  It
    implements the "SYS_Initialize" function, defines the configuration bits,
    and allocates any necessary global system resources, such as the
    sysObj structure that contains the object handles to all the MPLAB Harmony
    module objects in the system.
 *******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2013-2015 released Microchip Technology Inc.  All rights reserved.

Microchip licenses to you the right to use, modify, copy and distribute
Software only when embedded on a Microchip microcontroller or digital signal
controller that is integrated into your product or third party product
(pursuant to the sublicense terms in the accompanying license agreement).

You should refer to the license agreement accompanying this Software for
additional information regarding your rights and obligations.

SOFTWARE AND DOCUMENTATION ARE PROVIDED AS IS WITHOUT WARRANTY OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF
MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE.
IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER
CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR
OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR
CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF
SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
(INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.
 *******************************************************************************/
// DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include "system_config.h"
#include "system_definitions.h"


// ****************************************************************************
// ****************************************************************************
// Section: Configuration Bits
// ****************************************************************************
// ****************************************************************************
// <editor-fold defaultstate="collapsed" desc="Configuration Bits">

/*** DEVCFG0 ***/

#pragma config DEBUG =      ON
#pragma config JTAGEN =     OFF
#pragma config ICESEL =     ICS_PGx1
#pragma config PWP =        OFF
#pragma config BWP =        OFF
#pragma config CP =         OFF

/*** DEVCFG1 ***/

#pragma config FNOSC =      PRIPLL
#pragma config FSOSCEN =    ON
#pragma config IESO =       OFF
#pragma config POSCMOD =    XT
#pragma config OSCIOFNC =   OFF
#pragma config FPBDIV =     DIV_2
#pragma config FCKSM =      CSDCMD
#pragma config WDTPS =      PS1048576
#pragma config FWDTEN =     OFF
#pragma config WINDIS =     OFF
#pragma config FWDTWINSZ =  WINSZ_50
/*** DEVCFG2 ***/

#pragma config FPLLIDIV =   DIV_1
#pragma config FPLLMUL =    MUL_20
#pragma config FPLLODIV =   DIV_4
#pragma config UPLLIDIV =   DIV_1
#pragma config UPLLEN =     ON
/*** DEVCFG3 ***/

#pragma config USERID =     0xffff
#pragma config PMDL1WAY =   ON
#pragma config IOL1WAY =    ON
#pragma config FUSBIDIO =   ON
#pragma config FVBUSONIO =  ON
// </editor-fold>

// *****************************************************************************
// *****************************************************************************
// Section: Driver Initialization Data
// *****************************************************************************
// *****************************************************************************
// <editor-fold defaultstate="collapsed" desc="DRV_I2C Initialization Data">
// *****************************************************************************
/* I2C Driver Initialization Data
*/

const DRV_I2C_INIT drvI2C0InitData =
{
    .i2cId = DRV_I2C_PERIPHERAL_ID_IDX0,
    .i2cMode = DRV_I2C_OPERATION_MODE_IDX0,
    .portSCL = DRV_SCL_PORT_IDX0,
	.pinSCL  = DRV_SCL_PIN_POSITION_IDX0,
	.portSDA = DRV_SDA_PORT_IDX0,
	.pinSDA  = DRV_SDA_PIN_POSITION_IDX0,
    .baudRate = DRV_I2C_BAUD_RATE_IDX0,
    .busspeed = DRV_I2C_SLEW_RATE_CONTROL_IDX0,
    .buslevel = DRV_I2C_SMBus_SPECIFICATION_IDX0,
    .mstrInterruptSource = DRV_I2C_MASTER_INT_SRC_IDX0,
    .errInterruptSource = DRV_I2C_ERR_MX_INT_SRC_IDX0,
};




// </editor-fold>
// <editor-fold defaultstate="collapsed" desc="DRV_Timer Initialization Data">
// </editor-fold>
// <editor-fold defaultstate="collapsed" desc="DRV_USART Initialization Data">

const DRV_USART_INIT drvUsart0InitData =
{
    .moduleInit.value = DRV_USART_POWER_STATE_IDX0,
    .usartID = DRV_USART_PERIPHERAL_ID_IDX0, 
    .mode = DRV_USART_OPER_MODE_IDX0,
    .flags = DRV_USART_INIT_FLAGS_IDX0,
    .brgClock = DRV_USART_BRG_CLOCK_IDX0,
    .lineControl = DRV_USART_LINE_CNTRL_IDX0,
    .baud = DRV_USART_BAUD_RATE_IDX0,
    .handshake = DRV_USART_HANDSHAKE_MODE_IDX0,
    .interruptTransmit = DRV_USART_XMIT_INT_SRC_IDX0,
    .interruptReceive = DRV_USART_RCV_INT_SRC_IDX0,
    .interruptError = DRV_USART_ERR_INT_SRC_IDX0,
    .dmaChannelTransmit = DMA_CHANNEL_NONE,
    .dmaInterruptTransmit = DRV_USART_XMIT_INT_SRC_IDX0,    
    .dmaChannelReceive = DMA_CHANNEL_NONE,
    .dmaInterruptReceive = DRV_USART_RCV_INT_SRC_IDX0,    
};
// </editor-fold>
// <editor-fold defaultstate="collapsed" desc="DRV_USB Initialization Data">
/******************************************************
 * USB Driver Initialization
 ******************************************************/
/****************************************************
 * Endpoint Table needed by the Device Layer.
 ****************************************************/
uint8_t __attribute__((aligned(512))) endPointTable[DRV_USBFS_ENDPOINTS_NUMBER * 32];
const DRV_USBFS_INIT drvUSBInit =
{
    /* Assign the endpoint table */
    .endpointTable= endPointTable,

    /* Interrupt Source for USB module */
    .interruptSource = INT_SOURCE_USB_1,
    
    /* System module initialization */
    .moduleInit = {SYS_MODULE_POWER_SLEEP},
    
    .operationMode = DRV_USBFS_OPMODE_DEVICE,
    
    .operationSpeed = USB_SPEED_FULL,
    
    /* Stop in idle */
    .stopInIdle = false,

    /* Suspend in sleep */
    .suspendInSleep= true,

    /* Identifies peripheral (PLIB-level) ID */
    .usbID = USB_ID_1
};
// </editor-fold>

// *****************************************************************************
// *****************************************************************************
// Section: System Data
// *****************************************************************************
// *****************************************************************************

/* Structure to hold the object handles for the modules in the system. */
SYSTEM_OBJECTS sysObj;

// *****************************************************************************
// *****************************************************************************
// Section: Module Initialization Data
// *****************************************************************************
// *****************************************************************************
//<editor-fold defaultstate="collapsed" desc="SYS_DMA Initialization Data">
/*** System DMA Initialization Data ***/

const SYS_DMA_INIT sysDmaInit =
{
	.sidl = SYS_DMA_SIDL_DISABLE,

};
// </editor-fold>
// <editor-fold defaultstate="collapsed" desc="SYS_MSG Initialization Data">
/*** Message System Initialization Data ***/

static uint16_t queuePriorities0[2] = { 64, 32 };
SYS_MSG_INIT msg0Init =
{
    .nMaxMsgsDelivered = 1,
    .nMessagePriorities = 1,
};
// </editor-fold>

// *****************************************************************************
// *****************************************************************************
// Section: Library/Stack Initialization Data
// *****************************************************************************
// *****************************************************************************
// <editor-fold defaultstate="collapsed" desc="USB Stack Initialization Data">


/**************************************************
 * USB Device Function Driver Init Data
 **************************************************/
    const USB_DEVICE_CDC_INIT cdcInit0 =
    {
        .queueSizeRead = 1,
        .queueSizeWrite = 1,
        .queueSizeSerialStateNotification = 1
    };
/**************************************************
 * USB Device Layer Function Driver Registration 
 * Table
 **************************************************/
const USB_DEVICE_FUNCTION_REGISTRATION_TABLE funcRegistrationTable[1] =
{
    /* Function 1 */
    { 
        .configurationValue = 1,    /* Configuration value */ 
        .interfaceNumber = 0,       /* First interfaceNumber of this function */ 
        .speed = USB_SPEED_FULL,    /* Function Speed */ 
        .numberOfInterfaces = 2,    /* Number of interfaces */
        .funcDriverIndex = 0,  /* Index of CDC Function Driver */
        .driver = (void*)USB_DEVICE_CDC_FUNCTION_DRIVER,    /* USB CDC function data exposed to device layer */
        .funcDriverInit = (void*)&cdcInit0    /* Function driver init data */
    },
};

/*******************************************
 * USB Device Layer Descriptors
 *******************************************/
/*******************************************
 *  USB Device Descriptor 
 *******************************************/
const USB_DEVICE_DESCRIPTOR deviceDescriptor =
{
    0x12,                           // Size of this descriptor in bytes
    USB_DESCRIPTOR_DEVICE,          // DEVICE descriptor type
    0x0200,                         // USB Spec Release Number in BCD format
	USB_CDC_CLASS_CODE,         // Class Code
    USB_CDC_SUBCLASS_CODE,      // Subclass code
    0x00,                       // Protocol code
    USB_DEVICE_EP0_BUFFER_SIZE,     // Max packet size for EP0, see system_config.h
    0x04D8,                         // Vendor ID
    0x000A,                         // Product ID
    0x0100,                         // Device release number in BCD format
    0x01,                           // Manufacturer string index
    0x02,                           // Product string index
    0x00,                           // Device serial number string index
    0x01                            // Number of possible configurations
};


/*******************************************
 *  USB Full Speed Configuration Descriptor
 *******************************************/
const uint8_t fullSpeedConfigurationDescriptor[]=
{
    /* Configuration Descriptor */

    0x09,                                               // Size of this descriptor in bytes
    USB_DESCRIPTOR_CONFIGURATION,                       // Descriptor Type
    67,0,                //(67 Bytes)Size of the Config descriptor.e
    2,                                               // Number of interfaces in this cfg
    0x01,                                               // Index value of this configuration
    0x00,                                               // Configuration string index
    USB_ATTRIBUTE_DEFAULT | USB_ATTRIBUTE_SELF_POWERED, // Attributes
    50,                                                 // Max power consumption (2X mA)
    /* Descriptor for Function 1 - CDC     */ 
    
    /* Interface Descriptor */

    0x09,                                           // Size of this descriptor in bytes
    USB_DESCRIPTOR_INTERFACE,                       // Descriptor Type
    0,                                  // Interface Number
    0x00,                                           // Alternate Setting Number
    0x01,                                           // Number of endpoints in this interface
    USB_CDC_COMMUNICATIONS_INTERFACE_CLASS_CODE,    // Class code
    USB_CDC_SUBCLASS_ABSTRACT_CONTROL_MODEL,        // Subclass code
    USB_CDC_PROTOCOL_AT_V250,                       // Protocol code
    0x00,                                           // Interface string index

    /* CDC Class-Specific Descriptors */

    sizeof(USB_CDC_HEADER_FUNCTIONAL_DESCRIPTOR),               // Size of the descriptor
    USB_CDC_DESC_CS_INTERFACE,                                  // CS_INTERFACE
    USB_CDC_FUNCTIONAL_HEADER,                                  // Type of functional descriptor
    0x20,0x01,                                                  // CDC spec version

    sizeof(USB_CDC_ACM_FUNCTIONAL_DESCRIPTOR),                  // Size of the descriptor
    USB_CDC_DESC_CS_INTERFACE,                                  // CS_INTERFACE
    USB_CDC_FUNCTIONAL_ABSTRACT_CONTROL_MANAGEMENT,             // Type of functional descriptor
    USB_CDC_ACM_SUPPORT_LINE_CODING_LINE_STATE_AND_NOTIFICATION,// bmCapabilities of ACM

    sizeof(USB_CDC_UNION_FUNCTIONAL_DESCRIPTOR_HEADER) + 1,     // Size of the descriptor
    USB_CDC_DESC_CS_INTERFACE,                                  // CS_INTERFACE
    USB_CDC_FUNCTIONAL_UNION,                                   // Type of functional descriptor
    0,                                                       // com interface number
    1,

    sizeof(USB_CDC_CALL_MANAGEMENT_DESCRIPTOR),                 // Size of the descriptor
    USB_CDC_DESC_CS_INTERFACE,                                  // CS_INTERFACE
    USB_CDC_FUNCTIONAL_CALL_MANAGEMENT,                         // Type of functional descriptor
    0x00,                                                       // bmCapabilities of CallManagement
    1,                                                       // Data interface number

    /* Interrupt Endpoint (IN) Descriptor */

    0x07,                           // Size of this descriptor
    USB_DESCRIPTOR_ENDPOINT,        // Endpoint Descriptor
    1 | USB_EP_DIRECTION_IN,    // EndpointAddress ( EP1 IN INTERRUPT)
    USB_TRANSFER_TYPE_INTERRUPT,    // Attributes type of EP (INTERRUPT)
    0x10,0x00,                      // Max packet size of this EP
    0x02,                           // Interval (in ms)

    /* Interface Descriptor */

    0x09,                               // Size of this descriptor in bytes
    USB_DESCRIPTOR_INTERFACE,           // INTERFACE descriptor type
    1,      // Interface Number
    0x00,                               // Alternate Setting Number
    0x02,                               // Number of endpoints in this interface
    USB_CDC_DATA_INTERFACE_CLASS_CODE,  // Class code
    0x00,                               // Subclass code
    USB_CDC_PROTOCOL_NO_CLASS_SPECIFIC, // Protocol code
    0x00,                               // Interface string index

    /* Bulk Endpoint (OUT) Descriptor */

    0x07,                       // Size of this descriptor
    USB_DESCRIPTOR_ENDPOINT,    // Endpoint Descriptor
    2 | USB_EP_DIRECTION_OUT,   // EndpointAddress ( EP2 OUT )
	USB_TRANSFER_TYPE_BULK,     // Attributes type of EP (BULK)
    0x40,0x00,                  // Max packet size of this EP
    0x00,                       // Interval (in ms)

     /* Bulk Endpoint (IN)Descriptor */

    0x07,                       // Size of this descriptor
    USB_DESCRIPTOR_ENDPOINT,    // Endpoint Descriptor
    2 | USB_EP_DIRECTION_IN,    // EndpointAddress ( EP2 IN )
    0x02,                       // Attributes type of EP (BULK)
    0x40,0x00,                  // Max packet size of this EP
    0x00,                       // Interval (in ms)


};

/*******************************************
 * Array of Full speed config descriptors
 *******************************************/
USB_DEVICE_CONFIGURATION_DESCRIPTORS_TABLE fullSpeedConfigDescSet[1] =
{
    fullSpeedConfigurationDescriptor
};


/**************************************
 *  String descriptors.
 *************************************/

 /*******************************************
 *  Language code string descriptor
 *******************************************/
    const struct
    {
        uint8_t bLength;
        uint8_t bDscType;
        uint16_t string[1];
    }
    sd000 =
    {
        sizeof(sd000),          // Size of this descriptor in bytes
        USB_DESCRIPTOR_STRING,  // STRING descriptor type
        {0x0409}                // Language ID
    };
/*******************************************
 *  Manufacturer string descriptor
 *******************************************/
    const struct
    {
        uint8_t bLength;        // Size of this descriptor in bytes
        uint8_t bDscType;       // STRING descriptor type
        uint16_t string[25];    // String
    }
    sd001 =
    {
        sizeof(sd001),
        USB_DESCRIPTOR_STRING,
        {'M','i','c','r','o','c','h','i','p',' ','T','e','c','h','n','o','l','o','g','y',' ','I','n','c','.'}
		
    };

/*******************************************
 *  Product string descriptor
 *******************************************/
    const struct
    {
        uint8_t bLength;        // Size of this descriptor in bytes
        uint8_t bDscType;       // STRING descriptor type
        uint16_t string[22];    // String
    }
    sd002 =
    {
        sizeof(sd002),
        USB_DESCRIPTOR_STRING,
		{'S','i','m','p','l','e',' ','C','D','C',' ','D','e','v','i','c','e',' ','D','e','m','o'}
    }; 

/***************************************
 * Array of string descriptors
 ***************************************/
USB_DEVICE_STRING_DESCRIPTORS_TABLE stringDescriptors[3]=
{
    (const uint8_t *const)&sd000,
    (const uint8_t *const)&sd001,
    (const uint8_t *const)&sd002
};

/*******************************************
 * USB Device Layer Master Descriptor Table 
 *******************************************/
const USB_DEVICE_MASTER_DESCRIPTOR usbMasterDescriptor =
{
    &deviceDescriptor,          /* Full speed descriptor */
    1,                          /* Total number of full speed configurations available */
    fullSpeedConfigDescSet,     /* Pointer to array of full speed configurations descriptors*/
    NULL, 
    0, 
    NULL, 
    3,                          // Total number of string descriptors available.
    stringDescriptors,          // Pointer to array of string descriptors.
    NULL, 
    NULL
};


/****************************************************
 * USB Device Layer Initialization Data
 ****************************************************/
const USB_DEVICE_INIT usbDevInitData =
{
    /* System module initialization */
    .moduleInit = {SYS_MODULE_POWER_SLEEP},
    
    /* Number of function drivers registered to this instance of the
       USB device layer */
    .registeredFuncCount = 1,
    
    /* Function driver table registered to this instance of the USB device layer*/
    .registeredFunctions = (USB_DEVICE_FUNCTION_REGISTRATION_TABLE*)funcRegistrationTable,

    /* Pointer to USB Descriptor structure */
    .usbMasterDescriptor = (USB_DEVICE_MASTER_DESCRIPTOR*)&usbMasterDescriptor,

    /* USB Device Speed */
    .deviceSpeed = USB_SPEED_FULL,
    
    /* Index of the USB Driver to be used by this Device Layer Instance */
    .driverIndex = DRV_USBFS_INDEX_0,

    /* Pointer to the USB Driver Functions. */
    .usbDriverInterface = DRV_USBFS_DEVICE_INTERFACE,
    
};
// </editor-fold>

// *****************************************************************************
// *****************************************************************************
// Section: System Initialization
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    void SYS_Initialize ( void *data )

  Summary:
    Initializes the board, services, drivers, application and other modules.

  Remarks:
    See prototype in system/common/sys_module.h.
 */

void SYS_Initialize ( void* data )
{
    /* Core Processor Initialization */
    SYS_CLK_Initialize( NULL );
    SYS_DEVCON_Initialize(SYS_DEVCON_INDEX_0, (SYS_MODULE_INIT*)NULL);
    SYS_DEVCON_PerformanceConfig(SYS_CLK_SystemFrequencyGet());
    SYS_PORTS_Initialize();

    /* Initialize Drivers */
    sysObj.drvI2C0 = DRV_I2C_Initialize(DRV_I2C_INDEX_0, (SYS_MODULE_INIT *)&drvI2C0InitData);


    SYS_INT_VectorPrioritySet(INT_VECTOR_I2C1, INT_PRIORITY_LEVEL5);
    SYS_INT_VectorSubprioritySet(INT_VECTOR_I2C1, INT_SUBPRIORITY_LEVEL2);


    sysObj.sysDma = SYS_DMA_Initialize((SYS_MODULE_INIT *)&sysDmaInit);
    SYS_INT_VectorPrioritySet(INT_VECTOR_DMA0, INT_PRIORITY_LEVEL4);
    SYS_INT_VectorSubprioritySet(INT_VECTOR_DMA0, INT_SUBPRIORITY_LEVEL2);
    SYS_INT_VectorPrioritySet(INT_VECTOR_DMA2, INT_PRIORITY_LEVEL1);
    SYS_INT_VectorSubprioritySet(INT_VECTOR_DMA2, INT_SUBPRIORITY_LEVEL3);

    SYS_INT_SourceEnable(INT_SOURCE_DMA_0);
    SYS_INT_SourceEnable(INT_SOURCE_DMA_2);


    /* Initialize the OC Driver */
    DRV_OC0_Initialize();
    /*Initialize TMR0 */
    DRV_TMR0_Initialize();
    /*Initialize TMR1 */
    DRV_TMR1_Initialize();
    /*Initialize TMR2 */
    DRV_TMR2_Initialize();
 
     sysObj.drvUsart0 = DRV_USART_Initialize(DRV_USART_INDEX_0, (SYS_MODULE_INIT *)&drvUsart0InitData);
    SYS_INT_VectorPrioritySet(INT_VECTOR_UART1, INT_PRIORITY_LEVEL6);
    SYS_INT_VectorSubprioritySet(INT_VECTOR_UART1, INT_SUBPRIORITY_LEVEL1);
    /* RTCC System Service Initialization Call */
    sysObj.sysRtcc = SYS_RTCC_Initialize( );
    /* Initialize USB Driver */ 
    sysObj.drvUSBObject = DRV_USBFS_Initialize(DRV_USBFS_INDEX_0, (SYS_MODULE_INIT *) &drvUSBInit);

    /* Initialize System Services */

    /*** Interrupt Service Initialization Code ***/
    SYS_INT_Initialize();

    /*Setup the INT_SOURCE_EXTERNAL_0 and Enable it*/
    SYS_INT_VectorPrioritySet(INT_VECTOR_INT0, INT_PRIORITY_LEVEL1);
    SYS_INT_VectorSubprioritySet(INT_VECTOR_INT0, INT_SUBPRIORITY_LEVEL0);
    SYS_INT_ExternalInterruptTriggerSet(INT_EXTERNAL_INT_SOURCE0,INT_EDGE_TRIGGER_FALLING);
    SYS_INT_SourceEnable(INT_SOURCE_EXTERNAL_0);






    /*** Message Service Initialization Code ***/
    msg0Init.nQSizes = queuePriorities0;
    sysObj.sysMsg0 = SYS_MSG_Initialize(SYS_MSG_0, (SYS_OBJ_HANDLE)&msg0Init);

    /*** Random Service Initialization Code ***/
    SYS_RANDOM_Initialize(0, 0);

    /* Initialize Middleware */
    /* Set priority of USB interrupt source */
    SYS_INT_VectorPrioritySet(INT_VECTOR_USB1, INT_PRIORITY_LEVEL4);

    /* Set Sub-priority of USB interrupt source */
    SYS_INT_VectorSubprioritySet(INT_VECTOR_USB1, INT_SUBPRIORITY_LEVEL0);


    /* Initialize the USB device layer */
    sysObj.usbDevObject0 = USB_DEVICE_Initialize (USB_DEVICE_INDEX_0 , ( SYS_MODULE_INIT* ) & usbDevInitData);

    /* Initialize the Application */
    DOORBELL_Initialize();
    READ_UART_Initialize();
    T_SOUNDER_Initialize();
    WRITE_UART_Initialize();
}


/*******************************************************************************
 End of File
*/

