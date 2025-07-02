/********************************************************************
	created:	21:6:2006   12:23
	filename: 	UCPhy
	author:		LargeWC

	purpose:
*********************************************************************/

#ifndef _UCPhy_21_3_2007_4_16
#define _UCPhy_21_3_2007_4_16
#include "../data/cardata.h"
#include "sale.h"

#define KEY_UP		0		//弹起
#define KEY_DOWN	1		//按下
#define KEY_WAIT	2		//等待

#define GRD_EMPTH	0		//空中
#define GRD_FLOOR	1		//地面
#define GRD_RESET	2		//重起

#define JUMP_NONE	0
#define JUMP_WAIT	1
#define JUMP_PROC	2

#define BODY_RADIUS		54.0f	//身体半径

#define HUMANPOSY		-20.0f
#define INIT_Y			16.0f	//
ucINT DIS_Y = BODY_RADIUS - 20.0f;

#define MAX_ROT_X	0.6f
#define MIN_ROT_X	0.18f
#define STP_Max_ROT_X	0.03f
#define STP_ROT_X	0.018f

class UCKeyCheck
{
public:
	ucINT	Valid;

	ucINT	Press;
	ucINT	Click;
	UCKeyCheck()
	{
		Valid = 1;

		Press = 0;
		Click = 0;
	}
	ucVOID Check(ucINT Input)
	{
		Press = Input;
	}
	ucINT GetState()
	{
		return Press;
	}
	ucVOID Cancel()
	{
		Valid = 0;
	}
	ucVOID Reset()
	{
		Valid = 1;
	}
};

struct UCCarPhyInfo
{
	ucINT			FPS;
	uc3dxVector3	Pos;
	ucFLOAT			RotY;
	UCCarPhyInfo()
	{
		FPS = 0;
		Pos = uc3dxVector3(0.0f, 0.0f, 0.0f);
		RotY = 0.0f;
	}
	UCCarPhyInfo(ucCONST UCCarPhyInfo& CarPhyInfo)
	{
		FPS = CarPhyInfo.FPS;
		Pos = CarPhyInfo.Pos;
		RotY = CarPhyInfo.RotY;
	}
};
class UCCarPhysics
{
public:
	ucINT					Qualming;
	ucINT					Profession;

	ucINT					IsSelf;				//是否是玩家

	UCSceneCtl*				SceneCtrl;

	UCSMState				State;				//状态机	

	ucINT					PlayerNum;
	ucINT					JumpDust;

	ucINT					SkillLevel;			//技能等级

	ucINT					DriftPower;			//漂移能量槽	满了以后增加一个加速道具
	ucINT					DriftPowerBackup;	//能量备份
	ucINT					DriftLevel;			//漂移等级，最大二级，用来二级联票
	ucINT					DriftType;			//漂移类型
	ucINT					DriftFire;
	ucINT					DriftDir;			//漂移方向
	ucINT					DriftHandle;

	ucINT					DriftUp;			//小冲状态
	ucINT					DriftUpTime;		//小冲剩余时间

	ucFLOAT					SpeedBackup;		//备份速度

	ucFLOAT					WheellRotY;			//车轮转向

	ucINT					JumpState;

	UCKeyCheck				KeyJump;
	UCKeyCheck				KeyDrift;

	UCCarData				Data;

	uc3dxVector3			Head;					//头顶朝向

	UCMeshSphere			Center;					//物理

	UCMaterial				Material;				//材质
	UCMaterial				MaterialMetal;			//金属材质

	UCMesh					Body;					//整体
	UCMesh					CenterBody;				//
	UCMesh					CenterPoint;

	UCBrakeTrace			BrakeTraceL;			//轮胎印
	UCBrakeTrace			BrakeTraceR;			//轮胎印

	UCMesh					CarBody;						//车身
	UCMesh					CarVent;						//喷气筒
	UCMesh					CarVentFire[MAXVENTFIRE];		//喷火特效
	UCMesh					Ball;							//车身气球
	UCMesh					WheelCenter;					//车轮中心
	UCMesh					Wheel[MAX_WHEEL];				//车轮

	UCMesh					HumanBody;						
	UCMesh					HumanFace;
	UCMesh					HumanHair;
	UCMesh					HumanSuit;
	UCMesh					HumanPant;
	UCMesh					HumanHand;
	UCMesh					HumanShoe;

	UCMesh					DriftMeshRead;				//小冲模型
	UCMesh					DriftMeshMove;				//小冲模型

	UCImage					ImageName;
	UCLabel					Username;					//名字
	UC3DControl				Panel;						//面板
public:
	ucINT					Gender;							//性别

	UCGroupFiber			FiberStar;
	UCFiber					FiberShock;

	UCFiber					FiberDrift;						//漂移
	UCPortalObject			PortalObject;					//门径物体，判断所在空间

	ucINT					Mode;							//身体接触地面
	ucFLOAT					CameraRayLength;				//摄像头视线碰撞判断
	ucINT					CameraFixAble;					//摄像头碰撞修复

	UCFiber					FiberAutoDrive;					//自动驾驶

	ucINT					KeyL;							//左键按下
	ucINT					KeyR;							//右键按下
	ucINT					KeyD;							//下键按下

	UCEArray<UCCarPhyInfo>	AryPhyInfo;

