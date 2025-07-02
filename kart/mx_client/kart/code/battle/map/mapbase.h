#ifndef _MAPBASE_
#define _MAPBASE_

#define MAX_USER		8
#define MAX_WHEEL		4
//赛车喷火
#define MAXVENTFIRE		6	//赛车尾气个数<=MAXVENTFIRE
#define BIGFIRE			1
#define SMALLFIRE		0

#define MAX_PACK		3

#define GROUND_NORMAL		0	//普通道路
#define GROUND_GRASS		1	//草地
#define	GROUND_MUD			2	//泥地
#define GROUND_JianSuYun	3	//减速云
#define GROUND_JiaSuZone	4	//加速带
#define GROUND_HUAPU		5	//花圃

#define YELLOW2			0xFFF3E309//普通系统公告
#define	BLACK			0xFF000000
#define	WHITE			0xFFFFFFFF
#define	BLUE			0xFF10CEDE
#define	RED				0xFFFF0000
#define	BLUE1			0xFF14F4FC
#define ROOMCAN			0xFF0802FD
#define ROOMNOT			0xFFF23738
#define YELLOW			0xFF00FF0C//普通系统公告
#define ROSERED			0xFFFBFF00//系统赠送道具
#define GRASSGREEN		0xFF20FDBC//私聊
#define SEABLUE			0xFFFF9C00//小喇叭
#define	BILLCOLOR		0xFF00DEFF//billboard断线通知

#include "../player/human.h"
#include "../item/item.h"

//微缩地图
class UCScaleMap
{
public:
	UCImage				ScaleMap;
	UCImage				ScaleMapUser[MAX_USER];

	ucFLOAT				ScaleMapGrid;				//缩放
	UCPoint				ScaleMapOffset;				//偏移
};

class UCKartMap
{
public:
	//玩家数据
	UCHuman*				Self;				//自己
	UCHuman*				Users[MAX_USER];	//用户

	//场景相关
	UCEArray<UCLandMine*>	Wells;				//井盖,香蕉,假道具箱
	UCEArray<UCLandOil*>	WellOils;			//加油站
	UCEArray<UCFlyItem*>	FlyItems;			//飞行道具
	UCEArray<UCPropBox*>	PropBoxs;			//真道具

	UCMaterial				Material;			//材质
	UCMaterial				MaterialScene;			//材质
	UCCameraSmooth			Camera;				//摄像头

	UCMesh					Scene;				//场景
	UCMeshScene				Ground;				//物理
	UCMeshScene				Reset;				//重置
	UCMesh					BanJiangTai;		//颁奖台
	UCMesh					Yanhua;				//颁奖台
	UCMesh					SkyBox;				//天空盒

	UCMeshRay				CameraRay;			//摄像头射线

	UCPortalManage			Manage;				//门径管理

	UCSceneCtl*				SceneCtl;			//背景控件
public:
	ucINT					MaxLoop;			//最大圈数

	ucFLOAT					CameraRayLength;	//摄像头射线长度
	ucINT					CameraRayHitValid;	//摄像头碰撞有效
	ucFLOAT					CameraRayMinDis;	
public:
	//UI相关
	UCScaleMap				ScaleMap;			//微缩地图
public:
	UCKartMap()
	{
		MaxLoop = 0;

		CameraRayLength = 0.0f;
		CameraRayHitValid = ucTRUE;
		CameraRayMinDis = 0.0f;

		Camera.Mode = 0x01 | 0x02 | 0x04;
		Camera.Total.y = 1.0f;
		Camera.Speed = 0.02f;

		Self = ucNULL;
		for (ucINT i = 0; i < MAX_USER; i++)
			Users[i] = ucNULL;
	}
	~UCKartMap()
	{
		for (ucINT i = 0; i < MAX_USER; i++)
		{
			if (Users[i])
				delete Users[i];
		}
		for (ucINT i = 0; i < Wells.GetSize(); i++)
			delete Wells.GetAt(i);
		for (ucINT i = 0; i < WellOils.GetSize(); i++)
			delete WellOils.GetAt(i);
		for (ucINT i = 0; i < PropBoxs.GetSize(); i++)
			delete PropBoxs.GetAt(i);
		for (ucINT i = 0; i < FlyItems.GetSize(); i++)
			delete FlyItems.GetAt(i);
	}
	ucINT ManageAble(UCHuman* Curr)
	{
		if (Curr == 0) return 0;
		return Curr == Self || Curr->Physics.FiberAutoDrive.IsRun();
	}

