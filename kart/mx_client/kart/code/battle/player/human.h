#ifndef UCHUMAN
#define UCHUMAN

#include "physics.h"

#define SPEED_NORMAL	0
#define SPEED_DRIFT		1

#define CAM_BASE_LENGTH	180.0f
class UCHuman
{
public:
	UCMaterial			Mtl;		//无敌
	UCMaterial			MtlBlack;	//涂黑
	UCMaterial			MtlRed;		//涂红
	UCMaterial			MtlQualm;	//混乱（描紫边）
	UCMaterial			MtlFreeze;	//冰封（蓝图）
	UCMaterial			Mtl1;		//免疫

	UCMesh				JumpDustMesh;

	UCMesh				SceneMesh;
	UCMesh				GroundMesh;			//地面特效
	UCMesh				CaiTiaoMesh;		//结束的彩条特效

	UCMeshRay*			CameraRay;
	ucFLOAT				BaseCameraRayLength;

	ucINT				Winned;
	ucINT				GroundState;

	ucINT				SexKind;
	ucINT				ID;					//玩家编号
	ucINT				CarID;
	ucINT				VentID;
	ucINT				SuitID;
	ucINT				PantID;
	ucINT				ShoeID;
	ucINT				HandID;
	ucINT				HairID;
	ucINT				FaceID;
	ucINT				BodyID;
	ucINT				BallID;
	ucINT				LicenceID;

	ucINT				GroupID;

	ucINT				Profession;

	ucINT				ItemID;

	ucINT				StateSpeed;			//速度状态

	ucINT				StateDir;			//上次状态方向

	UCMesh				Mine0;				//箱子
	UCMesh				Mine1;				//香皂

	UCGroupFiber		StateJumpDust;		//烟雾

	UCGroupFiber		StateThrowMine0;		//扔箱子
	UCGroupFiber		StateThrowMine1;		//扔肥皂
	UCGroupFiber		StateThrowMine3;		//定时
	UCGroupFiber		StateThrowMine4;		//烟雾
	UCGroupFiber		StateThrowMine5;		//扔加速站
	UCGroupFiber		StateThrowMine6;		//扔闪光波
	UCGroupFiber		StateThrowMine7;		//扔拳击手套
	UCGroupFiber		StateThrowMine8;		//扔睡眠
	UCGroupFiber		StateThrowMine9;		//扔收买
	UCGroupFiber		StateThrowMine10;		//扔地震

	UCGroupFiber		StateThrowReduced;				//扔缩小
	UCGroupFiber		StateThrowQualm;				//扔混乱
	UCGroupFiber		StateThrowFeiDan;				//扔飞弹，炸出大水泡
	UCGroupFiber		StateThrowGenZongDan;			//扔跟踪弹，打击前面一个敌人
	UCGroupFiber		StateThrowPanPat;				//扔锅铲厨师，朝前飞行，打击一定范围里面路过的敌人
	UCGroupFiber		StateThrowShiftDown;			//仍法力燃烧道具,使敌人速度降低,漂移槽降低
	UCGroupFiber		StateThrowNumOne;				//扔打第一名
	UCGroupFiber		StateThrowChenMo;				//扔沉默道具
	UCGroupFiber		StateThrowQusan;				//扔驱散道具
	UCGroupFiber		StateThrowMianYi;				//扔免疫道具
	UCGroupFiber		StateThrowQiangHua;				//扔强化道具
	UCGroupFiber		StateThrowHuiMo;				//扔回魔道具
	UCGroupFiber		StateThrowSuperBox;				//扔超级飞弹
	UCGroupFiber		StateThrowDarkCloud;				//扔超级障碍
	UCGroupFiber		StateThrowJiaShuo;					//扔枷锁
	UCGroupFiber		StateThrowLaoLong;

	UCEventManager		OnMine0Throw;
	UCEventManager		OnMine1Throw;
	UCEventManager		OnMine2Throw;
	UCEventManager		OnMine3Throw;
	UCEventManager		OnMine4Throw;
	UCEventManager		OnMine5Throw;//产生飞弹，飞行一段时间炸出大水泡
	UCEventManager		OnMine6Throw;//产生跟踪弹，打击前面一个敌人
	UCEventManager		OnMine7Throw;//产生好坏加速站
	UCEventManager		OnMine8Throw;//产生厨师进行拍打
	UCEventManager		OnMine9Throw;//生成减速棒，使周围的敌人速度降低
	UCEventManager		OnMine10Throw;//生成闪光圈的前身

	UCEventManager		OnSuperBoxThrow;//生成超级障碍的前身
	UCEventManager		OnDarkCloudThrow;//生成闪光圈的前身

	UCEventManager		OnMine11Throw;//生成道具枷锁
	UCEventManager		OnMine12Throw;//生成正式牢笼的前身,这个是飞行的牢笼,飞行完毕,生成正式牢笼,发挥牢����作用

	UCEventManager		OnStateFirst;

	UCCarPhysics		Physics;
	ucINT				Power;

	ucINT				PanPated;

	ucINT				ShiftDown;

	UCControl			White;

	UCImage				ChengMoPic[3];
	UCImage				PaintBlack;
	UCImage				PaintBlack2;
	UCImage				PaintBlack3;
	UCImage				ShiftSpeedDownPic;

	ucINT				ItemSelect;
	ucINT				ItemSize;
	UCString			Item[MAX_PACK];

	ucFLOAT				AutoDriveSpeed;
	ucFLOAT				AutoDriveDisX;
	ucFLOAT				AutoDriveRotY;

	UCHuman()
	{
		Power = 0;

		BaseCameraRayLength = CAM_BASE_LENGTH;

		AutoDriveSpeed = 200.0f;
		AutoDriveDisX = 0.0f;
		AutoDriveRotY = 0.0f;
		Winned = 0;
		ItemSelect = -1;
		ItemSize = 0;

		CameraRay = 0;
		Profession = 1;

		ucINT iSpeed = 10000;
		ucFLOAT alpha = 0.5f;

		ucFLOAT alpha1 = 1.0f;

		Mtl.Name = UCString("res/shader/invincible.material");
		Mtl.SetScriptValue(UCString("g_iSpeed"), &iSpeed, typeof(iSpeed));
		Mtl.SetScriptValue(UCString("g_fAlpha"), &alpha1, typeof(alpha1));
		Mtl.AppendTexture(UCString("res/invincible.bmp"), 1);


		MtlBlack.Name = UCString("res/shader/invincible.material");
		MtlBlack.SetScriptValue(UCString("g_iSpeed"), &iSpeed, typeof(iSpeed));
		MtlBlack.SetScriptValue(UCString("g_fAlpha"), &alpha1, typeof(alpha1));
		MtlBlack.AppendTexture(UCString("res/invincibleblack.bmp"), 1);

		MtlRed.Name = UCString("res/shader/invincible.material");
		MtlRed.SetScriptValue(UCString("g_iSpeed"), &iSpeed, typeof(iSpeed));
		MtlRed.SetScriptValue(UCString("g_fAlpha"), &alpha, typeof(alpha));
		MtlRed.AppendTexture(UCString("res/invinciblered.bmp"), 1);

		MtlQualm.Name = UCString("res/shader/border.material");
		//紫色
		ucFLOAT colorR = 0.6f;
		ucFLOAT colorG = 0.05f;
		ucFLOAT colorB = 0.8f;
		MtlQualm.SetEffectValue(UCString("g_fColorR"), &colorR, typeof(colorR));
		MtlQualm.SetEffectValue(UCString("g_fColorG"), &colorG, typeof(colorG));
		MtlQualm.SetEffectValue(UCString("g_fColorB"), &colorB, typeof(colorB));
		ucFLOAT fWight = 0.85f;//加粗
		MtlQualm.SetEffectValue(UCString("g_fWeight"), &fWight, typeof(fWight));


		MtlFreeze.Name = UCString("res/shader/invincible.material");
		MtlFreeze.SetScriptValue(UCString("g_iSpeed"), &iSpeed, typeof(iSpeed));
		MtlFreeze.SetScriptValue(UCString("g_fAlpha"), &alpha, typeof(alpha));
		MtlFreeze.AppendTexture(UCString("res/invincible2.bmp"), 1);


		Mtl1.Name = UCString("res/shader/hightlight.material");
		Mtl1.SetScriptValue(UCString("g_iSpeed"), &iSpeed, typeof(iSpeed));
		alpha = 2.0f;
		Mtl1.SetScriptValue(UCString("g_fAlpha"), &alpha, typeof(alpha));
		Mtl1.AppendTexture(UCString("res/cartoon.bmp"), 1);

		SexKind = 0;

		ItemID = 0;

		PanPated = 0;

		ShiftDown = 0;

		StateDir = 0;

		StateSpeed = SPEED_NORMAL;

		Physics.SetHandle(this);

		JumpDustMesh.Name = UCString("res/model/action/jumpdust.dat");
		JumpDustMesh.Parent = &Physics.CenterPoint;
		JumpDustMesh.Scale = uc3dxVector3(1.5f, 1.5f, 1.5f);
		JumpDustMesh.Visible = 0;
		JumpDustMesh.Pos.y = -16.0f;
		JumpDustMesh.PlayAction(0);
		JumpDustMesh.SetActionSpeed(5.0f);

		GroundMesh.Parent = &Physics.CenterPoint;
		GroundMesh.Visible = 0;

		CaiTiaoMesh.Parent = &Physics.CenterPoint;
		CaiTiaoMesh.Visible = 0;
		CaiTiaoMesh.Name = UCString("res/model/action/finish_caitiao.dat");
		CaiTiaoMesh.Scale = uc3dxVector3(1.2f, 1.2f, 1.2f);
		CaiTiaoMesh.Pos.y = 0.0f;
		//CaiTiaoMesh.PlayAction(0);
		//CaiTiaoMesh.SetMaterial(&Physics.Material);

		Physics.FiberAutoDrive.FiberEvent = UCEvent(this, &UCHuman::OnFiberAutoDrive);
		//StateJumpDust.FiberEvent = UCEvent(this, &UCHuman::OnStateJumpDust);

		UCSMData* Data = 0;
		Data = Physics.State.GetData("透明");
		if (Data)	Data->OnStart = UCEvent(this, &UCHuman::OnSMBlank);

		Data = Physics.State.GetData("偏移");
		if (Data)	Data->OnStart = UCEvent(this, &UCHuman::OnSMSideFlip);

		Data = Physics.State.GetData("后退");
		if (Data)	Data->OnStart = UCEvent(this, &UCHuman::OnSMSideBack);

		Data = Physics.State.GetData("小冲");
		if (Data)	Data->OnStart = UCEvent(this, &UCHuman::OnSMDriftUp);

		Data = Physics.State.GetData("冲刺");
		if (Data)	Data->OnStart = UCEvent(this, &UCHuman::OnSMChongCi);

		Data = Physics.State.GetData("加速");
		if (Data)	Data->OnStart = UCEvent(this, &UCHuman::OnSMSpeedUp);

		Data = Physics.State.GetData("超级加速");
		if (Data)	Data->OnStart = UCEvent(this, &UCHuman::OnSMSuperSpeedUp);

		Data = Physics.State.GetData("无敌");
		if (Data)	Data->OnStart = UCEvent(this, &UCHuman::OnSMPower);

		Data = Physics.State.GetData("打滑");
		if (Data)	Data->OnStart = UCEvent(this, &UCHuman::OnSMSkid);

		Data = Physics.State.GetData("蝙蝠");
		if (Data)	Data->OnStart = UCEvent(this, &UCHuman::OnSMBat);

		Data = Physics.State.GetData("水泡");
		if (Data)	Data->OnStart = UCEvent(this, &UCHuman::OnSMHigh);

		Data = Physics.State.GetData("第一名");
		if (Data)	Data->OnStart = UCEvent(this, &UCHuman::OnSMFirst);

		Data = Physics.State.GetData("燃烧");
		if (Data)	Data->OnStart = UCEvent(this, &UCHuman::OnSMFire);

		Data = Physics.State.GetData("正翻");
		if (Data)	Data->OnStart = UCEvent(this, &UCHuman::OnSMDizzy);

		Data = Physics.State.GetData("侧翻");
		if (Data)	Data->OnStart = UCEvent(this, &UCHuman::OnSMSideRoll);

		Data = Physics.State.GetData("驱散");
		if (Data)	Data->OnStart = UCEvent(this, &UCHuman::OnSMQusan);

		Data = Physics.State.GetData("免疫");
		if (Data)	Data->OnStart = UCEvent(this, &UCHuman::OnSMMianYi);

		Data = Physics.State.GetData("强化");
		if (Data)	Data->OnStart = UCEvent(this, &UCHuman::OnSMQiangHua);

		Data = Physics.State.GetData("回魔");
		if (Data)	Data->OnStart = UCEvent(this, &UCHuman::OnSMHuimo);

		Data = Physics.State.GetData("缩小");
		if (Data)	Data->OnStart = UCEvent(this, &UCHuman::OnSMScaleDown);

		Data = Physics.State.GetData("混乱");
		if (Data)	Data->OnStart = UCEvent(this, &UCHuman::OnSMQualm);

		Data = Physics.State.GetData("盲目");
		if (Data)	Data->OnStart = UCEvent(this, &UCHuman::OnSMBlind);

		Data = Physics.State.GetData("沉默");
		if (Data)	Data->OnStart = UCEvent(this, &UCHuman::OnSMChengMo);

		Data = Physics.State.GetData("Reset");
		if (Data)	Data->OnStart = UCEvent(this, &UCHuman::OnSMReset);

		Data = Physics.State.GetData("超人");
		if (Data)	Data->OnStart = UCEvent(this, &UCHuman::OnSMRocket);

		Data = Physics.State.GetData("自动驾驶");
		if (Data)	Data->OnStart = UCEvent(this, &UCHuman::OnSMAutoDrive);

		Data = Physics.State.GetData("测试驾驶");
		if (Data)	Data->OnStart = UCEvent(this, &UCHuman::OnSMTestDrive);


		Data = Physics.State.GetData("第一名准备");
		if (Data)	Data->OnStart = UCEvent(this, &UCHuman::OnSMStateReadyFirst);

		Data = Physics.State.GetData("燃烧准备");
		if (Data)	Data->OnStart = UCEvent(this, &UCHuman::OnSMStateReadyFire);

		Data = Physics.State.GetData("蝙蝠准备");
		if (Data)	Data->OnStart = UCEvent(this, &UCHuman::OnSMStateReadyBianFu);

		Data = Physics.State.GetData("蝙蝠飞行");
		if (Data)	Data->OnStart = UCEvent(this, &UCHuman::OnSMBianFuFly);

		Data = Physics.State.GetData("厨师拍打");
		if (Data)	Data->OnStart = UCEvent(this, &UCHuman::OnSMPanPat);

		Data = Physics.State.GetData("回收");
		if (Data)	Data->OnStart = UCEvent(this, &UCHuman::OnSMRubbishSpeed);

		Data = Physics.State.GetData("涂黑");
		if (Data)	Data->OnStart = UCEvent(this, &UCHuman::OnSMPaintPat);

		Data = Physics.State.GetData("闪光");
		if (Data)	Data->OnStart = UCEvent(this, &UCHuman::OnSMShineShake);

		Data = Physics.State.GetData("减速棒");
		if (Data)	Data->OnStart = UCEvent(this, &UCHuman::OnSMSpeedDown);

		Data = Physics.State.GetData("法力燃烧");
		if (Data)	Data->OnStart = UCEvent(this, &UCHuman::OnSMShiftSpeedDown);

		Data = Physics.State.GetData("地震");
		if (Data)	Data->OnStart = UCEvent(this, &UCHuman::OnSMShock);

		Data = Physics.State.GetData("儿童");
		if (Data)	Data->OnStart = UCEvent(this, &UCHuman::OnSMChild);

		Data = Physics.State.GetData("睡眠");
		if (Data)	Data->OnStart = UCEvent(this, &UCHuman::OnSMSleep);

		Data = Physics.State.GetData("收买");
		if (Data)	Data->OnStart = UCEvent(this, &UCHuman::OnSMBuyOver);

		Data = Physics.State.GetData("拳击手");
		if (Data)	Data->OnStart = UCEvent(this, &UCHuman::OnSMHittedByBoxer);

		Data = Physics.State.GetData("恶魔附身");
		if (Data)	Data->OnStart = UCEvent(this, &UCHuman::OnSMAdhereByDevil);

		Data = Physics.State.GetData("牢笼");
		if (Data)	Data->OnStart = UCEvent(this, &UCHuman::OnSMHitByLaoLong);

		Data = Physics.State.GetData("牢笼准备");
		if (Data)	Data->OnStart = UCEvent(this, &UCHuman::OnSMStateReadyLaoLong);

		Data = Physics.State.GetData("警灯");
		if (Data)	Data->OnStart = UCEvent(this, &UCHuman::OnSMJingDeng);


		Data = Physics.State.GetData("枷锁");
		if (Data)	Data->OnStart = UCEvent(this, &UCHuman::OnSMHitByJiaSuo);

		Data = Physics.State.GetData("透视镜");
		if (Data)	Data->OnStart = UCEvent(this, &UCHuman::OnSMClearEye);

		Data = Physics.State.GetData("超级针");
		if (Data)	Data->OnStart = UCEvent(this, &UCHuman::OnSMNeedle);

		Data = Physics.State.GetData("盾牌");
		if (Data)	Data->OnStart = UCEvent(this, &UCHuman::OnSMPeoBox);


		Data = Physics.State.GetData("碰撞无效");
		if (Data)	Data->OnStart = UCEvent(this, &UCHuman::OnSMPengWuXiao);

		Data = Physics.State.GetData("倒霉后保护");
		if (Data)	Data->OnStart = UCEvent(this, &UCHuman::OnSMBadProtect);

		StateThrowMine0.FiberEvent = UCEvent(this, &UCHuman::OnStateThrowMine0);
		StateThrowMine1.FiberEvent = UCEvent(this, &UCHuman::OnStateThrowMine1);
		//StateThrowMine2.FiberEvent = UCEvent(this,&UCHuman::OnStateThrowMine2);
		StateThrowMine3.FiberEvent = UCEvent(this, &UCHuman::OnStateThrowMine3);
		StateThrowMine4.FiberEvent = UCEvent(this, &UCHuman::OnStateThrowMine4);
		StateThrowMine5.FiberEvent = UCEvent(this, &UCHuman::OnStateThrowMine5);
		StateThrowMine6.FiberEvent = UCEvent(this, &UCHuman::OnStateThrowMine6);
		StateThrowMine7.FiberEvent = UCEvent(this, &UCHuman::OnStateThrowMine7);
		StateThrowMine8.FiberEvent = UCEvent(this, &UCHuman::OnStateThrowMine8);
		StateThrowMine9.FiberEvent = UCEvent(this, &UCHuman::OnStateThrowMine9);
		StateThrowMine10.FiberEvent = UCEvent(this, &UCHuman::OnStateThrowMine10);



		StateThrowReduced.FiberEvent = UCEvent(this, &UCHuman::OnStateThrowReduced);
		StateThrowQualm.FiberEvent = UCEvent(this, &UCHuman::OnStateThrowQualm);
		StateThrowFeiDan.FiberEvent = UCEvent(this, &UCHuman::OnStateThrowFeiDan);
		StateThrowGenZongDan.FiberEvent = UCEvent(this, &UCHuman::OnStateThrowGenZongDan);
		StateThrowPanPat.FiberEvent = UCEvent(this, &UCHuman::OnStateThrowPanPat);
		StateThrowNumOne.FiberEvent = UCEvent(this, &UCHuman::OnStateThrowNumOne);
		StateThrowChenMo.FiberEvent = UCEvent(this, &UCHuman::OnStateThrowChenMo);
		StateThrowQusan.FiberEvent = UCEvent(this, &UCHuman::OnStateThrowQusan);
		StateThrowMianYi.FiberEvent = UCEvent(this, &UCHuman::OnStateThrowMianYi);
		StateThrowQiangHua.FiberEvent = UCEvent(this, &UCHuman::OnStateThrowQiangHua);
		StateThrowHuiMo.FiberEvent = UCEvent(this, &UCHuman::OnStateThrowHuiMo);
		//StateThrowSpeedStick.FiberEvent = UCEvent(this,&UCHuman::OnStateThrowSpeedStick);
		StateThrowShiftDown.FiberEvent = UCEvent(this, &UCHuman::OnStateThrowShiftDown);

		StateThrowSuperBox.FiberEvent = UCEvent(this, &UCHuman::OnStateThrowSuperBox);
		StateThrowDarkCloud.FiberEvent = UCEvent(this, &UCHuman::OnStateThrowDarkCloud);

		StateThrowJiaShuo.FiberEvent = UCEvent(this, &UCHuman::OnStateThrowJiaShuo);
		StateThrowLaoLong.FiberEvent = UCEvent(this, &UCHuman::OnStateThrowLaoLong);


	}
	~UCHuman()
	{
		Physics.State.ClearAll();
	}
	ucVOID InitParentFiber(UCFiber* ParentFiber)
	{
		ParentFiber->AddChild(&Physics.FiberAutoDrive);
		StateJumpDust.SetParent(ParentFiber);

		StateThrowMine0.SetParent(ParentFiber);
		StateThrowMine1.SetParent(ParentFiber);
		//StateThrowMine2.SetParent(ParentFiber);
		StateThrowMine3.SetParent(ParentFiber);
		StateThrowMine4.SetParent(ParentFiber);
		StateThrowMine5.SetParent(ParentFiber);
		StateThrowMine6.SetParent(ParentFiber);
		StateThrowMine7.SetParent(ParentFiber);
		StateThrowMine8.SetParent(ParentFiber);
		StateThrowMine9.SetParent(ParentFiber);
		StateThrowMine10.SetParent(ParentFiber);

		StateThrowReduced.SetParent(ParentFiber);
		StateThrowQualm.SetParent(ParentFiber);
		StateThrowFeiDan.SetParent(ParentFiber);
		StateThrowGenZongDan.SetParent(ParentFiber);
		StateThrowPanPat.SetParent(ParentFiber);
		StateThrowShiftDown.SetParent(ParentFiber);
		StateThrowNumOne.SetParent(ParentFiber);
		StateThrowChenMo.SetParent(ParentFiber);
		StateThrowQusan.SetParent(ParentFiber);
		StateThrowMianYi.SetParent(ParentFiber);
		StateThrowQiangHua.SetParent(ParentFiber);
		StateThrowHuiMo.SetParent(ParentFiber);
		StateThrowSuperBox.SetParent(ParentFiber);
		StateThrowDarkCloud.SetParent(ParentFiber);
		StateThrowJiaShuo.SetParent(ParentFiber);
		StateThrowLaoLong.SetParent(ParentFiber);

		Physics.State.SetParentFiber(ParentFiber);
	}
	ucVOID	Init(UCSceneCtl* SceneCtl, UCPortalManage* Manage)
	{
		Mine0.Parent = SceneCtl->Scene;
		Mine1.Parent = SceneCtl->Scene;
		Mine0.Name = UCString("res/model/action/xiangzi.dat");
		Mine1.Name = UCString("res/model/action/xiangjiao.dat");

		Mine0.Visible = 0;
		Mine1.Visible = 0;

		Physics.Profession = Profession;
		if (CarID == -1)
			CarID = 7001;
		Physics.Data.Init(CarID);

		VentThings.Init(VentID);

		UCString MeshPath;
		UCString SexPath;
// 		if (SexKind == 0)
// 		{
// 			MeshPath = BOY_PATH;
// 			SexPath = UCString("boy");
// 		}
// 		else
		{
			MeshPath = GIRL_PATH;
			SexPath = UCString("girl");
		}

		UCString iMeshName_Face = MeshPath + UCString("/face/saiche_") + SexPath + UCString("_basicface_") + ITOS(FaceID) + UCString(".dat");
		UCString iMeshName_Hair = MeshPath + UCString("/hair/saiche_") + SexPath + UCString("_hair_") + ITOS(HairID) + UCString(".dat");

		UCString iMeshName_Body = MeshPath + UCString("/body/saiche_") + SexPath + UCString("_body_") + ITOS(BodyID) + UCString(".dat");
		UCString iMeshName_Hand = MeshPath + UCString("/hand/saiche_") + SexPath + UCString("_hand_") + ITOS(HandID) + UCString(".dat");

		UCString iMeshName_Pant = MeshPath + UCString("/pant/saiche_") + SexPath + UCString("_pant_") + ITOS(PantID) + UCString(".dat");
		UCString iMeshName_Shoe = MeshPath + UCString("/shoe/saiche_") + SexPath + UCString("_shose_") + ITOS(ShoeID) + UCString(".dat");

// 		UCString iMeshName_Car = CAR_PATH + UCString("/car") + ITOS(7000 + CarID) + UCString(".dat");

		UCString iMeshName_Car = AllItemDataConfig.GetMeshPath(CarID);
		UCString iMeshName_Vent = AllItemDataConfig.GetMeshPath(VentID);
// 		UCString iMeshName_Hair = AllItemDataConfig.GetMeshPath(HairID);
// 		UCString iMeshName_Face = AllItemDataConfig.GetMeshPath(FaceID);
		UCString iMeshName_Suit = AllItemDataConfig.GetMeshPath(SuitID);
// 		UCString iMeshName_Shoe = AllItemDataConfig.GetMeshPath(ShoeID);
// 		UCString iMeshName_Hand = AllItemDataConfig.GetMeshPath(HandID);
// 		UCString iMeshName_Pant = AllItemDataConfig.GetMeshPath(PantID);
// 		UCString iMeshName_Body = AllItemDataConfig.GetMeshPath(BodyID);
		UCString iMeshName_Ball = AllItemDataConfig.GetMeshPath(BallID);
		UCString iMeshName_Licence = AllItemDataConfig.GetMeshPath(LicenceID);

		Physics.Gender = SexKind;
		Physics.Init(SceneCtl, iMeshName_Car, iMeshName_Vent, VentThings.VentSmallFire, iMeshName_Hair, iMeshName_Face, iMeshName_Body, iMeshName_Pant, iMeshName_Shoe, iMeshName_Hand, iMeshName_Suit, iMeshName_Ball, iMeshName_Licence, Physics.Data.Weight * WEIGHT_INC + 0.01f);
		Physics.PortalObject.Bind(Manage, 1);
		Physics.PortalObject.WayKind = 1;

		Physics.DriftMeshMove.Pos = CarThings.PiaoYiPos;
		Physics.DriftMeshRead.Pos = CarThings.PiaoYiPos;
		Physics.DriftMeshRead.Pos.y -= 15.0f;

		for (ucINT i = 0; i < 3; i++)
		{
			ChengMoPic[i].Size = UCSize(52, 52);
			ChengMoPic[i].Visible = 0;
			ChengMoPic[i].Picture.Name = UCString("res/ui/gameing/shop1.dds");
			ChengMoPic[i].Picture.Offset.x = -225;
			ChengMoPic[i].Picture.Offset.y = -457;
			SceneCtl->AddControl(&ChengMoPic[i]);
		}
		ChengMoPic[0].Location = UCPoint(10, 45);
		ChengMoPic[1].Location = UCPoint(118, 14);
		ChengMoPic[2].Location = UCPoint(174, 7);


		ShiftSpeedDownPic.Size = UCSize(141, 35);
		ShiftSpeedDownPic.Location = UCPoint(0, 8);
		ShiftSpeedDownPic.Visible = 0;
		ShiftSpeedDownPic.Picture.Name = UCString("res/ui/gameing/shiftfire.dds");
		SceneCtl->AddControl(&ShiftSpeedDownPic, 1000);

		White.Size = UCSize(SCREEN_CX, SCREEN_CY);
		White.Location = UCPoint(0, 0);

		White.Visible = 0;
		White.OnDraw <<= UCEvent(this, OnDrawWhiteBack);
		SceneCtl->AddControl(&White);

		PaintBlack.Size = UCSize(326, 250);
		PaintBlack.Location = UCPoint(100, 30);
		PaintBlack.Visible = 0;
		PaintBlack.Picture.Name = UCString("res/ui/paint.dds");
		SceneCtl->AddControl(&PaintBlack);

		PaintBlack2.Size = UCSize(326, 250);
		PaintBlack2.Location = UCPoint(40, 80);
		PaintBlack2.Visible = 0;
		PaintBlack2.Picture.Name = UCString("res/ui/paint2.dds");
		SceneCtl->AddControl(&PaintBlack2);

		PaintBlack3.Size = UCSize(326, 250);
		PaintBlack3.Location = UCPoint(40, 80);
		PaintBlack3.Visible = 0;
		PaintBlack3.Picture.Name = UCString("res/ui/paint3.dds");
		SceneCtl->AddControl(&PaintBlack3);
	}

