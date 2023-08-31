# Logger_Verifier + CLient + Server
```
Client: Client module
```
```
Logger: Capture frames and Send to Server
```
```
Verifier: Request data from Server and Verify frames. Send Verified result to Server
```
```
Server: Server module
```
```
DB: Database related code
```
```
command_parser: Request handling
```
```
openssl: For encryption and decryption
```
# How to Use
+ prerequisite
```
OpenCV 4.5.1 required -> See OpenCV_Download_manual.

Openssl required -> See Openssl_Download_manual.

MariaDB required -> See MariaDB_Download_manual.
```

After git clone please mv Logger_Verifier to hanium_2022
```
mv Logger_Verifier hanium_2022
```

+ Need to Change SERVER IP ADDRESS
```
cd Client
```
```
nano/vi/vim cfg.h
```
```
change SERVER_IP_ADDR to your RPI's working as Server
```
+ compile 
1. Logger
```
cd Logger 
```
```
make clean && make
```
2. Verifier
```
cd Verifier
```
```
make clean && make
```
3. Server
```
cd Server
```
```
make clean && make
```
# TEST
+ Logger
```
Logger: Capture frames by Webcam, and convert into YUV420.
        USE only Y-frames to find feature vector. USE feature vector to make hash
        Then send YUV420 video data, hash, Contents ID to SERVER
```
+ Verifier
```
Verifier: GET DATAS from SERVER and CHECK for video data's integrity
```
## 2022 09 01 changes -by MJ
Camera settings are moved to cfg.h. No changes are needed.

## 2022 09 07 changes -by MJ
Private Key and Public Key generation added

-> sign.cpp: codes for generating Key and sign hash

## 2022 09 25 changes -by MJ
Client Related condes were moved to Client folder
