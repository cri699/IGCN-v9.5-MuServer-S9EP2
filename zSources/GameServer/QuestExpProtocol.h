//////////////////////////////////////////////////////////////////////
// QuestExpProtocol.h
#ifndef QUESTEXPPROTOCOL_H
#define QUESTEXPPROTOCOL_H

#pragma once
#include "prodef.h"

struct PMSG_REQ_QUESTEXP
{
    PBMSG_HEAD2 h;
    DWORD dwQuestInfoIndexID;
    BYTE btResult;
};

struct PMSG_ANS_QUESTEXP
{
    PBMSG_HEAD2 h;
    BYTE btResult;
};

struct PMSG_REQ_QUESTEXP_COMPLETE
{
    PBMSG_HEAD2 h;
    DWORD dwQuestInfoIndexID;
};

struct PMSG_REQ_QUESTEXP_GIVEUP
{
    PBMSG_HEAD2 h;
    DWORD dwQuestInfoIndexID;
};

struct PMSG_REQ_QUESTEXP_ASK_COMPLETE
{
    PBMSG_HEAD2 h;
    DWORD dwQuestInfoIndexID;
};

struct PMSG_REQ_QUESTEXP_PROGRESS_LIST
{
    PBMSG_HEAD2 h;
    DWORD dwQuestInfoIndexID;
};

struct PMSG_REQ_QUESTEXP_PROGRESS_INFO
{
    PBMSG_HEAD2 h;
    DWORD dwQuestInfoIndexID;
};

struct PMSG_REQ_EVENT_ITEM_EP_LIST
{
    PBMSG_HEAD2 h;
};

struct PMSG_REQ_NPC_QUESTEXP
{
    PBMSG_HEAD2 h;
};

struct PMSG_REQ_ATTDEF_POWER_INC
{
    PBMSG_HEAD2 h;
};

void CGReqQuestSwitch(PMSG_REQ_QUESTEXP *pMsg, int aIndex);
void CGReqQuestProgress(PMSG_REQ_QUESTEXP *pMsg, int aIndex);
void CGReqQuestComplete(PMSG_REQ_QUESTEXP_COMPLETE *pMsg, int aIndex);
void CGReqQuestGiveUp(PMSG_REQ_QUESTEXP_GIVEUP *pMsg, int aIndex);
void CGReqTutorialKeyComplete(PMSG_REQ_QUESTEXP_ASK_COMPLETE *pMsg, int aIndex);
void CGReqProgressQuestList(PMSG_REQ_QUESTEXP_PROGRESS_LIST *pMsg, int aIndex);
void CGReqProgressQuestInfo(PMSG_REQ_QUESTEXP_PROGRESS_INFO *pMsg, int aIndex);
void CGReqEventItemQuestList(PMSG_REQ_EVENT_ITEM_EP_LIST *pMsg, int aIndex);

void CGReqQuestExp(PMSG_REQ_NPC_QUESTEXP *pMsg, int aIndex);
void CGReqAttDefPowerInc(PMSG_REQ_ATTDEF_POWER_INC *pMsg, int aIndex);

#endif

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

