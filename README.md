# SkyCOM

SkyCOM is an advanced serial communications protocol for communication between microcontroller systems on a network.

Origins start when working with RS232 with industrial CNC machines.

Designed for microcontroller on a network where commands and values need to be reliably send over a network.

**this project is a work in progress and under active development. Not finished yet, USE AT OWN RISK!**

## _features_
* reliable data transmission on a network
* no string manipulation neccesairy
* efficient
* build in system commands

## _supported datatypes_
* integer
* floating point
* ascii strings

## _TODO_

### short term
* merge TX and RX library's
* get comm between uC working w/ clock
* get comm between uC working w/o clock

### long term
* auto addressing


# how to use the SkyCOM library

## hardware

---
SkyCOM is designed to work with RS485 or ttl electronic standards and the library directly supports and manages RE (Reciever Enable) and DE (Drive Enable) outputs for use with RS485 IC's


## transmitting
---

**TEMPORY FUNCTION**
```
StartCOM(addr, version)
```
this sets the device address, which, at the moment, is static and the users firmware version. this is all included in the protocol so if interpertation of data structs or data changes over time, this is a build in way to make sure devices will be incompatable.
```
AddVal(val)
```
AddVal accepts positive and negative integers and floating point values and adds them to the message
```
AddStr(str)
```
AddStr adds, a string, to the message
```
DtaSct(code)
```
DtaSct sets the message structure, this value **can** be send along with the data to communicate its structure to the reciever(s). the value send as struct can be interpreted by the users firmware and bound to the according message processing and data interpretation.

*accepts 0-255*
```
DtaReq(code)
```
DtaReq adds a data request command to the message. this code can be interpreted by the reciever and may be used to prompt it to return certain data.

*accepts 0-255*
```
AddERR(code)
```
add an error code to the message.

*accepts 0-255*
```
DtaID(code)
```
works simularly as DtaStct function but is meant to be inserted before a single package.

*accepts 0-255*

```
int addrs[16] = {5,8,32,75,0,0,0,0,0,0,0,0,0,0,0,0};
GenrMsg(addrs)
```
the GenrMsg function generates the bits to be transmitted. the function takes an array of integers and passes them on as recievers.

The array must be initialized to 0 and have 16 entities

## transmission
```
SendMsg(BpS)
```
Since this library is meant to work on any uC and different IDE's use different pin function commands, the actual transmitting is done with the SendMsg function and must be adapted by the user to the user's IDE.

The bitrate can be set when usign the function in bits per second.
```
//SendMsg function, user must put microcontroller specific lines in this function to work.
//TTL + CLK function! no RS485 (yet)
void SendMsg(int BpS){
    //convert bits per second to delay, in this case BitDelay is in mS
    int BitDelay = BpS / 1000;

    for(int i = 0; i < GetBit(-1); i++){
        //put here your uC's CLK pin HIGH line

        //put here your uC's delay line with "2 ms" as value
        
        //put here your uC's CLK pin LOW line

        if(GetBit[i] == 0){
            //put here your uC's TX pin LOW line

        }
        else if(GetBit[i] == 1){
            //put here your uC's TX pin HIGH line

        }
        //put here your uC's delay line with "BitDelay" as value
    
    }
}

void main(){
    StartCOM(2,6);          //start protocol, device address 2, protocol 6
    AddStr("Hello there");  //add string package

    int addrs[16] = {5,8,32,75,0,0,0,0,0,0,0,0,0,0,0,0};
    GenrMsg(addrs);         //generate message bits
    SendMsg(100);           //transmit message at a 100 bit/second bitrate
}
```

## recieving
---


# License
MIT License
Copyright (c) [2021] [Milos de Wit]

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
