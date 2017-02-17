//////////////////////////////////////////////////////////////////////
// LastManStanding.h
#ifndef LASTMANSTANDING_H
#define LASTMANSTANDING_H

#define LMS_MAX_USER_PER_ROOM 50
#define LMS_MAX_ROOMS 20
#define LMS_OPENMINUTE 00
struct LMS_DATA
{
	int iIndex;
	int iScore;
	bool bWinner;
	int iDeaths;
};
// spawn map 40
// x 234-236
// y 20-30
struct LMS_CFG
{
	bool bEnable;
	int iDuration;
	int iPVPMap;
	int iPVPMapX1;
	int iPVPMapY1;
	int iPVPMapX2;
	int iPVPMapY2;
	int iRegTime;
	int iNPCId;
	int iRegNPCMap;
	int iRegNPCX;
	int iRegNPCY;
	int iRegNPCDir;
	int iDieCount;
	int iMinPlayers;
	int iMaxPlayers;
	int iReqItem;
	int iReqItemCount;
	int iReqItemCat;
	int iReqItemIdx;
	int iReqZen;
};
struct sLMS_START_TIME
{
	BYTE StartHour;
	BYTE StartMinute;
};
struct s_LMS_Rooms
{
	short MinReset;
	short MaxReset;
	BYTE Map;
	BYTE X1;
	BYTE Y1;
	BYTE X2;
	BYTE Y2;
	int regCount;
	int m_iLiveUser;
	int m_iEventCoolDown;
	BYTE bState;
	LMS_DATA m_Data[LMS_MAX_USER_PER_ROOM];
	
};
class CLastManStanding
{
private:
	
public:
	CLastManStanding();
	~CLastManStanding();

	void LoadConfig(LPSTR file);
	void Init();
	int AddUser(int index);
	void DelUser(int index);
	void UserQuit(int index);
	void Run();
	void UserDie(int aIndex,int aTargetIndex);
	void RewardUser(int aIndex);
	void MoveAllUser(int Room);
	void EndEvent(int room);
	void AllPlayerMsgSend( char* chatmsg);
	void SendToPlayers(LPBYTE lpMsg, int iSize);
	void SpawnRegNPC();
	// [0 - Inactive] [1 - Registering 15 minutes] [2 - move all players to eventmap ( 1 min ) ] [3 - event running]
	LMS_CFG m_Cfg;
	//LMS_DATA m_Data[500];
	s_LMS_Rooms m_Rooms[LMS_MAX_ROOMS];
	int m_iRoomCount;
	bool bTimerSent;
	std::vector<sLMS_START_TIME> m_vtStartList;
	int iRegNpcIndex;

}; extern CLastManStanding g_LastManStanding;

#endif

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

