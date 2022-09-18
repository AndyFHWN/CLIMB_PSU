# CLIMB_PSU

The PSU consists of three programmable microchips -> three firmware's. The Main controller (MC) and two communictaion controllers (CC's). The CC's are the interface between the MC and the OBC. The MC is connected to both CC's and communicates via UART. The CC's communicate with the OBC via I2C (OBC -> Master, CC's -> Slaves). The MC collects housekeeping data by communicating with all sensors  via I2C. 

Firmware Main Controller (MC):

The firmware of the main controller is responsible for:
- Converting the power on the PV1 and PV2 bus to 3V3 and 5V.
- Connecting battery to PV1 or PV2 bus.
- Gathering of houskeeping data via I2C (power and temperature sensors).

The firmware holds serveral modes: power up mode, debug mode, boot mode, flight mode, safe mode, power down mode.

- Power up mode: In this mode the PPU checks if the RBF is LOW or HIGH. If it is LOW, the EPS enters the debug mode and if HIGH enters the boot mode.
- Debug mode: This mode is only for debugging. When entering this mode, the code written in the debug section of the firmware is executed.
- Boot mode: The boot mode switches between flight, safe and power down mode.
- Flight mode: This mode is set by default and provides all functions of the EPS system. In this mode the EPS collects all sensor data, responds to commands,     communicates with the communication controllers (CC), manages the batteries, etc.
- Safe mode: The safe mode is entered if the communication to the CC's is lost. In this mode the MC will connect the batteries to the PV1 and PV2 bus and will   activate the 3.3V back up power and try to establish communication with the CC's.
- Power down mode: This mode is entered if the battery voltage or temperature is below a certain value. In this mode most power consumers are deactivated, less   sensors are used and the communication cycle is reduced. This mode is left when at least one of the batteries is sufficiently charged again.

Firmware Communication Controller 1/2 (CC1/2):
The CC's are the interface between the MC and the OBC. The controllers are in a sleep mode as long no requests occur. The controllers are sending/receiving data between the MC and OBC. The MC creates a data vector with all housekeeping data which is frequently send to the CC1/2 (strucutre of data vector at end of README). The CC1/2 adds its own housekeeping data (Temperature and Vcc) to the datavector and sends it to the OBC via I2C. The OBC itself sends data to the CC1/2 (e.g. turn on 3V3 on side x+) which are then send to the MC. All I2C communication between the CC and the OBC are handled with ISR's.
The controllers are also capable of connecting battery 1 to PV1 and can enable the 3V3_Backup voltage regulator. The CC2 controller is used as redundancy to the CC1.

Difference between CC1 and CC2:
- The main interface for the OBC is via CC1. Commands which are received via CC1 are handled with
  higher priority than commands via CC2 and, when conflicting, will be executed.
- CC1 also holds a safe mode which is active if the MC is not available. Not sure if also implemented on CC2?
- CC1 also holds a communication with TT&C but this was only implemented for PEGASUS and is not used for Climb.

How to receive data vector from CC1/2:
For the connection to the CC's, there are pins on the PSU for SDA and SCL for both CC's:

CC1:
 - side x-/C pin 17: SCL1
 - side x-/C pin 18: SDA1
 
CC2:
 - side x+/A pin 17: SCL2
 - side x+/A pin 18: SDA2

The adress of CC1 is 0x55 and the adress of CC2 is 0xAA. To receive the data vector from CC1/2 one has to send the adress of one of the CC and then the register number of the data vector. The strucure of the data vector is shown at the end of the README.

Example of an Arduino sketch used to read out the datavector:  
  CC2_address = 0xAA;
  datavector_reg_numb = 5;
  
 - Wire.beginTransmission(CC2_address);    //prepares data transmission to given adress
 - Wire.write(datavector_reg_numb);        //saves value/data to the send buffer
 - Wire.endTransmission();                 //send what is saved in buffer
 - Wire.requestFrom((int)(CC2_addr), 1);   //Requests the specified number of bytes from the specified device
 - datavector = Wire.read();               //reads vaule/data from receive buffer
  
  The variable "datavector" contains now the 5th byte of the received data vector from CC2.