	ucVOID OnDrawWhiteBack(UCObject* Sender, UCEventArgs* e)
	{
		UCPaintEventArgs* pArg = (UCPaintEventArgs*)e;

		ucDWORD dwBack[4];
		dwBack[0] = 0xFFFFFFFF;
		dwBack[1] = 0xFFFFFFFF;
		dwBack[2] = 0xFFFFFFFF;
		dwBack[3] = 0xFFFFFFFF;
		pArg->pDevice->DrawRect(pArg->RectScreen, pArg->ReDrawRect, dwBack);
	}
	ucVOID CheckHuman(UCHuman* Human)
	{
		uc3dxAABB aabbSelfMine0 = Mine0.GetScaleAABB();
		uc3dxAABB aabbSelfMine1 = Mine1.GetScaleAABB();
		uc3dxAABB aabbSelf = Physics.Center.GetScaleAABB();

		uc3dxAABB aabbHumanMine0 = Human->Mine0.GetScaleAABB();
		uc3dxAABB aabbHumanMine1 = Human->Mine1.GetScaleAABB();

		if (Mine0.Visible.Value)
		{
			if (aabbSelfMine0.Intersect(aabbHumanMine0))
			{
				Human->Mine0.Visible = 0;
				Mine0.Visible = 0;
			}
			else if (aabbSelfMine0.Intersect(aabbHumanMine1))
			{
				Human->Mine1.Visible = 0;
				Mine0.Visible = 0;
			}
		}
		if (Mine1.Visible.Value)
		{
			if (aabbSelfMine1.Intersect(aabbHumanMine0))
			{
				Human->Mine0.Visible = 0;
				Mine1.Visible = 0;
			}
			else if (aabbSelfMine1.Intersect(aabbHumanMine1))
			{
				Human->Mine1.Visible = 0;
				Mine1.Visible = 0;
			}
		}
		else if (Human->Mine0.Visible.Value && aabbSelf.Intersect(aabbHumanMine0))
		{
			ucINT Result = 0;
			Physics.SetStateNet("正翻", 0, &Result);
			Human->Mine0.Visible = 0;
		}
		else if (Human->Mine1.Visible.Value && aabbSelf.Intersect(aabbHumanMine1))
		{
			ucINT Result = 0;
			Physics.SetStateNet("打滑", 0, &Result);
			Human->Mine1.Visible = 0;
		}
	}
	ucVOID CheckMine()
	{
		Mine0.Pos = Physics.CenterBody.Pos.Value() -
			Physics.CenterBody.GetWay_Y() * 10.0f +
			Physics.CenterBody.GetWay_Z() * 100.0f;
		Mine0.Rot.y = Physics.CenterBody.Rot.y.Value;

		Mine1.Pos = Physics.CenterBody.Pos.Value() -
			Physics.CenterBody.GetWay_Z() * 80.0f;
		Mine1.Rot.y = Physics.CenterBody.Rot.y.Value + 1.57f;
	}
	ucVOID OnStateThrowMine0(UCObject* Sender, UCEventArgs* e)
	{
		UCTickFiberData* pData = (UCTickFiberData*)Sender;
		Physics.PlayAction(6);

		Mine0.Visible = 1;

		for (ucINT i = 0; i < 25; i++)
		{
			Mine0.Scale = uc3dxVector3(0.32f, 0.32f, 0.32f) * i;
			pData->SyncTick(1);
		}

		while (pData->IsValid() && MineHold0 != -1 && Mine0.Visible.Value)
		{
			pData->SyncTick(1);
		}

		if (Mine0.Visible.Value)
		{
			Mine0.Visible = 0;

			UCMesh Mine;
			Mine.Name = UCString("res/model/action/xiangzi.dat");
			Mine.Parent = &Physics.CenterBody;

			Mine.Pos.z = 100.0f;
			Mine.Scale = uc3dxVector3(8.0f, 8.0f, 8.0f);
			for (ucINT i = 0; i < 20; i++)
			{
				Mine.Pos.z -= 5.0f;
				Mine.Pos.y += 3.0f;
				//Mine.Scale -= 0.28f; 
				Mine.Rot.x -= 0.24f;
				pData->SyncTick(2);
			}

			for (ucINT i = 0; i < 20; i++)
			{
				Mine.Pos.z -= 4.0f;
				Mine.Pos.y -= 1.0f;
				//Mine.Scale += 0.28f; 
				Mine.Rot.x += 0.24f;
				pData->SyncTick(2);
			}


			if (pData->IsValid()) OnMine0Throw.Run((UCObject*)this, 0);
		}
		else
		{
			Mine0.Visible = 1;
			for (ucINT i = 0; i < 25; i++)
			{
				Mine0.Scale = uc3dxVector3(0.32f, 0.32f, 0.32f) * (25 - i - 1);
				pData->SyncTick(1);
			}
			Mine0.Visible = 0;
		}
	}
	ucVOID OnStateThrowMine1(UCObject* Sender, UCEventArgs* e)
	{
		UCTickFiberData* pData = (UCTickFiberData*)Sender;
		Physics.PlayAction(7);

		Mine1.Visible = 1;

		for (ucINT i = 0; i < 25; i++)
		{
			Mine1.Scale = uc3dxVector3(0.04f, 0.04f, 0.04f) * i;
			pData->SyncTick(1);
		}

		while (pData->IsValid() && MineHold1 != -1 && Mine1.Visible.Value)
		{
			pData->SyncTick(1);
		}

		if (Mine1.Visible.Value)
		{
			Mine1.Visible = 0;
			if (pData->IsValid())	OnMine1Throw.Run((UCObject*)this, 0);
		}
		else
		{
			Mine1.Visible = 1;
			for (ucINT i = 0; i < 25; i++)
			{
				Mine1.Scale = uc3dxVector3(0.04f, 0.04f, 0.04f) * (25 - i - 1);
				pData->SyncTick(1);
			}
			Mine1.Visible = 0;
		}
	}
	ucVOID OnStateThrowJiaShuo(UCObject* Sender, UCEventArgs* e)//扔枷锁
	{
		UCTickFiberData* pData = (UCTickFiberData*)Sender;
		Physics.PlayAction(7);
		UCMesh		Mine2;
		Mine2.Parent = &Physics.CenterBody;

		Mine2.Name = UCString("res/model/action/jiasuo.dat");

		Mine2.Scale = uc3dxVector3(0.0f, 0.0f, 0.0f);
		Mine2.Pos.y = 10.0f;
		Mine2.Pos.x = 15.0f;
		Mine2.Pos.z = -10.0f;
		for (ucINT i = 0; i < 25; i++)
		{
			Mine2.Scale = uc3dxVector3(0.02f, 0.02f, 0.02f) * i;
			pData->SyncTick(1);
		}
		for (ucINT i = 0; i < 25; i++)
		{
			Mine2.Scale = Mine2.Scale.Value() + uc3dxVector3(0.008f, 0.008f, 0.008f);
			Mine2.Pos.z -= 5.0f;
			pData->SyncTick(1);
		}
		if (pData->IsValid())	OnMine11Throw.Run((UCObject*)this, 0);

	}

