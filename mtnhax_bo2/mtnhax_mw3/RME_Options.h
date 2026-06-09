#pragma once
#include "stdafx.h"

void Option_115CamoAll();

extern bool bUnlimitedAmmo;
void Option_UnlimitedAmmo(bool toggle);

void Option_CbufWrite(const char* cmd);

extern int iSelectedClient;
extern void Option_Client115Camo(int clientNum);
