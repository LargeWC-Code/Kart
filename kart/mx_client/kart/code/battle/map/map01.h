#ifndef _MAP01_
#define _MAP01_

#include "mapbase.h"

class UCKartMap01 : public UCKartMap
{
public:
	UCMeshScene		GroundWater;
	UCMeshScene		GroundLuBian;

	UCKartMap01()
	{
		Manage.CreateTree(UCString("res/model/field/field1/door1.dat"));
		
		Scene.Name = UCString("res/model/field/field1/field1.dat");
		Scene.PlayAction(0);

		Ground.Name = UCString("res/model/field/field1/physics1.dat");
		GroundLuBian.Name = UCString("res/model/field/field1/field1_lubian.dat");

		Reset.Name = UCString("res/model/field/field1/reset1.dat");
		Reset.Visible = 0;
	}
	//初始化	UI、场景
	ucVOID Init(UCSceneCtl* Parent)
	{
		SceneCtl = Parent;
		SceneCtl->World->OnCollide += UCEvent(this, OnCollide);

		Scene.Parent = SceneCtl->Scene;
		Ground.Parent = SceneCtl->Scene;
		GroundWater.Parent = SceneCtl->Scene;
		GroundLuBian.Parent = SceneCtl->Scene;
		Reset.Parent = SceneCtl->Scene;

		MaxLoop = 4;

		Parent->SunDir = uc3dxVector3(0.0f, -1.0f, 0.0f);
		Parent->SunBase = uc3dxVector3(0.2f, 0.2f, 0.2f);
		Parent->SunBala = uc3dxVector3(0.8f, 0.8f, 0.8f);

		Material.Name = UCString("res/shader/shadowed_object.material");
		Material.AppendTexture(Parent->ShadowTex, 1);

		MaterialScene.Name = UCString("res/shader/shadowed_scene.material");
		MaterialScene.AppendTexture(Parent->ShadowTex, 1);

// 		Scene.SetMaterial(&MaterialScene, 1);
// 		Ground.SetMaterial(&MaterialScene, 1);
// 		GroundLuBian.SetMaterial(&MaterialScene, 1);

// 		Scene.FilterEnable(0, 1);
// 		Ground.FilterEnable(0, 1);
// 		GroundLuBian.FilterEnable(0, 1);

		ScaleMap.ScaleMapOffset = UCPoint(105, -112);
		ScaleMap.ScaleMapGrid = 0.00354f;
	}
	//初始化
	ucVOID Posite()
	{
		UCPortal* Portal = Manage.PortalAt(-1);
		uc3dxVector3	Up(0.0f, 1.0f, 0.0f);
		uc3dxVector3	Pos = Portal->Center - Up * 506.8f;//536.8f;
		uc3dxVector3	Lft = Portal->Normal % Up;

		ucINT MaxPerson = 0;
		for (ucINT i = 0; i < MAX_USER; i++)
		{
			UCHuman* Human = Users[i];
			if (Human) MaxPerson++;
		}
		ucFLOAT StartX1 = 0.0f;
		ucFLOAT StartX2 = 0.0f;
		if (MaxPerson > 4)
		{
			StartX2 = 62.00f * (MaxPerson - 5);
			StartX1 = 45.00f * 3.0f;
		}
		else
		{
			StartX1 = 45.00f * (MaxPerson - 1);
		}

		ucINT k = 0;
		for (ucINT i = 0; i < MAX_USER; i++)
		{
			UCHuman* Human = 0;
			Human = Users[i];
			if (Human)
			{
				if (k / 4 == 0)
				{
					ucFLOAT f = 100.0f * (k)-StartX1;
					Human->Physics.PortalObject.Locate(-1);
					Human->Physics.SetPosition(Pos + Lft * f - Portal->Normal * 10.0f);
					Human->Physics.PlayAction(0);
					Human->Physics.CarVent.PlayAction(0);
				}
				else
				{
					ucFLOAT f = 100.0f * (k - 4) - StartX2;
					Human->Physics.PortalObject.Locate(-1);
					Human->Physics.SetPosition(Pos + Lft * f - Portal->Normal * 100.0f);
					Human->Physics.PlayAction(0);
					Human->Physics.CarVent.PlayAction(0);
				}
				k++;
			}
		}
		PositeItemBox();
	}
	ucVOID PositeWinTable(ucINT	Type)
	{
		uc3dxVector3	Pos;
		uc3dxVector3	Lft;
		uc3dxVector3	Dow;
		uc3dxVector3	Up(0.0f, 1.0f, 0.0f);

		UCPortal* Portal = 0;

		Portal = Manage.PortalAt(-1);
		Pos = Portal->Center - Up * 249.0f;
		Lft = Portal->Normal % Up;
		ucFLOAT f = -10.0f;//值越大.越往右	
		BanJiangTai.Pos = Pos + Lft * f;
		BanJiangTai.Parent = &Scene;
		BanJiangTai.Face(Portal->Normal);

		BanJiangTai.Name = UCString("res/model/action/finish_4tai.dat");
		BanJiangTai.Scale = uc3dxVector3(5.0f, 5.0f, 5.0f);
	}
	ucVOID PositeItemBox()
	{
		uc3dxVector3	Pos;
		uc3dxVector3	Lft;
		uc3dxVector3	Dow;
		uc3dxVector3	Up(0.0f, 1.0f, 0.0f);

		UCPortal* Portal = 0;

		Portal = Manage.PortalAt(8);											//高
		Pos = Portal->Center - Up * 570.0f;
		Lft = Portal->Normal % Up;
		for (ucINT i = 0; i < 5; i++)
		{
			ucFLOAT f = 150.0f * (i)-400.0f;
			UCPropBox* RItem1 = new UCPropBox(Pos + Lft * f, &Scene);
			PropBoxs.Add(RItem1);
		}
		Portal = Manage.PortalAt(14);											//高
		Pos = Portal->Center - Up * 570.0f;
		Lft = Portal->Normal % Up;
		for (ucINT i = 0; i < 5; i++)
		{
			ucFLOAT f = 150.0f * (i)-240.0f;
			UCPropBox* RItem1 = new UCPropBox(Pos + Lft * f, &Scene);
			PropBoxs.Add(RItem1);
		}
		Portal = Manage.PortalAt(21);											//高
		Pos = Portal->Center - Up * 600.0f;
		Lft = Portal->Normal % Up;
		for (ucINT i = 0; i < 5; i++)
		{
			ucFLOAT f = 150.0f * (i)-240.0f;
			UCPropBox* RItem1 = new UCPropBox(Pos + Lft * f, &Scene);
			PropBoxs.Add(RItem1);
		}
		Portal = Manage.PortalAt(26);											//高
		Pos = Portal->Center - Up * 570.0f;
		Lft = Portal->Normal % Up;
		for (ucINT i = 0; i < 5; i++)
		{
			ucFLOAT f = 150.0f * (i)-240.0f;
			UCPropBox* RItem1 = new UCPropBox(Pos + Lft * f, &Scene);
			PropBoxs.Add(RItem1);
		}
		Portal = Manage.PortalAt(36);											//高
		Pos = Portal->Center - Up * 570.0f;
		Lft = Portal->Normal % Up;
		for (ucINT i = 0; i < 7; i++)
		{
			ucFLOAT f = 120.0f * (i)-340.0f;
			UCPropBox* RItem1 = new UCPropBox(Pos + Lft * f, &Scene);
			PropBoxs.Add(RItem1);
		}

	}
	ucVOID OnCollide(UCObject* Sender, UCEventArgs* e)
	{
		UCPContactArgs* Args = (UCPContactArgs*)e;

		CollideGround(Args);
		CollideUsers(Args);
		CollideReset(Args);

		UCMeshPhyBase* Body = 0;

		Body = 0;
		if (Args->Mesh1 == &GroundWater)	Body = Args->Mesh2;
		if (Args->Mesh2 == &GroundWater)	Body = Args->Mesh1;
		if (Body)
		{
			if (Body->Handle)
			{
				UCHuman* Human = (UCHuman*)Body->Handle;
				if (Human == Self)
				{
					UCPContacts	Contacts(Args);

					if (Contacts.Size)
					{
						uc3dxVector3 vLinearVel = Self->Physics.Center.LinearVel.Value();
						ucFLOAT vSpeed = vLinearVel | Self->Physics.CenterBody.GetWay_Z();

						if (vSpeed > 100.0f)
						{
							ucINT Result = 0;
							Self->Physics.SetStateNet("打滑", 0, &Result);
						}
						Contacts.Size = 0;
					}
				}
			}
		}
		Body = 0;
		if (Args->Mesh1 == &GroundLuBian)	Body = Args->Mesh2;
		if (Args->Mesh2 == &GroundLuBian)	Body = Args->Mesh1;
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
					if (Human->Physics.State.IsRunning("超人"))
						Contacts.Size = 0;
					else
						Human->Physics.RefreshWall(Args, Contacts);
				}
			}
		}
	}
};

#endif