	ucVOID OnSMBianFuFly(UCObject* Sender, UCEventArgs* e)
	{
		UCTickFiberData* pData = (UCTickFiberData*)Sender;
		Physics.PlayAction(7);

		UCMesh				Mine2;				//蝙蝠
		Mine2.Parent = &Physics.CenterBody;
		if (GroupID == 11)
			Mine2.Name = UCString("res/model/action/followboomred.dat");
		else
			Mine2.Name = UCString("res/model/action/followboomblue.dat");

		Mine2.PlayAction(0);

		Mine2.Rot.y = 3.14f;
		Mine2.Visible = 1;
		Mine2.Scale = uc3dxVector3(0.0f, 0.0f, 0.0f);
		Mine2.Pos.y = 32.0f;
		for (ucINT i = 0; i < 25 && pData->IsValid(); i++)
		{
			Mine2.Scale = Mine2.Scale.Value() + uc3dxVector3(0.3f, 0.3f, 0.3f);
			pData->SyncTick(3);
		}
		pData->SyncTick(30);

		Physics.PortalObject.RandPortalNext();
		Physics.Center.GravityMode = 0;

		//if (Physics.IsSelf) gCarSound.PlaySound(15);
		ucFLOAT Length = 0.0f;

		ucFLOAT	maxspeed = (VSPEED_BAS + VSPEED_INC * Physics.Data.MaxSpeed + VSPEED_UP / 2.0f) * Physics.CenterPoint.Scale.z.Value;

		for (ucINT i = 0; i < 400 && pData->IsValid(); i++)
		{
			uc3dxVector3	vDir;

			UCPortal* PortalLast = Physics.PortalObject.PortalLast;
			UCPortal* PortalNext = Physics.PortalObject.PortalNext;

			ucINT Valid = 0;
			while (PortalNext != 0 && PortalNext != PortalLast)
			{
				uc3dxVector3	vPos(Physics.Center.Pos.Value());
				uc3dxVector3	vTag(PortalNext->Center);
				vDir = vTag - vPos;	vDir.Normalize();

				ucFLOAT fLen = vDir | Physics.CenterBody.GetWay_Z();
				if (fLen > 0.0f)
				{
					Valid = 1;
					break;
				}

				Physics.PortalObject.RandPortalNext();
				PortalNext = Physics.PortalObject.PortalNext;
			}

			if (!Valid)
			{
				vDir = Physics.CenterBody.GetWay_Z();
			}

			Physics.Center.LinearVel = vDir * maxspeed;

			Physics.PortalObject.Update(Physics.Center.Pos.Value());

			ucFLOAT fPortalY = Physics.PortalObject.PortalDir().y;	while (fPortalY < -UC3D_PI)	fPortalY += UC3D_PI * 2.0f;	while (fPortalY > +UC3D_PI)	fPortalY -= UC3D_PI * 2.0f;
			ucFLOAT fCenterY = Physics.CenterBody.Rot.y.Value;		while (fCenterY < -UC3D_PI)	fCenterY += UC3D_PI * 2.0f;	while (fCenterY > +UC3D_PI)	fCenterY -= UC3D_PI * 2.0f;

			ucFLOAT fY = fPortalY - fCenterY;	while (fY < -UC3D_PI)	fY += UC3D_PI * 2.0f;	while (fY > +UC3D_PI)	fY -= UC3D_PI * 2.0f;

			Physics.CenterBody.Face(vDir);
			Physics.CenterBody.Rot.x = 0.0f;
			Physics.CenterBody.Rot.z = 0.0f;

			pData->SyncTick(1);


		}
		Mine2.Visible = 0;
		Physics.Center.GravityMode = 1;
		if (pData->IsValid())	OnMine2Throw.Run((UCObject*)this, 0);
	}
	ucVOID OnStateThrowMine3(UCObject* Sender, UCEventArgs* e)
	{
		UCTickFiberData* pData = (UCTickFiberData*)Sender;
		Physics.PlayAction(7);

		UCMesh				Mine3;				//定时
		Mine3.Parent = &Physics.CenterBody;
		Mine3.Name = UCString("res/model/action/timeboom.dat");
		Mine3.PlayAction(0);

		Mine3.Scale = uc3dxVector3(0.0f, 0.0f, 0.0f);
		Mine3.Pos = uc3dxVector3(0.0f, 0.0f, 0.0f);
		Mine3.Rot.x = 0.0f;

		for (ucINT i = 0; i < 25 && pData->IsValid(); i++)
		{
			Mine3.Scale = Mine3.Scale.Value() + uc3dxVector3(0.12f, 0.12f, 0.12f);
			Mine3.Pos.y += 3.0f;
			pData->SyncTick(1);
		}
		/*for (ucINT i=0;i<50;i++)
		{

			pData->SyncTick(1);
		}*/
		for (ucINT i = 0; i < 40 && pData->IsValid(); i++)
		{
			Mine3.Pos.y -= 1.25f;
			Mine3.Pos.z -= 3.75f;
			Mine3.Rot.x += 0.125f;
			pData->SyncTick(1);
		}

		Mine3.Visible = 0;
		if (pData->IsValid())	OnMine3Throw.Run((UCObject*)this, 0);
	}
	ucVOID OnStateThrowMine4(UCObject* Sender, UCEventArgs* e)
	{
		UCTickFiberData* pData = (UCTickFiberData*)Sender;
		Physics.PlayAction(7);

		UCMesh				Mine4;				//烟雾
		Mine4.Parent = &Physics.CenterBody;
		Mine4.Name = UCString("res/model/action/blindrog.dat");
		Mine4.PlayAction(0);

		uc3dxVector3	vY = uc3dxVector3(0.0f, 1.0f, 0.0f);
		uc3dxVector3	vZ = uc3dxVector3(0.0f, 0.0f, 1.0f);
		uc3dxVector3	vPos = -vZ * 80.0f + vY * 40.0f;
		Mine4.Pos = vPos;
		Mine4.Scale = uc3dxVector3(0.0f, 0.0f, 0.0f);
		for (ucINT i = 0; i < 40 && pData->IsValid(); i++)
		{
			Mine4.Scale = Mine4.Scale.Value() + uc3dxVector3(0.012f, 0.012f, 0.012f);
			pData->SyncTick(1);
		}
		pData->SyncTick(6);
		for (ucINT i = 0; i < 50 && pData->IsValid(); i++)
		{
			Mine4.Pos.y += 14.0f;
			pData->SyncTick(1);
		}

		Mine4.Visible = 0;
		if (pData->IsValid())	OnMine4Throw.Run((UCObject*)this, 0);
	}
	ucVOID OnStateThrowMine5(UCObject* Sender, UCEventArgs* e)
	{
		UCTickFiberData* pData = (UCTickFiberData*)Sender;
		Physics.PlayAction(7);

		UCMesh Mine;
		Mine.Name = UCString("res/model/action/oilstationred.dat");
		Mine.Parent = &Physics.CenterBody;

		for (ucINT i = 0; i < 20 && pData->IsValid(); i++)
		{
			Mine.Scale = uc3dxVector3(0.1f, 0.1f, 0.1f) * i;
			pData->SyncTick(1);
		}

		Mine.Pos.z = 80.0f;
		//Mine.Scale = uc3dxVector3(8.0f,8.0f,8.0f);
		for (ucINT i = 0; i < 25 && pData->IsValid(); i++)
		{
			Mine.Pos.z -= 5.0f;
			Mine.Pos.y += 4.5f;
			//Mine.Scale -= 0.28f; 
			//Mine.Rot.x -= 0.24f;
			pData->SyncTick(1);
		}

		for (ucINT i = 0; i < 20 && pData->IsValid(); i++)
		{
			Mine.Pos.z -= 5.0f;
			Mine.Pos.y -= 1.25f;
			Mine.Scale.x += 0.1f;
			Mine.Scale.y += 0.1f;
			Mine.Scale.z += 0.1f;
			Mine.Rot.x -= 0.3f;
			pData->SyncTick(2);
		}
		//Physics.State.Set("冲刺");
		if (pData->IsValid())	OnMine7Throw.Run((UCObject*)this, 0);

	}
	ucVOID OnStateThrowLaoLong(UCObject* Sender, UCEventArgs* e)
	{
		UCTickFiberData* pData = (UCTickFiberData*)Sender;

		Physics.PlayAction(6);
		UCMesh Mine;
		Mine.Name = UCString("res/model/action/alarm.dat");
		Mine.Parent = &Physics.CenterBody;
		Mine.Scale = uc3dxVector3(0.3f, 0.3f, 0.3f);
		Mine.Visible = 1;
		Mine.Rot.y = 3.14f;
		Mine.PlayAction(0);
		Mine.Pos = uc3dxVector3(18.0f, 10.0f, -10.0f);

		for (ucINT i = 0; i < 50 && pData->IsValid(); i++)
		{
			Mine.Pos.y += 3.0f;
			Mine.Rot.y += 0.2f;
			pData->SyncTick(1);
		}
		Mine.Visible = 0;

	}
	ucVOID OnStateThrowMine6(UCObject* Sender, UCEventArgs* e)
	{
		UCTickFiberData* pData = (UCTickFiberData*)Sender;

		UCMesh Mine;
		Mine.Name = UCString("res/model/action/shineshakeboom.dat");
		Mine.Parent = &Physics.CenterBody;
		Mine.Scale = uc3dxVector3(1.6f, 1.6f, 1.6f);
		Mine.Visible = 1;
		Mine.PlayAction(0);
		Mine.Pos = uc3dxVector3(18.0f, 10.0f, -10.0f);
		Physics.PlayAction(6);

		for (ucINT i = 0; i < 50 && pData->IsValid(); i++)
		{
			Mine.Rot.x += 0.1256f;
			pData->SyncTick(1);
		}
		Mine.Visible = 0;


		if (pData->IsValid())	OnMine10Throw.Run((UCObject*)this, 0);//闪光雷前身

	}
	ucVOID OnStateThrowMine7(UCObject* Sender, UCEventArgs* e)
	{
		UCTickFiberData* pData = (UCTickFiberData*)Sender;

		UCMesh Mine;
		Mine.Name = UCString("res/model/action/boxgloves.dat");
		Mine.Parent = &Physics.CenterBody;
		Mine.Scale = uc3dxVector3(1.5f, 1.5f, 1.5f);
		Mine.Visible = 1;
		Mine.PlayAction(0);
		Mine.Pos = uc3dxVector3(0.0f, 0.0f, 0.0f);
		Physics.PlayAction(6);

		pData->SyncTick(12);
		for (ucINT i = 0; i < 50 && pData->IsValid(); i++)
		{
			Mine.Pos.y += 8.0f;
			pData->SyncTick(1);
		}
		Mine.Visible = 0;

	}
	ucVOID OnStateThrowMine8(UCObject* Sender, UCEventArgs* e)
	{
		UCTickFiberData* pData = (UCTickFiberData*)Sender;

		UCMesh Mine;
		Mine.Name = UCString("res/model/action/sleep.dat");
		Mine.Parent = &Physics.CenterBody;
		Mine.Scale = uc3dxVector3(0.8f, 0.8f, 0.8f);
		Mine.Visible = 1;
		Mine.PlayAction(0);
		Mine.Pos = uc3dxVector3(20.0f, 0.0f, 0.0f);
		Physics.PlayAction(6);
		for (ucINT i = 0; i < 20 && pData->IsValid(); i++)
		{
			Mine.Rot.x += 0.1256f;
			pData->SyncTick(1);
		}
		for (ucINT i = 0; i < 50 && pData->IsValid(); i++)
		{

			Mine.Pos.y += 8.0f;
			pData->SyncTick(1);
		}
		Mine.Visible = 0;

	}
	ucVOID OnStateThrowMine9(UCObject* Sender, UCEventArgs* e)
	{
		UCTickFiberData* pData = (UCTickFiberData*)Sender;

		UCMesh Mine;
		Mine.Name = UCString("res/model/action/buyover.dat");
		Mine.Parent = &Physics.CenterBody;
		Mine.Scale = uc3dxVector3(2.5f, 2.5f, 2.5f);
		Mine.Visible = 1;
		Mine.PlayAction(0);
		Mine.Pos = uc3dxVector3(30.0f, 0.0f, 0.0f);
		Physics.PlayAction(6);
		for (ucINT i = 0; i < 20 && pData->IsValid(); i++)
		{
			Mine.Rot.z += 0.1256f;
			pData->SyncTick(1);
		}
		for (ucINT i = 0; i < 50 && pData->IsValid(); i++)
		{

			Mine.Pos.y += 8.0f;
			pData->SyncTick(1);
		}
		Mine.Visible = 0;

	}
	ucVOID OnStateThrowMine10(UCObject* Sender, UCEventArgs* e)
	{
		UCTickFiberData* pData = (UCTickFiberData*)Sender;

		Physics.PlayAction(6);
		UCMesh Mine;
		Mine.Name = UCString("res/model/action/fire.dat");
		Mine.Parent = &Physics.CenterBody;
		Mine.Scale = uc3dxVector3(2.0f, 2.0f, 2.0f);
		Mine.Visible = 1;
		Mine.PlayAction(0);
		Mine.Pos = uc3dxVector3(20.0f, 0.0f, 0.0f);
		Physics.PlayAction(6);
		for (ucINT i = 0; i < 20 && pData->IsValid(); i++)
		{
			Mine.Rot.z += 0.1256f;
			pData->SyncTick(1);
		}
		for (ucINT i = 0; i < 50 && pData->IsValid(); i++)
		{

			Mine.Pos.y += 8.0f;
			pData->SyncTick(1);
		}

		Mine.Visible = 0;

	}
	ucVOID OnStateThrowShiftDown(UCObject* Sender, UCEventArgs* e)
	{
		UCTickFiberData* pData = (UCTickFiberData*)Sender;

		UCMesh Mine;
		Mine.Name = UCString("res/model/action/shiftspeeddown.dat");
		Mine.Parent = &Physics.CenterBody;
		Mine.Scale = uc3dxVector3(2.0f, 2.0f, 2.0f);
		Mine.Visible = 1;
		Mine.PlayAction(0);
		Mine.Pos = uc3dxVector3(20.0f, 0.0f, 0.0f);
		Physics.PlayAction(6);
		for (ucINT i = 0; i < 20 && pData->IsValid(); i++)
		{
			Mine.Rot.z += 0.1256f;
			pData->SyncTick(1);
		}
		for (ucINT i = 0; i < 50 && pData->IsValid(); i++)
		{

			Mine.Pos.y += 8.0f;
			pData->SyncTick(1);
		}

		Mine.Visible = 0;

	}
	ucVOID OnStateThrowDarkCloud(UCObject* Sender, UCEventArgs* e)
	{
		UCTickFiberData* pData = (UCTickFiberData*)Sender;

		UCMesh Mine;
		Mine.Name = UCString("res/model/action/devil.dat");
		Mine.Parent = &Physics.CenterBody;
		Mine.Scale = uc3dxVector3(1.5f, 1.5f, 1.5f);
		Mine.Visible = 1;
		Mine.PlayAction(0);
		Mine.Pos = uc3dxVector3(18.0f, 10.0f, -10.0f);
		Physics.PlayAction(6);

		for (ucINT i = 0; i < 50 && pData->IsValid(); i++)
		{
			Mine.Rot.x += 0.1256f;
			pData->SyncTick(1);
		}
		Mine.Visible = 0;

		if (pData->IsValid())	OnDarkCloudThrow.Run((UCObject*)this, 0);//恶魔附身
	}
	ucVOID OnStateThrowSuperBox(UCObject* Sender, UCEventArgs* e)
	{
		UCTickFiberData* pData = (UCTickFiberData*)Sender;

		UCMesh Mine;
		Mine.Name = UCString("res/model/action/superboom.dat");
		Mine.Parent = &Physics.CenterBody;
		Mine.Scale = uc3dxVector3(1.5f, 1.5f, 1.5f);
		Mine.Visible = 1;
		Mine.PlayAction(0);
		Mine.Pos = uc3dxVector3(18.0f, 10.0f, -10.0f);
		Physics.PlayAction(6);

		for (ucINT i = 0; i < 50 && pData->IsValid(); i++)
		{
			Mine.Rot.x += 0.1256f;
			pData->SyncTick(1);
		}
		Mine.Visible = 0;

		if (pData->IsValid())	OnSuperBoxThrow.Run((UCObject*)this, 0);//闪光雷前身
	}
	ucVOID OnSMStateReadyFirst(UCObject* Sender, UCEventArgs* e)
	{
		UCTickFiberData* pData = (UCTickFiberData*)Sender;

		UCMesh Mesh;
		Mesh.Name = UCString("res/model/action/huangfeng.dat");
		Mesh.Scale = uc3dxVector3(2.0f, 2.0f, 2.0f);
		Mesh.Parent = Physics.SceneCtrl->Scene;

		Mesh.Pos.y = Physics.Center.Pos.y.Value + 160.0f;

		ucINT Valid = 1;
		while (pData->IsValid() && Valid)
		{
			Mesh.Pos.z.Value = Physics.Center.Pos.z.Value;
			Mesh.Pos.x.Value = Physics.Center.Pos.x.Value;
			Mesh.Rot.z = Mesh.Rot.z.Value + 0.314f;


			if (Mesh.Pos.y.Value > Physics.Center.Pos.y.Value)
			{
				Mesh.Pos.y = Mesh.Pos.y.Value - 1.2f;
			}
			else
			{
				Valid = 0;
			}
			pData->SyncTick(1);
		}
		//gCarSound.Stop(20);
		if (pData->IsValid())	OnStateFirst.Run((UCObject*)this, 0);
	}
	ucVOID OnSMStateReadyFire(UCObject* Sender, UCEventArgs* e)
	{
		UCTickFiberData* pData = (UCTickFiberData*)Sender;

		UCMesh	temp0;
		UCMesh	temp;
		temp0.Parent = &Physics.CenterPoint;
		temp.Parent = &temp0;
		temp.Name = UCString("res/model/action/genzongdan.dat");
		temp.PlayAction(0);
		temp.Visible = 1;

		uc3dxVector3	vY = uc3dxVector3(0.0, 1.0f, 0.0f);
		uc3dxVector3	vZ = uc3dxVector3(0.0, 0.0f, 1.0f);
		uc3dxVector3	vPos = -vZ * 70.0f + vY * 40.0f;
		temp.Pos = vPos;
		temp0.Rot.y = 3.1415926f;

		ucFLOAT v0 = 6.28f / 15.0f;
		ucFLOAT center = temp.Pos.y.Value;


		for (ucINT i = 0; i < 30 && pData->IsValid(); i++)
		{
			//temp.Pos.x.Value =Physics.Center.Pos.x.Value;
			//temp.Pos.z.Value =Physics.Center.Pos.z.Value;

			temp0.Rot.z = temp0.Rot.z.Value + v0;

			pData->SyncTick(2);
		}
		for (ucINT i = 0; i < 20 && pData->IsValid(); i++)
		{
			temp0.Rot.z = temp0.Rot.z.Value + v0;
			if (temp.Pos.z.Value < 0.0f)
			{
				temp.Pos.z = temp.Pos.z.Value + 6.5f;
			}
			if (temp.Pos.y.Value > 0.0f)
			{
				temp.Pos.y = temp.Pos.y.Value - 2.5f;
			}
			pData->SyncTick(1);

		}
		temp.Visible = 0;
		temp0.Visible = 0;

		if (pData->IsValid())	Physics.State.Set("燃烧", 0);
		//ucINT Result = 0;
		//SetStateNet("燃烧",0,&Result);
		//StartState(STATE_FIRE,5);
	}
	ucVOID OnSMStateReadyBianFu(UCObject* Sender, UCEventArgs* e)//
	{
		UCTickFiberData* pData = (UCTickFiberData*)Sender;

		UCMesh	temp0;
		UCMesh	temp;
		temp0.Parent = &Physics.CenterPoint;
		temp.Parent = &temp0;

		if (GroupID == 11)
			temp.Name = UCString("res/model/action/followboomblue.dat");
		else if (GroupID == 12)
			temp.Name = UCString("res/model/action/followboomred.dat");
		else
			temp.Name = UCString("res/model/action/followboom.dat");

		temp.PlayAction(0);
		temp.Visible = 1;
		temp.Scale = uc3dxVector3(5.0f, 5.0f, 5.0f);


		uc3dxVector3	vY = uc3dxVector3(0.0, 1.0f, 0.0f);
		uc3dxVector3	vZ = uc3dxVector3(0.0, 0.0f, 1.0f);
		uc3dxVector3	vPos = -vZ * 100.0f + vY * 80.0f;
		temp.Pos = vPos;
		//temp0.Rot.y =3.1415926f;

		for (ucINT i = 0; i < 50 && pData->IsValid(); i++)
		{

			temp.Pos.y -= 1.0f;
			pData->SyncTick(2);

		}

		temp.Visible = 0;
		temp0.Visible = 0;

		if (pData->IsValid())	Physics.State.Set("蝙蝠", 0);
		//		ucINT Result = 0;
		//		SetStateNet("蝙蝠",0,&Result);
				//StartState(STATE_WATER,5);
	}
	ucVOID OnStateThrowReduced(UCObject* Sender, UCEventArgs*)
	{
		UCTickFiberData* pData = (UCTickFiberData*)Sender;
		UCMesh	temp;
		temp.Parent = &Physics.CenterBody;
		temp.Name = UCString("res/model/action/scaledown.dat");
		temp.PlayAction(0);
		temp.Visible = 1;
		temp.Scale = uc3dxVector3(0.0f, 0.0f, 0.0f);
		temp.Pos = uc3dxVector3(20.0f, 0.0f, 40.0f);
		Physics.PlayAction(6);
		for (ucINT i = 0; i < 20 && pData->IsValid(); i++)
		{
			temp.Scale = temp.Scale.Value() + uc3dxVector3(0.22f, 0.22f, 0.22f);
			pData->SyncTick(1);
		}
		pData->SyncTick(16);
		for (ucINT i = 0; i < 50 && pData->IsValid(); i++)
		{
			temp.Pos.y += 15.0f;
			pData->SyncTick(2);
		}
		temp.Visible = 0;

	}
	ucVOID OnStateThrowQualm(UCObject* Sender, UCEventArgs*)
	{
		UCTickFiberData* pData = (UCTickFiberData*)Sender;
		UCMesh	temp;
		temp.Parent = &Physics.CenterBody;
		temp.Name = UCString("res/model/action/hunluan.dat");
		temp.PlayAction(0);
		temp.Visible = 1;
		temp.Scale = uc3dxVector3(0.0f, 0.0f, 0.0f);
		temp.Pos = uc3dxVector3(20.0f, -20.0f, 0.0f);
		Physics.PlayAction(6);
		for (ucINT i = 0; i < 16 && pData->IsValid(); i++)
		{
			temp.Scale = temp.Scale.Value() + uc3dxVector3(0.078f, 0.078f, 0.078f);
			pData->SyncTick(1);
		}

		pData->SyncTick(10);

		for (ucINT i = 0; i < 50 && pData->IsValid(); i++)
		{
			temp.Pos.y += 10.0f;
			pData->SyncTick(2);
		}
		temp.Visible = 0;

	}
	ucVOID OnStateThrowFeiDan(UCObject* Sender, UCEventArgs*)
	{
		UCTickFiberData* pData = (UCTickFiberData*)Sender;
		UCMesh	temp;
		temp.Parent = &Physics.CenterBody;
		temp.Name = UCString("res/model/action/shuidan.dat");
		temp.PlayAction(0);
		temp.Visible = 1;
		temp.Scale = uc3dxVector3(0.7f, 0.7f, 0.7f);
		temp.Pos = uc3dxVector3(20.0f, 0.0f, 0.0f);

		Physics.PlayAction(6);

		for (ucINT i = 0; i < 50 && pData->IsValid(); i++)
		{
			temp.Rot.x += 0.1256f;
			pData->SyncTick(1);
		}
		temp.Visible = 0;

		OnMine5Throw.Run((UCObject*)this, 0);

	}
	ucVOID OnStateThrowGenZongDan(UCObject* Sender, UCEventArgs*)
	{
		UCTickFiberData* pData = (UCTickFiberData*)Sender;
		UCMesh	temp;
		temp.Parent = &Physics.CenterBody;
		temp.Name = UCString("res/model/action/genzongdan.dat");

		temp.Visible = 1;
		temp.Pos = uc3dxVector3(15.0f, 10.0f, -10.0f);
		Physics.PlayAction(6);

		for (ucINT i = 0; i < 50 && pData->IsValid(); i++)
		{
			temp.Rot.x += 0.1256f;
			pData->SyncTick(1);
		}
		temp.Visible = 0;

		OnMine6Throw.Run((UCObject*)this, 0);

	}
	ucVOID OnStateThrowPanPat(UCObject* Sender, UCEventArgs*)
	{
		UCTickFiberData* pData = (UCTickFiberData*)Sender;
		UCMesh	temp;
		temp.Parent = &Physics.CenterBody;
		temp.Name = UCString("res/model/action/chief.dat");

		temp.Visible = 1;
		temp.Pos = uc3dxVector3(40.0f, 20.0f, -10.0f);
		Physics.PlayAction(6);


		for (ucINT i = 0; i < 40 && pData->IsValid(); i++)
		{
			temp.Pos.y += 0.125f;
			pData->SyncTick(1);
		}
		temp.Visible = 0;

		OnMine8Throw.Run((UCObject*)this, 0);

	}
	ucVOID OnStateThrowNumOne(UCObject* Sender, UCEventArgs*)
	{
		UCTickFiberData* pData = (UCTickFiberData*)Sender;
		UCMesh	temp;
		temp.Parent = &Physics.CenterBody;
		temp.Name = UCString("res/model/action/huangfeng.dat");
		temp.PlayAction(0);
		temp.Visible = 1;
		temp.Scale = uc3dxVector3(0.0f, 0.0f, 0.0f);
		temp.Pos = uc3dxVector3(20.0f, 0.0f, 0.0f);
		temp.Rot.x = 0.0f;
		Physics.PlayAction(6);
		for (ucINT i = 0; i < 20 && pData->IsValid(); i++)
		{
			temp.Scale = temp.Scale.Value() + uc3dxVector3(0.075f, 0.075f, 0.075f);
			pData->SyncTick(1);
		}

		pData->SyncTick(12);

		for (ucINT i = 0; i < 50 && pData->IsValid(); i++)
		{
			temp.Pos.y += 10.0f;
			temp.Rot.x += 0.5f;

			pData->SyncTick(2);
		}
		temp.Visible = 0;

	}
	ucVOID OnStateThrowChenMo(UCObject* Sender, UCEventArgs*)
	{
		UCTickFiberData* pData = (UCTickFiberData*)Sender;
		UCMesh	temp;
		temp.Parent = &Physics.CenterBody;
		temp.Name = UCString("res/model/action/chengmo.dat");
		temp.PlayAction(0);
		temp.Visible = 1;
		temp.Scale = uc3dxVector3(0.0f, 0.0f, 0.0f);
		temp.Pos = uc3dxVector3(0.0f, 0.0f, 0.0f);
		temp.Pos.y = -16.0f;
		Physics.PlayAction(6);
		for (ucINT i = 0; i < 20 && pData->IsValid(); i++)
		{
			temp.Scale = temp.Scale.Value() + uc3dxVector3(0.22f, 0.22f, 0.22f);
			pData->SyncTick(1);
		}

		pData->SyncTick(12);

		for (ucINT i = 0; i < 50 && pData->IsValid(); i++)
		{
			temp.Pos.y += 10.0f;
			//temp.Rot.y +=0.5f;

			pData->SyncTick(2);
		}
		temp.Visible = 0;

	}
	ucVOID OnStateThrowQusan(UCObject* Sender, UCEventArgs*)
	{
		UCTickFiberData* pData = (UCTickFiberData*)Sender;
		UCMesh	temp;
		temp.Parent = &Physics.CenterBody;
		temp.Name = UCString("res/model/action/qusan.dat");
		temp.PlayAction(0);
		temp.Visible = 1;
		temp.Scale = uc3dxVector3(0.0f, 0.0f, 0.0f);
		temp.Pos = uc3dxVector3(16.0f, 0.0f, 0.0f);
		temp.Pos.y = -10.0f;
		Physics.PlayAction(6);
		for (ucINT i = 0; i < 20 && pData->IsValid(); i++)
		{
			temp.Scale = temp.Scale.Value() + uc3dxVector3(0.1f, 0.1f, 0.1f);
			pData->SyncTick(1);
		}

		pData->SyncTick(12);

		for (ucINT i = 0; i < 50 && pData->IsValid(); i++)
		{
			temp.Pos.y += 10.0f;
			//temp.Rot.x +=0.5f;

			pData->SyncTick(2);
		}
		temp.Visible = 0;

	}
	ucVOID OnStateThrowMianYi(UCObject* Sender, UCEventArgs*)
	{
		UCTickFiberData* pData = (UCTickFiberData*)Sender;
		UCMesh	temp;
		temp.Parent = &Physics.CenterBody;
		temp.Name = UCString("res/model/action/mianyi.dat");
		//temp.PlayAction(0);		
		temp.Visible = 1;
		temp.Scale = uc3dxVector3(0.0f, 0.0f, 0.0f);
		temp.Pos = uc3dxVector3(20.0f, 0.0f, 0.0f);
		temp.Pos.y = -10.0f;
		Physics.PlayAction(6);
		for (ucINT i = 0; i < 20 && pData->IsValid(); i++)
		{
			temp.Scale = temp.Scale.Value() + uc3dxVector3(0.3f, 0.3f, 0.3f);
			pData->SyncTick(1);
		}

		pData->SyncTick(12);

		for (ucINT i = 0; i < 50 && pData->IsValid(); i++)
		{
			temp.Pos.y += 10.0f;
			//temp.Rot.x +=0.5f;

			pData->SyncTick(2);
		}
		temp.Visible = 0;

	}
	ucVOID OnStateThrowQiangHua(UCObject* Sender, UCEventArgs*)
	{
		UCTickFiberData* pData = (UCTickFiberData*)Sender;
		UCMesh	temp;
		temp.Parent = &Physics.CenterBody;
		temp.Name = UCString("res/model/action/qianghua.dat");
		//temp.PlayAction(0);		
		temp.Visible = 1;
		temp.Scale = uc3dxVector3(0.0f, 0.0f, 0.0f);
		temp.Pos = uc3dxVector3(-16.0f, 0.0f, -10.0f);
		//temp.Rot.x=-0.2f;
		Physics.PlayAction(6);
		for (ucINT i = 0; i < 20 && pData->IsValid(); i++)
		{
			temp.Scale = temp.Scale.Value() + uc3dxVector3(0.1f, 0.1f, 0.1f);
			pData->SyncTick(1);
		}

		pData->SyncTick(12);

		for (ucINT i = 0; i < 50 && pData->IsValid(); i++)
		{
			temp.Pos.y += 10.0f;

			pData->SyncTick(2);
		}
		temp.Visible = 0;

	}
	ucVOID OnStateThrowHuiMo(UCObject* Sender, UCEventArgs*)
	{
		UCTickFiberData* pData = (UCTickFiberData*)Sender;
		UCMesh	temp;
		temp.Parent = &Physics.CenterBody;
		temp.Name = UCString("res/model/action/huimothrowed.dat");
		temp.PlayAction(0);
		temp.Visible = 1;
		temp.Scale = uc3dxVector3(0.0f, 0.0f, 0.0f);
		temp.Pos = uc3dxVector3(20.0f, 0.0f, 0.0f);
		temp.Rot.x = -0.2f;
		Physics.PlayAction(6);
		for (ucINT i = 0; i < 20 && pData->IsValid(); i++)
		{
			temp.Scale = temp.Scale.Value() + uc3dxVector3(0.1f, 0.1f, 0.1f);
			pData->SyncTick(1);
		}

		pData->SyncTick(12);

		for (ucINT i = 0; i < 50 && pData->IsValid(); i++)
		{
			temp.Pos.y += 10.0f;

			pData->SyncTick(2);
		}
		temp.Visible = 0;

	}
	/*
	ucVOID OnStateBlindFiber(UCObject* Sender,UCEventArgs*)
	{
		UCTickFiberData* pData=(UCTickFiberData*)Sender;
		OnSMBlind(StateParam.Param,pData);
	}
	ucVOID OnStateQualmFiber(UCObject* Sender,UCEventArgs*)
	{
		UCTickFiberData* pData=(UCTickFiberData*)Sender;
		OnSMQualm(StateParam.Param,pData);
	}
	ucVOID OnStateChengMoFiber(UCObject* Sender,UCEventArgs*)
	{
		UCTickFiberData* pData=(UCTickFiberData*)Sender;
		OnSMChengMo(StateParam.Param,pData);
	}
	ucVOID OnStateMianYiFiber(UCObject* Sender,UCEventArgs*)
	{
		UCTickFiberData* pData=(UCTickFiberData*)Sender;
		OnSMMianYi(StateParam.Param,pData);
	}
	ucVOID OnStateQiangHuaFiber(UCObject* Sender,UCEventArgs*)
	{
		UCTickFiberData* pData=(UCTickFiberData*)Sender;
		OnSMQiangHua(StateParam.Param,pData);
	}
	ucVOID OnStateSmallFiber(UCObject* Sender,UCEventArgs* Arg)
	{
		UCTickFiberData* pData=(UCTickFiberData*)Sender;
		OnSMScaleDown(StateParam.Param,pData);
	}
	*/
	ucVOID OnStateFiber(UCObject* Sender, UCEventArgs* Arg)
	{
		/*
		UCTickFiberData*	Fiber = (UCTickFiberData*)Sender;
		//UCFiberArgs*	FiberArgs = (UCFiberArgs*)Arg;

		if (StateParam.State==STATE_SPEED_UP)
		{
			OnSMSpeedUp(StateParam.Param,Fiber);
		}
		else if(StateParam.State==STATE_DRIFT_UP)
		{
			OnSMDriftUp(StateParam.Param,Fiber);
		}
		else if(StateParam.State==STATE_ROCKET)
		{
			OnSMRocket(StateParam.Param,Fiber);
		}
		else if(StateParam.State==STATE_POWER)
		{
			OnSMPower(StateParam.Param,Fiber);
		}
		else if(StateParam.State==STATE_BLINK)
		{
			OnSMReset(StateParam.Param,Fiber);

		}
		else if(StateParam.State==STATE_DIZZY)
		{
			OnSMDizzy(StateParam.Param,Fiber);
		}
		else if(StateParam.State==STATE_HIGH)
		{
			OnSMHigh(StateParam.Param,Fiber);
		}
		else if(StateParam.State==STATE_FIRST)
		{
			OnSMFirst(StateParam.Param,Fiber);

		}
		else if(StateParam.State==STATE_SKID)
		{
			OnSMSkid(StateParam.Param,Fiber);
		}
		else if(StateParam.State==STATE_QUALM)
		{
			OnSMQualm(StateParam.Param,Fiber);
		}
		else if(StateParam.State==STATE_BLIND)
		{
			OnSMBlind(StateParam.Param,Fiber);
		}
		else if(StateParam.State==STATE_WATER)
		{
			OnSMBat(StateParam.Param,Fiber);
		}
		else if(StateParam.State==STATE_FIRE)
		{
			OnSMFire(StateParam.Param,Fiber);
		}
		else if(StateParam.State==STATE_ROLL)
		{
			OnSMSideRoll(StateParam.Param,Fiber);
		}
		else if(StateParam.State==STATE_SIDE_FLIP)
		{
			OnSMSideFlip(StateParam.Param,Fiber);
		}
		*/
	}

