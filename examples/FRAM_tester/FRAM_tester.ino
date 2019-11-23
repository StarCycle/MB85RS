/*
    Code written by Chia Jiun Wei @ 24-7-2017
    This code test the MB85RS256A FRAM library
*/

/*  PIN connection for a MSP432 LaunchPad board:
    P6.0 - > FRAM CS, define P6.0 as SPI chip select
*/

#include "MB85RS.h"
#include "DSerial.h"
#include "DSPI.h"

DSPI master;          // used EUSCI_B0
MB85RS fram1(master, GPIO_PORT_P1, GPIO_PIN0);   //initialize FRAM, Connect CS to pin 2 of MSP432 launchpad
DSerial serial;

unsigned char stat;

//OBC non-volatile parameter
unsigned short reset_count;
unsigned long uptime_total;
unsigned char init_time[8];
unsigned long frame_counter;
unsigned short V_Batt_threshold;
short T_Batt_threshold;
short T_Amp_threshold;
unsigned short activation_threshold;

//FRAM buffer
unsigned char buffer[25];

void setup()
{ 
  // initialize the UART
  serial.begin();
  delay(300);

  // initialise SPI:
  master.begin();
  delay(300);

  serial.print("**** Start FRAM test ****");
  serial.println();

  //initalise FRAM
  fram1.init();
  serial.print("FRAM initialised ****");
  serial.println();

  //Read status
  stat = fram1.read_Status();
  serial.print("Value of status register: ");
  serial.print(stat, DEC);
  serial.println();

  reset_count = 10;
  uptime_total = 33600;
  init_time[0] = 17;
  init_time[1] = 7;
  init_time[2] = 24;
  init_time[3] = 1;
  init_time[4] = 16;
  init_time[5] = 42;
  init_time[6] = 55;
  init_time[7] = 88;
  frame_counter = 101;
  V_Batt_threshold = 3500;
  T_Batt_threshold = 3500;
  T_Amp_threshold = 40;
  activation_threshold = 2100;

  serial.print("**** Defined Parameters ****");
  serial.println();
  serial.print(reset_count, DEC);
  serial.println();
  serial.print(uptime_total, DEC);
  serial.println();
  for (int i = 0; i < 8; i++)
  {
    serial.print(init_time[i], DEC);
    serial.print(" ");
  }
  serial.println();
  serial.print(frame_counter, DEC);
  serial.println();
  serial.print(V_Batt_threshold, DEC);
  serial.println();
  serial.print(T_Batt_threshold, DEC);
  serial.println();
  serial.print(T_Amp_threshold, DEC);
  serial.println();
  serial.print(activation_threshold, DEC);
  serial.println();
}

