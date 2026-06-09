#include "stdafx.h"
#include "RME_Options.h"

extern DWORD originalInstructions[4];
bool shouldUnload = false;
extern HMODULE gSelfHandle;
DWORD WINAPI UnloadThread(LPVOID);


XINPUT_STATE input;

DWORD(__cdecl *Address)() = (DWORD(__cdecl *)())ResolveFunction("xam.xex", 0x1CF);

bool dash, hax;

VOID Menu_PaintAllStub(DWORD r3, DWORD r4) {
	__asm {
		li r3, 3
		nop
		nop
		nop
		nop
		nop
		nop
		blr
	}
}

#define FONT_NORMAL				"fonts/bigDevFont"

int snprintf(char * buffer, size_t len, const char * fmt, ...) {
	va_list args;
	int i;
	va_start(args, fmt);
	i = vsnprintf(buffer, len, fmt, args);
	va_end(args);
	return i;
}

typedef struct { float r, g, b, a; } Color;
Color MainColor = { 0, .5, 1, 1 };
Color BackgroundColor = { 0, 0, 0, 1 };
Color White = { 1, 1, 1, 1 };
Color Blue = { 0, 0, 1, 1 };
Color OptionColor[99];

namespace data {
	bool menuOpen;
	int menuTimer, curScroll, maxScroll;
	char* curMenu = "Main Menu";
	int maxOptions = 8;
}

namespace pointers {
	void(*R_AddCmdDrawText)(const char *text, int maxChars, void *font, float x, float y, float xScale, float yScale, float rotation, const float *Color, int style) = (void(*)(const char *, int, void *, float, float, float, float, float, const float *, int))0x828B8BA0;
	void(*R_AddCmdDrawStretchPic)(float x, float y, float w, float h, float s0, float t0, float s1, float t1, const float *Color, void *material) = (void(*)(float, float, float, float, float, float, float, float, const float *, void *))0x828B86C0;
	void*(*R_RegisterFont)(const char *font, int imageTrack) = (void*(*)(const char *, int))0x82275F78;
	void*(*Material_RegisterHandle)(const char *name, int imageTrack) = (void*(*)(const char *, int))0x828B78F0;
	bool(*Dvar_GetBool)(const char *dvarName) = (bool(*)(const char *))0x824964C0;
	void*(*Cbuf_AddText)(int localClientNum, const char *text) = (void*(*)(int, const char *))0x824015E0;
	int(*Dvar_FindVar)(const char* dvarName) = (int(*)(const char*))0x82496430;
}

char* Dvar_GetString(char* dvarName)
{
    int dvar_t = pointers::Dvar_FindVar(dvarName);
    if (dvar_t == 0)
        return "";
    return (char*)(*(int*)(dvar_t + 0x18));
}

void drawShader(float x, float y, float w, float h, char *material, Color Color) {
	pointers::R_AddCmdDrawStretchPic(x, y, w, h, 0, 0, 1, 1, (float*)&Color, pointers::Material_RegisterHandle(material, 0));
}

void drawText(const char *text, float x, float y, char *font, float fontSize, Color Color) {
	pointers::R_AddCmdDrawText(text, 0x7FFFFFFF, pointers::R_RegisterFont(font, 0), x, y, fontSize, fontSize, 0, (float*)&Color, 0);
}

void Scroller() {
	for (int x = 1; x < 99; x++) {
		if (x == data::curScroll) OptionColor[data::curScroll] = MainColor; // Color for selected option
		else if (x != data::curScroll) OptionColor[x] = White; // Color for unselected options
	}
}

const char *arraySm(int arrayName, char *zero, char *first, char *second) {
	switch (arrayName) {
	case 0:
		return zero;
	case 1:
		return first;
	case 2:
		return second;
	}
}

const char *arrayMd(int arrayName, char *zero, char *first, char *second, char *third) {
	switch (arrayName) {
	case 0:
		return zero;
	case 1:
		return first;
	case 2:
		return second;
	case 3:
		return third;
	}
}

const char *arrayLg(int arrayName, char *zero, char *first, char *second, char *third, char *fourth) {
	switch (arrayName) {
	case 0:
		return zero;
	case 1:
		return first;
	case 2:
		return second;
	case 3:
		return third;
	case 4:
		return fourth;
	}
}

void drawOption(char* Option) {
	data::maxScroll++;
	if (data::curScroll <= data::maxOptions && data::maxScroll <= data::maxOptions) drawText(Option, 555, ((data::maxScroll * 26) + 85), FONT_NORMAL, 0.8, OptionColor[data::maxScroll]);
	else if ((data::maxScroll >(data::curScroll - data::maxOptions)) && data::maxScroll <= data::curScroll) drawText(Option, 555, (((data::maxScroll - (data::curScroll - data::maxOptions)) * 26) + 85), FONT_NORMAL, 0.8, OptionColor[data::maxScroll]);
}

