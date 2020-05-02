# Weather Station
We made this project as for part of our education at AGH UST. The main idea is make simple weather station on Atmega328 in C. Our application include stationary device which can measure humidity and temperature using DHT11 sensor. 16x2 display and real time clock are included too. You can switch displayed data with buttons. Station is connected with outdoor sensor with nRF24L01 radio module. All of our code was written by us based on documentations and public forums so it works best with similar applications (ex. connecting 2 devices for sending data). The weather station sends request for new measurments with specified period - feel free to change it. If devices lost connection and station will wait for data, sensor will interrupt itself after specified time and try to resend data.  

# Hardware requirements
- Arduino NANO for sensor
    - We supplied it with 3.7V battery but you can use any else (compatible with Arduino NANO)
    - nRF24L01 **HAVE TO** be connected to 3.3V with VCC
    - 47uF electrolytic capacitor parallel to GND and 3.3V
    - DHT11 sensor
    - 5k pull-up resistor for DHT11
    - Switch
    - Charging module (optional)
- Arduino UNO for station
    - Will see

![datei](/img/schemeSensor.png)
