#include "SkyCOM.h"
#include <stdio.h>

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
    StartCOM(2,6);      //start protocol, device address 2, protocol 6
    DtaSct(4);          //set data struct
    AddVal(32);         //add positive integer value
    AddVal(62.456);     //add positive floating point value
    DtaReq(8);          //request data 8, this can be anything. 8 can represent any type of data, for example the next value
    AddVal(-351.568);   //add negative floating point value
    AddStr("Hello there");

    int addrs[16] = {5,8,32,75,0,0,0,0,0,0,0,0,0,0,0,0};
    GenrMsg(addrs);
    SendMsg(100);
}