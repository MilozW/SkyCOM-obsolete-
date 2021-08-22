#include "SkyCOM.h"
#include <stdio.h>

void main(){
    StartCOM(2,6);      //start protocol, device address 2, protocol 6
    DtaSct(4);          //set data struct
    AddVal(32);         //add positive integer value
    AddVal(62.456);     //add positive floating point value
    DtaReq(8);          //request data 8, this can be anything. 8 can represent any type of data, for example the next value
    AddVal(-351.568);   //add negative floating point value
    AddStr("Hello there");

    int addrs[16] = {5,8,32,75,0,0,0,0,0,0,0,0,0,0,0,0};
    SendMsg(addrs);
}