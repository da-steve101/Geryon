/****************************************
This file is the interface to the multimeter
 ***************************************/

/* Define all types of measurement */
enum MEASUREMENT_TYPE {VOLTAGE_DC, VOLTAGE_AC, CURRENT_DC, 
			 CURRENT_AC, RESISTANCE, CAPACITANCE,
			 FREQUENCY, DUTY_CYCLE, TEMPERATURE_C,
			 TEMPERATURE_F, ERROR};

/* Define the Error value as -Inf */
#define DOUBLE_ERROR_VALUE 0xfff0000000000000 
/* Define infinity (For Resistance) */
#define DOUBLE_INFINITY 0x7ff0000000000000 
/* Define NULL */
#define NULL 0

/*******************************************
Initialize UART and anything else
 ******************************************/
int init_Multimeter(void);

/*******************************************
Stop UART and anything else
 ******************************************/
int close_Multimeter(void);

/*******************************************
Reset UART and anything else
 ******************************************/
int reset_Multimeter(void);

/****************************************
Returns the raw data from the multimeter
Or Null if Error
 ***************************************/
char * getString();

/****************************************
Get the type of measurement from the raw data
returns ERROR on error
 ***************************************/
enum MEASUREMENT_TYPE getType(char * raw_data);

/*****************************************
Write the type of Measurement as a string to
be displayed. Returns NULL on error
 *****************************************/
char * type2String( enum MEASUREMENT_TYPE type);

/*****************************************
Get the Value of the measurement in volts,
amps or ohms. Returns DOUBLE_ERROR_VALUE on
error
 ****************************************/
double getValue(char * raw_data, enum MEASUREMENT_TYPE type);

