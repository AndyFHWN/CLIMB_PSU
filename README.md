# CLIMB_PSU

The PSU consists of three programmable microchips -> three firmware's. The Main controller (MC) and two communictaion controllers (CC's). The CC's are the interface between the MC and the OBC. The MC is connected to both CC's and communicates via UART. The CC's communicate with the OBC via I2C.

Firmware Main Controller (MC):
The firmware of the main controller is responsible for:
- Converting the power on the PV1 and PV2 bus to 3V3 and 5V.
- Connecting battery to PV1 or PV2 bus.
- Gathering of houskeeping data via I2C (power and temperature sensors).

The firmware holds three modes: flight mode, safe mode, power down mode.

Firmware Communication Controller 1/2 (CC1/2):
The CC's are the interface between the MC and the OBC. This controller is in a sleep mode as long no requests occur. This controller does nothing then sending/receiving data between the MC and OBC. The MC creates a data vector with all housekeeping data which is frequently send to the CC1/2. The CC1/2 adds its own housekeeping data (Temperature and Vcc) to the datavector and sends it to the OBC via I2C. The OBC itself sends data to the CC1/2 (e.g. turn on 3V3 on side x+) which are then send to the MC. All I2C communication between the CC and the OBC are handeled with ISR's.

Difference between CC1 and CC2:
- The main interface for the OBC is via CC1. Commands which are received via CC1 are handled with
  higher priority than commands via CC2 and, when conflicting, will be executed.
- CC1 holds also a safe mode which is active if the MC is not available.
