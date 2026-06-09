#pragma once
#include "stdafx.h"

struct partyMember_s {
    char status;              //0x00
    char _0x0001[0x23];
    char headsetPresent;      //0x24
    char inLivePartyVoice;    //0x25
    char _0x0026[0xA];
    int  natType;             //0x30
    char _0x0034[0x4];
    unsigned long long XUID;  //0x38
    char gamertag[0x10];      //0x40
    char _0x0050[0x3C];
    char clanAbbrev[0x4];     //0x8C
    char _0x0090[0x20];
    unsigned int internalIP;  //0xB0
    unsigned int onlineIP;    //0xB4
    short sPort;              //0xB8
    BYTE macAddress[0x6];     //0xBA
    char _0x00C0[0x8];
    unsigned long long MachineID; //0xC8
    char _0x00D0[0x78];
    //0x148
};

struct partyData_s {
    char _0x0000[0x208];
    partyMember_s partyMember[18]; //0x208
    char _0x1918[0x7C80];
    int areWeHost;           //0x9598
    int joiningAnotherParty; //0x959C
    int inParty;             //0x95A0
    int party_systemActive;  //0x95A4
    char _0x95A8[0x1C];
    int partyId;             //0x95C4
    char _0x95C8[0x1D30];
};

struct cg_s {
    int clientNumber;    //0x0000
    int localClientNum;  //0x0004
    char _0x0008[0x20];
};

extern partyMember_s partyMember[18];

class Structs {
public:
    static cg_s* get_cg();
    static void  UpdateStructures();
};