	ucVOID OnStateJumpDust(UCObject* Sender, UCEventArgs*)
	{
		UCTickFiberData* pData = (UCTickFiberData*)Sender;
		JumpDustMesh.Visible = 1;

		for (ucINT i = 0; i < 4; i++)
		{
			Physics.CenterPoint.Pos.y -= 0.5f;

			pData->SyncTick(1);
		}
		for (ucINT i = 0; i < 2; i++)
		{
			Physics.CenterPoint.Pos.y += 1.0f;

			pData->SyncTick(1);
		}
		Physics.CenterPoint.Pos.y = 0.0f;
		JumpDustMesh.Visible = 0;
	}

	ucVOID OnFiberAutoDrive(UCObject* Sender, UCEventArgs* e)
	{
		UCTickFiberData* Fiber = (UCTickFiberData*)Sender;

		//if (Physics.IsSelf) gCarSound.PlaySound(15);

		ucINT DriftTick = 0;

		ucFLOAT HandleMax = HANDLE_MAX;
		ucFLOAT HandleAcc = HANDLE_BAS + Physics.Data.Handing * HANDLE_INC;

		ucFLOAT Handle = 0.0f;
		Physics.PortalObject.WayKind = 0;

		ucINT RandDir = 0;
		ucINT StopTime = 0;
		ucINT RandTime = 0;
		uc3dxVector3	StopPos = Physics.Center.Pos.Value();
		while (Fiber->IsValid())
		{
			//转向
			for (UCPortal* Portal = Physics.PortalObject.PortalNext;
				Portal != 0;
				Portal = Portal->Room2->OutPortals.GetAt(0))
			{
				uc3dxVector3 PortalCenter = Portal->Center;
				uc3dxVector3 PhysicsCenter = Physics.Center.Pos.Value();

				uc3dxVector3 Dir = PortalCenter - PhysicsCenter;
				ucFLOAT Len = Dir.Length();
				//如果太近，向下个目标行驶
				if (Len < 2000.0f)
					continue;

				Dir.Normalize();
				ucFLOAT WayX = Physics.CenterBody.GetWay_X() | Dir;

				Physics.KeyD = 0;
				Physics.KeyL = 0;
				Physics.KeyR = 0;

				if ((RandTime > 0 && RandDir == -1) || (RandTime == 0 && WayX < -0.1f) ||
					(Physics.DriftType == 1 && Physics.DriftDir == -1) ||
					(Physics.DriftType == 2 && Physics.DriftDir == +1))
				{
					Physics.KeyL = 1;
					//如果比较陡
					if (WayX < -0.32f || Physics.DriftType == 1)
					{
						//Physics.KeyD = 1;
					}
				}
				if ((RandTime > 0 && RandDir == +1) || (RandTime == 0 && WayX > +0.1f) ||
					(Physics.DriftType == 1 && Physics.DriftDir == +1) ||
					(Physics.DriftType == 2 && Physics.DriftDir == -1))
				{
					Physics.KeyR = 1;
					if (WayX > +0.32f || Physics.DriftType == 1)
					{
						//Physics.KeyD = 1;
					}
				}

				Physics.CenterBody.Rot = Physics.CenterBody.Rot.Value() + Physics.CenterBody.GetWay_Y() * Physics.WheellRotY * 0.05f;
				break;
			}

			if (RandTime > 0)
				RandTime--;

			uc3dxVector3 Dis = Physics.Center.Pos.Value() - StopPos;
			if (Dis.Length() < BODY_RADIUS * 4.0f)
			{
				StopTime++;
				if (StopTime > 250)
				{
					Physics.Mode = GRD_RESET;
					ucINT Result = 0;
					Physics.SetStateNet("Reset", 0, &Result);

					if (randint(0, 1))
						RandDir = +1;
					else
						RandDir = -1;
					RandTime = 250;
				}
			}
			else
			{	//更新新坐标
				StopPos = Physics.Center.Pos.Value();
				StopTime = 0;
			}
			Fiber->SyncTick(1);
		}
		Physics.KeyD = 0;
		Physics.KeyL = 0;
		Physics.KeyR = 0;

		Physics.PortalObject.WayKind = 1;
	}

	ucVOID OnFiberAutoDrive1(UCObject* Sender, UCEventArgs* e)
	{
		UCTickFiberData* Fiber = (UCTickFiberData*)Sender;
		Physics.PortalObject.PortalDir();

		//if (Physics.IsSelf) gCarSound.PlaySound(15);
		while (Fiber->IsValid())
		{
			Physics.PortalObject.Update(Physics.Center.Pos.Value());

			ucFLOAT MD = 0.1f;
			ucFLOAT MW = Physics.PortalObject.PortalLast->SizeCX / 6.0f;

			ucFLOAT yPortal = Physics.PortalObject.PortalDir().y;
			ucFLOAT yCenter = Physics.CenterBody.Rot.y.Value;

			ucFLOAT y = yPortal - yCenter;	if (y > UC3D_PI) y -= UC3D_PI * 2.0f;	if (y < -UC3D_PI) y += UC3D_PI * 2.0f;

			ucFLOAT Side = Physics.PortalObject.GetSide();
			if (Side > +MW)
			{
				Physics.KeyR = 1;
				Physics.KeyL = 0;
			}
			else if (Side < -MW)
			{
				Physics.KeyL = 1;
				Physics.KeyR = 0;
			}
			else
			{
				if (y > +MD && y < +UC3D_PI)
				{
					Physics.KeyR = 1;
					Physics.KeyL = 0;
				}
				else if (y<-MD && y>-UC3D_PI)
				{
					Physics.KeyL = 1;
					Physics.KeyR = 0;
				}
				else
				{
					Physics.KeyL = 0;
					Physics.KeyR = 0;
				}
			}

			Fiber->SyncTick(1);
		}
	}

