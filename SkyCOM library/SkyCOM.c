#include "SkyCOM.h"
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

bool DataBits[1024];    //holds the DataBits
bool SetpBits[188];     //holds address bits [2+10+4+(adresses(10)*16)+6+8]
bool sum[16] = {0};     //holds added binary values (checksum)
int  spot = 0;          //cursor in data array
int  PkgCnt = 0;        //count of value and string packages
int  DtaCnt = 0;        //count of commands
int  Ver = 1;           //set version
int TX_addr = 0;        //set default addresses

//to the power funtion bc I can't be arsed to include math.h
//I googled if it s assed or arsed
double power(int base, int exp){
    double result = 1.0;
    while (exp != 0) {
        result *= base;
        --exp;
    }
    return result;
}
//add two binary array values
void AddBins(bool a[16], bool b[16]){
	bool carry = 0;

    //no clue what the fuck is happening here, gotta figure it out one day
	for(int i = 16-1; i >= 0; i--){
		sum[i]=a[i] ^ b[i] ^ carry;
		carry=(a[i] & b[i]) + (carry*(a[i] ^ b[i]));
	}
    //if value more then full byte, add to next byte
    sum[0] = carry;
}

//convert value to bits in correct byte size and add to specific spot in DataBits or SetpBits array
void DECtoBIN(int val, int size, int cursor, bool arr){         //decimal value, byte size, location in array, array (0 = DataBits, 1 = SetpBits)
    bool BIN[size];     //holds local bits

    //initialize bits array because otherwise output will be fucky-wucky
    for(int i = 0; i < size; i++){
        BIN[i] = 0;
    }

    int j = size - 1;   //since counting from 0, counter = size -1

    //sore remainder in local bits array
    while(val > 0){
        BIN[j] = val %2;
        val = val / 2;
        j--;
    }

    //store bits in global DataBits/SetpBits array
    if(arr == 0){
        for(int k = 0; k < size; k++){
            DataBits[k+cursor] = BIN[k];
        }
    } else{
        for(int k = 0; k < size; k++){
            SetpBits[k+cursor] = BIN[k];
        }
    }
}
//glorified for loop
void ArrayArray(bool Byt[],int size){
    for(int i = 0; i < size; i++){
        DataBits[i + spot] = Byt[i];
    }
    spot = spot + size;
}
//check if value is an int or float
bool CheckDtaTpe(float val){

    int IntVal = val;               //store int of the float val
    float IntFloat = val - IntVal;  //convert the int val back to a float

    //if the value is equal to 0, it's an int, otherwise it's a float
    if(IntFloat == 0){
        return false;
    } else if(IntFloat != 0){
        return true;
    }
}
//return the ammount of 8bytes a value uses
int ValBytCntr(float val){
    int Len = 1;

    //calculate the ammount of bytes the value uses
    for(int i = 0; i < 8; i++){
        if(val < power(2.0, 8*(i+1))){
            break;
        }
        Len++;
    }
    return Len;
}
//add 4 bit command value and 8 bit value to array
void AddCmnd(bool Byt[4], int var){
    DtaCnt++;

    //store it in global DataBits array
    ArrayArray(Byt,4);

    DECtoBIN(var,8,spot,0);     //add actual bit value
    
    spot = spot + 8;            //move cursor 8byte
}

void StartCOM(int addr, int protocol){
    Ver = protocol;
    TX_addr = addr;
}

