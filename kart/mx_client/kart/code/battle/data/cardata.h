#ifndef UCCARDATA
#define UCCARDATA


//最大车重
#define WEIGHT_INC		1.0f

#define JUMP_BAS		40.0f
#define JUMP_INC		40.0f
#define JUMP_BIG		100.0f

//最快车速
#define VSPEED_BAS		140.0f
#define VSPEED_INC		100.0f
#define VSPEED_UP		20.0f

//加速度增益
#define ACCELE_BAS		0.4f		//加速度基础值
#define ACCELE_INC		0.9f		//加速度增强值

//转向相关
#define HANDLE_BAS		0.012f
#define HANDLE_INC		0.006f
//最大转向角度
#define HANDLE_MAX		0.20f

//转向恢复
#define HANDLE_FIX_BAS	0.8f
#define HANDLE_FIX_INC	0.8f


//转向速度
#define DRIFT_SPD		0.02f
#define DRIFT_INC		0.01f
#define DRIFT_BAS		0.8f

//能量回复
#define POWER_BAS		16.0f
#define POWER_INC		8.0f

#define DRIFT_POWER		100000

#define DFBASE_BAS		10			//计数器
#define DFBASE_INC		20

#define DFREAD_BAS		10			//计数器
#define DFREAD_INC		10

//飞行物品	飞行速度
#define FLYITEM_SP		60.0f
#define FLYITEM6_SP		70.0f

#define MDFT_SPED	0.4f	//最大漂移速度


//飞行物品	爆炸范围	射程范围
#define FLYITEM_ED		600.0f
//飞行物品	存活时间
#define FLYITEM_TYPE5	300

#define FLYITEM_LT		100
//水球波及圈 存活时间
#define FLYITEM_ADDLT		80
#define FLYTIM_TIME			120
//云球 存活时间
#define FLYITEM_YunBall		640

//定时炸弹敏感范围
#define FLYITEM_DingShi		800.0f

//定时炸弹爆炸时间
#define FLYTIME_DingShi		100

//道具选择	切换时间
#define USERPANE_SP		100
//滑动系数
#define SLIP_DEFAULT	0.4f
//显示高速状态的速度百分比
#define SHOW_SPEEDUP	0.92f

//按键设计
ucINT				KEY1UP = 0xC8;			//ucTCHAR a = 'W'
ucINT				KEY1DOWN = 0xD0;
ucINT				KEY1LEFT = 0xCB;
ucINT				KEY1RIGHT = 0xCD;

ucINT				bKEY1UP = 0x26;
ucINT				bKEY1DOWN = 0x28;
ucINT				bKEY1LEFT = 0x25;
ucINT				bKEY1RIGHT = 0x27;

ucINT				KEY2UP = 'W';			//ucTCHAR a = 'W'
ucINT				KEY2DOWN = 'S';
ucINT				KEY2LEFT = 'A';
ucINT				KEY2RIGHT = 'D';

ucINT				bKEY2UP = 'W';
ucINT				bKEY2DOWN = 'S';
ucINT				bKEY2LEFT = 'A';
ucINT				bKEY2RIGHT = 'D';

ucINT				KEY1SHIFT = 0x2A;

ucINT				KEY1TOOL1 = 0x11;
ucINT				KEY1TOOL2 = 'S';
ucINT				KEY1TOOL3 = 'Z';//			
ucINT				KEY1TOOL4 = 0x09;//Tab键
ucINT				KEY1JUMP = 0x2E;
ucINT				KEY1SUPER = 'X';

ucINT				bKEY1SHIFT = 0x10;	//VK_SHIFT

ucINT				bKEY1TOOL1 = 0x11;	//VK_CONTROL	道具
ucINT				bKEY1TOOL2 = 'V';	//暂时没用
ucINT				bKEY1TOOL3 = 'Z';	//第一个技能			
ucINT				bKEY1TOOL4 = 0x09;//Tab键
ucINT				bKEY1JUMP = 'C';	//C
ucINT				bKEY1SUPER = 'X';	//第二个技能

ucINT				KEY2SHIFT = 0x36;

ucINT				KEY2TOOL1 = 0x11;
ucINT				KEY2TOOL2 = 'S';
ucINT				KEY2TOOL3 = 0xBE;
ucINT				KEY2JUMP = 0x33;
ucINT				KEY2SUPER = 0xBF;

ucINT				bKEY2SHIFT = 0x10;	//VK_SHIFT

