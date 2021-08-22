# SkyCOM

SkyCOM is an advanced serial communications protocol for communication between microcontroller systems on a network.

Made after getting ptsd from working with RS232 and intended for industrial machine applications or microcontroller networks where commands and values need to be send on a network.

this project is a work in progress and under active development. Not finished yet, USE AT OWN RISK!

# how to use the SkyCOM library
```
StartCOM(addr, version)
```
this sets the device address, which, at the moment, is static and the users firmware version. this is all included in the protocol so if interpertation of structures or other data changes over time, this is a build in way to make sure devices will be incompatable.
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
DtaSct sets the message structure, this is an optional value send along with the data setting (part of) its structure. the value send as struct can be interpreted by the users firmware and bound to according message processing.
```
DtaReq(code)
```
DtaReq sends a data request with a data code. this code is interpreted by the users firmware and you can use this for infrequent data requests. Can be used in combination with a structure
```
AddERR(code)
```
add an error code to the message.
```
DtaID(code)
```
Same function as a struct and could be used to extend funtion of a struct, but it's main purpouse is to send an ID along with known values.

# License
MIT License
Copyright (c) [2021] [Milos de Wit]

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
