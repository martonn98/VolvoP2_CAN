# VolvoP2_CAN

Discover CAN Bus commands on a 2002 S60, part of [Volvo P2 platform](https://en.wikipedia.org/wiki/Volvo_P2_platform).

## Introduction

Main goal is to controll different features of the car from a mobile app, e.g. central locking or power windows. First you need point where you can connect to the CAN Bus network. This car features 2 separate network opearting on different speed, low speed on 125 Kbps and high speed on 250 Kbps. (After facelift (2004-) the HS bus speed is bumped up to 500 Kbps.) Speed critical ECU's are connected to the HS network like ECM, everything else is on the LS network. CEM has access to both network and wiring out to the OBD port, but on pre-facelift cars these signals are hidden with a diagnostic relay. To open this relay, I used the OEM diagnostic software [VIDA](https://www.volvoforums.org.uk/showthread.php?t=304063), a chinese copy of OEM hardware [VXDIAG](https://www.aliexpress.com/item/4000212452021.html) and an OBD Y splitter cable to connect an other device simultaneously. After starting a diagnostic session, the relay will stay open. Find more about Volvo specific abrevations [here](https://vidaresources.volvocars.biz/help-wbt/us/d9e40753e31bdbd9c0a801cc34911143.htm).

## Hardware

- Arduino Uno (chinese)
- MCP2515 (chinese)

## Projects

### String2DIM

POC to demonstrate the ability to send commands to the car from an arduino. Write a string to DIM's 2x16 character LCD screen, using the CAN ID of PHM. Works without an actual PHM.

### TODO

- Migrate from arduino to ESP32, needs a CAN transciever only (SJA1000)
- Find more CAN command
- Build a mobile app with Bluetooth connection to ESP32
- Send message on K-Line to [keep diagnostic relay open](https://www.swedespeed.com/threads/connecting-to-canbus-via-obdii.135397/), or use an other point for hardware connection like REM.