ucINT				bKEY2TOOL1 = 0x11;	//VK_CONTROL
ucINT				bKEY2TOOL2 = 'B';	//暂时没用
ucINT				bKEY2TOOL3 = 0xBE;	//			
ucINT				bKEY2TOOL4 = 0x09;//Tab键
ucINT				bKEY2JUMP = 0xBC;	//
ucINT				bKEY2SUPER = 0xBF;	//

ucINT				MineHold0 = 0;
ucINT				MineHold1 = 0;

struct UCKeyState
{
	ucBOOL				IsUP;
	ucBOOL				IsDOWN;
	ucBOOL				IsLEFT;
	ucBOOL				IsRIGHT;
	ucBOOL				IsSHIFT;
	ucBOOL				IsTOOL1;

	UCDevice3D*			Device3D;
	UCKeyState()
	{
		IsUP = ucFALSE;
		IsDOWN = ucFALSE;
		IsLEFT = ucFALSE;
		IsRIGHT = ucFALSE;
		IsSHIFT = ucFALSE;
		IsTOOL1 = ucFALSE;

		Device3D = UIGetDevice3D();
	}

	ucBOOL	KEYIsUp()
	{
		if (Device3D->Input->IsKeyDown(KEY1UP) || Device3D->IsKeyDown(bKEY1UP) ||
			Device3D->Input->IsKeyDown(KEY2UP) || Device3D->IsKeyDown(bKEY2UP))
			return ucTRUE;

		//矢量摇杆
		ucJOYSTATE* JoyState = Device3D->Input->JoyState(0);
		if (JoyState != 0 && JoyState->Y == -1000)
			return ucTRUE;
		return IsUP;
	}

	ucBOOL	KEYIsDown()
	{
		if (Device3D->Input->IsKeyDown(KEY1DOWN) || Device3D->IsKeyDown(bKEY1DOWN) ||
			Device3D->Input->IsKeyDown(KEY2DOWN) || Device3D->IsKeyDown(bKEY2DOWN))
			return ucTRUE;

		ucJOYSTATE* JoyState = Device3D->Input->JoyState(0);
		if (JoyState != 0 && JoyState->Y == 1000)
			return ucTRUE;
		return IsDOWN;
	}

	ucBOOL	KEYIsLeft()
	{
		if (Device3D->Input->IsKeyDown(KEY1LEFT) || Device3D->IsKeyDown(bKEY1LEFT) ||
			Device3D->Input->IsKeyDown(KEY2LEFT) || Device3D->IsKeyDown(bKEY2LEFT))
			return ucTRUE;

		ucJOYSTATE* JoyState = Device3D->Input->JoyState(0);
		if (JoyState != 0 && JoyState->X == -1000)
			return ucTRUE;
		return IsLEFT;
	}

	ucBOOL	KEYIsRight()
	{
		if (Device3D->Input->IsKeyDown(KEY1RIGHT) || Device3D->IsKeyDown(bKEY1RIGHT) ||
			Device3D->Input->IsKeyDown(KEY2RIGHT) || Device3D->IsKeyDown(bKEY2RIGHT))
			return ucTRUE;

		ucJOYSTATE* JoyState = Device3D->Input->JoyState(0);
		if (JoyState != 0 && JoyState->X == 1000)
			return ucTRUE;
		return IsRIGHT;
	}

	ucBOOL	KEYIsJump()
	{
		if (Device3D->IsKeyDownOnce(KEY1JUMP) || Device3D->IsKeyDownOnce(bKEY1JUMP) ||
			Device3D->IsKeyDownOnce(KEY2JUMP) || Device3D->IsKeyDownOnce(bKEY2JUMP))
			return ucTRUE;

		ucJOYSTATE* JoyState = Device3D->Input->JoyState(0);
		if (JoyState != 0 && JoyState->Button[0] == 128)
			return ucTRUE;
		return ucFALSE;
	}

	ucBOOL	KEYIsDrift()
	{
		if (Device3D->IsKeyDown(KEY1SHIFT) || Device3D->IsKeyDown(bKEY1SHIFT) ||
			Device3D->IsKeyDown(KEY2SHIFT) || Device3D->IsKeyDown(bKEY2SHIFT))
			return ucTRUE;
	
		ucJOYSTATE* JoyState = Device3D->Input->JoyState(0);
		if (JoyState != 0 && JoyState->Button[0] == 128)
			return ucTRUE;
		return IsSHIFT;

	}

