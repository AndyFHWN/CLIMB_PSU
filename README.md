# CLIMB_PSU

The PSU consists of three programmable microchips -> three firmware's. The Main controller (MC) and two communictaion controllers (CC's). The CC's are the interface between the MC and the OBC. The MC is connected to both CC's and communicates via UART. The CC's communicate with the OBC via I2C.

Firmware Main Controller (MC):
The firmware of the main controller is responsible for:
- Converting the power on the PV1 and PV2 bus to 3V3 and 5V.
- Connecting battery to PV1 or PV2 bus.
- Gathering of houskeeping data via I2C (power and temperature sensors).

The firmware holds serveral modes: power up mode, debug mode, boot mode, flight mode, safe mode, power down mode.

Power up mode: In this mode the PPU checks if the RBF is LOW or HIGH. If it is LOW, the EPS enters the debug mode and if HIGH enters the boot mode.
Debug mode: This mode is only for debugging. When entering this mode, the code written in the debug section of the firmware is executed.
Boot mode: The boot mode switches between flight, safe and power down mode.
Flight mode: This mode is set by default and provides all functions of the EPS system. In this mode the EPS collects all sensor data, responds to commands, communicates with the communication controllers (CC), manages the batteries, etc.
Safe mode: The safe mode is entered if the communication to the CC's is lost. In this mode the MC will connect the batteries to the PV1 and PV2 bus and will activate the 3.3V back up power and try to establish communication with the CC's.
Power down mode: This mode is entered if the battery voltage or temperature is below a certain value. In this mode most power consumers are deactivated, less sensors are used and the communication cycle is reduced. This mode is left when at least one of the batteries is sufficiently charged again.

Firmware Communication Controller 1/2 (CC1/2):
The CC's are the interface between the MC and the OBC. The controllers are in a sleep mode as long no requests occur. The controllers are sending/receiving data between the MC and OBC The MC creates a data vector with all housekeeping data which is frequently send to the CC1/2. The CC1/2 adds its own housekeeping data (Temperature and Vcc) to the datavector and sends it to the OBC via I2C. The OBC itself sends data to the CC1/2 (e.g. turn on 3V3 on side x+) which are then send to the MC. All I2C communication between the CC and the OBC are handled with ISR's.
The controllers are also capable of connecting battery 1 to PV1 and can enable the 3V3_Backup voltage regulator. The CC2 controller is used as redundancy to the CC1.

Difference between CC1 and CC2:
- The main interface for the OBC is via CC1. Commands which are received via CC1 are handled with
  higher priority than commands via CC2 and, when conflicting, will be executed.
- CC1 also holds a safe mode which is active if the MC is not available.
- CC1 also holds a communication with TT&C but this was only implemented for PEGASUS and is not used for Climb.
