ESP01S Smart Relay

This repository contains the code for an ESP01S relay module that can be used to control a home heater. The project is designed to work with Apple Homekit via https://github.com/dvlad96/esp32-thermostat-controller

The following HTTP GET requests are available:
  /relay_command?val=0 -> Open the relay
  /relay_command?val=1 -> Close the relay
  /relay_status -> Returns the status of the relay 0 = Opened & 1 = Closed
  