//add an ID to the message
void DtaID(int val){
    bool CmdCode[4] = {0,1,1,1};    //baked Data ID 4byte (7, Data ID)
    AddCmnd(CmdCode,val);
}
//add a struct to the message
void DtaSct(int val){           //structure ID
    bool CmdCode[4] = {0,1,0,1};    //baked struct 4byte (5, Structure set)
    AddCmnd(CmdCode,val);
}
//add a data request to the message
void DtaReq(int val){           //data ID
    bool CmdCode[4] = {1,0,0,1};    //baked data request 4byte (9, DataRequest)
    AddCmnd(CmdCode,val);
}
//add error code
void AddErr(int A, int B){
    DtaCnt++;
    
    bool Byt[4] = {0,1,1,0};    //baked error 4byte (6, error)
    ArrayArray(Byt,4);

    if(B == 0){
        DataBits[spot] = 0;
        spot++;
        DECtoBIN(A,9,spot,0);
        spot = spot + 9;
    } else{
        DataBits[spot] = 1;
        spot++;
        DECtoBIN(A,5,spot,0);
        spot = spot + 5;
        DECtoBIN(B,4,spot,0);
        spot = spot + 4;
    }
}

//add string to message
void AddStr(char Msg[]){
    bool SoP[4] = {0,0,0,1};    //baked start of string package byte
    bool EoP[4] = {1,1,1,0};    //baked end of package byte

    int len = strlen(Msg);     //get string length

    AddCmnd(SoP,len);           //use AddCmnd function bc adding a 4bit and 8bit value anyways

    //put ASCII values of the string characters bits array
    for(int i = 0; i < len; i++){
        DECtoBIN(Msg[i],8,spot,0);  //covert and add character to global data array
        spot = spot + 8;            //move cursor 8byte
    }

    //put end of package byte in bits array
    for(int i = 0; i < 4; i++){
        DataBits[i + spot] = EoP[i];
    }

    spot = spot + 4;    //move cursor half-byte
    PkgCnt++;           //add to package count
}
//add value to message
void AddVal(float val){

    bool DtaTpe;                    //1 = float, 0 = int
    bool Pol;

    if(val < 0){                //store, correct and check if value is negative
        Pol = false;
        val = val * -1;
    } else{
        Pol = true;
    }

    //if the value is equal to 0, it's an int, otherwise it's a float
    DtaTpe = CheckDtaTpe(val);

    bool SoP[6] = {0,0,1,0,DtaTpe,Pol};     //baked start of Val package byte + datatype bit + value polarity type
    bool EoP[4] = {1,1,1,0};                //baked end of package byte

    //put start of value package byte in bits array
    ArrayArray(SoP,6);

    //-----Integer---------------
    if(DtaTpe == 0){
        int DtaLen = ValBytCntr(val);   //stores the ammount of bytes the value takes up
        int Len = 8*DtaLen;

        DECtoBIN(DtaLen,4,spot,0);  //store size of value in 4 bytes
        spot = spot + 4;
        int IntVal = val;
        DECtoBIN(IntVal,Len,spot,0);
        spot = spot + Len;
    }
    //-----float---------------
    else{
        bool Int = false;
        int  Pnt = 0;

        //make value integer and store the decimal point location
        while(Int == false){
            val = val * 10;     //move decimal point (until val is an integer)

            int IntegerVal = val;               //store int of the float val
            float IntFlt = val - IntegerVal;    //convert the int val back to a float

            //if the value is equal to 0, it's an int, otherwise it's a float
            if(IntFlt == 0){
                Int = true;
            } else if(IntFlt != 0){
                Int = false;
            }
            Pnt++;
        }

        int DtaLen = ValBytCntr(val);   //stores the ammount of bytes the value takes up
        int Len    = 8 * DtaLen;

        DECtoBIN(DtaLen,4,spot,0);  //store size of value in a 4byte
        spot = spot + 4;

        DECtoBIN(Pnt,6,spot,0);     //store the decimal point spot in a 6byte
        spot = spot + 6;

        DECtoBIN(val,Len,spot,0);
        spot = spot + Len;
    }
    //--------------------

    //put end of package 4byte in bits array
    ArrayArray(EoP,4);
    PkgCnt++;
}

