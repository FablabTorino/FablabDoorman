Fablab Doorman
==============
An Arduino Yun based RFID access control system
-----------------------------------------------
This project was designed with the intent to allow the access to the Fablab/Makerspace to the registered members.
Every member is provided with an RFID tag which is stored in a users database. Using a database permits to dinamically manage the users subscription and associate different level of permission and time access profiles.

#### Components:
* [Arduino Yun](https://www.arduino.cc/en/Main/ArduinoBoardYun)
* [Adafruit PN532 NFC/RFID shield](https://www.adafruit.com/products/789)
* Micro SD card (any 
* power supply (TODO: specify parameters)
* TODO: add other componets 


#### Logical Structure
The Arduino YUN was chosen for its ability to combine the reliability of a microcontroller with the potential of a Linux system.  
The RFID reader, the electromechanical lock (or multiple ones), push buttons and other inuputs are connected to the microcontroller. Instead, on the OpenWRT there is implemented the user access logic and the database. Plus the feature to add/remove connecting remotly to the YUN.  
Every time a new RFID tag is read, the microntroller ask permission to open the door to the control access logic hosted on the OpenWRT side.

The communication between the two processors is done using the Bridge protocol, that allows the microcontroller to execute the python script with the control access logic only when a new RFID card code is available.


#### Setup Instructions
##### OpenWRT setup
It's recommended to use the latest Arduino YUN OpenWRT image, available [here](https://www.arduino.cc/en/Main/Software) and following the [sysupgrade guide](https://www.arduino.cc/en/Tutorial/YunSysupgrade) to flash it.

Prepare your SD card by creating an empty folder in the root directory named "arduino". When OpenWrt-Yun finds this folder on an attached storage device, it creates a link to the SD to the "/mnt/sd" path.

The Arduino YUN doesn't come with a database management system pre-installed so we need to install it including the dependencies to use with a python script. SQlite3 was the coiche for its lightweight footprint and for being designed to being embedded into programs.

Run this command inside the YUN OpenWRT shell to install all the dependencies:

``` bash
opkg install libsqlite3 sqlite3-cli python-sqlite3
```

##### Upload all files and sketch
As it's structured, this project can be uploaded completely by using only the ***Upload*** button on the Arduino IDE, you just have to worry about selecting the network port (instead of the serial port) as upload port in the *"Tools > Port"* menu.
The sketch would be normally compiled and flahsed on the microcontroller and the python script contained inside the *"www"* sketch folder will be copied at the following path on the Yun:  
```
/mnt/sd/arduino/www/FablabDoorman
```

##### Database Structure
The database is called *logger.db* and is structured with two tables called *allowedusers* and *inothedoor*.  
The **allowedusers** is the table containing the all the users who can open the door. Table structure:  
* id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL
* username TEXT NOT NULL, 
* cardcode TEXT NOT NULL  

The **intothedoor** is a table used to log all the access, structured with the following scheme:
* id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL
* datetime TEXT NOT NULL
* cardcode TEXT NOT NULL

For simplicty database structure is automatically created after the first execution. You can connect to your YUN via SSH and manually run the python script to setup the database.

##### Adding a New User
Adding a new user can be made only manually by inserting a new row to the *allowedusers* table. Instructions:
* connect to your YUN via SSH
* move to the *FablabDoorman* directory: ```$ cd /mnt/sd/arduino/www/FablamDoorman```
* open the database: ```sqlite3 logger.db```





