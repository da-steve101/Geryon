/****************************************
This file is a dummy interface for testing
 ***************************************/
#include "multimeterInterface.h"

/*******************************************
Initialize UART and anything else
 ******************************************/
int init_Multimeter(void){
  return 0;
}

/*******************************************
Stop UART and anything else
 ******************************************/
int close_Multimeter(void){
  return 0;
}

/*******************************************
Reset UART and anything else
 ******************************************/
int reset_Multimeter(void){
  return 0;
}

/****************************************
Returns the raw data from the multimeter
Or Null if Error
 ***************************************/
char * getString(){
  return NULL;
}

/****************************************
Get the type of measurement from the raw data
returns ERROR on error
 ***************************************/
enum MEASUREMENT_TYPE getType(char * raw_data){
  return ERROR;
}

/*****************************************
Write the type of Measurement as a string to
be displayed. Returns NULL on error
 *****************************************/
char * type2String( enum MEASUREMENT_TYPE type){
  return NULL;
}

/*****************************************
Get the Value of the measurement in volts,
amps or ohms. Returns DOUBLE_ERROR_VALUE on
error
 ****************************************/
double getValue(char * raw_data, enum MEASUREMENT_TYPE type){
  return DOUBLE_ERROR_VALUE;
}