	ucVOID OnSMSpeedUp(UCObject* Sender, UCEventArgs* e)
	{
		UCTickFiberData* Fiber = (UCTickFiberData*)Sender;
		UCSMFiber* Data = (UCSMFiber*)e;

		Physics.PlayAction(5);

		Physics.SetVentFire(BIGFIRE, 1.0f);

		UCMesh				StateMesh;
		StateMesh.Parent = &Physics.CenterPoint;
		StateMesh.Name = UCString("res/model/action/speedup.dat");
		StateMesh.Visible = 1;
		StateMesh.PlayAction(0);
		//if (Physics.IsSelf) gCarSound.PlaySound(11);
		Physics.Center.LinearVel = Physics.CenterBody.GetWay_Z() * (VSPEED_BAS + VSPEED_UP);
		ucDWORD nTim = UCGetTime();

		for (ucINT i = 0; i < 25; i++)
		{
			//if (i<50)
			//{
			//	if (CameraRay)
			//	{
			//		//CameraRay->Length = CameraRay->Length.Value + 2.0f;
			//	}
			//}
			//else if (i>=250)
			//{
			//	if (CameraRay)
			//	{
			//		//CameraRay->Length = CameraRay->Length.Value - 2.0f;
			//	}
			//}
			if (Physics.DriftType == 0)
			{
				ucFLOAT y = Physics.Center.LinearVel.y.Value;
				//Physics.Center.LinearVel = Physics.CenterBody.GetWay_Z() * (VSPEED_BAS + VSPEED_INC*Physics.Data.MaxSpeed + VSPEED_UP);
				Physics.Center.LinearVel.y = y;
			}
			Fiber->SyncTick(10);
		}

		StateMesh.Visible = 0;

		//Physics.CarVentFire.Visible =0;
		/*VentThings.Init(VentID);
		Physics.CarVentFire.Name =VentThings.VentSmallFire;
		Physics.CarVentFire.PlayAction(0,4.0f,0);*/
		Physics.SetVentFire(SMALLFIRE, 6.0f);

		/*ucINT a = UCGetTime() - nTim;
		ucTrace(UCString("加速：")+ITOS(a)+UCString("\r\n"));*/


	}
	ucVOID OnSMSuperSpeedUp(UCObject* Sender, UCEventArgs* e)//竞速赛的超级加速
	{
		UCTickFiberData* Fiber = (UCTickFiberData*)Sender;
		UCSMFiber* Data = (UCSMFiber*)e;

		Physics.PlayAction(5);

		/*VentThings.Init(VentID);
		Physics.CarVentFire.Name =VentThings.VentFire;
		Physics.CarVentFire.PlayAction(0);*/
		Physics.SetVentFire(BIGFIRE, 1.0f);


		UCMesh				StateMesh;
		StateMesh.Parent = &Physics.CenterPoint;
		StateMesh.Name = UCString("res/model/action/speedup.dat");
		StateMesh.Visible = 1;
		StateMesh.PlayAction(0);
		//if (Physics.IsSelf) gCarSound.PlaySound(11);
		Physics.Center.LinearVel = Physics.CenterBody.GetWay_Z() * 360.0f;
		for (ucINT i = 0; i < 40; i++)
		{
			//if (i<50)
			//{
			//	if (CameraRay)
			//	{
			//		//CameraRay->Length = CameraRay->Length.Value + 2.0f;
			//	}
			//}
			//else if (i>=250)
			//{
			//	if (CameraRay)
			//	{
			//		//CameraRay->Length = CameraRay->Length.Value - 2.0f;
			//	}
			//}
			if (Physics.DriftType == 0)
			{
				ucFLOAT y = Physics.Center.LinearVel.y.Value;
				Physics.Center.LinearVel = Physics.CenterBody.GetWay_Z() * (VSPEED_BAS + VSPEED_INC * Physics.Data.MaxSpeed + VSPEED_UP * 1.5f);
				Physics.Center.LinearVel.y = y;
			}
			Fiber->SyncTick(12);
		}

		StateMesh.Visible = 0;


		/*VentThings.Init(VentID);
		Physics.CarVentFire.Name =VentThings.VentSmallFire;
		Physics.CarVentFire.PlayAction(0,4.0f,0);*/
		Physics.SetVentFire(SMALLFIRE, 6.0f);
	}
	ucVOID OnSMSpeedDown(UCObject* Sender, UCEventArgs* e)//减速棒造成的减速
	{
		UCTickFiberData* Fiber = (UCTickFiberData*)Sender;
		UCSMFiber* Data = (UCSMFiber*)e;

		//Physics.PlayAction(5);

		UCMesh				StateMesh;
		StateMesh.Parent = &Physics.CenterPoint;
		StateMesh.Name = UCString("res/model/action/speeddown.dat");
		StateMesh.Visible = 1;
		StateMesh.Scale = uc3dxVector3(3.0f, 3.0f, 3.0f);
		StateMesh.PlayAction(0);
		//if (Physics.IsSelf) gCarSound.PlaySound(11);
		Fiber->SyncTick(250);

		StateMesh.Visible = 0;

		if (!KeyState.KEYIsUp())
			Physics.DriftUpTime = 20;
	}
	ucVOID OnSMSleep(UCObject* Sender, UCEventArgs* e)//睡眠
	{
		UCTickFiberData* Fiber = (UCTickFiberData*)Sender;
		UCSMFiber* Data = (UCSMFiber*)e;

		UCMesh QualmMesh;
		QualmMesh.Name = UCString("res/model/action/sleep.dat");
		QualmMesh.Parent = &Physics.CenterPoint;
		QualmMesh.Scale = uc3dxVector3(1.0f, 2.0f, 1.0f);
		QualmMesh.PlayAction(0);

		Physics.Wheel[0].SetMaterial(&MtlFreeze);
		Physics.Wheel[1].SetMaterial(&MtlFreeze);
		Physics.Wheel[2].SetMaterial(&MtlFreeze);
		Physics.Wheel[3].SetMaterial(&MtlFreeze);

		Physics.Center.GravityMode = 0;

		for (ucINT i = 0; i < 40; i++)
		{
			if (i < 10)QualmMesh.Scale = QualmMesh.Scale.Value() + uc3dxVector3(0.10f, 0.10f, 0.10f);

			if (i == 0)
				Physics.Ball.SetMaterial(&MtlFreeze);
			else if (i == 10)	Physics.CarBody.SetMaterial(&MtlFreeze);
			//else if (i==12) Physics.CarVent.SetMaterial(&MtlFreeze);	
			else if (i == 13) Physics.HumanPant.SetMaterial(&MtlFreeze);
			else if (i == 18)
			{
				//Physics.HumanFace.SetMaterial(&MtlFreeze);
				//Physics.HumanHair.SetMaterial(&MtlFreeze);
				Physics.SetMaterail(&MtlFreeze);
			}
			Fiber->SyncTick(2);

		}
		Physics.Center.LinearVel = uc3dxVector3(0.0f, 0.0f, 0.0f);
		Fiber->SyncTick(245);
		for (ucINT i = 0; i < 50; i++)
		{
			if (i > 5 && i < 25)QualmMesh.Scale = QualmMesh.Scale.Value() - uc3dxVector3(0.10f, 0.15f, 0.10f);

			if (i == 10)
			{
				Physics.HumanFace.SetMaterial(&Physics.Material);
				Physics.HumanHair.SetMaterial(&Physics.Material);
			}
			else if (i == 20)
			{
				//Physics.HumanBody.SetMaterial(&Physics.Material);
				Physics.SetBaseMaterail();
			}
			else if (i == 30)
			{
				Physics.CarBody.SetMaterial(0);

			}
			/*else if (i==42)
			{
				Physics.CarVent.SetMaterial(0);
			}*/
			else if (i == 46)
				Physics.Ball.SetMaterial(&Physics.Material);
			else if (i == 48)
			{
				Physics.Wheel[0].SetMaterial(&Physics.Material);
				Physics.Wheel[1].SetMaterial(&Physics.Material);
				Physics.Wheel[2].SetMaterial(&Physics.Material);
				Physics.Wheel[3].SetMaterial(&Physics.Material);
			}


			Fiber->SyncTick(1);

		}

		QualmMesh.Visible = 0;
		Physics.Center.GravityMode = 1;
		Physics.SetBaseMaterail();



	}
	ucVOID OnSMBuyOver(UCObject* Sender, UCEventArgs* e)//收买
	{
		UCTickFiberData* Fiber = (UCTickFiberData*)Sender;
		// 		UCSMFiber* Data = (UCSMFiber*)e;
		// 
		// 		ucINT initteam = Self_Pane->Group;
		// 
		// 		UCMesh QualmMesh;
		// 		QualmMesh.Name = UCString("res/model/action/buyover.dat");
		// 		QualmMesh.Parent = &Physics.CenterPoint;
		// 		QualmMesh.Scale = uc3dxVector3(4.0f,4.0f,4.0f); 
		// 		QualmMesh.Pos = uc3dxVector3(0.0f,80.0f,0.0f);
		// 		QualmMesh.PlayAction(0);
		// 
		// 		if (initteam==11)
		// 			Self_Pane->Group =12;
		// 		else if (initteam==12)
		// 			Self_Pane->Group =11;
		// 
		// 		GroupID =Self_Pane->Group;
		// 				
		// 		Physics.InitTeamName(Self_Pane->Group-11,UCString("已收买"));	//头顶面板变色					
		// 				
		// 		Fiber->SyncTick(290);
		// 
		// 		QualmMesh.Visible =0;
		// 		Self_Pane->Group =initteam;
		// 		GroupID =initteam;
		// 
		// 		Physics.InitTeamName(Self_Pane->Group-11,UCString("吊线玩家"));		
	}
	ucVOID OnSMChild(UCObject* Sender, UCEventArgs* e)//儿童
	{

		UCTickFiberData* Fiber = (UCTickFiberData*)Sender;
		UCSMFiber* Data = (UCSMFiber*)e;


		UCMesh QualmMesh;
		QualmMesh.Name = UCString("res/model/action/babycar.dat");
		//QualmMesh.Parent = &Physics.CenterPoint;
		QualmMesh.Parent = &Physics.Body;

		QualmMesh.Scale = uc3dxVector3(1.75f, 1.75f, 1.75f);
		QualmMesh.Pos.y = -5.0f;
		QualmMesh.Rot.y = 3.14f;
		QualmMesh.PlayAction(0);
		//QualmMesh.ShadowCaster = 1;

		ucFLOAT PosInit1 = Physics.DriftMeshMove.Pos.y.Value;
		ucFLOAT PosInit2 = Physics.DriftMeshRead.Pos.y.Value;

		UCMesh Mesh;
		Mesh.Name = UCString("res/model/action/babycarfire.dat");
		Mesh.Parent = &Physics.CenterPoint;
		Mesh.Scale = uc3dxVector3(1.0f, 1.0f, 1.0f);
		Mesh.Pos = uc3dxVector3(0.0f, -24.0f, 12.0f);
		Mesh.PlayAction(0);

		Physics.CarBody.Visible = 0;
		Physics.CarVent.Visible = 0;
		Physics.SetVentFireVisible(0);
		for (ucINT i = 0; i < MAX_WHEEL; i++)
			Physics.Wheel[i].Visible = 0;
		Physics.PlayAction(14);
		ucFLOAT PosInit = Physics.CenterPoint.Pos.y.Value;
		Physics.CenterPoint.Pos.y = 25.0f;
		Physics.DriftMeshMove.Pos.y = -5.0f;
		Physics.DriftMeshRead.Pos.y = -5.0f;

		Fiber->SyncTick(600);

		Physics.CarBody.Visible = 1;
		Physics.CarVent.Visible = 1;
		Physics.SetVentFireVisible(1);

		for (ucINT i = 0; i < MAX_WHEEL; i++)
			Physics.Wheel[i].Visible = 1;
		Physics.PlayAction(0);
		Physics.CenterPoint.Pos.y = PosInit;
		Physics.DriftMeshMove.Pos.y = PosInit1;
		Physics.DriftMeshRead.Pos.y = PosInit2;
	}
	ucVOID OnSMShock(UCObject* Sender, UCEventArgs* e)//地震
	{
		UCTickFiberData* Fiber = (UCTickFiberData*)Sender;
		UCSMFiber* Data = (UCSMFiber*)e;

		UCMesh Mesh;
		Mesh.Name = UCString("res/model/action/fire.dat");
		Mesh.Parent = &Physics.CenterPoint;
		Mesh.Scale = uc3dxVector3(3.0f, 3.0f, 3.0f);
		Mesh.PlayAction(0);

		for (ucINT j = 0; j < 2; j++)
		{
			for (ucINT i = 0; i < 56 && Fiber->IsValid(); i++)
			{
				if (i / 7 == 0 || i / 7 == 3 || i / 7 == 4 || i / 7 == 7)
				{
					Physics.CenterPoint.Rot.x = Physics.CenterPoint.Rot.x.Value - 0.15f;
					Physics.CenterPoint.Rot.z = Physics.CenterPoint.Rot.z.Value + 0.08f;

				}
				else
				{
					Physics.CenterPoint.Rot.x = Physics.CenterPoint.Rot.x.Value + 0.15f;
					Physics.CenterPoint.Rot.z = Physics.CenterPoint.Rot.z.Value - 0.08f;


				}
				if (i / 14 == 0 || i / 14 == 2)
				{
					Physics.CenterPoint.Pos.y = Physics.CenterPoint.Pos.y.Value + 3.0f;


				}
				else
				{
					Physics.CenterPoint.Pos.y = Physics.CenterPoint.Pos.y.Value - 3.0f;

				}
				Physics.Center.LinearVel = Physics.CenterBody.GetWay_Z() * 40.0f;


				Fiber->SyncTick(1);

			}
			if (KeyState.KEYIsSpace())
			{
				if (Physics.Mode != GRD_EMPTH)
				{
					Physics.Center.LinearVel.y = 120.0f;
					Physics.JumpState = JUMP_WAIT;
				}
				ucFLOAT v = 6.2832f / 20.0f;
				for (ucINT i = 0; i < 20 && Fiber->IsValid(); i++)
				{
					Physics.CenterPoint.Rot.x = Physics.CenterPoint.Rot.x.Value - v;

					Fiber->SyncTick(2);
				}

			}
			if (j == 0)
				Fiber->SyncTick(66);
		}

		Mesh.Visible = 0;
		Physics.CenterPoint.Rot.x = 0.0f;
		Physics.CenterPoint.Rot.z = 0.0f;
		Physics.CenterPoint.Pos.y = 0.0f;

		if (!KeyState.KEYIsUp())
			Physics.DriftUpTime = 20;
	}
	ucVOID OnSMHittedByBoxer(UCObject* Sender, UCEventArgs* e)//被拳击手打击
	{
		UCTickFiberData* Fiber = (UCTickFiberData*)Sender;
		UCSMFiber* Data = (UCSMFiber*)e;

		UCMesh QualmMesh1;
		QualmMesh1.Name = UCString("res/model/action/boxgloves.dat");
		QualmMesh1.Parent = Physics.SceneCtrl->Scene;
		QualmMesh1.Scale = uc3dxVector3(0.0f, 0.0f, 0.0f);
		QualmMesh1.Rot.y.Value = Physics.CenterBody.Rot.y.Value + 3.14f;
		//QualmMesh1.Rot.x.Value =0.0f;
		//QualmMesh1.Rot.z.Value =0.0f;
		QualmMesh1.Pos.z.Value = Physics.Center.Pos.z.Value;
		QualmMesh1.Pos.x.Value = Physics.Center.Pos.x.Value + 40.0f;//前后+参照物体前-后
		QualmMesh1.Pos.y = Physics.Center.Pos.y.Value + 10.0f;
		QualmMesh1.PlayAction(0);


		UCMesh Mesh;
		Mesh.Name = UCString("res/model/action/boxman2.dat");
		Mesh.Parent = &Physics.CenterPoint;
		Mesh.Scale = uc3dxVector3(4.0f, 4.0f, 4.0f);
		Mesh.PlayAction(0);
		Mesh.Visible = 0;

		Physics.Center.LinearVel = uc3dxVector3(0.0f, 0.0f, 0.0f);
		Physics.Center.GravityMode = 0;


		for (ucINT i = 0; i < 30 && Fiber->IsValid(); i++)
		{

			QualmMesh1.Scale = QualmMesh1.Scale.Value() + uc3dxVector3(0.08f, 0.08f, 0.08f);

			QualmMesh1.Pos.z.Value = Physics.Center.Pos.z.Value;
			QualmMesh1.Pos.x.Value = Physics.Center.Pos.x.Value + 20.0f;
			QualmMesh1.Pos.y = Physics.Center.Pos.y.Value + 10.0f;


			Fiber->SyncTick(1);
		}
		Mesh.Visible = 1;
		QualmMesh1.PlayAction(1, 2.4f, 0);


		//if (Physics.IsSelf) gCarSound.PlaySound(27);


		for (ucINT i = 0; i < 10 && Fiber->IsValid(); i++)
		{
			Physics.CenterPoint.Rot.z = Physics.CenterPoint.Rot.z.Value + 0.0585f;

			Fiber->SyncTick(1);
		}

		for (ucINT i = 0; i < 100 && Fiber->IsValid(); i++)
		{

			if ((i / 5) % 2 != 0)
			{
				Physics.CenterPoint.Rot.z = Physics.CenterPoint.Rot.z.Value - 0.1261f;
			}
			else
			{
				Physics.CenterPoint.Rot.z = Physics.CenterPoint.Rot.z.Value + 0.1261f;

			}



			QualmMesh1.Pos.z.Value = Physics.Center.Pos.z.Value;
			QualmMesh1.Pos.x.Value = Physics.Center.Pos.x.Value + 20.0f;
			QualmMesh1.Pos.y = Physics.Center.Pos.y.Value + 10.0f;

			if (i < 40)	Fiber->SyncTick(2);
			else		Fiber->SyncTick(1);

		}

		for (ucINT i = 0; i < 10 && Fiber->IsValid(); i++)
		{
			Physics.CenterPoint.Rot.z = Physics.CenterPoint.Rot.z.Value - 0.0585;
			QualmMesh1.Pos.z.Value = Physics.Center.Pos.z.Value;
			QualmMesh1.Pos.x.Value = Physics.Center.Pos.x.Value + 20.0f;
			QualmMesh1.Pos.y = Physics.Center.Pos.y.Value + 10.0f;

			Fiber->SyncTick(1);
		}
		Fiber->SyncTick(12);
		QualmMesh1.PlayAction(2);
		for (ucINT i = 0; i < 70 && Fiber->IsValid(); i++)
		{

			if (i < 30)
			{
				Physics.CenterPoint.Rot.y = Physics.CenterPoint.Rot.y.Value - 0.40f;
			}
			else if (i > 40)
			{
				Physics.CenterPoint.Rot.y = Physics.CenterPoint.Rot.y.Value - 0.80f;
			}
			else
			{
				Physics.CenterPoint.Rot.y = Physics.CenterPoint.Rot.y.Value - 0.40f * (i - 30 + 1);
			}
			QualmMesh1.Pos.z.Value = Physics.Center.Pos.z.Value;
			QualmMesh1.Pos.x.Value = Physics.Center.Pos.x.Value + 20.0f;
			QualmMesh1.Pos.y = Physics.Center.Pos.y.Value + 10.0f;

			Fiber->SyncTick(3);

		}

		QualmMesh1.PlayAction(0);
		QualmMesh1.Visible = 0;
		for (ucINT i = 0; i < 40; i++)
		{
			//QualmMesh1.Scale = QualmMesh1.Scale.Value()-uc3dxVector3(0.030f,0.030f,0.030f); 
			//QualmMesh.Scale = QualmMesh.Scale.Value()-uc3dxVector3(0.1f,0.1f,0.1f); 
			//QualmMesh1.Rot.z +=0.0125f;
			QualmMesh1.Pos.z.Value = Physics.Center.Pos.z.Value;
			QualmMesh1.Pos.x.Value = Physics.Center.Pos.x.Value + 20.0f;
			QualmMesh1.Pos.y = Physics.Center.Pos.y.Value + 10.0f;

			Physics.CenterPoint.Rot.y = Physics.CenterPoint.Rot.y.Value - 0.0105f * (40 - i);


			Fiber->SyncTick(3);

		}


		QualmMesh1.Visible = 0;
		Physics.Center.GravityMode = 1;
		Physics.CenterPoint.Rot.y = 3.1416f;
		Physics.CenterPoint.Rot.z = 0.0f;
		Physics.PlayAction(0);

	}
	ucVOID OnSMJingDeng(UCObject* Sender, UCEventArgs* e)//警灯
	{
		UCTickFiberData* Fiber = (UCTickFiberData*)Sender;
		UCMesh				StateMesh;
		StateMesh.Parent = &Physics.CenterPoint;
		StateMesh.Pos.y = -24.0f;
		StateMesh.Pos.z = 10.0f;
		StateMesh.Name = UCString("res/model/action/alarm.dat");
		StateMesh.Scale = uc3dxVector3(1.6f, 1.6f, 1.5f);
		StateMesh.PlayAction(0);

		Physics.SetVisible(0);
		Physics.ShowShadow(0);

		if (!Physics.IsSelf)
		{

			Physics.SceneCtrl->Fog.Enable = 1;
			Physics.SceneCtrl->Fog.Start = 100.0f;  //开始有雾的距离
			Physics.SceneCtrl->Fog.End = 10000.0f;  //全雾化的距离，超过了什么也看不到了		
			Physics.SceneCtrl->Fog.Color = 0xFFFB0522;//红	
		}


		if (Fiber->IsValid()) Fiber->SyncTick(500);


		Physics.SetVisible(1);
		Physics.ShowShadow(1);
		if (!Physics.IsSelf) Physics.SceneCtrl->Fog.Enable = 0;



	}
	ucVOID OnSMStateReadyLaoLong(UCObject* Sender, UCEventArgs* e)
	{
		UCTickFiberData* pData = (UCTickFiberData*)Sender;

		UCMesh				StateMesh;
		StateMesh.Parent = &Physics.CenterPoint;
		StateMesh.Name = UCString("res/model/action/laolong.dat");
		StateMesh.Visible = 1;
		StateMesh.Pos.y = 160.0f;
		StateMesh.Pos.z = -50.0f;
		StateMesh.Rot.x = -1.00f;
		StateMesh.Scale = uc3dxVector3(2.0f, 2.0f, 2.0f);
		StateMesh.PlayAction(0);
		ucINT dir = 1;
		for (ucINT i = 0; i < 24 && pData->IsValid(); i++)
		{
			StateMesh.Pos.y = StateMesh.Pos.y.Value - 5 * dir;
			StateMesh.Pos.z = StateMesh.Pos.z.Value + 10 * dir;
			if (StateMesh.Pos.y.Value > 160)
			{
				dir = -dir;
			}
			else if (StateMesh.Pos.y.Value < 140)
			{
				dir = -dir;
			}
			pData->SyncTick(4);
		}
		if (pData->IsValid())	Physics.State.Set("牢笼", 0);
	}
	ucVOID OnSMHitByLaoLong(UCObject* Sender, UCEventArgs* e)//牢笼
	{
		UCTickFiberData* Fiber = (UCTickFiberData*)Sender;


		UCMesh				StateMesh;
		StateMesh.Parent = &Physics.CenterPoint;
		StateMesh.Name = UCString("res/model/action/laolong.dat");
		StateMesh.Visible = 1;
		StateMesh.Pos.y = 160.0f;
		StateMesh.Pos.z = -50.0f;
		StateMesh.Rot.x = -1.00f;
		StateMesh.Scale = uc3dxVector3(2.0f, 2.0f, 2.0f);
		StateMesh.PlayAction(0);
		StateMesh.Pos.z = -15.0f;

		for (ucINT i = 0; i < 20 && Fiber->IsValid(); i++)
		{
			StateMesh.Pos.y = StateMesh.Pos.y.Value - 8;
			StateMesh.Rot.x = StateMesh.Rot.x.Value + 0.05f;
			Fiber->SyncTick(1);
		}
		UCMesh DustMesh;
		DustMesh.Name = UCString("res/model/action/jumpdust.dat");
		DustMesh.Parent = &Physics.CenterPoint;
		DustMesh.Scale = uc3dxVector3(2.5f, 2.5f, 2.5f);
		DustMesh.Visible = 1;
		DustMesh.Pos.y = -16.0f;
		DustMesh.PlayAction(0);
		Fiber->SyncTick(50);
		DustMesh.Visible = 0;

		for (ucINT i = 0; i < 15 && Fiber->IsValid(); i++)
		{
			Physics.Center.LinearVel.y = 0.0f;
			Physics.Center.LinearVel.x = 0.0f;
			Physics.Center.LinearVel.z = 0.0f;
			Fiber->SyncTick(20);
		}
	}
	ucVOID OnSMHitByJiaSuo(UCObject* Sender, UCEventArgs* e)//枷锁
	{
		UCTickFiberData* Fiber = (UCTickFiberData*)Sender;
		UCMesh				StateMesh;
		StateMesh.Parent = &Physics.CenterPoint;
		StateMesh.Name = UCString("res/model/action/jiasuo.dat");
		StateMesh.Visible = 1;
		StateMesh.Scale = uc3dxVector3(0.6f, 0.6f, 0.6f);
		StateMesh.PlayAction(0);
		StateMesh.Pos.y = 30.0f;
		StateMesh.Pos.z = 200.0f;
		for (ucINT i = 0; i < 30; i++)
		{
			if (i < 10)
			{
				StateMesh.Rot.x = StateMesh.Rot.x.Value + 0.0523f;
				StateMesh.Scale = StateMesh.Scale.Value() + uc3dxVector3(0.2f, 0.2f, 0.2f);
				StateMesh.Pos.z = StateMesh.Pos.z.Value - 20.0f;
				Fiber->SyncTick(2);

			}
			else
			{
				StateMesh.Rot.x = StateMesh.Rot.x.Value - 0.077f;
				StateMesh.Pos.y = StateMesh.Pos.y.Value - 3.0f;
				StateMesh.Pos.z = StateMesh.Pos.z.Value + 2.3f;
				Fiber->SyncTick(1);
			}

		}

		Fiber->SyncTick(200);

	}