	~UCCarPhysics()
	{
		FiberDrift.Stop();
	}
	UCCarPhysics()
	{
		PlayerNum = 0;
		Gender = 0;

		KeyL = 0;
		KeyR = 0;
		KeyD = 0;

		DriftHandle = 0;
		DriftFire = 0;
		WheellRotY = 0.0f;

		FiberStar.FiberEvent = UCEvent(this, OnFiberStar);
		FiberShock.FiberEvent = UCEvent(this, OnFiberShock);
		FiberDrift.FiberEvent = UCEvent(this, OnFiberDrift);

		IsSelf = 0;
		State.LoadFromCSV(UCString("res/csv/state.csv"));

		Profession = 1;	//默认是冲锋

		Qualming = 0;
		Panel.Enable = 0;
		Panel.Visible = 0;
		Head = uc3dxVector3(0.0f, 1.0f, 0.0f);

		SkillLevel = 0;

		JumpDust = 0;
		JumpState = JUMP_NONE;

		DriftPower = 0;			//漂移能量槽	满了以后增加一个加速道具
		DriftPowerBackup = 0;	//能量备份
		DriftLevel = 0;			//漂移等级，最大二级，用来二级联票

		DriftType = 0;			//漂移类型
		DriftDir = 0;

		DriftFire = 0;
		DriftHandle = 0;

		DriftUp = 0;
		DriftUpTime = 0;		//小冲剩余时间

		SpeedBackup = 0.0f;

		Mode = GRD_EMPTH;
		CameraRayLength = 0.0f;
		CameraFixAble = 0;

		PortalObject.WayKind = 0;
	}
	ucVOID	Clear()
	{
		WheellRotY = 0.0f;

		SkillLevel = 0;
		JumpDust = 0;
		JumpState = JUMP_NONE;

		DriftPower = 0;			//漂移能量槽	满了以后增加一个加速道具
		DriftPowerBackup = 0;	//能量备份
		DriftLevel = 0;			//漂移等级，最大二级，用来二级联票

		DriftType = 0;			//漂移类型

		DriftUpTime = 0;		//小冲剩余时间

		Mode = GRD_EMPTH;

		PortalObject.WayKind = 0;
	}
	ucVOID	Init(UCSceneCtl* SceneCtl, UCString	CarName, UCString VentName, UCString VentFireName, UCString HairName, UCString FaceName, UCString BodyName, UCString pantName, UCString shoetName, UCString handName, UCString suitname, UCString BallName, UCString LicenceName, ucFLOAT Mass)
	{
		this->SceneCtrl = SceneCtl;
		Panel.CameraFollows(1, 1, 0);

		ImageName.Size = UCSize(85, 31);
		ImageName.Picture.Name = UCString("res/ui/gameing/002.dds");
		Panel.AddControl(&ImageName);

		Username.Size = UCSize(85, 16);
		Username.Location = UCPoint(4, 2);
		Username.Font.Weight = 800;
		Username.Font.Name = UCString("黑体");
		Username.Align = 0;
		ImageName.AddControl(&Username);

		Panel.Pos.y = 70.0f;
		Panel.Rot.y = 3.141592657f;
		Panel.BindMesh(&CenterPoint);

		SceneCtl->AddControl(&Panel);

		Panel.Size = UCSize(85, 20);//76,32

		BrakeTraceL.SceneCtl = SceneCtl;
		BrakeTraceR.SceneCtl = SceneCtl;

		BrakeTraceL.Image = UCString("res/trace.tga");
		BrakeTraceL.Radius = 8.0f;
		BrakeTraceL.Size = 10000;

		BrakeTraceR.Image = UCString("res/trace.tga");
		BrakeTraceR.Radius = 8.0f;
		BrakeTraceR.Size = 10000;

		Center.Mass = Mass;
		Center.Radius = BODY_RADIUS;
		Center.Name = UCString("res/model/mesh/sphere.dat");
		if (Center.GetAABB().m_vExtents.x > 0.1f)
			Center.Scale = uc3dxVector3(BODY_RADIUS / Center.GetAABB().m_vExtents.x,
				BODY_RADIUS / Center.GetAABB().m_vExtents.x,
				BODY_RADIUS / Center.GetAABB().m_vExtents.x);
		Center.ID = 0x4000;

		CenterPoint.Parent = &Body;
		CenterPoint.Rot.y = UC3D_PI;

		CarBody.Name = CarName;
		CarBody.Parent = &CenterPoint;
		CarBody.Pos.y = -20.0f;

		for (ucINT i = 0; i < MAXVENTFIRE; i++)
			CarVentFire[i].Parent = &CarBody;
		SetVentFire(SMALLFIRE, 0.5f);

		HumanFace.Parent = &HumanBody;
		HumanBody.Parent = &CenterPoint;
		HumanHair.Parent = &HumanBody;
		HumanPant.Parent = &HumanBody;
		HumanShoe.Parent = &HumanBody;
		HumanHand.Parent = &HumanBody;
		HumanSuit.Parent = &HumanBody;

		HumanBody.Pos.y = HUMANPOSY;

		HumanFace.Name = FaceName;

		Ball.Name = BallName;
		Ball.Parent = &HumanFace;
		Ball.BindByName(UCString("Bip01 Head"));//面具
		Ball.Rot = uc3dxVector3(3.05f, 0.12f, -1.6f);
		Ball.Pos = uc3dxVector3(6.8f, 0.0f, 2.6f);

		if (suitname.IsEmpty())
		{
			HumanBody.Name = BodyName;
			HumanHair.Name = HairName;
			HumanShoe.Name = shoetName;
			HumanPant.Name = pantName;
			HumanHand.Name = handName;
			HumanSuit.Name = UCString("");
		}
		else
		{
			HumanBody.Name = UCString("");
			HumanHair.Name = UCString("");
			HumanShoe.Name = UCString("");
			HumanPant.Name = UCString("");
			HumanHand.Name = UCString("");
			HumanSuit.Name = suitname;
		}

		//HumanBody.SetActionFile(UCString("res/model/action/action.dat"));		
		//HumanHair.SetActionFile(UCString("res/model/action/action.dat"));	

		SetActionFile();

#ifndef TEST_PHY
		Center.Visible = 0;
#else
		HumanBody.Visible = 0;
		HumanFace.Visible = 0;
		HumanHair.Visible = 0;
		HumanSuit.Visible = 0;
		HumanShoe.Visible = 0;
		HumanPant.Visible = 0;
		HumanHand.Visible = 0;
		CarBody.Visible = 0;
		CarVent.Visible = 0;
		for (ucINT i = 0; i < 4; i++)
		{
			Wheel[i].Visible = 0;
		}
#endif

		WheelCenter.Parent = &CarBody;
		for (ucINT i = 0; i < MAX_WHEEL; i++)
		{
			Wheel[i].Pos = Data.WheelPosition[i];
			Wheel[i].Name = Data.WheelName[i];

			Wheel[i].Parent = &WheelCenter;
		}

		DriftMeshRead.Name = UCString("res/model/action/piaoyismall.dat");
		DriftMeshRead.Parent = &CenterPoint;
		DriftMeshRead.Visible = 0;
		DriftMeshRead.PlayAction(0);

		DriftMeshMove.Name = UCString("res/model/action/piaoyi.dat");
		DriftMeshMove.Parent = &CenterPoint;
		DriftMeshMove.Visible = 0;
		DriftMeshMove.PlayAction(0);

		Material.Name = UCString("res/shader/object.material");
		//Material.Name = UCString("res/shader/shadowed_smooth.material");

		SetBaseMaterail();

		AddAction(1, -1, 0, 1);	//00-正常
		AddAction(10, 9, 1, 1);	//01-扭头
		AddAction(5, 4, 1, 1);	//02-左转
		AddAction(3, 2, 1, 1);	//03-右转
		AddAction(6, 6, 1, 1);	//04-准备
		AddAction(8, 7, 8, 2);	//05-加速
		AddAction(12, 12, 1, 2);	//06-向前仍道具
		AddAction(11, 11, 1, 2);	//07-向后仍道具
		AddAction(13, 13, 8, 2);	//08-愤怒
		AddAction(14, 14, 6, 2);	//09-兴奋
		AddAction(15, 15, 1, 2);	//10-致盲
		AddAction(15, 15, 1, 2);	//11-眩晕
		AddAction(17, 17, 6, 3);	//12-失败
		AddAction(16, 16, 6, 3);	//13-胜利
		AddAction(18, 18, 8, 2);	//14-儿童
		AddAction(19, 19, 4, 2);	//15-被恶魔附身
		AddAction(20, 20, 0, 4);	//16-奖台男胜利
		AddAction(21, 21, 0, 4);	//17-奖台女胜利
		AddAction(22, 22, 0, 4);	//18-奖台失败

		CenterBody.Parent = SceneCtl->Scene;
		Body.Parent = &CenterBody;
		Center.Parent = SceneCtl->Scene;

		ShowShadow(ucTRUE);
	}
	ucVOID	SetVentFireVisible(ucINT nVisible)
	{
		if (!IsSelf) 
			return;
		for (ucINT i = 0; i < MAXVENTFIRE; i++)
			CarVentFire[i].Visible = nVisible;
	}
	ucFLOAT	GetWheelFireSpeed()
	{
		ucFLOAT Speed = Center.LinearVel.Value().Length();
		Speed = 1.0f + Speed * 0.2f;
		if (Speed > 28.0f)
			Speed = 28.0f;
		return Speed;
	}
	ucVOID	SetVentFire(ucINT nType, ucFLOAT nSpeed)
	{
		if (!IsSelf) 
			return;
		if (nType == BIGFIRE)//大火
		{
			for (ucINT i = 0; i < MAXVENTFIRE; i++)
			{
				if (CarVentFire[i].Name.Value != Data.BifFireName[i])
				{
					CarVentFire[i].Name = Data.BifFireName[i];
					CarVentFire[i].Pos = Data.BigFirePos[i];
				}
				if (!CarVentFire[i].Name.Value.IsEmpty())
					CarVentFire[i].PlayAction(0, nSpeed, 0);
			}
		}
		else
		{
			for (ucINT i = 0; i < MAXVENTFIRE; i++)
			{
				if (CarVentFire[i].Name.Value != Data.SmallFireName[i])
				{
					CarVentFire[i].Name = Data.SmallFireName[i];
					CarVentFire[i].Pos = Data.SmallFirePos[i];
				}
				for (ucINT i = 0; i < MAXVENTFIRE; i++)
					CarVentFire[i].PlayAction(0, GetWheelFireSpeed(), 0);;
			}
		}
	}
	ucVOID	ShowShadow(ucINT SW)
	{
		HumanHair.FilterEnable(0, SW);
		HumanFace.FilterEnable(0, SW);
		HumanBody.FilterEnable(0, SW);
		HumanSuit.FilterEnable(0, SW);;
		HumanPant.FilterEnable(0, SW);
		HumanHand.FilterEnable(0, SW);
		HumanShoe.FilterEnable(0, SW);

		CarBody.FilterEnable(0, SW);
		CarVent.FilterEnable(0, SW);

		Wheel[0].FilterEnable(0, SW);
		Wheel[1].FilterEnable(0, SW);
		Wheel[2].FilterEnable(0, SW);
		Wheel[3].FilterEnable(0, SW);
	}
	ucVOID	InitTeamName(ucINT Team, UCString Name)
	{
		if (IsSelf)
		{
			Panel.Visible = 0;
		}
		else
		{
			Panel.Visible = 1;
			ImageName.Picture.Offset = UCPoint(-4, -5 - 36 * Team);
			Username.Text = Name;
			//Username.Font.Weight=800;
			if (Team == 0)
			{
				Username.FontColor.TextColor[UCDISPLAYMODE_LIGHT].Outline = RED;
				Username.FontColor.TextColor[UCDISPLAYMODE_DARK].Outline = RED;
			}
			else
			{
				Username.FontColor.TextColor[UCDISPLAYMODE_LIGHT].Outline = BLUE;
				Username.FontColor.TextColor[UCDISPLAYMODE_DARK].Outline = BLUE;
			}

			CarVentFire[0].Visible = 0;
			CarVentFire[1].Visible = 0;
			CarVentFire[2].Visible = 0;
			CarVentFire[3].Visible = 0;
			CarVentFire[4].Visible = 0;
			CarVentFire[5].Visible = 0;

			Username.Align = 0;
		}
	}
	ucINT	ControlAble()
	{
		return !State.IsRunning("打滑") && !State.IsRunning("蝙蝠") &&
			!State.IsRunning("水泡") && !State.IsRunning("第一名") &&
			!State.IsRunning("燃烧") && !State.IsRunning("正翻") &&
			!State.IsRunning("侧翻") && !State.IsRunning("睡眠") &&
			!State.IsRunning("拳击手") && !State.IsRunning("厨师拍打") &&
			!State.IsRunning("牢笼") && !State.IsRunning("后退") &&
			!State.IsRunning("Reset");
	}
	ucVOID	CheckDriftPower()
	{
		if (SkillLevel != 2)
		{
			//如果不是漂移训练模式
			if (!(SkillLevel == 1) && DriftFire == 2)
			{
				ucINT PlayNumCount = PlayerNum;	if (PlayNumCount < 2) PlayNumCount = 2;
				ucINT Sequence = PortalObject.Sequence / 2 + 1;
				ucINT Power = 1 * (POWER_BAS + Data.PowerRecover * POWER_INC) * 4.0f;
				ucINT PowerSpeed = (PlayNumCount - Sequence) * Power / (PlayNumCount / 2);

				DriftPower += PowerSpeed * 4;
			}
		}
	}
	ucVOID	CheckPowerAutoUp()
	{
		if (PlayerNum < 2) PlayerNum = 2;
		ucINT Sequence = PortalObject.Sequence / 2 + 1;
		ucINT PowerSpeed = (Sequence + 1) * (POWER_BAS + POWER_INC * Data.PowerRecover) / (PlayerNum / 2);

		if (DriftPower < 0) PowerSpeed = 0;

		if (State.IsRunning("沉默") || State.IsRunning("枷锁"))
		{
			PowerSpeed = 0;
		}

		if (State.IsRunning("回魔"))
		{
			PowerSpeed *= 8;
		}

		if (DriftPower >= 0 && !State.IsRunning("法力燃烧")) DriftPower += PowerSpeed * 2;
		if (DriftPower > DRIFT_POWER)	DriftPower = DRIFT_POWER;

		if (State.IsRunning("法力燃烧"))
		{
			DriftPower -= 80;
			if (DriftPower < 0)  DriftPower = 0;
		}
	}
	ucVOID	CheckUp(uc3dxVector3 vLinearVel)
	{
		ucFLOAT	maxspeed = (VSPEED_BAS + VSPEED_INC * Data.MaxSpeed) * CenterPoint.Scale.z.Value;

		ucFLOAT length_x = vLinearVel | CenterBody.GetWay_X();
		ucFLOAT length_y = vLinearVel | CenterBody.GetWay_Y();
		ucFLOAT length_z = vLinearVel | CenterBody.GetWay_Z();
		if (State.IsRunning("无敌") || State.IsRunning("强化") ||
			State.IsRunning("小冲") || State.IsRunning("儿童") ||
			State.IsRunning("加速"))
		{	//无敌状态下加速能力全满
			if (State.IsRunning("儿童"))
			{
				if (length_z < 600.0f)
				{
					ucFLOAT	accspeed = ACCELE_BAS + (ACCELE_INC);
					length_z += CenterPoint.Scale.z.Value * accspeed * 1.6f;
				}
			}
			else if (State.IsRunning("加速"))
			{
				if (DriftType < 2 && length_z < maxspeed + VSPEED_UP)
				{
					ucFLOAT	accspeed = ACCELE_BAS + (ACCELE_INC);
					SpeedBackup += CenterPoint.Scale.z.Value * accspeed;
					length_z += CenterPoint.Scale.z.Value * accspeed;
				}
			}
			else if (State.IsRunning("小冲"))
			{
				//没有进入正式漂移之前
				if (DriftType < 2 && length_z < maxspeed)
				{
					ucFLOAT	accspeed = ACCELE_BAS + (ACCELE_INC);
					SpeedBackup += CenterPoint.Scale.z.Value * accspeed;
					length_z += CenterPoint.Scale.z.Value * accspeed;
				}
			}
			else if (State.IsRunning("无敌") ||
				State.IsRunning("强化"))
			{
				if (length_z < maxspeed + VSPEED_UP)
				{
					ucFLOAT	accspeed = ACCELE_BAS + (ACCELE_INC);
					length_z += CenterPoint.Scale.z.Value * accspeed * 1.2f;
				}
			}
		}
		else if (State.IsRunning("减速棒"))
		{
			if (length_z > 120.0f)
			{
				length_z -= CenterPoint.Scale.z.Value * 10.0f;
			}
		}
		else if (State.IsRunning("法力燃烧"))
		{
			if (length_z > 1000.0f)
			{
				length_z -= CenterPoint.Scale.z.Value * 0.5f;
			}
		}
		else
		{	
			if (length_z < 0.0f)
			{
				ucFLOAT	accspeed = (ACCELE_BAS + ACCELE_INC);
				length_z += accspeed;
			}
			else if (length_z < maxspeed)
			{
				ucFLOAT	accspeed = (ACCELE_BAS + (Data.Acceleration * ACCELE_INC));
				length_z += accspeed;
			}
			else
				length_z = maxspeed;

		}

		vLinearVel = CenterBody.GetWay_X() * length_x +
			CenterBody.GetWay_Y() * length_y +
			CenterBody.GetWay_Z() * length_z;

		ucFLOAT fDir = CenterBody.GetWay_Z() | uc3dxVector3(0.0f, 1.0f, 0.0f);
		if (fDir >= 0.0f) //朝上
			vLinearVel.y += CenterBody.GetWay_Z().y * 20.0f;

		Center.LinearVel = vLinearVel;
	}
	ucINT	DriftAble()
	{
		if (FiberAutoDrive.IsRun() && KeyD == 0)
			return 0;

		uc3dxVector3 vLinearVel = Center.LinearVel.Value();
		if (!State.IsRunning("超人") &&//!State.IsRunning("无敌")&&
			!State.IsRunning("枷锁") && !State.IsRunning("牢笼") &&
			!State.IsRunning("睡眠"))
		{
			KeyDrift.Check(KeyState.KEYIsDrift());
			if (KeyDrift.GetState() == 0)	KeyDrift.Valid = 1;

			if ((KeyDrift.GetState() != 0 || KeyD != 0) && vLinearVel.Length() > 30.0f && Mode == GRD_FLOOR)
			{
				return 1;;
			}
		}
		return 0;
	}
	ucVOID	DriftCheck()
	{
		//检查能量自增长模式
		CheckPowerAutoUp();
	}
	ucVOID	MoveCheck()
	{
		uc3dxVector3	vLinearVel(Center.LinearVel.Value());

		ucINT iFrequency = 10000 + vLinearVel.Length() * 210;
		//gCarSound.SetFrequency(7,iFrequency);

#define MIN_LEN	20.0f
#define FST_LEN	16.0f

		if (JumpDust)
		{
			//StateJumpDust.Stop();
			//StateJumpDust.StartNew(0);
			JumpDust = 0;
		}

		uc3dxVector3 vZero(0.0f, 0.0f, 0.0f);
		UCDevice3D* Device = (UCDevice3D*)UIGetDevice();

#ifdef AUTODRIVEON
		if (Device->IsKeyDownOnce('T'))
		{
			if (FiberAutoDrive.IsRun())
				FiberAutoDrive.SetUnValid();
			else
				FiberAutoDrive.Run(0);
		}
#endif

		ucINT AutoDrive = FiberAutoDrive.IsRun();

		if (ControlAble())
		{
			uc3dxVector3 vLinearVelLength = uc3dxVector3(vLinearVel.x, 0.0f, vLinearVel.z);
			ucFLOAT Length = vLinearVelLength.Length();

			ucFLOAT WayZSpeed = vLinearVelLength | CenterBody.GetWay_Z();

			if (DriftType > 0)
			{
				CameraRayLength = SpeedBackup * 0.6f;
			}
			else
			{
				if (WayZSpeed >= 0.0f)
					CameraRayLength = WayZSpeed * 0.6f;
				else
					CameraRayLength = 0.0f;
			}

			ucFLOAT Dir = 0.0f;
			ucFLOAT DirV = (vLinearVel | CenterBody.GetWay_Z());
			if (Length >= 0.1f)
			{
				if (DirV > 0.0f) Dir = 1.0f;	else if (DirV < 0.0f) Dir = -1.0f;	else Dir = 0.0f;
			}
			else
				Dir = 1.0f;

			PlayAction(0);

			ucJOYSTATE* JoyState = Device->Input->JoyState(0);

			if (KeyState.KEYIsUp() && Mode != GRD_EMPTH || State.IsRunning("冲刺"))
			{
				if (Length < MIN_LEN)
				{
					Length = MIN_LEN;
					Dir = 2.0f;
				}
				CheckUp(vLinearVel);
			}

			ucFLOAT fVel = 0.0f;
			if (//State.IsRunning("无敌")||
				State.IsRunning("强化"))
			{
				fVel = (HANDLE_BAS + HANDLE_INC) * 1.2f;
			}
			else
			{
				ucFLOAT	maxspeed = VSPEED_BAS * CenterPoint.Scale.z.Value;
				ucFLOAT	length_z = vLinearVel | CenterBody.GetWay_Z();
				ucFLOAT	handing = (maxspeed - length_z) / maxspeed;

				if (handing > Data.Handing)
				{
					fVel = HANDLE_BAS + handing * HANDLE_INC;
				}
				else
				{
					if (KeyState.KEYIsUp())
					{
						fVel = HANDLE_BAS + Data.Handing * HANDLE_INC;
					}
					else if (KeyState.KEYIsDown())
					{
						fVel = (HANDLE_BAS + Data.Handing * HANDLE_INC) * 2.4f;
					}
					else
					{
						fVel = (HANDLE_BAS + Data.Handing * HANDLE_INC) * 1.8f;
					}
				}
			}
			ucFLOAT HandleMax = HANDLE_MAX;

			Wheel[0].Rot.y = 0.0f;				Wheel[1].Rot.y = 0.0f;
			if ((Length > FST_LEN || (State.IsRunning("儿童") || State.IsRunning("枷锁")))
				&& (KeyState.KEYIsLeft() || KeyL != 0))
			{
				if (State.IsRunning("儿童") || State.IsRunning("枷锁"))
				{
					if (Mode == GRD_EMPTH)
						fVel = fVel * 1.8f;
					else
					{
						ucFLOAT length_z = vLinearVel | CenterBody.GetWay_Z();

						if (length_z > 40.0f)	fVel = 0.0f;
						Dir = 1.0f;
					}
				}
				else
				{
					if (Mode == GRD_EMPTH)
						fVel = fVel * 1.8f;
				}

				if (WheellRotY > -HandleMax)
					WheellRotY -= fVel;
				PlayAction(2);
			}
			if ((Length > FST_LEN || (State.IsRunning("儿童") || State.IsRunning("枷锁")))
				&& (KeyState.KEYIsRight() || KeyR != 0))
			{
				if (State.IsRunning("儿童") || State.IsRunning("枷锁"))
				{
					if (Mode == GRD_EMPTH)
						fVel = fVel * 1.8f;
					else
					{
						ucFLOAT length_z = vLinearVel | CenterBody.GetWay_Z();

						if (length_z > 40.0f)	fVel = 0.0f;
						Dir = 1.0f;
					}
				}
				else
				{
					if (Mode == GRD_EMPTH)
						fVel = fVel * 1.8f;
				}

				if (WheellRotY < +HandleMax)
					WheellRotY += fVel;
				PlayAction(3);
			}

			if ((!KeyState.KEYIsLeft() && !KeyState.KEYIsRight()) ||
				WheellRotY > +HandleMax ||
				WheellRotY < -HandleMax)
			{
				fVel *= HANDLE_FIX_BAS + Data.Handing * HANDLE_FIX_INC;
				if (WheellRotY < -fVel)	WheellRotY += fVel;
				if (WheellRotY > +fVel)	WheellRotY -= fVel;
			}
			if (KeyState.KEYIsDown() || Qualming)
				CenterBody.Rot = CenterBody.Rot.Value() - CenterBody.GetWay_Y() * WheellRotY * 0.05f;
			else
				CenterBody.Rot = CenterBody.Rot.Value() + CenterBody.GetWay_Y() * WheellRotY * 0.05f;

			Wheel[0].Rot.y = WheellRotY;				Wheel[1].Rot.y = WheellRotY;

			if (SceneCtrl != 0)
			{
				SceneCtrl->ShadowTex->Center->Pos = Center.Pos.Value();
			}

			if (KeyState.KEYIsDown() && Mode != GRD_EMPTH && DriftType == 0)
			{
				ucFLOAT max_speed = 80.0f * CenterPoint.Scale.z.Value;

				ucFLOAT Speed = vLinearVel.Length();
				if (Dir >= 0.0f || Speed < max_speed)
				{
					vLinearVel = vLinearVel - CenterBody.GetWay_Z() * 1.2f;
					Center.LinearVel = vLinearVel;
				}
			}

			if (Dir < 0.0f)
			{
				PlayAction(1);
			}

			if (KeyState.KEYIsReset())
			{
				if (Length < 10.0f)
				{
					Mode = GRD_RESET;
					ucINT Result = 0;
					SetStateNet("Reset", 0, &Result);
				}
			}
		}

		if (Center.LinearVel.y.Value < -160.0f)
		{
			Center.LinearVel.y = -160.0f;
		}

		DriftCheck();
		if (DriftUpTime == 0)
		{
			DriftUp = 0;
		}
		if (DriftUpTime > 0)
		{
			DriftUpTime--;

			if (!KeyState.KEYIsUp() && (DriftType == 0 || DriftType == 3))//上按键必须释放一次
			{
				DriftUp = 1;
			}

			if (KeyState.KEYIsUp() && DriftUp == 1)
			{
				ucINT Result = 0;
				SetStateNet("小冲", 0, &Result);
				DriftUpTime = 0;
				DriftUp = 0;
			}
		}

		if (DriftFire == 2)
		{
			DriftMeshMove.Visible = 1;
			DriftMeshRead.Visible = 0;
		}
		if (DriftFire == 1)
		{
			DriftMeshMove.Visible = 0;
			DriftMeshRead.Visible = 1;
		}
		if (DriftFire == 0)
		{
			DriftMeshMove.Visible = 0;
			DriftMeshRead.Visible = 0;
		}
		/*
		if (DriftType==0)
		{
			DriftMeshRead.Visible = 0;
			DriftMeshMove.Visible = 0;
		}
		else
		{
			DriftMeshRead.Visible = 0;
			DriftMeshMove.Visible = 1;
		}
		*/
	}

#define DRIFT_INIT	0.6f
	ucVOID	OnFiberDrift(UCObject* Sender, UCEventArgs*)
	{
		UCTickFiberData* FiberData = (UCTickFiberData*)Sender;

		UCDevice3D* Device = (UCDevice3D*)UIGetDevice();
		UCCameraSmooth* Camera = (UCCameraSmooth*)CenterBody.Camera;

		DriftHandle = 0;
		uc3dxVector3		SpeedZBackup;		//备份方向	UCHuman()
		while (FiberData->IsValid())
		{
			if (Camera == 0)
			{
				FiberData->SyncTick(1);
				continue;
			}

			if (DriftType == 3)
			{
				//高速状态
				DriftHandle = 40;
				DriftType = 0;
			}

			//默认状态
			while (FiberData->IsValid())
			{
				if (HumanBody.Rot.z.Value < -0.002f)
					HumanBody.Rot.z += 0.002f;
				else if (HumanBody.Rot.z.Value > +0.002f)
					HumanBody.Rot.z -= 0.002f;
				else
					HumanBody.Rot.z = 0.0f;

				//小冲有效阶段
				if (DriftHandle > 0)
				{
					DriftUpTime = 40;
					DriftHandle--;
				}


				ucINT KeyIsL = KeyState.KEYIsLeft() | KeyL;
				ucINT KeyIsR = KeyState.KEYIsRight() | KeyR;

				ucINT DriftDir_New = 0;	if (KeyIsL)	DriftDir_New = -1;	else if (KeyIsR)	DriftDir_New = +1;
				//如果可以漂移
				if (DriftAble() && DriftDir_New != 0)
				{
					uc3dxVector3 vLinearVel = Center.LinearVel.Value();	vLinearVel.y = 0.0f;

					SpeedBackup = vLinearVel.Length();
					SpeedZBackup = CenterBody.GetWay_Z();

					WheellRotY = 0;
					DriftType = 1;
					DriftPowerBackup = DriftPower;

					DriftDir = DriftDir_New;

					break;
				}

				FiberData->SyncTick(1);
			}

			ucINT DirTick = 60;
			DriftFire = 1;
			while (FiberData->IsValid() && DriftType == 1)
			{
				uc3dxVector3 vLinearVel = Center.LinearVel.Value();
				uc3dxVector3 vLinearDir = vLinearVel;	vLinearDir.Normalize();

				ucFLOAT WayX = CenterBody.GetWay_X() | vLinearDir;

				ucINT KeyIsL = KeyState.KEYIsLeft() | KeyL;
				ucINT KeyIsR = KeyState.KEYIsRight() | KeyR;

				ucINT DriftDir_New = 0;	if (KeyIsL)	DriftDir_New = -1;	else if (KeyIsR)	DriftDir_New = +1;
				if (DriftDir_New * DriftDir < 0)
				{
					if (Camera->Distance.y > -0.1f && DriftDir == -1)
					{
						DriftType = 0;
						break;
					}
					if (Camera->Distance.y < +0.1f && DriftDir == +1)
					{
						DriftType = 0;
						break;
					}
				}
				DirTick--;
				if (DirTick <= 0)
				{
					DriftType = 0;
					break;
				}
				//如果在左方向
				if (WayX > +DRIFT_INIT && DriftDir == -1 && Camera->Distance.y < -0.2f)
				{
					DriftDir = -1;
					DriftType = 2;
					break;
				}
				//如果在右方向
				if (WayX<-DRIFT_INIT && DriftDir == +1 && Camera->Distance.y > +0.2f)
				{
					DriftDir = +1;
					DriftType = 2;
					break;
				}

				ucFLOAT SpdZ = vLinearVel.Length() / (VSPEED_BAS + Data.MaxSpeed * VSPEED_INC);

				ucFLOAT DriftData = DRIFT_BAS + DRIFT_INC * Data.Drift;
				//如果在左方向
				if (DriftDir_New == -1)
				{
					if (WheellRotY > -DriftData)
						WheellRotY -= DRIFT_SPD + DRIFT_INC * SpdZ;
				}
				//如果在右方向
				if (DriftDir_New == +1)
				{
					if (WheellRotY < +DriftData)
						WheellRotY += DRIFT_SPD + DRIFT_INC * SpdZ;
				}
				Center.LinearVel = vLinearDir * SpeedBackup;

				FiberData->SyncTick(1);
			}
			SpeedBackup = Center.LinearVel.Value().Length();

			ucINT DriftDirWait = 0;
			DriftFire = 2;

			//漂移进行中维持阶段
			if (FiberData->IsValid() && DriftType == 2)
			{
				while (FiberData->IsValid())
				{
					uc3dxVector3 vLinearVel = Center.LinearVel.Value();
					uc3dxVector3 vLinearDir = vLinearVel;	vLinearDir.Normalize();

					ucFLOAT WayZ = vLinearDir | CenterBody.GetWay_Z();	//角度

					ucINT KeyIsL = KeyState.KEYIsLeft() | KeyL;
					ucINT KeyIsR = KeyState.KEYIsRight() | KeyR;

					ucINT DriftDir_New = 0;	if (KeyIsL)	DriftDir_New = -1;	else if (KeyIsR)	DriftDir_New = +1;

					ucFLOAT SpdZ = vLinearVel.Length() / (VSPEED_BAS + Data.MaxSpeed * VSPEED_INC);
					ucFLOAT WayX = CenterBody.GetWay_X() | vLinearDir;

					if (DriftDir * DriftDir_New < 0)
						SpdZ *= 1.6f;
					ucFLOAT DriftData = DRIFT_BAS + DRIFT_INC * Data.Drift;
					//如果在左方向
					if (DriftDir_New == -1)
					{
						if (WheellRotY > -DriftData)
							WheellRotY -= DRIFT_SPD + DRIFT_INC * SpdZ;
					}
					//如果在右方向
					if (DriftDir_New == +1)
					{
						if (WheellRotY < +DriftData)
							WheellRotY += DRIFT_SPD + DRIFT_INC * SpdZ;
					}

					if (DriftDir == -1)
					{
						//if ()
						if (Camera->Distance.y > -0.2f)
						{
							DriftType = 3;
							break;
						}
					}
					if (DriftDir == +1)
					{
						if (Camera->Distance.y < +0.2f)
							//if ()
						{
							DriftType = 3;
							break;
						}
					}

					CheckDriftPower();

					if (DriftType == 0)
						break;

					vLinearDir.Normalize();
					ucFLOAT Dis = vLinearDir | CenterBody.GetWay_Z();
					if (Dis < 0.0f)
					{
						ucFLOAT fY = Center.LinearVel.y.Value;
						if (DriftDir == -1)
							Center.LinearVel = CenterBody.GetWay_X() * +SpeedBackup;
						if (DriftDir == +1)
							Center.LinearVel = CenterBody.GetWay_X() * -SpeedBackup;
						Center.LinearVel.y = fY;
					}
					else
					{
						Center.LinearVel = vLinearDir * SpeedBackup;
					}

					if (SpeedBackup < 40.0f)
						DriftFire = 1;
					if (SpeedBackup < 20.0f)
						DriftFire = 0;

					if (SpeedBackup <= 10.0f)
						DriftType = 0;

					//gCarSound.SetVolume(23,-1000);
					//gCarSound.PlaySoundL(23);
					FiberData->SyncTick(1);
				}
			}

			DriftFire = 0;

			//gCarSound.Stop(23);

			FiberData->SyncTick(1);
		}
	}
	ucVOID	SetVisible(ucINT Visible)
	{
		CarBody.Visible = Visible;
		CarVent.Visible = Visible;
		if (IsSelf)
		{
			for (ucINT i = 0; i < MAXVENTFIRE; i++)
				CarVentFire[i].Visible = Visible;
		}

		Ball.Visible = Visible;
		HumanBody.Visible = Visible;
		HumanHair.Visible = Visible;
		HumanFace.Visible = Visible;
		HumanSuit.Visible = Visible;
		HumanPant.Visible = Visible;
		HumanShoe.Visible = Visible;
		HumanHand.Visible = Visible;

		Wheel[0].Visible = Visible;
		Wheel[1].Visible = Visible;
		Wheel[2].Visible = Visible;
		Wheel[3].Visible = Visible;
	}
	ucVOID	SetHandle(ucVOID* Handle)
	{
		//Shadow.Handle = Handle;
		Center.Handle = Handle;
	}
	ucVOID	SetPosition(uc3dxVector3& Position)
	{
		uc3dxVector3 Rotation(PortalObject.Dir);

		uc3dxVector3 vZero(0.0f, 0.0f, 0.0f);

		Center.Pos = Position;
		CenterBody.Pos = uc3dxVector3(Position.x, Position.y - INIT_Y, Position.z);
		CenterBody.Rot = Rotation;
		CenterBody.Rot = Rotation;

		Center.LinearVel = vZero;
		Center.AngularVel = vZero;

		PortalObject.Update(Center.Pos.Value());
	}
	ucVOID	Reset()
	{
		UCPortal* Portal = PortalObject.PortalReset;
		if (Portal == 0)
			return;

		uc3dxVector3 Dis = Portal->Center - Center.Pos.Value();
		if (Dis.Length() < 600.0f)
			Portal = Portal->Room1->InPortals.GetAt(0);

		uc3dxVector3	Position = Portal->Center - Portal->Normal * 10.0f;
		uc3dxVector3	Rotation = Portal->Normal;

		uc3dxVector3 vZero(0.0f, 0.0f, 0.0f);

		Center.Pos = uc3dxVector3(Position.x, Position.y, Position.z);
		CenterBody.Face(Rotation);
		CenterBody.Face(Rotation);

		Center.LinearVel = vZero;
		Center.AngularVel = vZero;
	}
	ucVOID	ZeroY()
	{
		uc3dxVector3 LinearVel = Center.LinearVel.Value();
		ucFLOAT vX = LinearVel | CenterBody.GetWay_X();
		ucFLOAT vZ = LinearVel | CenterBody.GetWay_Z();

		Center.LinearVel = CenterBody.GetWay_X() * vX + CenterBody.GetWay_Z() * vZ;
	}
	ucVOID	OnFiberStar(UCObject* Sender, UCEventArgs*)
	{
		UCTimeFiberData* FiberData = (UCTimeFiberData*)Sender;

		UCMesh	MeshStar;
		MeshStar.Name = UCString("res/model/action/peng.dat");
		MeshStar.Parent = &Center;
		MeshStar.PlayAction(0, 1.0f, 1);

		while (!MeshStar.IsActionEnd() && FiberData->IsValid())
			FiberData->Delay(100);
	}
	ucVOID	RefreshWall(UCPContactArgs* Args, UCPContacts& Contacts)
	{
		UCMeshPhyBase* Wall = 0;
		if (Args->Mesh1 == &Center)	Wall = Args->Mesh2;
		if (Args->Mesh2 == &Center)	Wall = Args->Mesh1;

		if (Contacts.Size == 0)
			return;

		if (Wall)
		{
			KeyDrift.Cancel();
			DriftType = 0;
			SpeedBackup = 0.0f;

			uc3dxVector3 LinearVel = Center.LinearVel.Value();
			ucFLOAT length_z = LinearVel | CenterBody.GetWay_Y();

			ucFLOAT fDepth = Contacts.DepthAt(0);
			uc3dxVector3 Nor = Contacts.NormalAt(0);


			if (Nor.x > 0.25f || Nor.z > 0.25f || Nor.x < -0.25f || Nor.z < -0.25f)
			{
				Nor.y = 0.0f;
				Nor.Normalize();

				Contacts.SetNormal(0, Nor);
			}

			if (Nor.y > 0.9f)
			{
				ucFLOAT fN = Nor | CenterBody.GetWay_Z();
				if (fN < 0.0f)
					Center.LinearVel = -LinearVel / 2.0f;
			}
			else
			{
				ucFLOAT Weight = Nor | LinearVel;
				if (Weight < -40.0f)
				{
					if (IsSelf)
					{
						//gCarSound.PlaySound(22);
						FiberStar.RunNew(0);
					}

					ucFLOAT fN = Nor | CenterBody.GetWay_Z();
					if (fN < 0.0f)
					{
						ucFLOAT fD = CenterBody.GetWay_X() | Nor;
						if (fD > 0.0f)
						{
							CenterBody.Rot = CenterBody.Rot.Value() + CenterBody.GetWay_Y() * 0.4f;
						}
						if (fD < 0.0f)
						{
							CenterBody.Rot = CenterBody.Rot.Value() - CenterBody.GetWay_Y() * 0.4f;
						}

						Center.LinearVel = CenterBody.GetWay_Z() * ((LinearVel | CenterBody.GetWay_Z()) * 0.6f) + Nor * ((LinearVel | Nor) * 0.6f);//前���?值：速度损失，后一个值：反弹力?
					}
				}
			}


			if (State.IsRunning("儿童"))
			{
				ucFLOAT fN = CenterBody.GetWay_Z() | LinearVel;
				if (fN > 0.0f)	Center.LinearVel = -LinearVel;
				State.Set("正翻");
			}

			if (DriftType != 0)	DriftPower = DriftPowerBackup;
		}
	}
	ucVOID	RefreshGround(UCPContactArgs* Args, UCPContacts& Contacts)
	{
		UCMeshScene* Face = 0;
		if (Args->Mesh1 == &Center)	Face = (UCMeshScene*)Args->Mesh2;
		if (Args->Mesh2 == &Center)	Face = (UCMeshScene*)Args->Mesh1;

		if (IsSelf)
		{
			ucFLOAT CarBodyMax = 0.24f;
			ucFLOAT CarVentMax = 0.12f;
			ucFLOAT CarBodySpeed = 0.004f;
			ucFLOAT CarBodyRestore = 0.004f;
			ucFLOAT CarVentSpeed = 0.008f;
			if (DriftType == 2 && Center.LinearVel.Value().Length() > 40.0f)
			{
				if (DriftDir == -1)
				{
					if (CarBody.Rot.z.Value < +CarBodyMax)
						CarBody.Rot.z += CarBodySpeed;
				}
				if (DriftDir == +1)
				{
					if (CarBody.Rot.z.Value > -CarBodyMax)
						CarBody.Rot.z -= CarBodySpeed;
				}
			}
			else
			{
				if (CarBody.Rot.z.Value > +CarBodyRestore)
					CarBody.Rot.z -= CarBodyRestore;
				if (CarBody.Rot.z.Value < -CarBodyRestore)
					CarBody.Rot.z += CarBodyRestore;
			}
			if (CarBody.Rot.z.Value > +CarVentMax && WheelCenter.Rot.z.Value > -CarVentMax)
				WheelCenter.Rot.z -= CarVentSpeed;
			if (CarBody.Rot.z.Value < -CarVentMax && WheelCenter.Rot.z.Value < +CarVentMax)
				WheelCenter.Rot.z += CarVentSpeed;
			if (CarBody.Rot.z.Value < +CarVentMax && CarBody.Rot.z.Value > -CarVentMax)
			{
				WheelCenter.Rot.z = -CarBody.Rot.z.Value;
			}
		}

		if (Face)
		{
			if (JumpState == JUMP_NONE)
			{
				uc3dxVector3 LinearVel = Center.LinearVel.Value();

				ucFLOAT vX = LinearVel | CenterBody.GetWay_X();
				ucFLOAT vY = LinearVel | CenterBody.GetWay_Y();
				ucFLOAT vZ = LinearVel | CenterBody.GetWay_Z();

				Center.LinearVel = CenterBody.GetWay_X() * vX + CenterBody.GetWay_Z() * vZ;

				if (vY <= -50.0f)
				{
					JumpDust = 1;
				}
			}

			uc3dxVector3 Pos = Contacts.PointAt(0);
			Head = Contacts.NormalAt(0);

			if (DriftFire == 2)
			{
				ucFLOAT RadiusX = BODY_RADIUS * 0.5f;
				ucFLOAT RadiusZ = BODY_RADIUS * 0.3f;
				uc3dxVector3 vPosL = Pos - CenterBody.GetWay_X() * RadiusX - CenterBody.GetWay_Z() * RadiusZ;
				uc3dxVector3 vPosR = Pos + CenterBody.GetWay_X() * RadiusX - CenterBody.GetWay_Z() * RadiusZ + uc3dxVector3(0.0f, 2.0f, 0.0f);

				BrakeTraceL.Trace(vPosL, Head, 0.0f);
				BrakeTraceR.Trace(vPosR, Head, 0.0f);
			}
			else
			{
				BrakeTraceL.Break();
				BrakeTraceR.Break();
			}

			Mode = GRD_FLOOR;
		}
	}
	ucVOID	SetStateNet(ucTCHAR* Name, ucINT Param, ucINT* Result)
	{
		DriftType = 0;
		KeyDrift.Cancel();

		ucINT Ret = State.Set(Name, Param);
		if (Ret)
		{
			UCSMData* Data = State.GetData(Name);
		}
		*Result = Ret;
	}
	ucVOID	OnFiberShock(UCObject* Sender, UCEventArgs*)
	{
		UCTimeFiberData* FiberData = (UCTimeFiberData*)Sender;
	}
	ucVOID	RefreshDirection()
	{
		if (Mode == GRD_FLOOR)	
			CenterBody.Head(Head);

		uc3dxVector3	vPos = Center.Pos.Value();
		CenterBody.Pos = uc3dxVector3(vPos.x, vPos.y - DIS_Y, vPos.z);

		if (CenterBody.Camera)
			CenterBody.Camera->Rot.z = HumanBody.Rot.z;
	}