	ucBOOL	KEYIsTool1()
	{
		if (Device3D->IsKeyDownOnce(KEY1TOOL1) || Device3D->IsKeyDownOnce(bKEY1TOOL1) ||
			Device3D->IsKeyDownOnce(KEY2TOOL1) || Device3D->IsKeyDownOnce(bKEY2TOOL1))
			return ucTRUE;

		ucJOYSTATE* JoyState = Device3D->Input->JoyState(0);
		if (JoyState != 0 && JoyState->Button[1] == 128)
			return ucTRUE;
		return IsTOOL1;
	}

	ucBOOL	KEYIsReset()
	{
		if (Device3D->IsKeyDownOnce('R'))
			return ucTRUE;

		ucJOYSTATE* JoyState = Device3D->Input->JoyState(0);
		if (JoyState != 0 && JoyState->Button[9] == 128)
			return ucTRUE;
		return ucFALSE;
	}

	ucBOOL	KEYIsSpace()//实际为回车按键
	{
		if (Device3D->IsKeyDownOnce(0x0d))//
			return ucTRUE;

		ucJOYSTATE* JoyState = Device3D->Input->JoyState(0);
		if (JoyState != 0 && JoyState->Button[5] == 128)
			return ucTRUE;
		return ucFALSE;
	}

	ucBOOL	KEYIsEnter()
	{
		if (Device3D->IsKeyDownOnce(0x0d))
			return ucTRUE;
		return ucFALSE;
	}

	ucBOOL	KEYIsSuper()
	{
		if (Device3D->IsKeyDownOnce(KEY1SUPER) || Device3D->IsKeyDownOnce(bKEY1SUPER) ||
			Device3D->IsKeyDownOnce(KEY2SUPER) || Device3D->IsKeyDownOnce(bKEY2SUPER))
			return ucTRUE;
		return ucFALSE;
	}
};

UCKeyState	KeyState;

#define MESHTYPE				23
//物品分类
#define  ROWELEMENT     0
#define  MeshFace		1
#define  MeshHair		2
#define  MeshBody		3
#define  MeshPant		4
#define  MeshShoe		5
#define  MeshHand		6
#define  MeshCar		7
#define  MeshCarVent	8
#define  MeshBall		9
#define  MeshLicense	10
#define  PicGameTool	11
#define  PicMail		12
#define  PicExp			13
#define	 MeshRING0		14//性能元件：引擎
#define	 MeshRING1		15//性能元件：轮胎
#define	 MeshRING2		16//性能元件：底盘框架
#define	 MeshRING3		17//性能元件：转向
#define	 PicDraw		18//图纸
#define	 MeshSuit		19//套装
#define	 MeshNew		20
#define	 MeshLiBao		21//大礼包形式
#define	 MeshTaskTool	22//任务道具

struct cBaseInfo
{
	ucINT ID;
	ucINT Num;
	cBaseInfo()
	{
		ID = 0;
		Num = 0;
	}
};

class UCItemIno
{
public:
	ucINT	ItemID;
	ucINT	ItemType;

	ucINT	SexOn;
	ucINT	Price;
	ucINT	ByScore;				//是否积分购买0-游戏币；1-积分
	ucINT	BuyLevel;

	ucFLOAT	PercentBuy;
	ucFLOAT	PercentSell;

	ucINT	Validity;				//有效期（天）
	ucINT	Use;					//使用次数
	ucINT	DeleEnable;				//是否可以删除
	ucINT	TradeEnable;			//是否允许交易
	ucINT	LeiJia;					//是否可以累加
	ucINT	SaledScore;				//卖出可获的积分

	UCString	ItemName;
	UCString	ItemDescribe;
	UCString	FunctionName;
	ucINT		Show3D;				//物品显示方式1-3D；0-2D
	UCString	MeshPath;
	UCString	PicName;
	UCSize		PicSize;
	UCPoint		PicOffset;
	ucFLOAT		Distance;
	ucFLOAT		PosY;
	ucFLOAT		PosX;
	ucFLOAT		PosZ;				//实为RotY
	ucFLOAT		RotX;
	ucFLOAT		SellScale;			//商品陈列大小
	ucFLOAT		PutOnScale;			//商品装备以后大小

	UCString	sConsum;
	UCString	sSize;
	UCString	sPoint;