	ucVOID OnCameraRayCollide(UCObject*, UCEventArgs* e)
	{
		Self->Physics.CameraFixAble = 1;
		UCPContactArgs* Args = (UCPContactArgs*)e;

		UCMeshPhyBase* Target = Args->Mesh2;

		UCCamera* CameraSelf = Self->Physics.CenterBody.Camera;

		if (Target->Handle != 0) return;
		if (Target != 0 && Target != &Self->Physics.Center)
		{
			UCPContacts	Contacts(Args);
			if (Contacts.Size)
			{
				if (CameraSelf != 0)
				{
					for (ucINT i = 0; i < Contacts.Size; i++)
					{
						uc3dxVector3 vDis = Contacts.PointAt(i) - Self->Physics.CenterBody.Pos.Value();
						ucFLOAT Dis = vDis.Length();
						if (CameraRayMinDis > Dis)
						{
							CameraRayHitValid = 1;
							if (Target == &Ground)
								CameraRayHitValid = 0;
							CameraRayMinDis = Dis;
						}
					}
				}
				Contacts.Size = 0;
			}
		}
	}

	ucVOID CollideReset(UCPContactArgs* Args)
	{
		UCMeshPhyBase* Body = 0;
		//如果模型碰到了草地
		if (Args->Mesh1 == &Reset)	Body = Args->Mesh2;
		if (Args->Mesh2 == &Reset)	Body = Args->Mesh1;

		if (Body != 0 && Body->Handle != 0)
		{
			UCHuman* Human = (UCHuman*)Body->Handle;
			if (ManageAble(Human))
			{
				UCPContacts	Contacts(Args);
				if (Contacts.Size)
				{
					Human->Physics.Mode = GRD_RESET;
				}
			}
		}
	}

	ucVOID CollideUsers(UCPContactArgs* Args)
	{
		if (Args->Mesh1 != 0 && Args->Mesh2 != 0)
		{
			if (Args->Mesh1->Handle != 0 && Args->Mesh2->Handle != 0)
			{
				UCHuman* Human1 = (UCHuman*)Args->Mesh1->Handle;
				UCHuman* Human2 = (UCHuman*)Args->Mesh2->Handle;

				UCHuman* Human = 0;
				if (Human1 == Self)	Human = Human2;
				if (Human2 == Self)	Human = Human1;
				if (Human)
				{
					UCPContacts	Contacts(Args);
					if (Contacts.Size)
					{
						Contacts.Size = 0;
						uc3dxVector3 vDir = Human->Physics.Center.Pos.Value() - Self->Physics.Center.Pos.Value();

						ucINT dir = 0;
						ucFLOAT	fDir = vDir | Self->Physics.CenterBody.GetWay_X();

						if (fDir > 0.0f)	dir = -1;	else	dir = +1;

						//判断自己是否可以免疫
						if (!Self->Physics.State.IsRunning("无敌") && !Self->Physics.State.IsRunning("超人") &&
							!Human->Physics.State.IsRunning("缩小"))
						{
							//侧翻的条件
							if (Human->Physics.State.IsRunning("无敌") || Human->Physics.State.IsRunning("超人") ||
								Self->Physics.State.IsRunning("缩小"))
							{
								ucINT Result = 0;
								Self->Physics.SetStateNet("侧翻", 0, &Result);
							}
							else
							{
								if (Self->Physics.Data.Weight <= Human->Physics.Data.Weight)
								{
									//正翻的判定
									if (Self->Physics.State.IsRunning("偏移") && Self->StateDir * dir < 0)
									{
										ucINT Result = 0;
										Self->Physics.SetStateNet("正翻", 0, &Result);
									}
									else
									{
										ucINT Len = 10.0f * (1.0f - Self->Physics.Data.Weight);

										Len *= dir;
										//第二个参数是被碰撞后运动的方向和幅度
										ucINT Result = 0;
										Self->Physics.SetStateNet("偏移", Len, &Result);
										//gCarSound.PlaySound(5);
									}
								}
							}
						}
						//判断对方是否可以免疫
						if (!Human->Physics.State.IsRunning("无敌") && !Human->Physics.State.IsRunning("超人") &&
							!Self->Physics.State.IsRunning("缩小") && !Self->Physics.State.IsRunning("碰撞无效") && !Self->Physics.State.IsRunning("倒霉后保护"))
						{
							//侧翻的条件
							if (Self->Physics.State.IsRunning("无敌") || Self->Physics.State.IsRunning("超人") ||
								Human->Physics.State.IsRunning("缩小"))
							{
								ucINT Result = 0;
								Human->Physics.SetStateNet("侧翻", 0, &Result);
							}
							else
							{
								if (Human->Physics.Data.Weight <= Self->Physics.Data.Weight)
								{
									//正翻的判定
									if (Human->Physics.State.IsRunning("偏移") && Human->StateDir * dir < 0)
									{
										ucINT Result = 0;
										Human->Physics.SetStateNet("正翻", 0, &Result);
									}
									else
									{
										ucINT Len = 10.0f * (1.0f - Human->Physics.Data.Weight);

										Len *= -dir;
										//第二个参数是被碰撞后运动的方向和幅度
										ucINT Result = 0;
										Human->Physics.SetStateNet("偏移", Len, &Result);
										//gCarSound.PlaySound(5);
									}
								}
							}
						}
					}
				}
			}
		}
	}