	ucVOID OnPhysicsError(UCObject*, UCEventArgs*)
	{
		Physics.Mode = GRD_RESET;

		ucINT Result = 0;
		Physics.SetStateNet("Reset", 0, &Result);
	}

	ucVOID OnSMAdhereByDevil(UCObject* Sender, UCEventArgs* e)//恶魔附身
	{
		UCTickFiberData* Fiber = (UCTickFiberData*)Sender;
		UCSMFiber* Data = (UCSMFiber*)e;

		Physics.PlayAction(15);

		UCMesh				StateMesh;
		StateMesh.Parent = &Physics.CenterPoint;
		StateMesh.Name = UCString("res/model/action/lighting.dat");
		StateMesh.Visible = 1;
		StateMesh.Scale = uc3dxVector3(2.5f, 10.0f, 2.5f);
		StateMesh.PlayAction(0, 3.0f, 0);
		//if (Physics.IsSelf) gCarSound.PlaySound(11);

		if (Physics.IsSelf)
		{
			Physics.SceneCtrl->Fog.Enable = 1;
			Physics.SceneCtrl->Fog.Color = 0xFF000000;//,FBE3BC,,FC9D22,FAC178
			Physics.SceneCtrl->Fog.Start = 100.0f;  //开始有雾的距离
			Physics.SceneCtrl->Fog.End = 10000.0f;  //全雾化的距离，超过了什么也看不到了
		}

		ucFLOAT v = 6.28f / 25.0f;
		for (ucINT i = 0; i < 50; i++)
		{

			Physics.CenterPoint.Rot.y = Physics.CenterPoint.Rot.y.Value + v;
			Physics.CenterPoint.Scale = Physics.CenterPoint.Scale.Value() - uc3dxVector3(0.01f, 0.01f, 0.01f);
			Fiber->SyncTick(1);
		}
		StateMesh.Visible = 0;
		Fiber->SyncTick(160);

		for (ucINT i = 0; i < 50; i++)
		{

			Physics.CenterPoint.Scale = Physics.CenterPoint.Scale.Value() + uc3dxVector3(0.01f, 0.01f, 0.01f);
			Fiber->SyncTick(1);
		}

		Physics.PlayAction(0);
		if (Physics.IsSelf)	Physics.SceneCtrl->Fog.Enable = 0;
		Physics.CenterPoint.Rot.y = 3.1416f;
		Physics.CenterPoint.Scale = uc3dxVector3(1.0f, 1.0f, 1.0f);
	}
	ucVOID OnSMShiftSpeedDown(UCObject* Sender, UCEventArgs* e)//法力被燃烧
	{
		UCTickFiberData* Fiber = (UCTickFiberData*)Sender;
		UCSMFiber* Data = (UCSMFiber*)e;
		Physics.PlayAction(15);
		UCMesh				StateMesh;
		StateMesh.Parent = &Physics.CenterPoint;
		StateMesh.Name = UCString("res/model/action/shiftspeeddown.dat");
		StateMesh.Visible = 1;
		StateMesh.PlayAction(0);
		StateMesh.Scale = uc3dxVector3(1.0f, 1.0f, 1.0f);
		//if (Physics.IsSelf) gCarSound.PlaySound(11);

		Physics.SetMaterail(&MtlRed);

		//Black.Visible =1;
		ucFLOAT a = ShiftSpeedDownPic.Picture.Scale_V.Value;
		ucINT b = ShiftSpeedDownPic.Size.cy.Value;
		ucINT c = ShiftSpeedDownPic.Location.y.Value;
		if (Physics.IsSelf)
		{
			ShiftSpeedDownPic.Visible = 1;

		}

		for (ucINT i = 0; i < 20; i++)
		{
			if (Physics.IsSelf)
			{
				if (i % 2)
				{
					ShiftSpeedDownPic.Picture.Scale_V += 0.4f;
					ShiftSpeedDownPic.Location.y -= 14;
				}
				else
				{
					ShiftSpeedDownPic.Picture.Scale_V -= 0.4f;
					ShiftSpeedDownPic.Location.y += 14;
				}

			}

			Fiber->SyncTick(8);
		}
		//Black.Visible =0;

		Physics.PlayAction(0);
		ShiftDown = 0;
		StateMesh.Visible = 0;
		if (Physics.IsSelf)
		{
			ShiftSpeedDownPic.Visible = 0;
			ShiftSpeedDownPic.Picture.Scale_V = a;
			ShiftSpeedDownPic.Size.cy = b;
			ShiftSpeedDownPic.Location.y = c;
		}

		Physics.SetBaseMaterail();
	}
	ucVOID OnSMChongCi(UCObject* Sender, UCEventArgs* e)
	{
		UCTickFiberData* Fiber = (UCTickFiberData*)Sender;
		UCSMFiber* Data = (UCSMFiber*)e;

		/*VentThings.Init(VentID);
		Physics.CarVentFire.Name =VentThings.VentFire;
		Physics.CarVentFire.PlayAction(0);*/
		Physics.SetVentFire(BIGFIRE, 1.0f);

		UCMesh				StateMesh;
		StateMesh.Parent = &Physics.CenterPoint;
		StateMesh.Name = UCString("res/model/action/speedup.dat");
		StateMesh.PlayAction(0);
		StateMesh.Visible = 1;
		for (ucINT i = 0; i < 12 && Fiber->IsValid(); i++)
		{
			Physics.Center.LinearVel = Physics.CenterBody.GetWay_Z() * (VSPEED_BAS + VSPEED_INC * Physics.Data.MaxSpeed + VSPEED_UP);//Physics.CenterBody.GetWay_Z() * 270.0f;

			Fiber->SyncTick(1);

		}



		StateMesh.Visible = 0;
		/*	VentThings.Init(VentID);
			Physics.CarVentFire.Name =VentThings.VentSmallFire;
			Physics.CarVentFire.PlayAction(0,4.0f,0);*/
		Physics.SetVentFire(SMALLFIRE, 6.0f);
	}
	ucVOID OnSMDriftUp(UCObject* Sender, UCEventArgs* e)
	{
		UCTickFiberData* Fiber = (UCTickFiberData*)Sender;
		UCSMFiber* Data = (UCSMFiber*)e;

		/*VentThings.Init(VentID);
		Physics.CarVentFire.Name =VentThings.VentFire;
		Physics.CarVentFire.PlayAction(0,1.5f,0);*/
		Physics.SetVentFire(BIGFIRE, 1.5f);

		UCMesh				StateMesh;
		StateMesh.Parent = &Physics.CenterPoint;
		StateMesh.Name = UCString("res/model/action/speedup.dat");
		StateMesh.PlayAction(0);
		StateMesh.Visible = 1;
		//Physics.Center.LinearVel = Physics.CenterBody.GetWay_Z() * 300.0f;
		//Physics.Center.LinearVel = Physics.Center.LinearVel.Value() + Physics.CenterBody.GetWay_Z() * 140.0f;

		if (Physics.DriftUpTime < 35)
		{
			Fiber->SyncTick(Physics.DriftUpTime * 2);
		}
		else
		{
			Fiber->SyncTick(75);
		}
		Physics.DriftUpTime = 0;

		StateMesh.Visible = 0;

		/*	VentThings.Init(VentID);
			Physics.CarVentFire.Name =VentThings.VentSmallFire;
			Physics.CarVentFire.PlayAction(0,4.0f,0);*/
		Physics.SetVentFire(SMALLFIRE, 6.0f);
	}
	ucVOID OnSMRocket(UCObject* Sender, UCEventArgs* e)
	{
		UCTickFiberData* Fiber = (UCTickFiberData*)Sender;
		UCSMFiber* Data = (UCSMFiber*)e;

		Physics.CenterPoint.Rot.x = 0.0f;
		Physics.CenterPoint.Rot.z = 0.0f;
		UCMesh				StateMesh;
		StateMesh.Parent = &Physics.CenterPoint;
		StateMesh.Name = UCString("res/model/action/changetohuojian.dat");
		StateMesh.PlayAction(0);
		StateMesh.Visible = 1;	StateMesh.Scale = uc3dxVector3(2.0f, 2.0f, 2.0f);
		StateMesh.SetMaterial(&Physics.Material);
		Physics.SetVisible(0);
		Physics.PortalObject.PortalDir();

		//if (Physics.IsSelf) gCarSound.PlaySound(15);
		ucFLOAT Length = 0.0f;

		if (CameraRay)	Length = CameraRay->Length.Value;

		Physics.ShowShadow(0);
		Physics.Center.GravityMode = 0;
		ucDWORD nTim = UCGetTime();

		UCPortal* PortalLastBackup = Physics.PortalObject.PortalLast;
		for (ucINT i = 0; i < 250 && Fiber->IsValid(); i++)
		{
			//if (i<100)
			//{
			//	if (CameraRay)
			//	{
			//		//CameraRay->Length = CameraRay->Length.Value + 1.2f;
			//	}
			//}
			//else if (i>=550)
			//{
			//	if (CameraRay)
			//	{
			//		//CameraRay->Length = CameraRay->Length.Value - 1.2f;
			//	}
			//}

			uc3dxVector3	vDir;

			UCPortal* PortalLast = Physics.PortalObject.PortalLast;
			UCPortal* PortalNext = Physics.PortalObject.PortalNext;

			ucINT Valid = 0;
			while (PortalNext != 0 && PortalNext != PortalLast)
			{
				uc3dxVector3	vPos(Physics.Center.Pos.Value());
				uc3dxVector3	vTag(PortalNext->Center);
				vDir = vTag - vPos;	vDir.Normalize();

				ucFLOAT fLen = vDir | Physics.CenterBody.GetWay_Z();
				if (fLen > 0.0f)
				{
					Valid = 1;
					break;
				}

				Physics.PortalObject.RandPortalNext();
				PortalNext = Physics.PortalObject.PortalNext;
			}

			if (!Valid)
			{
				vDir = Physics.CenterBody.GetWay_Z();
			}

			Physics.Center.LinearVel = vDir * 300.0f;

			Physics.PortalObject.Update(Physics.Center.Pos.Value());

			ucFLOAT fPortalY = Physics.PortalObject.PortalDir().y;	while (fPortalY < -UC3D_PI)	fPortalY += UC3D_PI * 2.0f;	while (fPortalY > +UC3D_PI)	fPortalY -= UC3D_PI * 2.0f;
			ucFLOAT fCenterY = Physics.CenterBody.Rot.y.Value;		while (fCenterY < -UC3D_PI)	fCenterY += UC3D_PI * 2.0f;	while (fCenterY > +UC3D_PI)	fCenterY -= UC3D_PI * 2.0f;

			ucFLOAT fY = fPortalY - fCenterY;	while (fY < -UC3D_PI)	fY += UC3D_PI * 2.0f;	while (fY > +UC3D_PI)	fY -= UC3D_PI * 2.0f;

			Physics.CenterBody.Face(vDir);
			Physics.CenterBody.Rot.x = 0.0f;
			Physics.CenterBody.Rot.z = 0.0f;
			Fiber->SyncTick(2);
		}

		//if (CameraRay)	CameraRay->Length = Length;

		Physics.SetVisible(1);
		if (Physics.IsSelf) Physics.ShowShadow(1);

		StateMesh.Visible = 0;
		Physics.Center.GravityMode = 1;

		ucINT a = UCGetTime() - nTim;
		ucTrace(UCString("chaoren：") + ITOS(a) + UCString("\r\n"));

	}

	ucVOID OnSMAutoDrive(UCObject* Sender, UCEventArgs* e)
	{
		UCTickFiberData* Fiber = (UCTickFiberData*)Sender;
		UCSMFiber* Data = (UCSMFiber*)e;



		Physics.Body.Rot.x = 0.0f;
		Physics.Body.Rot.z = 0.0f;
		Physics.CenterBody.Rot.x = 0.0f;
		Physics.CenterBody.Rot.z = 0.0f;
		Physics.CenterPoint.Rot.x = 0.0f;
		Physics.CenterPoint.Rot.z = 0.0f;
		Physics.HumanBody.Rot.z = 0.0f;
		Physics.DriftType = 0;
		Physics.DriftMeshRead.Visible = 0;
		Physics.DriftMeshMove.Visible = 0;
		Physics.PortalObject.PortalDir();

		//if (Physics.IsSelf) gCarSound.PlaySound(15);
		ucFLOAT Length = 0.0f;

		for (ucINT i = 0; i < 1500 && Fiber->IsValid(); i++)
		{
			ucFLOAT y = Physics.Center.LinearVel.y.Value;
			UCPortal* RandPortal = Physics.PortalObject.PortalNext;
			if (RandPortal)
			{
				uc3dxVector3	vPos(Physics.Center.Pos.Value());
				uc3dxVector3	vTag(RandPortal->Center);
				vTag += RandPortal->Normal * 60.0f;

				uc3dxVector3	vDir = vTag - vPos;
				vDir.Normalize();

				if (vDir.y < 0.0f)	vDir.y = Physics.Center.LinearVel.y.Value;
				Physics.Center.LinearVel = vDir * 200.0f;

				Physics.Center.LinearVel.y = y;

			}
			else
			{
				uc3dxVector3 vDir = Physics.CenterBody.GetWay_Z();
				if (vDir.y < 0.0f)	vDir.y = Physics.Center.LinearVel.y.Value;
				Physics.Center.LinearVel = vDir * 200.0f;

				Physics.Center.LinearVel.y = y;

			}

			Physics.PortalObject.Update(Physics.Center.Pos.Value());
			Physics.CenterBody.Rot.y = Physics.PortalObject.PortalDir().y;


			Fiber->SyncTick(1);
		}
		if (Physics.IsSelf)
			Physics.PlayAction(0);
	}
	ucVOID OnSMTestDrive(UCObject* Sender, UCEventArgs* e)
	{
		UCTickFiberData* Fiber = (UCTickFiberData*)Sender;
		UCSMFiber* Data = (UCSMFiber*)e;

		Physics.IsSelf = 1;

		Physics.Body.Rot.x = 0.0f;
		Physics.Body.Rot.z = 0.0f;
		Physics.CenterBody.Rot.x = 0.0f;
		Physics.CenterBody.Rot.z = 0.0f;
		Physics.CenterPoint.Rot.x = 0.0f;
		Physics.CenterPoint.Rot.z = 0.0f;
		Physics.DriftType = 0;
		Physics.DriftMeshRead.Visible = 0;
		Physics.DriftMeshMove.Visible = 0;
		Physics.PortalObject.PortalDir();

		//AutoDriveDisX = randfloat(-122.0f,+122.0f);

		ucFLOAT Length = 0.0f;


		while (Fiber->IsValid())
		{
			if (Physics.ControlAble())
			{
				ucFLOAT y = Physics.Center.LinearVel.y.Value;
				UCPortal* RandPortal = Physics.PortalObject.PortalNext;
				if (RandPortal)
				{
					uc3dxVector3	vPos(Physics.Center.Pos.Value());
					uc3dxVector3	vTag(RandPortal->Center);
					vTag += RandPortal->Normal * 60.0f;

					uc3dxVector3	vUp(0.0f, 1.0f, 0.0f);
					uc3dxVector3	vX = RandPortal->Normal % vUp;
					vX.Normalize();
					vTag += vX * AutoDriveDisX;

					if (Physics.Profession == 0)
					{
						uc3dxVector3	vDir = Physics.CenterBody.GetWay_Z();

						ucFLOAT MaxSpeed = AutoDriveSpeed * Physics.CenterPoint.Scale.z.Value;

						uc3dxVector3 LinearVel = Physics.Center.LinearVel.Value();
						if (AutoDriveRotY == 0.0f)
						{
							LinearVel += vDir * 4.0f;
						}
						else
						{
							LinearVel -= Physics.CenterBody.GetWay_X() * AutoDriveRotY;
						}

						ucFLOAT CurSpeed = LinearVel.Length();
						if (CurSpeed < MaxSpeed)
							Physics.Center.LinearVel = LinearVel;
						Physics.Center.LinearVel.y = y;

					}
					else
					{
						uc3dxVector3	vDir = Physics.CenterBody.GetWay_Z();
						//if (vDir.y<0.0f)	vDir.y = Physics.Center.LinearVel.y.Value;
						Physics.Center.LinearVel = vDir * AutoDriveSpeed * Physics.CenterPoint.Scale.z.Value + Physics.CenterBody.GetWay_X() * AutoDriveRotY;

						Physics.Center.LinearVel.y = y;
					}
				}
				else
				{
					uc3dxVector3 vDir = Physics.CenterBody.GetWay_Z();
					if (vDir.y < 0.0f)	vDir.y = Physics.Center.LinearVel.y.Value;
					Physics.Center.LinearVel = vDir * AutoDriveSpeed * Physics.CenterPoint.Scale.z.Value;;

					Physics.Center.LinearVel.y = y;
					//ucTrace(UCString("NotPortal：")+FTOS(y)+UCString("\r\n"));
				}

				Physics.PortalObject.Update(Physics.Center.Pos.Value());
				Physics.CenterBody.Rot.y = Physics.PortalObject.PortalDir().y;

			}

			Fiber->SyncTick(1);
		}
	}

	ucVOID OnSMReset(UCObject* Sender, UCEventArgs* e)//被Reset以后
	{
		UCTickFiberData* Fiber = (UCTickFiberData*)Sender;
		UCSMFiber* Data = (UCSMFiber*)e;

		Physics.ShowShadow(0);

		UCMesh				StateMesh;
		StateMesh.Parent = &Physics.CenterPoint;
		StateMesh.Name = UCString("res/model/action/resetcircle.dat");
		StateMesh.Scale = uc3dxVector3(1.5f, 1.5f, 1.5f);

		StateMesh.PlayAction(0);
		StateMesh.Visible = 1;
		Physics.SetVisible(1);

		Fiber->SyncTick(25);
		Physics.SetVisible(0);
		Fiber->SyncTick(25);
		Physics.SetVisible(1);
		Fiber->SyncTick(25);
		Physics.SetVisible(0);
		Fiber->SyncTick(25);
		Physics.SetVisible(1);
		Fiber->SyncTick(25);
		Physics.SetVisible(0);
		Fiber->SyncTick(42);



		Physics.SetVisible(1);
		StateMesh.Visible = 0;

		if (Physics.IsSelf) Physics.ShowShadow(1);
	}

