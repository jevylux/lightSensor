# lightSensor
LightSensor with BH1750 and esp32 mini d1

The lightsensor projects uses a BH1750 Modul to read the ambient light.
The value is published every second to an mqtt topic.
This mqtt Topic may then be available in a home automation system ( i.e. iobroker) and be used to control other devices
The application is developped with an Arduino IDE and downloaded to an esp32 mini d1 board.
The complete system, including power transformator is then inserted in a 3d-printed box.
The box containes an insert at the top to include a small element printed with transparent PLA. The BH1750 fits  exactly into this small insert.


