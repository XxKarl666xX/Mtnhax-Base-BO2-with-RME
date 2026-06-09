#include "structs.h"
#include "stdafx.h"

partyMember_s partyMember[18] = {};

static partyData_s* get_active_party()
{
    partyData_s* partyData  = (partyData_s*)(0x82CA0898);
    partyData_s* lobbyData  = (partyData_s*)(0x82CAC160);
    bool inParty = partyData->party_systemActive == 1 && partyData->inParty;
    return inParty ? partyData : lobbyData;
}

cg_s* Structs::get_cg()
{
    return (cg_s*)*(int*)(0x82BBAE68);
}

void Structs::UpdateStructures()
{
    partyData_s* party = get_active_party();
    if (!party) return;
    for (int i = 0; i < 18; i++)
        memcpy(&partyMember[i], &party->partyMember[i], sizeof(partyMember_s));
}