	ucVOID OnSMPower(UCObject* Sender, UCEventArgs* e)//无敌
	{
		UCTickFiberData* Fiber = (UCTickFiberData*)Sender;
		UCSMFiber* Data = (UCSMFiber*)e;

		Physics.SetMaterail(&Mtl);
		Physics.CarBody.SetMaterial(&Mtl);
		Physics.Wheel[0].SetMaterial(&Mtl);
		Physics.Wheel[1].SetMaterial(&Mtl);
		Physics.Wheel[2].SetMaterial(&Mtl);
		Physics.Wheel[3].SetMaterial(&Mtl);

		UCMesh				StateMesh;
		StateMesh.Parent = &Physics.CenterPoint;
		StateMesh.Name = UCString("res/model/action/wudi.dat");
		StateMesh.PlayAction(0);

		Physics.PlayAction(9);
		Physics.SetVentFireVisible(0);
		Physics.DriftType = 0;

		Fiber->SyncTick(450);

		//StateMesh.Visible = 0;
		Physics.PlayAction(0);
		Physics.SetVentFireVisible(1);


		Physics.SetBaseMaterail();

		//gCarSound.SetMusicVolume(1.0);
	}
	ucVOID OnSMChengMo(UCObject* Sender, UCEventArgs* e)//沉默
	{
		UCTickFiberData* Fiber = (UCTickFiberData*)Sender;
		UCSMFiber* Data = (UCSMFiber*)e;

		UCMesh temp;
		temp.Parent = &Physics.CenterBody;
		temp.Name = UCString("res/model/action/chengmo.dat");
		temp.PlayAction(0);
		temp.Scale = uc3dxVector3(8.0f, 8.0f, 8.0f);
		temp.Visible = 1;

		if (Physics.Profession == 12)
			Fiber->SyncTick(167);
		else
			Fiber->SyncTick(333);

		if (Physics.IsSelf)
		{
			for (ucINT i = 0; i < 3; i++)
				ChengMoPic[i].Visible = 0;
		}
		temp.Visible = 0;
	}
	ucVOID OnSMQusan(UCObject* Sender, UCEventArgs* e)//免疫
	{
		UCTickFiberData* Fiber = (UCTickFiberData*)Sender;
		UCSMFiber* Data = (UCSMFiber*)e;

		UCMesh temp;
		temp.Parent = &Physics.CenterBody;
		temp.Name = UCString("res/model/action/qusan.dat");
		temp.PlayAction(0);
		temp.Scale = uc3dxVector3(8.0f, 8.0f, 8.0f);
		temp.Visible = 1;
		Fiber->SyncTick(167);
		temp.Visible = 0;
	}
	ucVOID OnSMMianYi(UCObject* Sender, UCEventArgs* e)//免疫
	{
		UCTickFiberData* Fiber = (UCTickFiberData*)Sender;
		UCSMFiber* Data = (UCSMFiber*)e;

		UCMesh temp;
		temp.Parent = &Physics.CenterBody;
		temp.Name = UCString("res/model/action/mianyi.dat");
		temp.PlayAction(0);
		temp.Scale = uc3dxVector3(8.0f, 8.0f, 8.0f);
		temp.Visible = 1;


		Physics.SetMaterail(&Mtl1);
		Physics.HumanFace.SetMaterial(&Physics.Material);
		//Physics.HumanBody.SetMaterial(&Mtl1);		
		//Physics.HumanHair.SetMaterial(&Mtl1);
		//Physics.Licence.SetMaterial(&Mtl1);
		//Physics.Ball.SetMaterial(&Mtl1);

		Physics.CarBody.SetMaterial(&Mtl1);
		//Physics.CarVent.SetMaterial(&Mtl1);

		Physics.Wheel[0].SetMaterial(&Mtl1);
		Physics.Wheel[1].SetMaterial(&Mtl1);
		Physics.Wheel[2].SetMaterial(&Mtl1);
		Physics.Wheel[3].SetMaterial(&Mtl1);

		Fiber->SyncTick(420);
		temp.Visible = 0;


		Physics.SetBaseMaterail();

	}
	ucVOID  OnSMHuimo(UCObject* Sender, UCEventArgs* e)//回魔
	{
		UCTickFiberData* Fiber = (UCTickFiberData*)Sender;
		UCSMFiber* Data = (UCSMFiber*)e;

		UCMesh temp;
		temp.Parent = &Physics.CenterBody;
		temp.Name = UCString("res/model/action/jiamo.dat");
		temp.PlayAction(0);


		temp.Scale = uc3dxVector3(3.0f, 3.0f, 3.0f);
		//temp.Rot.y =3.14f;
		temp.Visible = 1;
		Fiber->SyncTick(333);

	}
	ucVOID  OnSMQiangHua(UCObject* Sender, UCEventArgs* e)//强化
	{
		UCTickFiberData* Fiber = (UCTickFiberData*)Sender;
		UCSMFiber* Data = (UCSMFiber*)e;

		UCMesh temp;
		temp.Parent = &Physics.CenterBody;
		temp.Name = UCString("res/model/action/qianghua.dat");
		temp.PlayAction(0);
		temp.Scale = uc3dxVector3(3.0f, 3.0f, 3.0f);
		temp.Visible = 1;
		Fiber->SyncTick(250);
		temp.Visible = 0;

	}
	ucVOID OnSMScaleDown(UCObject* Sender, UCEventArgs* e)//缩小
	{
		UCTickFiberData* Fiber = (UCTickFiberData*)Sender;
		UCSMFiber* Data = (UCSMFiber*)e;


		Physics.PlayAction(8);
		//if (Physics.IsSelf)		gCarSound.PlaySound(14);

		UCMesh	temp;
		temp.Parent = &Physics.CenterPoint;
		temp.Name = UCString("res/model/action/scaledown.dat");
		temp.PlayAction(0);
		temp.Visible = 1;	temp.Scale = uc3dxVector3(6.0f, 6.0f, 6.0f);

		ucFLOAT fYBackup = Physics.CenterPoint.Pos.y.Value;
		ucFLOAT fY = fYBackup;
		for (ucINT i = 0; i < 50 && Fiber->IsValid(); i++)
		{
			if (i % 2 == 0)
			{
				Physics.CenterPoint.Scale = uc3dxVector3(0.6f, 0.6f, 0.6f);
				Physics.CenterPoint.Pos.y = fY - 7.5f;
			}
			else
			{
				Physics.CenterPoint.Scale = uc3dxVector3(0.8f, 0.8f, 0.8f);
				Physics.CenterPoint.Pos.y = fY - 9.0f;
			}
			Fiber->SyncTick(1);
		}
		for (ucINT i = 0; i < 20 && Fiber->IsValid(); i++)
		{
			Physics.CenterPoint.Scale = Physics.CenterPoint.Scale.Value() - uc3dxVector3(0.02f, 0.02f, 0.02f);
			fY -= 0.75f;
			Physics.CenterPoint.Pos.y = fY;
			Fiber->SyncTick(1);
		}

		Physics.Center.LinearVel = Physics.CenterBody.GetWay_Z() * 20.0f;
		Fiber->SyncTick(240);

		for (ucINT i = 0; i < 20 && Fiber->IsValid(); i++)
		{
			fY += 0.75f;
			Physics.CenterPoint.Pos.y = fY;
			Physics.CenterPoint.Scale = Physics.CenterPoint.Scale.Value() + uc3dxVector3(0.02f, 0.02f, 0.02f);
			Fiber->SyncTick(1);
		}

		temp.Visible = 0;
		Physics.PlayAction(0);
		Physics.CenterPoint.Scale = uc3dxVector3(1.0f, 1.0f, 1.0f);
		Physics.CenterPoint.Pos.y = 0.0f;
	}
	ucVOID OnSMPaintPat(UCObject* Sender, UCEventArgs* e)//口水
	{
		UCTickFiberData* Fiber = (UCTickFiberData*)Sender;
		UCSMFiber* Data = (UCSMFiber*)e;

		ucINT a = 0, b = 0, c = 0, a2 = 0, b2 = 0, c2 = 0, a3 = 0, b3 = 0, c3 = 0;
		a = PaintBlack.Picture.Scale_V.Value;
		b = PaintBlack.Size.cy.Value;
		c = PaintBlack.Picture.Color.Value >> 24;



		a2 = PaintBlack2.Picture.Scale_V.Value;
		b2 = PaintBlack2.Size.cy.Value;
		c2 = PaintBlack2.Picture.Color.Value >> 24;

		a3 = PaintBlack3.Picture.Scale_V.Value;
		b3 = PaintBlack3.Size.cy.Value;
		c3 = PaintBlack3.Picture.Color.Value >> 24;


		ucINT Tempa = randint(7, 22);
		if (Physics.IsSelf)
		{
			ucINT temp = randint(0, 9);

			PaintBlack.Location = UCPoint(50 + Tempa * 24, 20 + temp * 20);
			PaintBlack.Visible = 1;

		}


		Fiber->SyncTick(30);

		Physics.SetMaterail(&MtlBlack);




		if (Physics.IsSelf)
		{
			ucINT temp1 = randint(0, 9);
			Tempa = randint(7, 22);
			PaintBlack2.Location = UCPoint(50 + Tempa * 24, 20 + temp1 * 20);
			if (Physics.IsSelf) PaintBlack2.Visible = 1;

		}


		Fiber->SyncTick(30);

		if (Physics.IsSelf)
		{
			ucINT temp2 = randint(0, 9);
			Tempa = randint(7, 22);
			PaintBlack3.Location = UCPoint(50 + Tempa * 24, 20 + temp2 * 20);
			if (Physics.IsSelf) PaintBlack3.Visible = 1;

		}

		Fiber->SyncTick(80);

		ucDWORD Alpha = 0xFF;
		for (ucINT i = 0; i < 45 && Fiber->IsValid(); i++)
		{
			Alpha -= (i / 10) + 1;
			if (Physics.IsSelf)
			{
				PaintBlack.Picture.Scale_V += 0.05f;
				PaintBlack.Size.cy += 13;
				PaintBlack.Location.y += i / 10 + 7;

				PaintBlack2.Picture.Scale_V += 0.05f;
				PaintBlack2.Size.cy += 13;
				PaintBlack2.Location.y += i / 10 + 7;

				PaintBlack3.Picture.Scale_V += 0.05f;
				PaintBlack3.Size.cy += 13;
				PaintBlack3.Location.y += i / 10 + 7;

				PaintBlack.Picture.SetAlpha(Alpha);
				PaintBlack2.Picture.SetAlpha(Alpha);
				PaintBlack3.Picture.SetAlpha(Alpha);
			}

			Fiber->SyncTick(8);
		}

		Physics.SetBaseMaterail();



		if (Physics.IsSelf)
		{
			PaintBlack.Visible = 0;
			PaintBlack.Picture.Scale_V.Value = a;
			PaintBlack.Size.cy.Value = b;
			PaintBlack.Picture.SetAlpha(c);


			PaintBlack2.Visible = 0;
			PaintBlack2.Picture.Scale_V.Value = a2;
			PaintBlack2.Size.cy.Value = b2;
			PaintBlack2.Picture.SetAlpha(c2);

			PaintBlack3.Visible = 0;
			PaintBlack3.Picture.Scale_V.Value = a3;
			PaintBlack3.Size.cy.Value = b3;
			PaintBlack3.Picture.SetAlpha(c3);
		}

	}
	ucVOID OnSMShineShake(UCObject* Sender, UCEventArgs* e)//闪光
	{
		UCTickFiberData* Fiber = (UCTickFiberData*)Sender;
		UCSMFiber* Data = (UCSMFiber*)e;

		if (Physics.IsSelf)
		{
			White.Alpha = 145;

			White.Visible = 1;
		}
		if (Physics.IsSelf) White.Alpha = 255;
		Fiber->SyncTick(100);
		for (ucINT i = 0; i < 100 && Fiber->IsValid(); i++)
		{
			if (Physics.IsSelf) White.Alpha -= 2;

			Fiber->SyncTick(1);
		}
		if (Physics.IsSelf) White.Visible = 0;
	}
	ucVOID OnSMPanPat(UCObject* Sender, UCEventArgs* e)//厨师拍打
	{
		UCTickFiberData* Fiber = (UCTickFiberData*)Sender;
		UCSMFiber* Data = (UCSMFiber*)e;

		Physics.PlayAction(8);

		//if (Physics.IsSelf)		gCarSound.PlaySound(14);

		UCMesh	temp;
		temp.Parent = &Physics.CenterBody;
		temp.Name = UCString("res/model/action/pan.dat");
		//temp.PlayAction(0);
		temp.Visible = 1;	temp.Scale = uc3dxVector3(2.0f, 2.0f, 2.0f);
		temp.Pos.y = 100.0f;

		for (ucINT i = 0; i < 17 && Fiber->IsValid(); i++)//欲拍打
		{
			if (i < 5)
			{
				temp.Rot.x += 0.3f;
				temp.Pos.y += 3.0f;
				Fiber->SyncTick(4);
			}
			else if (i == 5)
			{
				Fiber->SyncTick(16);

			}
			else if (i < 11)
			{
				temp.Rot.x -= 0.3f;
				temp.Pos.y -= 3.0f;
				Fiber->SyncTick(5);
			}
			else if (i == 11)
			{
				Fiber->SyncTick(16);

			}
			else if (i < 17)
			{
				temp.Rot.x += 0.3f;//0.5
				temp.Pos.y += 4.0f;
				Fiber->SyncTick(4);

			}


		}
		Fiber->SyncTick(25);


		//if (Physics.IsSelf)		gCarSound.PlaySound(28);

		for (ucINT i = 0; i < 4 && Fiber->IsValid(); i++)//拍打
		{
			temp.Pos.y -= 10.53f * (4 - i);
			temp.Rot.x -= 0.65f;//0.120
			Physics.CenterPoint.Scale.y = Physics.CenterPoint.Scale.y.Value - 0.08f * (4 - i);
			Physics.Center.LinearVel = uc3dxVector3(0.0f, 0.0f, 0.0f);
			Fiber->SyncTick(1);
		}
		Physics.Center.LinearVel = uc3dxVector3(0.0f, 0.0f, 0.0f);
		Fiber->SyncTick(80);
		temp.Visible = 0;
		Physics.Center.LinearVel = Physics.CenterBody.GetWay_Z() * 20.0f;
		Fiber->SyncTick(150);


		for (ucINT i = 0; i < 15 && Fiber->IsValid(); i++)//恢复
		{
			Physics.CenterPoint.Scale.y += 0.0045f * (15 - i);

			Fiber->SyncTick(1);
		}

		temp.Visible = 0;
		Physics.PlayAction(0);
		Physics.CenterPoint.Scale = uc3dxVector3(1.0f, 1.0f, 1.0f);
		//Physics.CenterPoint.Pos.y = 8.0f;
	}
	ucVOID OnSMRubbishSpeed(UCObject* Sender, UCEventArgs* e)//垃圾车OnRubbish
	{
		UCTickFiberData* Fiber = (UCTickFiberData*)Sender;
		UCSMFiber* Data = (UCSMFiber*)e;

		ucINT RubbishTime = 5;

		UCMesh				StateMesh;
		StateMesh.Parent = &Physics.CenterPoint;
		StateMesh.Name = UCString("res/model/action/rubbishrecover.dat");
		StateMesh.PlayAction(0);
		StateMesh.Scale = uc3dxVector3(4.0f, 4.0f, 4.0f);
		StateMesh.Visible = 1;

		for (ucINT i = 0; i < RubbishTime; i++)
		{
			Fiber->SyncTick(100);
		}

		StateMesh.Visible = 0;

		RubbishTime = 5;
	}

	ucVOID OnSMSkid(UCObject* Sender, UCEventArgs* e)//踩在香皂上
	{
		UCTickFiberData* Fiber = (UCTickFiberData*)Sender;
		UCSMFiber* Data = (UCSMFiber*)e;

		uc3dxVector3	vLinearVel;
		ucFLOAT len = 0.0f;
		//if (Physics.IsSelf) gCarSound.PlaySound(24);

		{
			vLinearVel = Physics.Center.LinearVel.Value();
			len = vLinearVel.Length();
			vLinearVel.Normalize();
		}

		ucINT	Time0 = 60;
		ucFLOAT v0 = 6.2832f / 30.0f;
		for (ucINT i = 0; i < Time0 && Fiber->IsValid(); i++)
		{
			Physics.CenterPoint.Rot.y = Physics.CenterPoint.Rot.y.Value + v0;
			Fiber->SyncTick(1);
		}

		{
			Physics.CenterPoint.Rot.y = 3.1416f;
			len = len * 0.3f;
			Physics.Center.LinearVel = vLinearVel * len;
		}
	}
	ucVOID OnSMDizzy(UCObject* Sender, UCEventArgs* e)//撞在假箱子上
	{
		UCTickFiberData* Fiber = (UCTickFiberData*)Sender;
		UCSMFiber* Data = (UCSMFiber*)e;

		ucFLOAT			Length = 0.0f;
		uc3dxVector3	vLinearVel;
		ucFLOAT Dir = 0.0f;
		//if (Physics.IsSelf) gCarSound.PlaySound(6);

		if (Physics.IsSelf)
		{
			Physics.KeyDrift.Cancel();
			Physics.DriftType = 0;
		}
		{
			vLinearVel = Physics.Center.LinearVel.Value();
			Length = vLinearVel.Length();
			vLinearVel.Normalize();

			Dir = Physics.CenterBody.GetWay_Z() | vLinearVel;
			if (Dir < 0.0f)	Dir = 1.0f;	else Dir = -1.0f;

			Physics.Center.LinearVel = vLinearVel * (Length * 0.5f);
			Physics.Center.LinearVel.y = 120.0f;
			Physics.JumpState = JUMP_WAIT;
		}
		UCMesh				StateMesh;
		StateMesh.Parent = &Physics.CenterBody;
		StateMesh.Name = UCString("res/model/action/zhuangfan.dat");
		//StateMesh.Scale = uc3dxVector3(1.25f,1.25f,1.25f);
		StateMesh.PlayAction(0, 1.0f, 1);


		ucFLOAT v = 6.2832f / 60.0f;
		for (ucINT i = 0; i < 60 && Fiber->IsValid(); i++)
		{
			Physics.CenterPoint.Rot.x = Physics.CenterPoint.Rot.x.Value + v * Dir;
			Fiber->SyncTick(1);
		}

		{
			Physics.CenterPoint.Rot.x = 0.0f;
			Physics.Center.LinearVel = vLinearVel * 10.0f;
		}
		if (Physics.IsSelf) Physics.State.Set("倒霉后保护");
	}
	ucVOID OnSMBat(UCObject* Sender, UCEventArgs* e)
	{
		UCTickFiberData* Fiber = (UCTickFiberData*)Sender;
		UCSMFiber* Data = (UCSMFiber*)e;

		if (Physics.IsSelf)
		{
			Physics.KeyDrift.Cancel();
			Physics.DriftType = 0;
		}
		UCMesh				StateMesh;
		StateMesh.Parent = &Physics.CenterPoint;
		{
			if (GroupID == 11)
				StateMesh.Name = UCString("res/model/action/followboomblue.dat");
			else if (GroupID == 12)
				StateMesh.Name = UCString("res/model/action/followboomred.dat");
			else
				StateMesh.Name = UCString("res/model/action/followboom.dat");

			StateMesh.PlayAction(0);
			StateMesh.Visible = 1;	StateMesh.Scale = uc3dxVector3(5.0f, 5.0f, 5.0f);
			Physics.Center.LinearVel = uc3dxVector3(0.0f, 0.0f, 0.0f);
			StateMesh.Pos.y = 20.0f;
		}
		Physics.Center.GravityMode = 0;

		ucFLOAT Speed = 5.5f;
		for (ucINT i = 0; i < 25 && Fiber->IsValid(); i++)
		{
			Speed = Speed - 0.24f;
			Physics.CenterPoint.Pos.y = Physics.CenterPoint.Pos.y.Value + Speed;
			Physics.CenterPoint.Rot.x = Physics.CenterPoint.Rot.x.Value + 0.01f;

			StateMesh.Pos.y += Speed / 3.0f;
			Fiber->SyncTick(1);
		}
		Physics.PlayAction(8);
		ucFLOAT v = 0.01f;
		ucFLOAT dir = 1.0f;
		ucFLOAT p0 = Physics.CenterPoint.Rot.x.Value;
		ucFLOAT p1 = Physics.CenterPoint.Rot.z.Value;
		/*
		for (ucINT i=0;i<125&&Fiber->IsValid();i++)
		{
			Physics.CenterPoint.Rot.x = Physics.CenterPoint.Rot.x.Value + v*dir;
			Physics.CenterPoint.Rot.z = Physics.CenterPoint.Rot.z.Value + v*dir;
			if (Physics.CenterPoint.Rot.x.Value>p0+0.4f)
			{
				dir =-1.0f;
			}
			else if (Physics.CenterPoint.Rot.x.Value<p0-0.4f)
			{
				dir =1.0f;
			}

			Fiber->Delay(100);
		}
		*/

		Fiber->SyncTick(250);

		StateMesh.Visible = 0;
		Physics.PlayAction(0);

		for (ucINT i = 0; i < 25 && Fiber->IsValid(); i++)
		{
			Speed = Speed - 0.12f;
			Physics.CenterPoint.Pos.y = Physics.CenterPoint.Pos.y.Value + Speed;
			Physics.CenterPoint.Rot.x = Physics.CenterPoint.Rot.x.Value - 0.015f;
			Fiber->SyncTick(1);
		}

		Physics.Center.GravityMode = 1;
		Physics.CenterPoint.Pos.y = 0.0f;
		Physics.CenterPoint.Rot.x = 0.0f;
		Physics.CenterPoint.Rot.z = 0.0f;
		StateMesh.Pos.y = 0.0f;
		StateJumpDust.StopAll();
		StateJumpDust.StartNew(0);

		if (!KeyState.KEYIsUp())
			Physics.DriftUpTime = 20;
	}

