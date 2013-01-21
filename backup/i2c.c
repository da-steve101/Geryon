/***********************************************************
 \author Stephen Tridgell
 \date   01.09.2012
 \file   i2c.c

This file implements a I2C Single Master interface for the LM3S3748
Some of this code is from http://e2e.ti.com/support/microcontrollers/stellaris_arm_cortex-m3_microcontroller/f/473/t/55810.aspx
It has been converted to this board, and reformated but was most helpful
This Program Implements an I2C Single Master interface using pin 34 and 35 of
the LM3S3478. Pin 34 is the PA6 pin and the SCL of the I2C. Pin 35 is the PA7 pin and the SDA of the I2C
See page 603/604 of datasheet
***********************************************************/

/**********************************************************
Includes
*********************************************************/
#include "driverlib/pin_map.h"
#include "inc/hw_sysctl.h"
#include "inc/hw_i2c.h"
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_adc.h"// send/recieve functions
#include "driverlib/rom.h"
#include "driverlib/timer.h"
#include "data-acq.h"

#include "driverlib/debug.h"
#include "driverlib/i2c.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"
#include "i2c.h"

//#define TESTING 1

#ifdef TESTING
// Interupt and error handlers, leave blank for now
void
SysTickIntHandler(void){
  while(1){
  }
}

void 
__error__(char *pcFilename, unsigned long ulLine){
  while(1){
  }
}
#endif

void
I2C1IntHandler(void){
  //  I2CMasterIntClear(I2C1_MASTER_BASE);
  while(1){
  }

}

/********************************************************
This method initializes the GPIO with the desired settings
for this application
********************************************************/
void I2C_init( tBoolean FastSpeed){

  /***************************************************
  Enable the I2C Master
   **************************************************/
  I2CMasterEnable(I2C1_MASTER_BASE);

  /**********************************************************************
  Enable Interrupts
   *********************************************************************/
  //  I2CMasterIntEnable(I2C1_MASTER_BASE);
  //  IntEnable(INT_I2C1);

  /*******************************************************************
  Configure the Pins as I2C
  *******************************************************************/
  GPIOPinTypeI2CSCL(GPIO_PORTA_BASE, GPIO_PIN_6);
  GPIOPinTypeI2C(GPIO_PORTA_BASE, GPIO_PIN_7);

  /*********************************************************
  Configure PA6 = SCL and PA7 = SDA for I2C: driverlib/gpio.c
  ********************************************************/

  /**********************************************************************
  Initialize Master I2C control with 100kbps or 400kbps: driverlib/i2c.c
  **********************************************************************/
  I2CMasterInitExpClk(I2C1_MASTER_BASE, SysCtlClockGet(), FastSpeed);
}


/************************************************************************************
This function is used to send burst data to the slave device: process on page 599 of datasheet
************************************************************************************/
void I2C_Master_Send(unsigned char slaveAddress, unsigned char * data, unsigned int length){

  /************************************************************************************
Check for errors
  ************************************************************************************/
  unsigned int i = 0;
  if( length < 1)
    return;

  if( data == NULL )
    return;
  int count = 0;
  while( I2CMasterBusBusy(I2C1_MASTER_BASE) || I2CMasterBusy(I2C1_MASTER_BASE) ){
    if(count > 10000)
	return;
    else
	count++;
  } // Ensure Nothing is Sending

  /**********************************************************************************
  Set the slave address and false for write: driverlib/i2c.c
  *********************************************************************************/
  I2CMasterSlaveAddrSet(I2C1_MASTER_BASE, slaveAddress, false);

  /*************************************************************************************
  Put the first byte of data in the Master Data Register and Move to Next Byte
  ****************************************************************************************/
  I2CMasterDataPut(I2C1_MASTER_BASE, data[i]);
  i++; 

  /*****************************************************************************************
  Set the Master Control Register To Start
  ***************************************************************************************/
  if( i != length )
    I2CMasterControl(I2C1_MASTER_BASE, I2C_MASTER_CMD_BURST_SEND_START);
  else
    I2CMasterControl(I2C1_MASTER_BASE, I2C_MASTER_CMD_SINGLE_SEND);

  /***************************************************************************************
  Wait until finished sending
  ***************************************************************************************/
  while(I2CMasterBusy(I2C1_MASTER_BASE) ){
    if(count > 10000)
	return;
    else
	count++;
  } // STUCK HERE: possibly the 

  /********************************************************************************************
  Start Loop Through the Data: Get each Byte, send it and Send the Continue Signal, Check for errors
  then wait until finished sending
  **********************************************************************************************/
  for( ; i < (length - 1); i++ ){

    I2CMasterDataPut(I2C1_MASTER_BASE, data[i]); // Get Each Byte and Send It

    I2CMasterControl(I2C1_MASTER_BASE, I2C_MASTER_CMD_BURST_SEND_CONT); // Send Continue Signal

    if( I2CMasterErr(I2C1_MASTER_BASE) != I2C_MASTER_ERR_NONE ){      // Check for Errors
      I2CMasterControl(I2C1_MASTER_BASE, I2C_MASTER_CMD_BURST_SEND_ERROR_STOP);
      return;
    }

    while( I2CMasterBusy(I2C1_MASTER_BASE) ){
    if(count > 10000)
	return;
    else
	count++;
    } // Ensure Master is Ready
  }

  /**********************************************************************************************
For the Last Byte Get the data and send it then send the finish signal
  **************************************************************************************************/
  if( i != length ){
    I2CMasterDataPut(I2C1_MASTER_BASE, data[i]); // Get Byte and Send It
    I2CMasterControl(I2C1_MASTER_BASE, I2C_MASTER_CMD_BURST_SEND_FINISH); // Send Finish Signal
  }
  
  while( I2CMasterBusBusy(I2C1_MASTER_BASE) || I2CMasterBusy(I2C1_MASTER_BASE) ){
    if(count > 10000)
	return;
    else
	count++;
  } // Ensure Stopped
}