Strucutre of the data vector that is send to the OBC:
In flight mode the MC creates a data vector with all housekeeping data which is frequently send to the CC1/2 and then further to the OBC. The data vector is created in the function "void CreateDataVector(uint8_t *dataVector)" which is implmented in the file "CCinterface.c".
The vector is created in firmware of the MC and is structred as follows:
- dataVector[0] = '$';
- dataVector[1] = 'D';
- // 5V VI data
- dataVector[2] = 'P'; //global_5V.i1Low;
- dataVector[3] = global_5V.i1High;
- dataVector[4] = global_5V.i2Low;
- dataVector[5] = global_5V.i2High;
- dataVector[6] = global_5V.v1Low;
- dataVector[7] = global_5V.v1High;
- dataVector[8] = global_5V.v2Low;
- dataVector[9] = global_5V.v2High;
- // 3V3 VI data
- dataVector[10] = global_3V3.i1Low;
- dataVector[11] = global_3V3.i1High;
- dataVector[12] = global_3V3.i2Low;
- dataVector[13] = global_3V3.i2High;
- dataVector[14] = global_3V3.v1Low;
- dataVector[15] = global_3V3.v1High;
- dataVector[16] = global_3V3.v2Low;
- dataVector[17] = global_3V3.v2High;
- // edge temperature
- dataVector[18] = global_tEdge.tempLow;
- dataVector[19] = global_tEdge.tempHigh;
- // center temperature
- dataVector[20] = global_tCenter.tempLow;
- dataVector[21] = global_tCenter.tempHigh;
- // HV VI data
- dataVector[22] = global_HV.i1Low;
- dataVector[23] = global_HV.i1High;
- dataVector[24] = global_HV.i2Low;
- dataVector[25] = global_HV.i2High;
- dataVector[26] = global_HV.v1Low;
- dataVector[27] = global_HV.v1High;
- dataVector[28] = global_HV.v2Low;
- dataVector[29] = global_HV.v2High;
- // battery 1 VI data
- dataVector[30] = global_viBat1.i1Low;
- dataVector[31] = global_viBat1.i1High;
- dataVector[32] = global_viBat1.i2Low;
- dataVector[33] = global_viBat1.i2High;
- dataVector[34] = global_viBat1.v1Low;
- dataVector[35] = global_viBat1.v1High;
- dataVector[36] = global_viBat1.v2Low;
- dataVector[37] = global_viBat1.v2High;
- // battery 2 VI data
- dataVector[38] = global_viBat2.i1Low;
- dataVector[39] = global_viBat2.i1High;
- dataVector[40] = global_viBat2.i2Low;
- dataVector[41] = global_viBat2.i2High;
- dataVector[42] = global_viBat2.v1Low;
- dataVector[43] = global_viBat2.v1High;
- dataVector[44] = global_viBat2.v2Low;
- dataVector[45] = global_viBat2.v2High;
- // battery 1 temperature
- dataVector[46] = global_tBat1.tempLow;
- dataVector[47] = global_tBat1.tempHigh;
- // battery 2 temperature
- dataVector[48] = global_tBat2.tempLow;
- dataVector[49] = global_tBat2.tempHigh;
- // status registers (not correctly implemented yet)
- dataVector[50] = GetEEStatus1();
- dataVector[51] = GetEEStatus2();
- dataVector[52] = GetEEStatus3();
- // estimated capacity battery 1 and 2 (not implemented yet)
- dataVector[53] = 0x01;
- dataVector[54] = 0x02;
- // number of reboots MC, CC1 and CC2 (not implemented yet)
- dataVector[55] = GetEERebootCount();
- dataVector[56] = 0x01;
- dataVector[57] = 0x02;
- // VCC_CC1, TEMP_CC1, VCC_CC2, TEMP_CC2 (not implemented yet)
- dataVector[58] = 0x01;
- dataVector[59] = 0x01;
- dataVector[60] = 0x02;
- dataVector[61] = 0x02;
- // status CC1, CC2 (not implemented yet)
- dataVector[62] = 0x01;
- dataVector[63] = 0x02;
- // TBD, TBD, CCx FOVR, CCx OVR (not implemented yet)
- dataVector[64] = 0x33;
- dataVector[65] = 0x33;
- dataVector[66] = 0x33;
- dataVector[67] = 0x33;
- // checksum of byte 0 to 65 via XOR
- dataVector[68] = dataVector[0];

This data vector with the size of 69 byte is send to the CC1/2. In the firmware of ht CC1/2 the vector is extended to the size of 88 byte and filled with data from CC1/2. In the firmware of CC2 the data vector is extaded as follows:
The dataVector from the MC is received from CC2 via UART byte per byte and saved in a new vector called "data".

main.h:
- #define rb_ct           data[55]  //reboot counter of
- #define VCC             data[58]	//supply voltage Vcc
- #define TEMP            data[59]  //internal temperature
- #define status          data[61]  //status of CC2
- #define FOVR            data[81]  //force output value register
- #define OVR             data[82]  //output value register
- #define new_data_flag   data[87]  //if new data from OBC received since last com. with MC 1, else 0.

uart.c:
- data[56]=uart_string_R[58];						//VCC CC1
- data[57]=uart_string_R[59];						//temperature of CC1
- data[60]=uart_string_R[62];						//status CC1
- //data[62]=uart_string_R[64];					//TBD
- data[62]=0xAB;									      //ID register
- data[63]=uart_string_R[65];						//TBD
- data[81]=uart_string_R[66];						//CC2 FOVR
- data[82]=uart_string_R[67];						//CC2 OVR

The whole new data vector is then send via I2C to the OBC if the I2C interrupt routine is triggered.
