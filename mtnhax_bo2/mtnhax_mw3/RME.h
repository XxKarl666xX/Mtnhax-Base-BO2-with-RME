#pragma once
#include "stdafx.h"
#include <xtl.h>



typedef unsigned char      BYTE;
typedef unsigned short     WORD;
typedef unsigned long      DWORD;
typedef unsigned long long QWORD;

enum netsrc_t     { NS_CLIENT = 0, NS_SERVER = 1 };
enum netadrtype_t { NA_BOT=0, NA_BAD=1, NA_LOOPBACK=2, NA_BROADCAST=3, NA_IP=4 };

//needed structs
struct netAdr_t {
    unsigned char IPAddress[4];
    unsigned int  port;
    netadrtype_t  type;
    netsrc_t      localNetID;
    unsigned int  serverID;
};

struct msg_t {
    int      overflowed, readOnly;
    char*    data;
    char*    splitData;
    int      maxsize, cursize, splitSize, readcount, bit, lastEntityRef, flush;
    netsrc_t targetLocalNetID;
};

struct netchan_t {
    int      outgoingSequence;
    netsrc_t sock;
    char     _0x08[0x8];
    netAdr_t remoteAddress;
    char     _0x24[0x10];
    int      unsentFragments, unsentOnLoan, unsentFragmentStart, unsentLength;
    char     _0x44[0x8];
    int      reliable_fragments;
    char     fragment_send_count[128];
    unsigned int fragment_ack[4];
    int      lowest_send_count;
    char     _0xE4[0x5E0];
};

struct closest_addr_t {
    unsigned int address, closest_address, closest_offset;
    short        closest_length;
    int          delta;
};

// pointer declaration
extern void  (*MSG_Init)      (msg_t*, char*, int);
extern void  (*MSG_WriteLong) (msg_t*, int);
extern void  (*MSG_WriteByte) (msg_t*, int);
extern void  (*MSG_WriteShort)(msg_t*, int);
extern void  (*MSG_WriteData) (msg_t*, char*, int);
extern void  (*NET_SendPacket)(netsrc_t, int, char*, netAdr_t);

// core
void RME_Write(DWORD address, const void* value, int size);
void RME_Setup();
bool setup_packet_header(msg_t* msg, int size, closest_addr_t closest_addr);

// write helper
inline void RME_WriteByte(DWORD address, BYTE value)
{
	RME_Write(address, &value, sizeof(BYTE));
}

inline void RME_WriteShort(DWORD address, short value)
{
    RME_Write(address, &value, sizeof(short));
}

inline void RME_WriteInt(DWORD address, DWORD value)
{
    RME_Write(address, &value, sizeof(DWORD));
}

inline void RME_WriteFloat(DWORD address, float value)
{
    RME_Write(address, &value, sizeof(float));
}

inline void RME_WriteInt64(DWORD address, QWORD value)
{
    RME_Write(address, &value, sizeof(QWORD));
}

inline void RME_WriteString(DWORD address, const char* value)
{
    RME_Write(address, value, strlen(value) + 1);
}

//playerstate struct
inline DWORD gClient(int clientNum) { return 0x83551A10 + (clientNum * 0x57F8); }