/**********************************************************************************************
This function is used to recieve burst data from the slave device: process on page 600 of datasheet
**********************************************************************************************/
unsigned char * I2C_Master_Read(unsigned char slaveAddress, unsigned char * readData, unsigned int length){

  /********************************************************************************************************
  Check For Errors
  **************************************************************************************************************/
  unsigned int i = 0;
  if( readData == NULL )
    return NULL;

  if( length < 1 )
    return readData;

  while( I2CMasterBusBusy(I2C1_MASTER_BASE) || I2CMasterBusy(I2C1_MASTER_BASE) ){} // Ensure Nothing is Sending


  /**********************************************************************************
  Set the slave address and true for read: driverlib/i2c.c
  *********************************************************************************/
  I2CMasterSlaveAddrSet(I2C1_MASTER_BASE, slaveAddress, true);

  /*********************************************************************************************
  Set Master To Receive Data
  ****************************************************************************************************/
  while( I2CMasterBusy(I2C1_MASTER_BASE) ){} // Ensure Master Not Busy
  if( length == 1 )
    I2CMasterControl(I2C1_MASTER_BASE, I2C_MASTER_CMD_SINGLE_RECEIVE);
  else
    I2CMasterControl(I2C1_MASTER_BASE, I2C_MASTER_CMD_BURST_RECEIVE_START);

  /*******************************************************************************************************************
  Read Data From the I2C
  *********************************************************************************************************************/
  while( I2CMasterBusy(I2C1_MASTER_BASE) ){} // Ensure Nothing is Sending
  readData[i] = I2CMasterDataGet(I2C1_MASTER_BASE);
  i++;

  /**************************************************************************************************************
  Send the Continue Signal, Check For errors and wait to continue, then read
  ********************************************************************************************************************/
  for( ; i < (length - 1); i++){
    I2CMasterControl(I2C1_MASTER_BASE, I2C_MASTER_CMD_BURST_RECEIVE_CONT);

    if( I2CMasterErr(I2C1_MASTER_BASE) != I2C_MASTER_ERR_NONE ){      // Check for Errors
      I2CMasterControl(I2C1_MASTER_BASE, I2C_MASTER_CMD_BURST_RECEIVE_ERROR_STOP);
      return readData;
    }

    while( I2CMasterBusy(I2C1_MASTER_BASE) ){} // Ensure Master Not Busy
    readData[i] = I2CMasterDataGet(I2C1_MASTER_BASE);
  }

  if( i < length ){
    I2CMasterControl(I2C1_MASTER_BASE, I2C_MASTER_CMD_BURST_RECEIVE_FINISH);
    while( I2CMasterBusy(I2C1_MASTER_BASE) ){} // Ensure Master Not Busy
    readData[i] = I2CMasterDataGet(I2C1_MASTER_BASE);
  }
  while( I2CMasterBusBusy(I2C1_MASTER_BASE) || I2CMasterBusy(I2C1_MASTER_BASE) ){} // Ensure Stopped

  return readData;
}

