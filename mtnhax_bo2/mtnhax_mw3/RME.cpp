
#include "stdafx.h"
#include "RME.h"
//this is intentionally a very shitty RME implementation, each time you do a write it will freeze for a moment (specially seen when you do a option which does something to every client in the lobby)
//this can be fixed BUT thats something that YOU have to figure out by yourself
//as it stands this way of RME is actually WORSE than Sunset, as in Sunset it can queue the writes properly
//Tho that isnt the only problem, this code is as close to Sunset as it can be without any new bs, but Sunset has done some things which are just wasteful, find these things and you can have a faster RME than Sunsets normal RME
//as a tip: finding the flaw in here doesnt mean that you need to know fully how RME works to improve the speed


void  (*MSG_Init)      (msg_t*, char*, int)             = (void(*)(msg_t*, char*, int))             0x824191F8;
void  (*MSG_WriteLong) (msg_t*, int)                    = (void(*)(msg_t*, int))                    0x8241A440;
void  (*MSG_WriteByte) (msg_t*, int)                    = (void(*)(msg_t*, int))                    0x82416D98;
void  (*MSG_WriteShort)(msg_t*, int)                    = (void(*)(msg_t*, int))                    0x8241A3D8;
void  (*MSG_WriteData) (msg_t*, char*, int)             = (void(*)(msg_t*, char*, int))             0x82416DD0;
void  (*NET_SendPacket)(netsrc_t, int, char*, netAdr_t) = (void(*)(netsrc_t, int, char*, netAdr_t)) 0x8241D3C0;

static inline netchan_t* GetChan()
{
    return (netchan_t*)(*(int*)(0x82C70F50) + 0x24350);
}

static closest_addr_t GetClosestAddress(DWORD target_address, DWORD base_address)
{
    closest_addr_t closest_addr;
    closest_addr.address = target_address;
    closest_addr.closest_address = base_address;
    closest_addr.closest_offset = 0;
    closest_addr.closest_length = 0;

    for (BYTE offset = 0; offset < 0xFF; ++offset)
    {
        for (short length = SHRT_MIN; length < SHRT_MAX; ++length)
        {
            DWORD current_address =
                base_address + static_cast<DWORD>(offset) * length;

            if (current_address > target_address)
            {
                continue;
            }

            if (target_address - current_address <
                target_address - closest_addr.closest_address)
            {
                closest_addr.closest_address = current_address;
                closest_addr.closest_offset = offset;
                closest_addr.closest_length = length;
            }
        }
    }

    closest_addr.delta = target_address - closest_addr.closest_address;

    return closest_addr;
}



void RME_Setup()
{
    netchan_t* chan = (netchan_t*)(*(int*)(0x82C70F50) + 0x24350);
	msg_t msg = msg_t();
	CHAR buf[0x4F0];

	MSG_Init(&msg, buf, 0x4F0);
	MSG_WriteLong(&msg, chan->outgoingSequence | (chan->reliable_fragments ? 0xC0000000 : 0x80000000));

	MSG_WriteByte(&msg, 0xA);
	MSG_WriteByte(&msg, 0);
	MSG_WriteShort(&msg, 0x9A2D);
	MSG_WriteShort(&msg, 0x6);

	MSG_WriteByte(&msg, 0);
	MSG_WriteByte(&msg, 0);

	MSG_WriteLong(&msg, 0x83557000);

	NET_SendPacket(chan->sock, msg.cursize, msg.data, chan->remoteAddress);
}

bool setup_packet_header(msg_t* msg, int size, closest_addr_t closest_addr)
{
	netchan_t* chan = (netchan_t*)(*(int*)(0x82C70F50) + 0x24350);
	if (chan == nullptr)
		return false;

	MSG_WriteLong(msg, chan->outgoingSequence | (chan->reliable_fragments ? 0xC0000000 : 0x80000000));

	MSG_WriteByte(msg, closest_addr.closest_offset);
	MSG_WriteByte(msg, 0x00);
	MSG_WriteShort(msg, closest_addr.closest_length);
	MSG_WriteShort(msg, size + closest_addr.delta);
	return true;
}

void RME_Write(DWORD address, const void* value, int size)
{
    netchan_t* chan = (netchan_t*)(*(int*)(0x82C70F50) + 0x24350);

	closest_addr_t closest_addr = GetClosestAddress(address, 0x83557000);

	msg_t msg = msg_t();
	CHAR msg_buf[0x4F0];

	MSG_Init(&msg, msg_buf, sizeof(msg_buf));

	if (!setup_packet_header(&msg, size, closest_addr))
		return;

	CHAR data[0x500];
	memset(&data, 0, 0x500);
	memcpy(&data[closest_addr.delta], value, size);

	MSG_WriteData(&msg, data, size + closest_addr.delta);

	NET_SendPacket(chan->sock, msg.cursize, msg.data, chan->remoteAddress);
}