	ucVOID CollideGround(UCPContactArgs* Args)
	{
		UCMeshPhyBase* Body = 0;
		if (Args->Mesh1 == &Ground)	Body = Args->Mesh2;
		if (Args->Mesh2 == &Ground)	Body = Args->Mesh1;
		if (Body)
		{
			if (Body->Handle)
			{
				UCHuman* Human = (UCHuman*)Body->Handle;

				UCPContacts	Contacts(Args);
				Contacts.Param.Mu = 0.0f;

				Contacts.Param.SoftERP = 0.8f;
				Contacts.Param.SoftCFM = 0.2f;

				if (Contacts.Size)
				{
					ucINT SideBackup = -1;
					for (ucINT i = 0; i < Contacts.Size; i++)
					{
						uc3dxVector3 Nor = Contacts.NormalAt(i);

						ucINT Side1 = Contacts.Side1At(i);
						ucINT Side2 = Contacts.Side2At(i);

						ucINT Side = Side1;
						if (Side == -1)
							Side = Side2;

						uc3dxVector3 p0 = Ground.GetFaceIndexAt(Side, 0);
						uc3dxVector3 p1 = Ground.GetFaceIndexAt(Side, 1);
						uc3dxVector3 p2 = Ground.GetFaceIndexAt(Side, 2);

						//重新计算精确面和精确法线
						//if (p0!=0&&p1!=0&&p2!=0)
						{
							uc3dxVector3 v0 = p0 * Ground.matWorld;
							uc3dxVector3 v1 = p1 * Ground.matWorld;
							uc3dxVector3 v2 = p2 * Ground.matWorld;

							//求面的法线
							Nor = (v1 - v0) % (v2 - v0);
							Nor.Normalize();

						}

						//如果坡度小于0.6f,当墙面
						if (Nor.y < +0.6f)
							Human->Physics.RefreshWall(Args, Contacts);
						//当地面处理
						else
						{
							if (Side == -1 || SideBackup == Side)
								continue;
							else
							{
								Human->Physics.Head = Contacts.NormalAt(0);
								Human->Physics.RefreshGround(Args, Contacts);
							}
						}
						SideBackup = Side;
					}
				}
			}
		}
	}

	ucVOID OnCollide(UCObject* Sender, UCEventArgs* e)
	{
		CollideGround((UCPContactArgs*)e);
	}

	ucVOID CheckItemCollision()
	{
// 		for (ucINT i = 0; i < MAX_USER; i++)
// 		{
// 			UCHuman* Curr = Users[i];
// 			if (ManageAble(Curr))
// 			{
// 				CheckItemRubbish(Curr);
// 				CheckItemBox(Curr);
// 				CheckItemMine(Curr);
// 				CheckItemOil(Curr);
// 				CheckItemFollow(Curr, Curr == Self);
// 				CheckItemJingDeng(Curr);
// 			}
// 		}
// 		CheckItemRocket();
// 		CheckAchieve();
	}