void runAlternatingIOPinTest(){
  unsigned char configdata[] = { 0x03, 0x00 };
  unsigned char data0[] = { 0x01, 0x00, 0x00 };
  unsigned char data1[] = { 0x01, 0xFF, 0xFF };
  unsigned int length = 2;
  unsigned char readData[length];

  I2C_Master_Send( DIGITAL_IO_ADDRESS, configdata, 2);

  // DIGITAL_IO Test write
  while(1){
    I2C_Master_Send( DIGITAL_IO_ADDRESS, data0, length);
    I2C_Master_Send( DIGITAL_IO_ADDRESS, data1, length);
  }
  // DIGITAL_IO Test Read
  while(1){
    I2C_Master_Read( DIGITAL_IO_ADDRESS, readData, length);
  }
}


/**
Command = [0,0,0,0,0,0,B,B] : BB = 00 for input, 01 for output
Data = [0,Gain2B,Gain2A,ABsel,Gain1B,Gain1A,AC2,AC1]
AC1/2 = select inputs to Oscilloscope (P101 = AC1 / P104 = AC2)
Gain1: BA = 00 for unity, bigger number more attenuation
Gain2: A = X, B=Y, ABsel = Z 
*/
int OscControl(unsigned char command, unsigned char data){
  unsigned char dataOut[2];
  dataOut[0] = command;
  dataOut[1] = data;
  I2C_Master_Send(OSCILLOSCOPE_ADDRESS, dataOut, 2);
  return 0; 
}


void oscInitTest(){
  //
  // Initiate the Oscilloscope On the Testing Board
  //
  unsigned char command = 0x01; // output
  /*
    unity gain( Gain1AB = 00 )
    Mild attenuate( Gain2AB = 00 )
    ABSel = B = 1
    AC1/2 = 11
  */
  unsigned char data = 0x13;
  OscControl(command,data);
}

// send/recieve functions

// initiate the function generator
int FunctInit(void){
  // initiate digital POT ( NO offset )
  unsigned char data[2];

  data[0] = POT_WRITE | POT_OFFSET;
  data[1] = MID; // No Offset
  I2C_Master_Send(FUNCTION_GEN_ADDRESS_POT, data, 2);

  data[0] = POT_WRITE | POT_ATTENUATE;
  data[1] = MAX; // NO attenuation
  I2C_Master_Send(FUNCTION_GEN_ADDRESS_POT, data, 2);

  // initiate DAC (Min voltage)
  data[0] = MIN;
  I2C_Master_Send(FUNCTION_GEN_ADDRESS_DAC, data, 1);
  
  return 0;
}


void DigInit(void){
  // initiate the Digital I/O pins



}


/************************************************************************
This is used for testing only
***********************************************************************/
#ifdef TESTING
int main(void){

  /****************************************************
  Set The SYSCTL_RCC register (Offset = 0x060):
  page 198 of datasheet, 20MHZ with PLL
  ******************************************************/
  SysCtlClockSet( SYSCTL_SYSDIV_10 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN );
  //SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);
  //SysCtlClockSet(SYSCTL_SYSDIV_10 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_4MHZ);

  /**************************************************
  Enable The Pins Controlling the I2C: Port A pins: driverlib/sysctl.c
  ******************************************************/
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

  /**************************************************
  Enable and reset the I2C1 peripheral: driverlib/sysctl.c
  *****************************************************/
  SysCtlPeripheralReset(SYSCTL_PERIPH_I2C1);
  SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C1);

  /********************************************************
  This Section enables sleep mode on the peripherals, if the
  CPU is put to sleep then they will be also
   *******************************************************/
  //  SysCtlPeripheralSleepEnable(SYSCTL_PERIPH_I2C1);
  //  SysCtlPeripheralSleepEnable(SYSCTL_PERIPH_GPIOA);
  // SysCtlPeripheralEnable(SYSCTL_PERIPH_SSI); // serial to parallel?
  //  SysCtlPeripheralClockGating(true);

  I2C_init(false);

  oscInitTest();

  // go to infinite loop
  while(1){

  }
  return 0;
}
#endif



