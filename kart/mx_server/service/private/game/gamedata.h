#ifndef _gamedata_service_H_
#define _gamedata_service_H_

struct CQuestData
{
	ucINT m_iQuestID;
	ucINT m_iType;
	ucINT m_iAtOnce;
	ucINT m_iNeedNum;
	ucINT m_iDeadEnd;
	ucINT m_iAllLength;
};

struct CRoomData
{
	ucINT mapid;
};

struct CGameData
{
	UCIntVoidMap	QuestMap;
	UCIntVoidMap	MainQuestMap;
	UCIntVoidMap	LongShortQuestMap[3];
	UCIntVoidMap	EmergencyQuestMap;


	UCIntVoidMap	Map_RoomMap;
	CGameData()
	{
		UCCsv	csv;
		csv.ReadFromFile("../../csv/task.csv");
		ucINT row = csv.GetRowSize();
		for (ucINT i=5;i<=row;i++)
		{
			CQuestData* quest = new CQuestData;

			csv.GetInt(i, 1, quest->m_iQuestID);
			csv.GetInt(i, 3, quest->m_iType);
			csv.GetInt(i, 4, quest->m_iAtOnce);
			csv.GetInt(i, 7, quest->m_iNeedNum);
			csv.GetInt(i, 8, quest->m_iDeadEnd);
			csv.GetInt(i, 10, quest->m_iAllLength);
			QuestMap.Add(quest->m_iQuestID, quest);

			if (quest->m_iQuestID > 1000)
			{
				EmergencyQuestMap.Add(quest->m_iQuestID, quest);
			}
			else
			{
				MainQuestMap.Add(quest->m_iQuestID, quest);
				LongShortQuestMap[quest->m_iType].Add(quest->m_iQuestID, quest);
			}
		}


		csv.ReadFromFile("../../csv/room.csv");
		row = csv.GetRowSize();
		for (ucINT i=5;i<=row;i++)
		{
			CRoomData* room = new CRoomData;
			csv.GetInt(i, 1, room->mapid);
			Map_RoomMap.Add(room->mapid, room);
		}
	}

	~CGameData()
	{

	}
	void RemoveAll()
	{
		for (ucINT i=QuestMap.GetSize()-1;i>=0;i--)
		{
			CQuestData* pdata = (CQuestData*)QuestMap.GetValueAt(i);
			if (pdata)
			{
				delete pdata;
			}
		}
		QuestMap.RemoveAll();
		MainQuestMap.RemoveAll();
		EmergencyQuestMap.RemoveAll();

		for (ucINT i=Map_RoomMap.GetSize()-1;i>=0;i--)
		{
			CRoomData* pdata = (CRoomData*)Map_RoomMap.GetValueAt(i);
			if (pdata)
			{
				delete pdata;
			}
		}
		Map_RoomMap.RemoveAll();
	}

	CQuestData* GetQuestInfo(ucINT questid)
	{
		ucINT pos = QuestMap.FindKey(questid);
		if (pos < 0)
			return 0;
		CQuestData* quest = (CQuestData*)QuestMap.GetValueAt(pos);
		return quest;
	}

	CRoomData* GetRoomInfo(ucINT mapid)
	{
		ucINT pos = Map_RoomMap.FindKey(mapid);
		if (pos < 0)
			return 0;
		CRoomData* room = (CRoomData*)Map_RoomMap.GetValueAt(pos);
		return room;
	}
};
extern CGameData sGameData;
#endif	//_moba_service_H_