	UCVoidArray CombinedResList;
	UCItemIno()
	{
		ItemID = -1;
		SaledScore = 0;
		SexOn = 0;
		ItemType = 0;
	}
	~UCItemIno()
	{
		for (ucINT i = CombinedResList.GetSize() - 1; i >= 0; i--)
		{
			cBaseInfo* p = (cBaseInfo*)CombinedResList.GetAt(i);
			if (p)
				delete p;
		}
		CombinedResList.RemoveAll();
	}
};

class UCAllItemData
{
public:
	UCIntVoidMap	Items;
	UCAllItemData()
	{
		UCCsv		Slk;
		if (Slk.ReadFromFile("res/csv/itemlist.csv"))
		{
			//从表里读取数量
			ucINT i = 0;
			UCString Itemid;
			ucINT Gameid = 0;
			Slk.GetString(3 + i, 1, Itemid);
			while (!Itemid.IsEmpty())
			{
				UCItemIno* pack = new UCItemIno;
				pack->ItemID = STOI(Itemid);

				if (Find(pack->ItemID))
					ucTrace(UCString("ItemList.slk发现重复ItemID：行号") + ITOS(i) + UCString("，ItemID =") + ITOS(pack->ItemID) + UCString("\r\n"));

				Slk.GetInt(3 + i, 2, pack->ItemType);
				Slk.GetInt(3 + i, 3, pack->SexOn);
				Slk.GetInt(3 + i, 4, pack->Price);
				Slk.GetInt(3 + i, 5, pack->ByScore);
				Slk.GetInt(3 + i, 6, pack->BuyLevel);

				Slk.GetFloat(3 + i, 7, pack->PercentBuy);
				Slk.GetFloat(3 + i, 8, pack->PercentSell);

				Slk.GetInt(3 + i, 9, pack->Validity);
				Slk.GetInt(3 + i, 10, pack->Use);

				Slk.GetInt(3 + i, 11, pack->DeleEnable);
				Slk.GetInt(3 + i, 12, pack->TradeEnable);
				Slk.GetInt(3 + i, 13, pack->LeiJia);

				Slk.GetInt(3 + i, 14, pack->SaledScore);

				Slk.GetString(3 + i, 16, pack->ItemName);
				Slk.GetString(3 + i, 17, pack->ItemDescribe);
				Slk.GetString(3 + i, 18, pack->FunctionName);
				Slk.GetInt(3 + i, 19, pack->Show3D);

				Slk.GetString(3 + i, 20, pack->MeshPath);
				Slk.GetString(3 + i, 21, pack->PicName);

				UCString nTemp;

				Slk.GetString(3 + i, 22, nTemp);
				pack->sSize = nTemp;
				UCPoint ntemp1 = STOP(nTemp);
				pack->PicSize = UCSize(ntemp1.x, ntemp1.y);
				nTemp = UCString("");
				Slk.GetString(3 + i, 23, nTemp);
				pack->sPoint = nTemp;
				pack->PicOffset = STOP(nTemp);

				Slk.GetFloat(3 + i, 24, pack->Distance);
				Slk.GetFloat(3 + i, 25, pack->PosY);
				Slk.GetFloat(3 + i, 26, pack->PosX);
				Slk.GetFloat(3 + i, 27, pack->PosZ);
				Slk.GetFloat(3 + i, 28, pack->RotX);
				Slk.GetFloat(3 + i, 29, pack->SellScale);
				Slk.GetFloat(3 + i, 30, pack->PutOnScale);

				nTemp = UCString("");
				Slk.GetString(3 + i, 31, nTemp);
				pack->sConsum = nTemp;
				while (!nTemp.IsEmpty())
				{
					ucINT nsize = nTemp.Find(';', 0);
					UCString a = nTemp.Left(nsize);

					if (!a.IsEmpty())
					{
						ucINT msize = a.Find(',', 0);
						UCString sid = a.Left(msize);
						a.Delete(0, msize + 1);
						UCString snum = a;
						cBaseInfo* pNew = new cBaseInfo;
						pNew->ID = STOI(sid);
						pNew->Num = STOI(snum);
						pack->CombinedResList.Add(pNew);
					}
					else
					{
						break;
					}
					nTemp.Delete(0, nsize + 1);
				}
				Items.Add(pack->ItemID, pack);

				i++;
				Itemid = UCString("");
				Slk.GetString(3 + i, 1, Itemid);
			}
		}
	}
	UCItemIno* Find(ucINT Key)
	{
		ucINT Pos = Items.FindKey(Key);

		if (Pos >= 0)
		{
			UCItemIno* SpaceInfo = (UCItemIno*)Items.GetValueAt(Pos);
			return SpaceInfo;
		}
		return 0;
	}
	UCString GetMeshPath(ucINT Key)
	{
		UCString t;
		ucINT Pos = Items.FindKey(Key);

		if (Pos >= 0)
		{
			UCItemIno* SpaceInfo = (UCItemIno*)Items.GetValueAt(Pos);
			t = SpaceInfo->MeshPath;
		}
		return t;
	}
	~UCAllItemData()
	{
		for (ucINT i = Items.GetSize() - 1; i >= 0; i--)
		{
			UCItemIno* CarBase = (UCItemIno*)Items.GetValueAt(i);
			delete CarBase;
		}
		Items.RemoveAll();
	}
};
UCAllItemData AllItemDataConfig;