void GenrMsg(int RX_addrs[16]){     //address array, MUST be an initialized 16 enttity array
    int addrs;

    for(int i = 0; i < 16; i++){
        if(RX_addrs[i] == 0){
            addrs = i;
            break;
        } else{
            DECtoBIN(RX_addrs[i],10,16+(10*i),1);
        }
    }
    printf("\naddress count: %d",addrs);

    //calculate setup bit length (start bit + transmitter adress + reciever count + reciever addresses + version + package count) = (2 + 10 + 4 + 10*reciever_count) + 6 + 8)
    int SetpLen = 16+(10*addrs);

    //add transmitter address, protovol version and message package count
    DECtoBIN(TX_addr,10,2,1);   //place 2 bc at start of setup array, only start bits prior (2)
    DECtoBIN(Ver,6,SetpLen,1);
    
    if(PkgCnt == 0){
        SetpBits[SetpLen+6] = 1;
        DECtoBIN(DtaCnt,7,SetpLen+7,1);
    } else{
        SetpBits[SetpLen+6] = 0;
        DECtoBIN(PkgCnt,7,SetpLen+7,1);
    }
    SetpLen = SetpLen + 14;     //add 14 bits to SetpLen

    DECtoBIN(addrs,4,12,1);     //add reciever count

    bool SoM[2] = {1,1};        //start of message bits
    bool CKS[4] = {1,1,0,1};    //baked checksum bits

    //add checksum 4byte to array
    for(int i = 0; i < 4; i++){
        DataBits[i + spot] = CKS[i];
    }
    spot = spot + 4;
    
    for(int i = 0; i < 2; i++){
        SetpBits[i] = SoM[i];
    }

    int MsgLenght = SetpLen + spot;

    //merge setup and data arrays
    bool FullMessage[MsgLenght+20];     //lenght of both arrays, + 16 bit checksum + 4 bit end of message
    for(int i = 0; i < MsgLenght; i++){
        if(i < SetpLen){
            FullMessage[i] = SetpBits[i];
        } else{
            FullMessage[i] = DataBits[i-SetpLen];
        }
    }

    int MsgByteItr;     //message byte itterations

    if(CheckDtaTpe((MsgLenght+2)/8) == 0){
        MsgByteItr = (MsgLenght+2)/8;
    } else if(CheckDtaTpe((MsgLenght-2)/8) == 0){
        MsgByteItr = (MsgLenght-2)/8;
    }

    int BytCnt = 0;

    printf("\n");

    for(int j = 0; j < MsgByteItr; j++){
        bool BinVal[16] = {0};
        char str0[1];
        for(int i = 0; i < 8; i++){
            BinVal[i+8] = FullMessage[i+BytCnt];
            sprintf(str0, "%d", BinVal[i+8]);
            printf(str0);
        }
        printf(" ");
        AddBins(sum, BinVal);
        BytCnt = BytCnt + 8;
    }

    for(int i = 0; i < 16; i++){
        FullMessage[i+MsgLenght] = sum[i];
    }

    for(int i = 0; i < 4; i++){
        FullMessage[i+MsgLenght+16] = 1;
    }

////////////////////////////////////////////////////////////////
    
    char str1[1];
    char str2[1];
    char str3[1];
    char str4[1];

    printf("\n%d%s%d%s%d", SetpLen, " + ", spot, " = ", SetpLen+spot);
    printf("\n");
/*
    for(int i = 0; i < SetpLen; i++){
        sprintf(str1, "%d", SetpBits[i]);
        printf(str1);
        
    }

    for(int i = 0; i < spot; i++){
        sprintf(str2, "%d", DataBits[i]);
        printf(str2);
    }
*/
    printf("\n bytes >%d\n", MsgByteItr);

    for(int i = 0; i < MsgLenght+20; i++){
        sprintf(str3, "%d", FullMessage[i]);
        printf(str3);
    }
    printf("\n\n checksum >");

    for(int i = 0; i < 16; i++){
        sprintf(str4, "%d", sum[i]);
        printf(str4);
    }

    printf("\n");

////////////////////////////////////////////////////////////////
    spot = 0;
    PkgCnt = 0;
}

int GetBit(int bit){
    if(bit == -1){
        return MsgLenght + 20;
    } else{
        return FullMessage[bit];
    }
}