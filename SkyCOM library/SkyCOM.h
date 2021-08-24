#ifndef SkyCOM_H_   /* Include guard */
#define SkyCOM_H_

void StartCOM(int addr, int protocol);

void DtaID(int val);
void DtaSct(int val);
void DtaReq(int val);

void AddErr(int A, int B);

void AddStr(char Msg[]);
void AddVal(float val);

void GenrMsg(int RX_addrs[16]);

int  GetBit(int bit);

#endif