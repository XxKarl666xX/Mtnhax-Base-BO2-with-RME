#include "stdafx.h"
#include "RME_Options.h"

//example for a single set option, something you only set once when you use the option, here it actually is applying the effect to every client
void Option_115CamoAll()
{
    RME_Setup();
    for (int i = 0; i < 18; i++)
        RME_WriteByte(gClient(i) + 0x2DB, 0x2B);
}

//example for a threaded option, something that can run continuesly while other stuff happens, really good when you want options that re-apply some effect (god mode, ammo, speed, noclip, forgemode and so on)
bool bUnlimitedAmmo = false;
static const int  CLIP_AMMO_TARGET     = 0x45; //nice
//memory where we can see our loccal current ammo, could also do tacticals and lethals (something you could right now search for and try to get it to work, its a easy task to see if you truly wanna do this sort of developing)
static const DWORD ADDR_PRIMARY_CLIP   = 0x82C9AB44;
static const DWORD ADDR_SECONDARY_CLIP = 0x82C9AB40;

DWORD WINAPI AmmoRefillThread(LPVOID)
{
    while (bUnlimitedAmmo)
    {
        int iClientNum    = Structs::get_cg()->clientNumber;
        int primaryClip   = *(int*)ADDR_PRIMARY_CLIP;
        int secondaryClip = *(int*)ADDR_SECONDARY_CLIP;

        if (primaryClip < CLIP_AMMO_TARGET)
        {
            RME_Setup();
            RME_WriteByte(gClient(iClientNum) + 0x43F, CLIP_AMMO_TARGET);
        }
        if (secondaryClip < CLIP_AMMO_TARGET)
        {
            RME_Setup();
            RME_WriteByte(gClient(iClientNum) + 0x43B, CLIP_AMMO_TARGET);
        }
        Sleep(100);
    }
    return 0;
}

void Option_UnlimitedAmmo(bool toggle)
{
    bUnlimitedAmmo = toggle;
    if (toggle)
        CreateThread(NULL, 0, AmmoRefillThread, NULL, 0, NULL);
}

//general dvar write
void Option_CbufWrite(const char* cmd)
{
    RME_Setup();
    RME_WriteString(0x837B3F50, cmd);
    RME_WriteInt(0x837F683C + 0x8, strlen(cmd));
}

// seperate client option
int iSelectedClient = 0;

void Option_Client115Camo(int clientNum)
{
    RME_Setup();
    RME_WriteByte(gClient(clientNum) + 0x2DB, 0x2B);
}