struct cRingInfo
{
	ucINT ItemID;
	ucINT Level;
	ucINT CanStrongNum;
	ucINT SuitID;//-1:不是套装
	ucINT ColorLevel;//颜色等级
	UCString Name;
	ucINT	Drift;			//甩尾
	ucINT	Speed;			//速度
	ucINT	Power;			//能量
	ucINT	Weight;			//重量
	ucINT	Handing;		//操作
	ucINT	Acceleration;	//加速

	ucFLOAT	DriftAppend;			//甩尾
	ucFLOAT	SpeedAppend;			//速度
	ucFLOAT	PowerAppend;			//能量
	ucFLOAT	WeightAppend;			//重量
	ucFLOAT	HandingAppend;		//操作
	ucFLOAT	AccelerationAppend;	//加速

};

struct UCCarDataBase
{
	~UCCarDataBase()
	{
		for (ucINT i = CombinedResList.GetSize() - 1; i >= 0; i--)
		{
			cBaseInfo* p = (cBaseInfo*)CombinedResList.GetAt(i);
			if (p)
				delete p;
		}
		CombinedResList.RemoveAll();
	}
	void ReadFromCsv(UCCsv* Csv, ucINT Position)
	{
		if (Csv == ucNULL) 
			return;
		Csv->GetInt(Position + 3, 1, CarID);
		Csv->GetFloat(Position + 3, 2, Drift);
		Csv->GetFloat(Position + 3, 3, Weight);
		Csv->GetFloat(Position + 3, 4, Handing);
		Csv->GetFloat(Position + 3, 5, Speed);
		Csv->GetFloat(Position + 3, 6, Acceleration);
		Csv->GetFloat(Position + 3, 7, Power);

		Csv->GetString(Position + 3, 9, PiaoYiPos);

		Csv->GetString(Position + 3, 10, Descibe);


		Csv->GetString(Position + 3, 12, WheelName0);
		Csv->GetString(Position + 3, 13, WheelName1);
		Csv->GetString(Position + 3, 14, WheelName2);
		Csv->GetString(Position + 3, 15, WheelName3);

		Csv->GetString(Position + 3, 16, WheelPosition0);
		Csv->GetString(Position + 3, 17, WheelPosition1);
		Csv->GetString(Position + 3, 18, WheelPosition2);
		Csv->GetString(Position + 3, 19, WheelPosition3);
		Csv->GetFloat(Position + 3, 20, WheelScale);

		Csv->GetFloat(1, 2, PointDrift);
		Csv->GetFloat(1, 3, PointWeight);
		Csv->GetFloat(1, 4, PointHanding);
		Csv->GetFloat(1, 5, PointSpeed);
		Csv->GetFloat(1, 6, PointAcceleration);
		Csv->GetFloat(1, 7, PointPower);

		Csv->GetInt(Position + 3, 21, NeedMakeTime);
		Csv->GetInt(Position + 3, 22, NeedChejianLevel);
		Csv->GetInt(Position + 3, 23, BelongDraw);
		Csv->GetInt(Position + 3, 24, NeedBaseCar);
		Csv->GetInt(Position + 3, 25, CarLevel);

		UCString nTemp;
		Csv->GetString(Position + 3, 26, nTemp);
		while (!nTemp.IsEmpty())
		{
			ucINT nsize = nTemp.Find(';', 0);
			UCString a = nTemp.Left(nsize);

			if (!a.IsEmpty())
			{
				ucINT msize = a.Find(',', 0);
				UCString sid = a.Left(msize);
				a.Delete(0, msize + 1);
				UCString snum = a;
				cBaseInfo* pNew = new cBaseInfo;
				pNew->ID = STOI(sid);
				pNew->Num = STOI(snum);
				CombinedResList.Add(pNew);
			}
			else
			{
				break;
			}
			nTemp.Delete(0, nsize + 1);

		}
		Csv->GetInt(Position + 3, 27, ManLevel);
		Csv->GetInt(Position + 3, 28, NeedScore);

		UCItemIno* pInfo = AllItemDataConfig.Find(CarID);
		if (pInfo)
			Name = pInfo->ItemName;

		for (ucINT r = 0; r < MAXVENTFIRE; r++)//读入尾气配置
		{

			Csv->GetString(Position + 3, 32 + r, SmallFireName[r]);
			UCString t1;
			Csv->GetString(Position + 3, 38 + r, t1);
			if (!t1.IsEmpty())
				SmallFirePos[r] = STOV(t1);

			Csv->GetString(Position + 3, 44 + r, BifFireName[r]);
			UCString t2;
			Csv->GetString(Position + 3, 50 + r, t2);
			if (!t2.IsEmpty())
				BigFirePos[r] = STOV(t2);
		}

		UCString a1;
		UCString a2;
		Csv->GetString(Position + 3, 60, a1);
		Csv->GetString(Position + 3, 61, a2);
		SamplePos = STOV(a1);
		SampleRot = STOV(a2);
	}
	ucINT CarID;
	UCString Name;
	UCString Descibe;
	ucFLOAT	Drift;			//甩尾
	ucFLOAT	Speed;			//速度
	ucFLOAT	Power;			//能量
	ucFLOAT	Weight;			//重量
	ucFLOAT	Handing;		//操作
	ucFLOAT	Acceleration;	//加速