	ucVOID OnSMFire(UCObject* Sender, UCEventArgs* e)//被跟踪弹打倒
	{
		UCTickFiberData* Fiber = (UCTickFiberData*)Sender;
		UCSMFiber* Data = (UCSMFiber*)e;

		if (Physics.IsSelf)
		{
			Physics.KeyDrift.Cancel();
			Physics.DriftType = 0;
		}
		UCMesh Mesh0;
		Mesh0.Parent = Physics.SceneCtrl->Scene;
		Mesh0.Name = UCString("res/model/action/bigfireball.dat");
		Mesh0.Scale = uc3dxVector3(5.0f, 5.0f, 5.0f);
		Mesh0.Pos.z = Physics.Center.Pos.z.Value;
		Mesh0.Pos.x = Physics.Center.Pos.x.Value;
		Mesh0.Pos.y = Physics.Center.Pos.y.Value;
		Mesh0.PlayAction(0);


		UCMesh				StateMesh;
		StateMesh.Parent = &Physics.CenterPoint;
		StateMesh.Name = UCString("res/model/action/fire.dat");
		StateMesh.PlayAction(0);
		StateMesh.Visible = 1;
		Physics.Center.LinearVel = uc3dxVector3(0.0f, 0.0f, 0.0f);

		ucFLOAT Speed = 9.0f;
		ucFLOAT v1 = 6.2832f / 30.0f;
		ucFLOAT v2 = 6.2832f / 60.0f;
		ucFLOAT d1 = 0.0f, d2 = 0.0f;
		if (Physics.CenterBody.Camera) { d1 = Physics.CenterBody.Camera->Pos.y.Value; d2 = Physics.CenterBody.Camera->Rot.x.Value; }

		//if (Physics.IsSelf) gCarSound.PlaySound(22);

		for (ucINT i = 0; i < 30 && Fiber->IsValid(); i++)
		{

			//Physics.CenterPoint.Rot.x = Physics.CenterPoint.Rot.x.Value + v1*1.0f;
			if (Physics.CenterBody.Camera)
			{
				Physics.CenterBody.Camera->Pos.y = Physics.CenterBody.Camera->Pos.y.Value + 3.0f;//-2.0f,-0.06f
				Physics.CenterBody.Camera->Rot.x = Physics.CenterBody.Camera->Rot.x.Value - 0.06f;
			}

			Speed = Speed - 0.3f;
			Physics.CenterPoint.Pos.y = Physics.CenterPoint.Pos.y.Value + Speed;
			Physics.CenterPoint.Rot.x = Physics.CenterPoint.Rot.x.Value + v1 * 1.0f;


			Fiber->SyncTick(1);
		}
		Mesh0.Visible = 0;
		for (ucINT i = 0; i < 30 && Fiber->IsValid(); i++)
		{
			Physics.CenterPoint.Rot.x = Physics.CenterPoint.Rot.x.Value + v2 * 1.0f;
			Fiber->SyncTick(1);
		}
		for (ucINT i = 0; i < 30 && Fiber->IsValid(); i++)
		{
			Speed = Speed - 0.3f;
			Physics.CenterPoint.Rot.x = Physics.CenterPoint.Rot.x.Value + v2 * 1.0f;
			Physics.CenterPoint.Pos.y = Physics.CenterPoint.Pos.y.Value + Speed;
			//if (i>20)
			{
				if (Physics.CenterBody.Camera)
				{
					Physics.CenterBody.Camera->Pos.y = Physics.CenterBody.Camera->Pos.y.Value - 3.0f;//+2.0f,+0.06f
					Physics.CenterBody.Camera->Rot.x = Physics.CenterBody.Camera->Rot.x.Value + 0.06f;
				}
			}

			Fiber->SyncTick(1);
		}
		StateMesh.Visible = 0;

		for (ucINT i = 0; i < 24 && Fiber->IsValid(); i++)
		{
			if (i / 6 == 0 || i / 6 == 3)
			{
				Physics.CenterPoint.Rot.z = Physics.CenterPoint.Rot.z.Value + 0.05f;
			}
			else  if (i / 6 == 1 || i / 6 == 2)
			{
				Physics.CenterPoint.Rot.z = Physics.CenterPoint.Rot.z.Value - 0.05f;

			}
			if (i < 12)
			{
				Physics.CenterPoint.Pos.y = Physics.CenterPoint.Pos.y.Value + 1.0f;
			}
			else
			{
				Physics.CenterPoint.Pos.y = Physics.CenterPoint.Pos.y.Value - 1.0f;

			}


			Fiber->SyncTick(1);

		}


		if (Physics.CenterBody.Camera)
		{
			Physics.CenterBody.Camera->Pos.y = d1;
			Physics.CenterBody.Camera->Rot.x = d2;
		}

		Physics.CenterPoint.Pos.y = 0.0f;
		Physics.CenterPoint.Rot.x = 0.0f;
		Physics.CenterPoint.Rot.z = 0.0f;
		StateMesh.Visible = 0;

		if (!KeyState.KEYIsUp())
			Physics.DriftUpTime = 20;
	}

	ucVOID OnSMHigh(UCObject* Sender, UCEventArgs* e)//抬高 水泡击中
	{
		UCTickFiberData* Fiber = (UCTickFiberData*)Sender;
		UCSMFiber* Data = (UCSMFiber*)e;

		if (Physics.IsSelf)
		{
			Physics.KeyDrift.Cancel();
			Physics.DriftType = 0;
		}
		UCMesh				StateMesh;
		StateMesh.Parent = &Physics.CenterPoint;
		//if(Physics.IsSelf) gCarSound.PlaySound(18);
		{
			StateMesh.Name = UCString("res/model/action/smallwaterball.dat");
			StateMesh.PlayAction(0);
			StateMesh.Visible = 1;	StateMesh.Scale = uc3dxVector3(10.0f, 10.0f, 10.0f);//StateMesh.RenderMode = 1;
			Physics.Center.LinearVel = uc3dxVector3(0.0f, 0.0f, 0.0f);
		}
		Physics.Center.GravityMode = 0;

		Physics.Center.LinearVel = uc3dxVector3(0.0f, 0.0f, 0.0f);

		ucFLOAT Speed = 6.0f;
		for (ucINT i = 0; i < 25 && Fiber->IsValid(); i++)
		{
			Speed = Speed - 0.24f;
			Physics.CenterPoint.Pos.y = Physics.CenterPoint.Pos.y.Value + Speed;
			Fiber->SyncTick(1);
		}

		ucFLOAT v = 6.2832f / 200.0f;
		for (ucINT i = 0; i < 200 && Fiber->IsValid(); i++)
		{
			Physics.CenterPoint.Rot.x = Physics.CenterPoint.Rot.x.Value + v;
			Physics.CenterPoint.Rot.z = Physics.CenterPoint.Rot.z.Value + v;
			Fiber->SyncTick(1);
		}


		for (ucINT i = 0; i < 25 && Fiber->IsValid(); i++)
		{
			Speed = Speed - 0.12f;
			Physics.CenterPoint.Pos.y = Physics.CenterPoint.Pos.y.Value + Speed;
			Fiber->SyncTick(1);
		}

		Physics.CenterPoint.Pos.y = 0.0f;
		Physics.CenterPoint.Rot.x = 0.0f;
		Physics.CenterPoint.Rot.z = 0.0f;
		StateMesh.Visible = 0;
		Physics.Center.GravityMode = 1;

		if (!KeyState.KEYIsUp())
			Physics.DriftUpTime = 20;

	}

	ucVOID OnSMFirst(UCObject* Sender, UCEventArgs* e)//第一名抬高 水泡击中
	{
		UCTickFiberData* Fiber = (UCTickFiberData*)Sender;
		UCSMFiber* Data = (UCSMFiber*)e;

		UCMesh				StateMesh;
		StateMesh.Parent = &Physics.CenterPoint;

		//if (Physics.IsSelf)	gCarSound.PlaySound(18);
		if (Physics.IsSelf)
		{
			Physics.KeyDrift.Cancel();
			Physics.DriftType = 0;
		}
		{

			StateMesh.Name = UCString("res/model/action/smallwaterball.dat");
			StateMesh.PlayAction(0);
			StateMesh.Visible = 1;	StateMesh.Scale = uc3dxVector3(10.0f, 10.0f, 10.0f);//StateMesh.RenderMode = 1;
			Physics.Center.LinearVel = uc3dxVector3(0.0f, 0.0f, 0.0f);
		}
		Physics.Center.GravityMode = 0;

		Physics.Center.LinearVel = uc3dxVector3(0.0f, 0.0f, 0.0f);

		ucFLOAT Speed = 6.0f;
		for (ucINT i = 0; i < 25 && Fiber->IsValid(); i++)
		{
			Speed = Speed - 0.24f;
			Physics.CenterPoint.Pos.y = Physics.CenterPoint.Pos.y.Value + Speed;
			Fiber->SyncTick(1);
		}

		ucFLOAT v = 6.2832f / 200.0f;
		for (ucINT i = 0; i < 200 && Fiber->IsValid(); i++)
		{
			Physics.CenterPoint.Rot.x = Physics.CenterPoint.Rot.x.Value + v;
			Physics.CenterPoint.Rot.z = Physics.CenterPoint.Rot.z.Value + v;
			Fiber->SyncTick(1);
		}


		for (ucINT i = 0; i < 25 && Fiber->IsValid(); i++)
		{
			Speed = Speed - 0.12f;
			Physics.CenterPoint.Pos.y = Physics.CenterPoint.Pos.y.Value + Speed;
			Fiber->SyncTick(1);
		}


		Physics.CenterPoint.Pos.y = 0.0f;
		Physics.CenterPoint.Rot.x = 0.0f;
		Physics.CenterPoint.Rot.z = 0.0f;
		StateMesh.Visible = 0;

		Physics.Center.GravityMode = 1;

		if (!KeyState.KEYIsUp())
			Physics.DriftUpTime = 20;
	}

	ucVOID OnSMQualm(UCObject* Sender, UCEventArgs* e)//混乱，方向反向
	{
		UCTickFiberData* Fiber = (UCTickFiberData*)Sender;
		UCSMFiber* Data = (UCSMFiber*)e;

		UCMesh QualmMesh;
		QualmMesh.Name = UCString("res/model/action/hunluan.dat");
		QualmMesh.Parent = &Physics.CenterPoint;
		QualmMesh.Scale = uc3dxVector3(3.0f, 3.0f, 3.0f);
		//QualmMesh.Pos.y =-6.0f;
		QualmMesh.PlayAction(0);

		{
			Physics.PlayAction(11);
		}
		//if (Physics.IsSelf) gCarSound.PlaySound(16);


		Physics.SetMaterail(&MtlQualm);
		Physics.HumanFace.SetMaterial(&Physics.Material);
		//Physics.HumanBody.SetMaterial(&MtlQualm);
		//Physics.Licence.SetMaterial(&MtlQualm);
		//Physics.Ball.SetMaterial(&MtlQualm);
		////Physics.HumanFace.SetMaterial(&MtlQualm);
		//Physics.HumanHair.SetMaterial(&MtlQualm);
		Physics.CarBody.SetMaterial(&MtlQualm);
		//Physics.CarVent.SetMaterial(&MtlQualm);		
		Physics.Wheel[0].SetMaterial(&MtlQualm);
		Physics.Wheel[1].SetMaterial(&MtlQualm);
		Physics.Wheel[2].SetMaterial(&MtlQualm);
		Physics.Wheel[3].SetMaterial(&MtlQualm);

		QualmMesh.Visible = 1;
		Physics.Qualming = 1;
		Fiber->SyncTick(333);

		QualmMesh.Visible = 0;
		Physics.Qualming = 0;

		QualmMesh.Visible = 0;

		Physics.PlayAction(0);

		Physics.SetBaseMaterail();

	}

	ucVOID OnSMBlind(UCObject* Sender, UCEventArgs* e)
	{
		UCTickFiberData* Fiber = (UCTickFiberData*)Sender;
		UCSMFiber* Data = (UCSMFiber*)e;

		UCMesh				StateMesh;
		StateMesh.Parent = &Physics.CenterPoint;
		{
			StateMesh.Name = UCString("res/model/action/blindrog.dat");
			StateMesh.PlayAction(0);
			StateMesh.Visible = 1;	StateMesh.Scale = uc3dxVector3(5.0f, 5.0f, 5.0f);
			Fiber->SyncTick(416);
		}

		{
			StateMesh.Visible = 0;
		}
	}

	ucVOID OnSMSideRoll(UCObject* Sender, UCEventArgs* e)
	{
		UCTickFiberData* Fiber = (UCTickFiberData*)Sender;
		UCSMFiber* Data = (UCSMFiber*)e;

		StateDir = Data->ParamINT;

		if (Physics.IsSelf)
		{
			Physics.KeyDrift.Cancel();
			Physics.DriftType = 0;
		}
		UCMesh				StateMesh;
		StateMesh.Parent = &Physics.CenterPoint;
		ucFLOAT DirH = 0.0f;;
		{
			if (Data->ParamINT > 0)	DirH = 1.0f;	else	DirH = -1.0f;

			ucFLOAT Dir = Physics.CenterBody.GetWay_Z() | Physics.Center.LinearVel.Value();
			if (Dir < 0.0f)	Dir = -20.0f;	else	Dir = 20.0f;

			Physics.Center.LinearVel = Physics.CenterBody.GetWay_Z() * Dir + Physics.CenterBody.GetWay_X() * DirH * 30.0f;
			Physics.Center.LinearVel.y = 120.0f;
			Physics.JumpState = JUMP_WAIT;
		}

		ucFLOAT v = 6.2832f / 60.0f;
		for (ucINT i = 0; i < 60 && Fiber->IsValid(); i++)
		{
			Physics.CenterPoint.Rot.z = Physics.CenterPoint.Rot.z.Value + v * DirH;
			Fiber->SyncTick(1);
		}

		{
			Physics.CenterPoint.Rot.z = 0.0f;
		}
		if (Physics.IsSelf) Physics.State.Set("倒霉后保护");
	}

	ucVOID OnSMBlank(UCObject* Sender, UCEventArgs* e)
	{
		UCTickFiberData* Fiber = (UCTickFiberData*)Sender;
		UCSMFiber* Data = (UCSMFiber*)e;

		Fiber->SyncTick(416);
	}

	ucVOID OnSMSideFlip(UCObject* Sender, UCEventArgs* e)
	{
		UCTickFiberData* Fiber = (UCTickFiberData*)Sender;
		UCSMFiber* Data = (UCSMFiber*)e;

		ucINT Param = Data->ParamINT;
		ucFLOAT Dis = 0.0f;
		ucFLOAT Der = 0.0f;
		uc3dxVector3 Vel;
		{
			if (Param > 0)
			{
				Dis = 12.0f * Param;
				Der = +80.0f;
			}
			if (Param < 0)
			{
				Dis = 12.0f * Param;
				Der = -80.0f;
			}

			Vel = Physics.Center.LinearVel.Value();
		}
		ucFLOAT v = 6.2832f / 250.0f;
		for (ucINT i = 0; i < 10 && Fiber->IsValid(); i++)
		{
			Physics.CenterPoint.Rot.y = Physics.CenterPoint.Rot.y.Value + v;
			Physics.Center.LinearVel = Physics.CenterBody.GetWay_X() * Der;
			Fiber->SyncTick(1);
		}
		Physics.Center.LinearVel = Vel + Physics.CenterBody.GetWay_X() * Dis;
		for (ucINT i = 0; i < 20 && Fiber->IsValid(); i++)
		{
			Physics.CenterPoint.Rot.y = Physics.CenterPoint.Rot.y.Value - v;
			Fiber->SyncTick(1);
		}
		for (ucINT i = 0; i < 10 && Fiber->IsValid(); i++)
		{
			Physics.CenterPoint.Rot.y = Physics.CenterPoint.Rot.y.Value + v;
			Fiber->SyncTick(1);
		}

		{
			Physics.CenterPoint.Rot.y = 3.1416f;
		}
	}

	ucVOID OnSMSideBack(UCObject* Sender, UCEventArgs* e)
	{
		UCTickFiberData* Fiber = (UCTickFiberData*)Sender;
		UCSMFiber* Data = (UCSMFiber*)e;

		uc3dxVector3 Dir = -Physics.CenterBody.GetWay_Z();
		ucFLOAT Spd = 120.0f;
		for (ucINT i = 0; i < 60 && Fiber->IsValid(); i++)
		{
			Spd -= 1.0f;
			Physics.Center.LinearVel = Dir * Spd;
			Fiber->SyncTick(1);
		}
	}

	ucVOID OnSMClearEye(UCObject* Sender, UCEventArgs* e)
	{
		UCTickFiberData* Fiber = (UCTickFiberData*)Sender;
		UCSMFiber* Data = (UCSMFiber*)e;

		UCMesh				StateMesh;
		StateMesh.Parent = &Physics.CenterPoint;
		StateMesh.Name = UCString("res/model/action/glass.dat");
		StateMesh.PlayAction(0);
		StateMesh.SetActionSpeed(2.0f);
		StateMesh.Visible = 1;

		Fiber->SyncTick(125);
		Physics.SetActionFile();
	}

	ucVOID OnSMNeedle(UCObject* Sender, UCEventArgs* e)
	{
		UCTickFiberData* Fiber = (UCTickFiberData*)Sender;
		UCSMFiber* Data = (UCSMFiber*)e;

		UCMesh				StateMesh;
		StateMesh.Parent = &Physics.CenterPoint;
		StateMesh.Name = UCString("res/model/action/needle.dat");
		StateMesh.PlayAction(0);
		Fiber->SyncTick(16);
		Physics.SetActionFile();
	}

	ucVOID OnSMPeoBox(UCObject* Sender, UCEventArgs* e)
	{
		UCTickFiberData* Fiber = (UCTickFiberData*)Sender;
		UCSMFiber* Data = (UCSMFiber*)e;

		UCMaterial			PinkMtl;
		PinkMtl.Name = UCString("res/shader/invinciblepink.material");

		/*Physics.HumanBody.SetMaterial(&PinkMtl);
		Physics.HumanFace.SetMaterial(&PinkMtl);
		Physics.HumanHair.SetMaterial(&PinkMtl);
		Physics.Licence.SetMaterial(&PinkMtl);
		Physics.Ball.SetMaterial(&PinkMtl);*/
		Physics.SetMaterail(&PinkMtl);

		Physics.CarBody.SetMaterial(&PinkMtl);
		//Physics.CarVent.SetMaterial(&PinkMtl);
		Physics.Wheel[0].SetMaterial(&PinkMtl);
		Physics.Wheel[1].SetMaterial(&PinkMtl);
		Physics.Wheel[2].SetMaterial(&PinkMtl);
		Physics.Wheel[3].SetMaterial(&PinkMtl);

		UCMesh				StateMesh;
		StateMesh.Parent = &Physics.CenterPoint;
		StateMesh.Name = UCString("res/model/action/dunpai.dat");
		StateMesh.PlayAction(0);
		StateMesh.SetActionSpeed(1.5f);

		//Physics.PlayAction(9);
		Physics.SetVentFireVisible(0);

		ucDWORD nTim = UCGetTime();
		if (Fiber->IsValid()) Fiber->SyncTick(200);
		ucINT a = UCGetTime() - nTim;
		ucTrace(UCString("dunpai：") + ITOS(a) + UCString("\r\n"));

		Physics.PlayAction(0);
		Physics.SetVentFireVisible(1);

		/*Physics.HumanBody.SetMaterial(&Physics.Material);
		Physics.HumanFace.SetMaterial(&Physics.Material);
		Physics.HumanHair.SetMaterial(&Physics.Material);
		Physics.Licence.SetMaterial(&Physics.Material);
		Physics.Ball.SetMaterial(&Physics.Material);*/

		Physics.SetBaseMaterail();

		Physics.SetActionFile();

	}
	ucVOID OnSMPengWuXiao(UCObject* Sender, UCEventArgs* e)
	{
		UCTickFiberData* Fiber = (UCTickFiberData*)Sender;
		if (Fiber->IsValid()) Fiber->SyncTick(333);
	}
	ucVOID OnSMBadProtect(UCObject* Sender, UCEventArgs* e)
	{
		UCTickFiberData* Fiber = (UCTickFiberData*)Sender;
		if (Fiber->IsValid()) Fiber->SyncTick(125);
	}
};



#endif