	ucVOID Step()
	{
		if (SceneCtl)
		{
			UCDevice* Device = UIGetDevice();

			if (Self->Physics.PortalObject.Loop != MaxLoop)
			{
				if (MineHold0 == 0)
				{
					if (!KeyState.KEYIsTool1())
						MineHold0 = -1;
				}
				if (MineHold1 == 0)
				{
					if (!KeyState.KEYIsTool1())
						MineHold1 = -1;
				}
				if (!Self->Physics.State.IsRunning("沉默") &&
					!Self->Physics.State.IsRunning("睡眠") &&
					!Self->Physics.State.IsRunning("枷锁"))
				{
					if (KeyState.KEYIsTool1())
						Self->ItemSelect = 0;
				}
			}
			UCCamera* CameraSelf = Self->Physics.CenterBody.Camera;
			if (CameraSelf)
			{
				if (CameraRayLength < Self->Physics.CameraRayLength)
					CameraRayLength += 1.0f;
				if (CameraRayLength > Self->Physics.CameraRayLength)
					CameraRayLength -= 1.0f;

				if (CameraSelf->distance.Value < -Self->BaseCameraRayLength - CameraRayLength)
					CameraSelf->distance = CameraSelf->distance.Value + 1.4f;
				if (CameraSelf->distance.Value > -Self->BaseCameraRayLength - CameraRayLength)
					CameraSelf->distance = CameraSelf->distance.Value - 1.4f;
				CameraSelf->Pos.y = 60.0f - CameraRayLength * 0.16f;

				CameraRay.Pos = Self->Physics.Center.Pos.Value();
				CameraRay.Rot = Self->Physics.CenterBody.Rot.Value();
				CameraRay.Rot.y += UC3D_PI;
			}

			for (ucINT i = 0; i < MAX_USER; i++)
			{
				UCHuman* Human = Users[i];
				if (Human)
				{
					if (Human->Physics.Mode == GRD_RESET)
					{
						ucINT Result = 0;
						Human->Physics.SetStateNet("Reset", 0, &Result);

						Human->Physics.Reset();
						Human->Physics.Mode = GRD_EMPTH;
					}

					Human->Physics.RefreshMode();
					Human->GroundState = GROUND_NORMAL;
				}
			}
			CheckItemCollision();
			Manage.Sort();

			CameraRayHitValid = 0;
			CameraRayMinDis = 10000.0f;
			SceneCtl->QuickStep(0.2f);

			if (CameraRayHitValid != 0)
			{
				Self->Physics.CameraRayLength = -CameraRayMinDis;
				Self->Physics.CameraFixAble = 0;

				ucFLOAT RotX = (CameraRay.Length.Value - CameraRayMinDis) / CameraRay.Length.Value * 1.87f;
				if (CameraSelf->Rot.x.Value < RotX)
					CameraSelf->Rot.x += STP_ROT_X;
			}
			/*
			//炸弹检测
			for (ucINT i = BombArray.GetSize() - 1; i >= 0; i--)
			{
				UCBombStruct* Bomb = (UCBombStruct*)BombArray.GetAt(i);

				if (Bomb->Valid == 0)
				{
					uc3dxVector3 Pos = Bomb->MeshRay.Pos.Value() + Bomb->MeshRay.GetWay_Z() * Bomb->Length;

					UCLandMine* Well = new UCLandMine(6, Bomb->Human, Pos, &Scene);
					Wells.Add(Well);

					BombArray.RemoveAt(i);
					delete Bomb;
				}
			}
			*/
			for (ucINT i = 0; i < MAX_USER; i++)
			{
				UCHuman* Human = Users[i];
				if (Human)
				{
					UCImage* UserImage = &(ScaleMap.ScaleMapUser[i]);

					ucINT X = Human->Physics.CenterBody.Pos.x.Value * ScaleMap.ScaleMapGrid;
					ucINT Y = Human->Physics.CenterBody.Pos.z.Value * ScaleMap.ScaleMapGrid;

					UserImage->Location = ScaleMap.ScaleMapOffset + UCPoint(X, ScaleMap.ScaleMap.Size.cy.Value - Y - 1);

					if (Human != Self)
						Self->CheckHuman(Human);

					Human->CheckMine();
					if (Human->Physics.Mode == GRD_FLOOR)
					{
						UCMesh* Body = &(Human->Physics.CenterBody);
						UCMeshPhyBody* Center = &(Human->Physics.Center);

						uc3dxVector3 vSpeed = Center->LinearVel.Value();
						ucFLOAT fX = Body->GetWay_X() | vSpeed;
						ucFLOAT fY = Body->GetWay_Y() | vSpeed;
						ucFLOAT fZ = Body->GetWay_Z() | vSpeed;

						if (Human->Physics.DriftType != 0)
							Human->Physics.SpeedBackup *= 0.995f;
						else if (Human->Physics.State.IsRunning("冲刺") ||
							Human->Physics.State.IsRunning("儿童"))
						{
							fX = fX * 0.60f;//转弯摩擦减少0.06倍 FZ 直道没有摩擦损失
						}
						else if (Human->Physics.State.IsRunning("无敌") ||
							Human->Physics.State.IsRunning("强化"))
						{
							fX = fX * 0.60f;
						}
						else if (Human->Physics.State.IsRunning("加速"))
						{
							fX = fX * 0.60f;//转弯摩擦减少0.06倍 FZ 直道没有摩擦损失
						}
						else if (Human->Physics.State.IsRunning("超级加速"))
						{
							fX = fX * 0.85f;//转弯摩擦减少0.85倍
						}
						else if (!Human->Physics.State.IsRunning("测试驾驶"))
						{
							if (Human->Physics.DriftUpTime == 0)
							{
								if (!KeyState.KEYIsUp())
									fZ = fZ * 0.995f;
								fX = fX * 0.9f;
							}
							else
							{
								fX = fX * 0.96f;
							}
						}

						if (fY < -2.0f)	fY = -2.0f;

						vSpeed = Body->GetWay_X() * fX + Body->GetWay_Y() * fY + Body->GetWay_Z() * fZ;

						//Body->LinearVel = vSpeed;
						Center->LinearVel = vSpeed;
					}

					Human->Physics.RefreshDirection();
					if (Human->Physics.Center.LinearVel.Value().Length() > 10.0f)
					{
						//更新状态
						if (Human->GroundState == GROUND_GRASS)
						{
							if (Human->GroundMesh.Name.Value != UCString("res/model/action/cao.dat"))
							{
								Human->GroundMesh.Name = UCString("res/model/action/cao.dat");
								Human->GroundMesh.Scale = uc3dxVector3(5.0f, 5.0f, 5.0f);
								Human->GroundMesh.PlayAction(0);
							}
							Human->GroundMesh.Visible = 1;
						}
						else if (Human->GroundState == GROUND_MUD)
						{
							if (Human->GroundMesh.Name.Value != UCString("res/model/action/ni.dat"))
							{
								Human->GroundMesh.Name = UCString("res/model/action/ni.dat");
								Human->GroundMesh.Scale = uc3dxVector3(5.0f, 5.0f, 5.0f);
								Human->GroundMesh.PlayAction(0);
							}
							Human->GroundMesh.Visible = 1;
						}
						else if (Human->GroundState == GROUND_JianSuYun)
						{
							if (Human->GroundMesh.Name.Value != UCString("res/model/action/filed3_slowdownyun.dat"))
							{
								Human->GroundMesh.Name = UCString("res/model/action/filed3_slowdownyun.dat");
								Human->GroundMesh.Scale = uc3dxVector3(3.0f, 3.0f, 3.0f);
								Human->GroundMesh.PlayAction(0);
							}
							Human->GroundMesh.Visible = 1;

						}
						else if (Human->GroundState == GROUND_JiaSuZone)
						{
							if (Human->GroundMesh.Name.Value != UCString("res/model/action/speedup.dat"))
							{
								Human->GroundMesh.Name = UCString("res/model/action/speedup.dat");
								Human->GroundMesh.Scale = uc3dxVector3(8.0f, 8.0f, 8.0f);
								Human->GroundMesh.PlayAction(0);
							}
							Human->GroundMesh.Visible = 1;
						}
						else if (Human->GroundState == GROUND_HUAPU)
						{
							if (Human->GroundMesh.Name.Value != UCString("res/model/action/huapu.dat"))
							{
								Human->GroundMesh.Name = UCString("res/model/action/huapu.dat");
								Human->GroundMesh.Scale = uc3dxVector3(5.0f, 5.0f, 5.0f);
								Human->GroundMesh.PlayAction(0);
							}
							Human->GroundMesh.Visible = 1;
						}
						else
						{
							Human->GroundMesh.Visible = 0;
						}
						if (Human->Physics.CarVentFire[0].Name == Human->Physics.Data.SmallFireName[0])//当前非加速等状态
						{
							Human->Physics.SetVentFire(SMALLFIRE, 6.0f);
						}
					}
					else
					{
						Human->GroundMesh.Visible = 0;

						//Human->Physics.SetVentFireVisible(0);
						Human->Physics.SetVentFire(SMALLFIRE, 0.5f);
					}
				}
			}
		}
	}
};

#endif