	ucFLOAT	PointDrift;			//甩尾
	ucFLOAT	PointSpeed;			//速度
	ucFLOAT	PointPower;			//能量
	ucFLOAT	PointWeight;		//重量
	ucFLOAT	PointHanding;		//操作
	ucFLOAT	PointAcceleration;	//加速






	UCString	WheelName0;		//轮胎0模型
	UCString	WheelName1;		//轮胎1模型
	UCString	WheelName2;		//轮胎2模型
	UCString	WheelName3;		//轮胎3模型	



	UCString	WheelPosition0;		//轮胎0位置
	UCString	WheelPosition1;		//轮胎1位置
	UCString	WheelPosition2;		//轮胎2位置
	UCString	WheelPosition3;		//轮胎3位置
	UCString	PiaoYiPos;//漂移粒子的位置

	uc3dxVector3 BigFirePos[MAXVENTFIRE];
	UCString BifFireName[MAXVENTFIRE];
	uc3dxVector3 SmallFirePos[MAXVENTFIRE];
	UCString SmallFireName[MAXVENTFIRE];

	uc3dxVector3 SamplePos;//展示的时候，赛车的位置
	uc3dxVector3 SampleRot;//展示的时候，赛车的旋转


	ucFLOAT WheelScale;

	ucINT NeedScore;//消耗积分
	UCVoidArray CombinedResList;
	ucINT BelongDraw;//0-基础图谱1-特殊图谱
	ucINT NeedBaseCar;//BelongDraw=0时：0-不需要，只要原料即可；1即为需要等级车辆//BelongDraw=1时：0-不需要，只要原料即可；〉0即为需木咛宄盗綢D
	ucINT NeedChejianLevel;
	ucINT NeedMakeTime;
	ucINT CarLevel;
	ucINT ManLevel;//车手最低等级（>=manLevel）
};

struct UCCarDataConfig
{
	UCIntVoidMap Items;

	UCCarDataConfig()
	{
		UCCsv		Csv;
		if (Csv.ReadFromFile("res/csv/cardata.csv"))
		{
			//从表里读取数量
			ucINT MaxCar = 0;

			Csv.GetInt(1, 1, MaxCar);

			for (ucINT i = 0; i < MaxCar; i++)
			{
				UCCarDataBase* CarBase = new UCCarDataBase;
				CarBase->ReadFromCsv(&Csv, i);

				Items.Add(CarBase->CarID, CarBase);
			}
		}
	}
	UCCarDataBase* GetAt(ucINT nCarid)
	{
		if (nCarid == -1)//默认车辆
			return ucNULL
		else//其他车的ID
		{
			ucINT pos = Items.FindKey(nCarid);
			if (pos >= 0)
				return (UCCarDataBase*)Items.GetValueAt(pos);
		}
		return ucNULL;
	}
	~UCCarDataConfig()
	{
		for (ucINT i = Items.GetSize() - 1; i >= 0; i--)
		{
			UCCarDataBase* CarBase = (UCCarDataBase*)Items.GetValueAt(i);
			delete CarBase;
		}
		Items.RemoveAll();
	}
};