void drawCharOption(char *Option, const char *buf) {
	data::maxScroll++;
	if (data::curScroll <= data::maxOptions && data::maxScroll <= data::maxOptions) {
		drawText(Option, 555, ((data::maxScroll * 26) + 85), FONT_NORMAL, 0.8, OptionColor[data::maxScroll]);
		drawText(buf, 720, ((data::maxScroll * 26) + 85), FONT_NORMAL, 0.8, OptionColor[data::maxScroll]);
	}
	else if ((data::maxScroll > (data::curScroll - data::maxOptions)) && data::maxScroll <= data::curScroll) {
		drawText(Option, 555, (((data::maxScroll - (data::curScroll - data::maxOptions)) * 26) + 85), FONT_NORMAL, 0.8, OptionColor[data::maxScroll]);
		drawText(buf, 720, (((data::maxScroll - (data::curScroll - data::maxOptions)) * 26) + 85), FONT_NORMAL, 0.8, OptionColor[data::maxScroll]);
	}
}

void drawBoolOption(char* Option, bool Bool) {
	char buf[30];
	if (Bool) {
		snprintf(buf, sizeof(buf), "^2ON", Option);
		drawCharOption(Option, buf);
	}
	else {
		snprintf(buf, sizeof(buf), "^1OFF", Option);
		drawCharOption(Option, buf);
	}
}

VOID startMenu() {
	data::curScroll = 1;
	if (!data::menuOpen) data::menuOpen = true;
	else data::menuOpen = false;		
}

VOID initializeMenu(char* menuName) {
	data::curMenu = menuName;
	data::curScroll = 1;
}

VOID renderMenu() {
	drawShader(548, 58, 304, 254, "white", MainColor);
	drawShader(550, 60, 300, 250, "white", BackgroundColor);
	drawText("BO2 RME Menu", 590, 80, FONT_NORMAL, 0.9, MainColor);
}

VOID renderOptions() {
	if (data::curMenu == "Main Menu") {
		
		drawOption("Client List");
		drawOption("Sub Menu 2");
		drawOption("Sub Menu 3");
		drawOption("Unload Menu");
		drawOption("115 Camo All");
		drawBoolOption("Unlimited Ammo", bUnlimitedAmmo);
		drawOption("Super Speed Infection");

		//drawOption("");
		
	}
	else if (data::curMenu == "Client List")
{
    char* hostName = Dvar_GetString("party_hostname");
    int hostIndex  = -1;
    int localClient = Structs::get_cg() ? Structs::get_cg()->clientNumber : -1;

    for (int j = 0; j < 18; j++)
    {
        if (partyMember[j].XUID != 0 && !strcmp(partyMember[j].gamertag, hostName))
        { hostIndex = j; break; }
    }

    for (int i = 0; i < 18; i++)
    {
        if (partyMember[i].XUID == 0 || partyMember[i].gamertag[0] == '\0')
        {
            drawOption("^8[Empty Slot]");
            continue;
        }

        char name[64];
        const char* prefix = "";
        if      (i == localClient) prefix = "^7[^2YOU^7] ";
        else if (i == hostIndex)   prefix = "^7[^1HOST^7] ";

        if (partyMember[i].clanAbbrev[0] != '\0')
            snprintf(name, sizeof(name), "%s[%s] %s", prefix, partyMember[i].clanAbbrev, partyMember[i].gamertag);
        else
            snprintf(name, sizeof(name), "%s%s", prefix, partyMember[i].gamertag);

        drawOption(name);
    }
}
else if (data::curMenu == "Client Options")
{
    char header[64];
    snprintf(header, sizeof(header), "Client: %s", partyMember[iSelectedClient].gamertag);
    drawText(header, 555, 95, FONT_NORMAL, 0.7, White);
    drawOption("115 Camo");
    drawOption("Ban Client");
}
	else if (data::curMenu == "Sub Menu 2") {
		drawOption("Sub Option 1");
		drawOption("Sub Option 2");
		drawOption("Sub Option 3");
		drawOption("Sub Option 4");
		drawOption("Sub Option 5");
		drawOption("Sub Option 6");
		drawOption("Sub Option 7");
		drawOption("Sub Option 8");
		drawOption("Sub Option 9");
		drawOption("Sub Option 10");
	}
	else if (data::curMenu == "Sub Menu 3") {
		drawOption("Sub Option 1");
		drawOption("Sub Option 2");
		drawOption("Sub Option 3");
		drawOption("Sub Option 4");
		drawOption("Sub Option 5");
		drawOption("Sub Option 6");
		drawOption("Sub Option 7");
		drawOption("Sub Option 8");
		drawOption("Sub Option 9");
		drawOption("Sub Option 10");
	}
}