void loop()
{
  memcpy(&buffer[0], &reset_count, 2);
  memcpy(&buffer[2], &uptime_total, 4);
  memcpy(&buffer[6], init_time, 8);
  memcpy(&buffer[14],&frame_counter, 4);
  memcpy(&buffer[18], &V_Batt_threshold, 2);
  memcpy(&buffer[20], &T_Batt_threshold, 2);
  memcpy(&buffer[22], &T_Amp_threshold, 2);
  memcpy(&buffer[24], &activation_threshold, 2);

  delay(300);
  serial.print("Writing parameters to FRAM");
  serial.println();
  
  fram1.write(0x00, &buffer[0], 2);
  fram1.write(0x02, &buffer[2], 4);
  fram1.write(0x06, &buffer[6], 8);
  fram1.write(0x14, &buffer[14], 4);
  fram1.write(0x18, &buffer[18], 2);
  fram1.write(0x20, &buffer[20], 2);
  fram1.write(0x22, &buffer[22], 2);
  fram1.write(0x24, &buffer[24], 2);  

  serial.print("Reading parameters from FRAM");
  serial.println();
  
  fram1.read(0x00, &buffer[0], 2);  
  fram1.read(0x02, &buffer[2], 4);  
  fram1.read(0x06, &buffer[6], 8);  
  fram1.read(0x14, &buffer[14], 4);  
  fram1.read(0x18, &buffer[18], 2);
  fram1.read(0x20, &buffer[20], 2);  
  fram1.read(0x22, &buffer[22], 2);
  fram1.read(0x24, &buffer[24], 2);

  memcpy(&reset_count, &buffer[0], 2);
  memcpy(&uptime_total, &buffer[2], 4);
  memcpy(init_time, &buffer[6], 8);
  memcpy(&frame_counter, &buffer[14], 4);
  memcpy(&V_Batt_threshold, &buffer[18], 2);
  memcpy(&T_Batt_threshold, &buffer[20], 2);
  memcpy(&T_Amp_threshold, &buffer[22], 2);
  memcpy(&activation_threshold, &buffer[24], 2);

  serial.print("**** FRAM Parameters ****");
  serial.println();
  serial.print(reset_count, DEC);
  serial.println();
  serial.print(uptime_total, DEC);
  serial.println();
  for (int i = 0; i < 8; i++)
  {
    serial.print(init_time[i], DEC);
    serial.print(" ");
  }
  serial.println();
  serial.print(frame_counter, DEC);
  serial.println();
  serial.print(V_Batt_threshold, DEC);
  serial.println();
  serial.print(T_Batt_threshold, DEC);
  serial.println();
  serial.print(T_Amp_threshold, DEC);
  serial.println();
  serial.print(activation_threshold, DEC);
  serial.println();

  serial.print("**** Erase all ****");
  serial.println();
  fram1.erase_All();

  serial.print("Reading erased parameters from FRAM");
  serial.println();
  fram1.read(0x00, &buffer[0], 2);
  fram1.read(0x02, &buffer[2], 4);
  fram1.read(0x06, &buffer[6], 8);
  fram1.read(0x14, &buffer[14], 4);
  fram1.read(0x18, &buffer[18], 2);
  fram1.read(0x20, &buffer[20], 2);
  fram1.read(0x22, &buffer[22], 2);
  fram1.read(0x24, &buffer[24], 2);

  memcpy(&reset_count, &buffer[0], 2);
  memcpy(&uptime_total, &buffer[2], 4);
  memcpy(init_time, &buffer[6], 8);
  memcpy(&frame_counter, &buffer[14], 4);
  memcpy(&V_Batt_threshold, &buffer[18], 2);
  memcpy(&T_Batt_threshold, &buffer[20], 2);
  memcpy(&T_Amp_threshold, &buffer[22], 2);
  memcpy(&activation_threshold, &buffer[24], 2);

  serial.print("**** Erased FRAM Parameters ****");
  serial.println();
  serial.print(reset_count, DEC);
  serial.println();
  serial.print(uptime_total, DEC);
  serial.println();
  for (int i = 0; i < 8; i++)
  {
    serial.print(init_time[i], DEC);
    serial.print(" ");
  }
  serial.println();
  serial.print(frame_counter, DEC);
  serial.println();
  serial.print(V_Batt_threshold, DEC);
  serial.println();
  serial.print(T_Batt_threshold, DEC);
  serial.println();
  serial.print(T_Amp_threshold, DEC);
  serial.println();
  serial.print(activation_threshold, DEC);
  serial.println();

  reset_count = 10;
  uptime_total = 33600;
  init_time[0] = 17;
  init_time[1] = 7;
  init_time[2] = 24;
  init_time[3] = 1;
  init_time[4] = 16;
  init_time[5] = 42;
  init_time[6] = 55;
  init_time[7] = 88;
  frame_counter = 101;
  V_Batt_threshold = 3500;
  T_Batt_threshold = 3500;
  T_Amp_threshold = 40;
  activation_threshold = 2100;

  memcpy(&buffer[0], &reset_count, 2);
  memcpy(&buffer[2], &uptime_total, 4);
  memcpy(&buffer[6], init_time, 8);
  memcpy(&buffer[14],&frame_counter, 4);
  memcpy(&buffer[18], &V_Batt_threshold, 2);
  memcpy(&buffer[20], &T_Batt_threshold, 2);
  memcpy(&buffer[22], &T_Amp_threshold, 2);
  memcpy(&buffer[24], &activation_threshold, 2);

  serial.print("Writing parameters to FRAM");
  serial.println();
  fram1.write(0x00, &buffer[0], 2);
  fram1.write(0x02, &buffer[2], 4);
  fram1.write(0x06, &buffer[6], 8);
  fram1.write(0x14, &buffer[14], 4);
  fram1.write(0x18, &buffer[18], 2);
  fram1.write(0x20, &buffer[20], 2);
  fram1.write(0x22, &buffer[22], 2);
  fram1.write(0x24, &buffer[24], 2);

  serial.print("Reading parameters from FRAM");
  serial.println();
  fram1.read(0x00, &buffer[0], 2);
  fram1.read(0x02, &buffer[2], 4);
  fram1.read(0x06, &buffer[6], 8);
  fram1.read(0x14, &buffer[14], 4);
  fram1.read(0x18, &buffer[18], 2);
  fram1.read(0x20, &buffer[20], 2);
  fram1.read(0x22, &buffer[22], 2);
  fram1.read(0x24, &buffer[24], 2);

  memcpy(&reset_count, &buffer[0], 2);
  memcpy(&uptime_total, &buffer[2], 4);
  memcpy(init_time, &buffer[6], 8);
  memcpy(&frame_counter, &buffer[14], 4);
  memcpy(&V_Batt_threshold, &buffer[18], 2);
  memcpy(&T_Batt_threshold, &buffer[20], 2);
  memcpy(&T_Amp_threshold, &buffer[22], 2);
  memcpy(&activation_threshold, &buffer[24], 2);

  serial.print("**** FRAM Parameters ****");
  serial.println();
  serial.print(reset_count, DEC);
  serial.println();
  serial.print(uptime_total, DEC);
  serial.println();
  for (int i = 0; i < 8; i++)
  {
    serial.print(init_time[i], DEC);
    serial.print(" ");
  }
  serial.println();
  serial.print(frame_counter, DEC);
  serial.println();
  serial.print(V_Batt_threshold, DEC);
  serial.println();
  serial.print(T_Batt_threshold, DEC);
  serial.println();
  serial.print(T_Amp_threshold, DEC);
  serial.println();
  serial.print(activation_threshold, DEC);
  serial.println();

  while (1);
}