UCCarDataConfig CarDataConfig;

class UCCarData
{
public:
	UCCarData()
	{
		Reset();

	}
	~UCCarData()
	{
	}
	void Init(ucINT Num)
	{
		UCCarDataBase* CarBase = 0;
		CarBase = ucNULL;
		CarBase = CarDataConfig.GetAt(Num);
		if (CarBase)
		{
			CarID = CarBase->CarID;

			WheelName[0] = CarBase->WheelName0;
			WheelName[1] = CarBase->WheelName1;
			WheelName[2] = CarBase->WheelName2;
			WheelName[3] = CarBase->WheelName3;

			Type = MeshCar;

			Drift = CarBase->Drift;
			Weight = CarBase->Weight;
			Handing = CarBase->Handing;
			MaxSpeed = CarBase->Speed;
			Acceleration = CarBase->Acceleration;
			PowerRecover = CarBase->Power;

			WheelPosition[0] = STOV(CarBase->WheelPosition0);
			WheelPosition[1] = STOV(CarBase->WheelPosition1);
			WheelPosition[2] = STOV(CarBase->WheelPosition2);
			WheelPosition[3] = STOV(CarBase->WheelPosition3);
			PiaoYiPos = STOV(CarBase->PiaoYiPos);

			ScaleWheel = CarBase->WheelScale;

			Name = CarBase->Name;
			Decribe = CarBase->Descibe;


			SamplePos = CarBase->SamplePos;
			SampleRot = CarBase->SampleRot;


			for (ucINT i = 0; i < MAXVENTFIRE; i++)
			{
				SmallFireName[i] = CarBase->SmallFireName[i];
				SmallFirePos[i] = CarBase->SmallFirePos[i];
				BifFireName[i] = CarBase->BifFireName[i];
				BigFirePos[i] = CarBase->BigFirePos[i];
			}

			CarLevel = CarBase->CarLevel;
			ManLevel = CarBase->ManLevel;
		}
		else
		{
			Reset();
		}
	}

	void Reset()
	{
		CarID = -1;

		WheelName[0] = UCString("");
		WheelName[1] = UCString("");
		WheelName[2] = UCString("");
		WheelName[3] = UCString("");

		Type = MeshCar;

		Drift = 0.0f;
		Weight = 0.0f;
		Handing = 0.0f;
		MaxSpeed = 0.0f;
		Acceleration = 0.0f;
		PowerRecover = 0.0f;

		ScaleWheel = 1.0f;

		Name = UCString("");
		Decribe = UCString("");


		for (ucINT i = 0; i < MAXVENTFIRE; i++)
		{
			SmallFireName[i] = UCString("");
			BifFireName[i] = UCString("");
		}

		CarLevel = 0;
		ManLevel = 0;
	}

	//赛车操作数据
	ucFLOAT		Drift;
	ucFLOAT		Weight;
	ucFLOAT		Handing;
	ucFLOAT		MaxSpeed;
	ucFLOAT		Acceleration;
	ucFLOAT		PowerRecover;	//能量修复


	//轮子文件
	UCString		WheelName[4];
	uc3dxVector3	WheelPosition[4];
	ucFLOAT			ScaleWheel;
	uc3dxVector3	PiaoYiPos;
	//尾气配置
	uc3dxVector3	BigFirePos[MAXVENTFIRE];
	UCString		BifFireName[MAXVENTFIRE];
	uc3dxVector3	SmallFirePos[MAXVENTFIRE];
	UCString		SmallFireName[MAXVENTFIRE];

	uc3dxVector3	SamplePos;
	uc3dxVector3	SampleRot;

	ucINT			CarID;

	UCString		Name;
	UCString		Decribe;

	ucINT			CarLevel;
	ucINT			ManLevel;//车手最低等级（>=manLevel）

	ucINT			Type;
}; 

UCCarData		CarThings;

#endif