VOID selectOptions() {
	if (data::curMenu == "Main Menu") {
		switch (data::curScroll) {
		case 1: initializeMenu("Client List"); Structs::UpdateStructures(); break;
		case 2: initializeMenu("Sub Menu 2"); break;
		case 3: initializeMenu("Sub Menu 3"); break;
		case 4: CreateThread(NULL, 0, UnloadThread, NULL, 0, NULL); break;
		case 5: Option_115CamoAll(); break;
		case 6: bUnlimitedAmmo = !bUnlimitedAmmo;Option_UnlimitedAmmo(bUnlimitedAmmo); break;
		case 7: Option_CbufWrite("g_speed 400"); break;

		}
	}
	else if (data::curMenu == "Client List")
{
	Structs::UpdateStructures();
    int count = 0;
    for (int i = 0; i < 18; i++)
    {
        count++;
        if (count == data::curScroll)
        {
            if (partyMember[i].XUID == 0 || partyMember[i].gamertag[0] == '\0')break;
            iSelectedClient = i;
            initializeMenu("Client Options");
            break;
        }
    }
}
else if (data::curMenu == "Client Options")
{
    switch (data::curScroll)
    {
    case 1: Option_Client115Camo(iSelectedClient); break;
    case 2:char kickCmd[32];snprintf(kickCmd, sizeof(kickCmd), "banClient %d", iSelectedClient);Option_CbufWrite(kickCmd);break;
    }
}
	else if (data::curMenu == "Sub Menu 2") {
		switch (data::curScroll) {
		case 1: break;
		case 2: break;
		case 3: break;
		case 4: break;
		case 5: break;
		case 6: break;
		case 7: break;
		case 8: break;
		case 9: break;
		case 10: break;
		}
	}
	else if (data::curMenu == "Sub Menu 3") {
		switch (data::curScroll) {
		case 1: break;
		case 2: break;
		case 3: break;
		case 4: break;
		case 5: break;
		case 6: break;
		case 7: break;
		case 8: break;
		case 9: break;
		case 10: break;
		}
	}
}

VOID exitMenu() {
	if (data::curMenu == "Main Menu") startMenu();
	else if (data::curMenu == "Sub Menu 1") initializeMenu("Main Menu");
	else if (data::curMenu == "Sub Menu 2") initializeMenu("Main Menu");
	else if (data::curMenu == "Sub Menu 3") initializeMenu("Main Menu");
	else if (data::curMenu == "Client List")   initializeMenu("Main Menu");
	else if (data::curMenu == "Client Options") initializeMenu("Client List");
}

VOID paintallHook(DWORD r3, DWORD r4) {
    Menu_PaintAllStub(r3, r4);

    if (data::menuOpen) {
		data::maxScroll = 0;
        renderMenu();
        Scroller();
        renderOptions();
        drawText("Press LT+Dpad Left to Close", 800, 710, FONT_NORMAL, 0.7, White);
    }
    else {
        drawText("Press LT+Dpad Left to Open", 800, 710, FONT_NORMAL, 0.7, White);
    }

    ZeroMemory(&input, sizeof(XINPUT_STATE));
    XInputGetState(0, &input);
    data::menuTimer += 1;

    if (data::menuTimer > 20) {
        if ((input.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT) && input.Gamepad.bLeftTrigger > 30) {
			startMenu();
			data::menuTimer = 0;
		}
        else if (input.Gamepad.wButtons == XINPUT_GAMEPAD_DPAD_UP && data::menuOpen) {
            data::curScroll--;
            if (data::curScroll < 1) data::curScroll = data::maxScroll;
            data::menuTimer = 0;
        }
        else if (input.Gamepad.wButtons == XINPUT_GAMEPAD_DPAD_DOWN && data::menuOpen) {
            data::curScroll++;
            if (data::curScroll > data::maxScroll) data::curScroll = 1;
            data::menuTimer = 0;
        }
        else if (input.Gamepad.wButtons == XINPUT_GAMEPAD_A && data::menuOpen) {
            selectOptions();
            data::menuTimer = 0;
        }
        else if (input.Gamepad.wButtons == XINPUT_GAMEPAD_B && data::menuOpen) {
            exitMenu();
            data::menuTimer = 0;
        }
    }
}


