# Lora-Tracking-System
 This is an IoT project where I used two TTGO lora32 with a GPS module to send GPS coordinates from two separate locations.

## Tools Used

Hardware:
```
TTGO LoRa	x2
GPS (NEO 6m)	x1
```
Software:
```
Libraries: Lora, TinyGPS++, SoftwareSerial
```



![image](https://user-images.githubusercontent.com/81851926/169840425-fabbc120-6af9-4be3-8255-6057ad85c2d8.png)



## About lora:
- LoRa consumes less power than other transmitters
- LoRa can send data through long distances ~5Km


## Changing the spreading factor SF and the Coding Rate CR based on our needs:

- The spreading factor controls the chirp rate, and thus controls the speed of data transmission. Lower sf means higher bit rate, but lower sensitivity and thus more scrambled data, but will drain the battery slower.

- a higher coding rate will not increase the range, but will make a link more reliable if interference is present, Higher coding rate will dedicate a bigger part of the packet for error correction