	ucVOID	RefreshMode()
	{
		UCDevice3D* Device3D = (UCDevice3D*)UIGetDevice();

		uc3dxVector3 vLinearVel = Center.LinearVel.Value();

		UCCamera* Camera = CenterBody.Camera;
		if (Mode == GRD_EMPTH && Center.GravityMode.Value != 0)
		{
			ucFLOAT fY = vLinearVel | CenterBody.GetWay_Y();
			if (fY > 10.0f)
			{
				if (CenterBody.Rot.x.Value > -0.3f)
					CenterBody.Rot.x.Value -= 0.01f;
				if (HumanBody.Pos.y.Value < 0.0f)
					HumanBody.Pos.y += 1.0f;
				if (HumanBody.Rot.x.Value > -0.5f)
					HumanBody.Rot.x -= 0.02f;
				if (WheelCenter.Pos.y.Value > -4.0f)
				{
					CenterBody.Pos.y += 0.4f;
					WheelCenter.Pos.y -= 0.4f;
				}
			}
			if (fY < -10.0f)
			{
				if (CenterBody.Rot.x.Value < 0.3f)
					CenterBody.Rot.x.Value += 0.02f;
				if (HumanBody.Pos.y.Value > HUMANPOSY)
					HumanBody.Pos.y -= 0.25f;
				if (HumanBody.Rot.x.Value < 0.0f)
					HumanBody.Rot.x += 0.01f;
			}

			//如果没有跳跃，修正向上的力
			if (JumpState == JUMP_NONE && vLinearVel.y > 0.0f)
				Center.LinearVel.y = 0.0f;
			//如果在跳跃准备，进入跳跃过程
			if (JumpState == JUMP_WAIT)
				JumpState = JUMP_PROC;
		}
		else if (Mode == GRD_FLOOR)
		{
			if (WheelCenter.Pos.y.Value < -1.6f)
			{
				CenterBody.Pos.y -= 1.6f;
				WheelCenter.Pos.y += 1.6f;
			}
			else if (WheelCenter.Pos.y.Value < +0.0f)
			{
				CenterBody.Pos.y += WheelCenter.Pos.y.Value;
				WheelCenter.Pos.y = 0.0f;
			}
			if (Camera != 0 && CameraFixAble != 0)
			{
				if (Camera->Rot.x.Value > MIN_ROT_X)
					Camera->Rot.x -= STP_ROT_X;
				if (Camera->Rot.x.Value < -MIN_ROT_X)
					Camera->Rot.x += STP_ROT_X;
			}


			HumanBody.Pos.y = HUMANPOSY;
			HumanBody.Rot.x = 0.0f;

			//如果在跳跃过程，转成结束
			if (JumpState == JUMP_PROC)
				JumpState = JUMP_NONE;
		}

		Mode = GRD_EMPTH;

		ucFLOAT Speed = Center.LinearVel.Value().Length();
		if ((vLinearVel | CenterBody.GetWay_Z()) > 0.0f)
		{
			Speed = -Speed;
		}

		Wheel[0].Rot.x += Speed / (3.14f * Wheel[0].GetAABB().GetExtents().y * 2.0f);
		Wheel[1].Rot.x += Speed / (3.14f * Wheel[1].GetAABB().GetExtents().y * 2.0f);
		Wheel[2].Rot.x += Speed / (3.14f * Wheel[2].GetAABB().GetExtents().y * 2.0f);
		Wheel[3].Rot.x += Speed / (3.14f * Wheel[3].GetAABB().GetExtents().y * 2.0f);

		for (ucINT i = 0; i < MAXVENTFIRE; i++)
			CarVentFire[i].SetActionSpeed(GetWheelFireSpeed());

		PortalObject.Update(Center.Pos.Value());

		Center.AngularVel = uc3dxVector3(0.0f, 0.0f, 0.0f);
	}
	ucVOID	SetMaterail(UCMaterial* nvalue)
	{
		HumanBody.SetMaterial(nvalue);
		HumanFace.SetMaterial(nvalue);
		HumanHair.SetMaterial(nvalue);
		HumanPant.SetMaterial(nvalue);
		HumanShoe.SetMaterial(nvalue);
		HumanHand.SetMaterial(nvalue);
		HumanSuit.SetMaterial(nvalue);
		Ball.SetMaterial(nvalue);

	}
	ucVOID	SetBaseMaterail()
	{
		HumanBody.SetMaterial(&Material);
		HumanFace.SetMaterial(&Material);
		HumanHair.SetMaterial(&Material);
		HumanPant.SetMaterial(&Material);
		HumanShoe.SetMaterial(&Material);
		HumanHand.SetMaterial(&Material);

		HumanSuit.SetMaterial(&Material);
		Ball.SetMaterial(&Material);

		CarBody.SetMaterial(&Material);
		Wheel[0].SetMaterial(&Material);
		Wheel[1].SetMaterial(&Material);
		Wheel[2].SetMaterial(&Material);
		Wheel[3].SetMaterial(&Material);
	}
	ucVOID	AddAction(ucINT Index, ucINT IndexBreak, ucINT Times, ucINT PRI)
	{
		HumanBody.ActionManage.AddAction(Index, IndexBreak, Times, PRI);
		HumanFace.ActionManage.AddAction(Index, IndexBreak, Times, PRI);
		HumanHair.ActionManage.AddAction(Index, IndexBreak, Times, PRI);
		HumanSuit.ActionManage.AddAction(Index, IndexBreak, Times, PRI);
		HumanPant.ActionManage.AddAction(Index, IndexBreak, Times, PRI);
		HumanShoe.ActionManage.AddAction(Index, IndexBreak, Times, PRI);
		HumanHand.ActionManage.AddAction(Index, IndexBreak, Times, PRI);;
	}
	ucVOID	ActionSpeed(ucFLOAT Speed)
	{
		HumanBody.SetActionSpeed(Speed);
		HumanFace.SetActionSpeed(Speed);
		HumanHair.SetActionSpeed(Speed);
		HumanSuit.SetActionSpeed(Speed);
		HumanPant.SetActionSpeed(Speed);
		HumanShoe.SetActionSpeed(Speed);
		HumanHand.SetActionSpeed(Speed);
	}
	ucVOID	ActionTimes(ucINT Times)
	{
		HumanBody.SetActionTimes(Times);
		HumanFace.SetActionTimes(Times);
		HumanHair.SetActionTimes(Times);
		HumanSuit.SetActionTimes(Times);
		HumanPant.SetActionTimes(Times);
		HumanShoe.SetActionTimes(Times);
		HumanHand.SetActionTimes(Times);
	}
	ucVOID	PlayAction(ucINT Pos)
	{
		HumanBody.PlayActionSmooth(Pos);
		HumanFace.PlayActionSmooth(Pos);
		HumanHair.PlayActionSmooth(Pos);
		HumanSuit.PlayActionSmooth(Pos);
		HumanPant.PlayActionSmooth(Pos);
		HumanShoe.PlayActionSmooth(Pos);
		HumanHand.PlayActionSmooth(Pos);
	}
	ucVOID	SetActionFile()
	{
		UCString nDat = UCString("res/model/avatar/action/action.dat");

		if (Gender == 0 || Gender == 2)
			nDat = UCString("res/model/avatar/action/boy_action.dat");
		HumanBody.SetActionFile(nDat);
		HumanFace.SetActionFile(nDat);
		HumanHair.SetActionFile(nDat);
		HumanSuit.SetActionFile(nDat);
		HumanPant.SetActionFile(nDat);
		HumanShoe.SetActionFile(nDat);
		HumanHand.SetActionFile(nDat);
	}
};

#endif