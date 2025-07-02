/********************************************************************
created:	2007-7-23   17:01
filename: 	UCMap_012
author:		Amber

purpose:	
*********************************************************************/

#ifndef _UCMap_012_23_7_2007_17_01
#define _UCMap_012_23_7_2007_17_01

#include "../Map.h"

class UCMap12 : public UCMap
{
   public:
	UCMeshScene		GroundLuBian;
       // UCMeshScene		JSP;   //加速底座
	UCMesh			JY;    //鲸鱼
	UCMesh                  MY;    //鳗鱼赛道
        UCMesh                  HS;    //火山
        UCMesh                  HC;    //海草
        UCMesh                  CFT;   //船飞艇
        UCMesh                  FS;    //鳗鱼赛道的水面反射
        UCMesh		        ys[12];    //椰树
        UCMesh		        HM;    //海面
        UCMeshScene	        JS;   //加速
        UCMap12()
	{
		Reset.Name=UCString("MagicXBig/Res/Model/Field/Field12/Reset12.dat");
		Reset.Visible = 0;
		Manage.CreateTree(UCString("MagicXBig/Res/Model/Field/Field12/Door12.dat"));
		Scene.Name = UCString("MagicXBig/Res/Model/Field/Field12/Field12.dat");
		Scene.PlayAction(0);
		Ground.Name = UCString("MagicXBig/Res/Model/Field/Field12/Physics12.dat");	
		GroundLuBian.Name = UCString("MagicXBig/Res/Model/Field/Field12/Field12_LuBian.dat");
		JY.Name = UCString("MagicXBig/Res/Model/Field/Field12/Field12_JY.dat");
		JY.PlayAction(0);
                MY.Name = UCString("MagicXBig/Res/Model/Field/Field12/Field12_MY.dat");
                MY.PlayAction(0);
                HS.Name = UCString("MagicXBig/Res/Model/Field/Field12/Field12_HS.dat");
                HS.PlayAction(0);
                HC.Name = UCString("MagicXBig/Res/Model/Field/Field12/Field12_HC.dat");
                HC.PlayAction(0);
                FS.Name = UCString("MagicXBig/Res/Model/Field/Field12/Field12_FS.dat");
                FS.PlayAction(0);
                CFT.Name = UCString("MagicXBig/Res/Model/Field/Field12/Field12_CFT.dat");
                CFT.PlayAction(0);
                HM.Name = UCString("MagicXBig/Res/Model/Field/Field12/Field12_HM.dat");
                HM.PlayAction(0);
                JS.Name = UCString("MagicXBig/Res/Model/Field/Field12/Field12_JS.dat");
                JS.PlayAction(0);
               // JSP.Name = UCString("MagicXBig/Res/Model/Field/Field12/Field12_JSP.dat");
                //JSP.PlayAction(0);
               for (int i=0;i<12;i++)
		{
			ys[i].Name = UCString("MagicXBig/Res/Model/Field/Field12/Field12_YS.dat");		
			ys[i].PlayAction(0);
		}		
		ys[0].Pos =uc3dxVector3(-17337.674f,1926.56f,6030.347f);
		ys[1].Pos =uc3dxVector3(-16319.855f,1934.932f,6030.347);
		ys[2].Pos =uc3dxVector3(-11941.606f,1934.933f,1881.835f);
		ys[3].Pos =uc3dxVector3(-11075.81f,1934.933f,6030.347f);
                ys[4].Pos =uc3dxVector3(-7234.486f,1953.09f,4988.834f);
                ys[5].Pos =uc3dxVector3(10028.63f,1934.933f,-129.82f);
                ys[6].Pos =uc3dxVector3(9928.5618f,1948.611f,2170.295f);
                ys[7].Pos =uc3dxVector3(-13081.632f,2314.219f,5966.598f);
                ys[7].Scale=uc3dxVector3(2.0f,2.0f,2.0f);
                ys[8].Pos =uc3dxVector3(-9926.945f,2402.159f,1325.654f);
                ys[8].Scale=uc3dxVector3(2.0f,2.0f,2.0f);
                ys[9].Pos =uc3dxVector3(-9239.881f,2314.219f,5057.028f);
                ys[9].Scale=uc3dxVector3(2.0f,2.0f,2.0f);
                ys[10].Pos =uc3dxVector3(-5317.698f,2402.159f,1325.654f);
                ys[10].Scale=uc3dxVector3(2.0f,2.0f,2.0f);
                ys[11].Pos =uc3dxVector3(8501.64f,2320.553f,204.797f);
                ys[11].Scale=uc3dxVector3(2.0f,2.0f,2.0f);
	}
 
	//初始化	UI、场景
	void Init(UCSceneCtl*	Parent)
	{
		SceneCtl = Parent;
		Ground.Parent = SceneCtl->Scene;
		GroundLuBian.Parent = SceneCtl->Scene;
 
		JY.Parent = SceneCtl->Scene;
                CFT.Parent = SceneCtl->Scene;
                MY.Parent = SceneCtl->Scene;
                HS.Parent = SceneCtl->Scene;
                HC.Parent = SceneCtl->Scene;
                FS.Parent = SceneCtl->Scene;
                for (int i=0;i<12;i++)
		{
	         ys[i].Parent = SceneCtl->Scene;
		}
               JS.Parent = SceneCtl->Scene;
               HM.Parent = SceneCtl->Scene;
               // JSP.Parent = SceneCtl->Scene;
                Reset.Parent = SceneCtl->Scene;
		SceneCtl->World->OnCollide = UCEvent(this,&UCMap12::OnCollide);
		MaxLoop = 3;

		Parent->SunDir = (uc3dxVector3(0.04f,-1.0f,0.36f));
#ifdef SHADOW
		Parent->ShadowTex->Material.Name = UCString("Res/Shader/shadow_map.material");
		Material.Name = UCString("Res/Shader/shadowed_smooth.material");
		Material.AppendTexture(Parent->ShadowTex);
		Material.OnUpdate += UCEvent(this,UCMap::OnShadowUpdate);
		Ground.SetMaterial(&Material);
               // JSP.SetMaterial(&Material);
#endif

		ScaleMap.Pic.Offset = UCPoint(-576,-384);
		ScaleMap.Visible = 1;

		ScaleMapOffset = UCPoint(92,-82);
		ScaleMapGrid = 0.00434f;
	}
	//初始化
	void Posite(int type)
	{
		UCPortal*		Portal = Manage.PortalAt(-1);
		uc3dxVector3	Up(0.0f,1.0f,0.0f);
		uc3dxVector3	Pos = Portal->Center - Up*338.0f;
		uc3dxVector3	Lft = Portal->Normal % Up;

		float StartX = -144.0f;

		int k=0;
		for (int i=0;i<MAX_USER;i++)
		{
			UCHuman*	Human;
			Human = Users[i];				
			if (Human)
			{
				if (k/4==0)
				{				
					float f = 100.0f*(k)-180.0f;	
					Human->Physics.PortalObject.Locate(-1);
					Human->Physics.SetPosition(Pos + Lft*f - Portal->Normal*10.0f);
					Human->Physics.PlayAction(0);
					Human->Physics.CarVent.PlayAction(0);
					k++;
				}
				else
				{
					float f = 100.0f*(k-4)-230.0f;
					Human->Physics.PortalObject.Locate(-1);
					Human->Physics.SetPosition(Pos + Lft*f - Portal->Normal*100.0f);
					Human->Physics.PlayAction(0);
					Human->Physics.CarVent.PlayAction(0);
					k++;

				}

			}
		}	
		if (type==ROOM_TEAM||type==ROOM_SNGL)
			PositeItemBox();
	}
	void PositeWinTable(int	Type)
	{
		uc3dxVector3	Pos;
		uc3dxVector3	Lft;
		uc3dxVector3	Dow;
		uc3dxVector3	Up(0.0f,1.0f,0.0f);

		UCPortal*		Portal;

		Portal = Manage.PortalAt(-1);
		Pos = Portal->Center - Up*378.0f;
		Lft = Portal->Normal % Up;		
		float f =20.0f;//值越大.越往右	
		BanJiangTai.Pos = Pos + Lft*f;
		BanJiangTai.Parent = &Scene;
		BanJiangTai.Face(Portal->Normal);

		if (Type==ROOM_TEAM||Type==ROOM_TEAM_SIDE)
		{
			BanJiangTai.Name= UCString("MagicXBig/Res/Model/Action/Finish_4Tai.dat");
			BanJiangTai.Scale=uc3dxVector3(5.0f,5.0f,5.0f);
		}
		//else if(Type==ROOM_SNGL)
		else
		{
			BanJiangTai.Name= UCString("MagicXBig/Res/Model/Action/Finish_3Tai.dat");			
			BanJiangTai.Scale=uc3dxVector3(5.0f,5.0f,5.0f);
		}			
	}
	void PositeItemBox()
	{
		uc3dxVector3	Pos;
		uc3dxVector3	Lft;
		uc3dxVector3	Dow;
		uc3dxVector3	Up(0.0f,1.0f,0.0f);

		UCPortal*		Portal;

		Portal = Manage.PortalAt(8);
		Pos = Portal->Center - Up*330.0f;
		Lft = Portal->Normal % Up;
		for (int i=0;i<5;i++)
		{
			float f = 150.0f*(i)-280.0f;											//向右
			UCPropBox* RItem1 = new UCPropBox(Pos + Lft*f,&Scene);
			PropBoxs.Add(RItem1);
		}

		Portal = Manage.PortalAt(22);
		Pos = Portal->Center - Up*250.0f;										//高
		Lft = Portal->Normal % Up;
		for (int i=0;i<5;i++)
		{
			float f = 150.0f*(i)-270.0f;
			UCPropBox* RItem1 = new UCPropBox(Pos + Lft*f,&Scene);
			PropBoxs.Add(RItem1);
		}

		Portal = Manage.PortalAt(38);
		Pos = Portal->Center - Up*200.0f;
		Lft = Portal->Normal % Up;
		for (int i=0;i<4;i++)
		{
			float f = 150.0f*(i)-230.0f;
			UCPropBox* RItem1 = new UCPropBox(Pos + Lft*f,&Scene);
			PropBoxs.Add(RItem1);
		}
                Portal = Manage.PortalAt(52);
		Pos = Portal->Center - Up*220.0f;
		Lft = Portal->Normal % Up;
		for (int i=0;i<5;i++)
		{
			float f = 150.0f*(i)-280.0f;
			UCPropBox* RItem1 = new UCPropBox(Pos + Lft*f,&Scene);
			PropBoxs.Add(RItem1);
		}
                Portal = Manage.PortalAt(70);
		Pos = Portal->Center - Up*410.0f;
		Lft = Portal->Normal % Up;
		for (int i=0;i<5;i++)
		{
			float f = 150.0f*(i)-280.0f;
			UCPropBox* RItem1 = new UCPropBox(Pos + Lft*f,&Scene);
			PropBoxs.Add(RItem1);
		}
                Portal = Manage.PortalAt(80);
		Pos = Portal->Center - Up*300.0f;
		Lft = Portal->Normal % Up;
		for (int i=0;i<5;i++)
		{
			float f = 150.0f*(i)-280.0f;
			UCPropBox* RItem1 = new UCPropBox(Pos + Lft*f,&Scene);
			PropBoxs.Add(RItem1);
		}
		 Portal = Manage.PortalAt(90);
		Pos = Portal->Center - Up*350.0f;
		Lft = Portal->Normal % Up;
		for (int i=0;i<5;i++)
		{
			float f = 150.0f*(i)-280.0f;
			UCPropBox* RItem1 = new UCPropBox(Pos + Lft*f,&Scene);
			PropBoxs.Add(RItem1);
		}
	}
	void UC_PROC OnCollide(UCObject* Sender,UCEventArgs* e)
	{
		UCPContactArgs* Args = (UCPContactArgs*)e;

		//Reset判断
		CollideReset(Args);
		CollideUsers(Args);

		UCMeshPhyBase*	Body;
		

		Body = 0;
		if (Args->Mesh1==&GroundLuBian)	Body = Args->Mesh2;//碰到路边的护栏
		if (Args->Mesh2==&GroundLuBian)	Body = Args->Mesh1;
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
					{
						Contacts.Size = 0;
					}
					else
					{
						Human->Physics.RefreshWall(Args,Contacts);
					}
				}
			}
		}
  //              Body = 0;
		//if (Args->Mesh1==&JSP)	Body = Args->Mesh2;//碰到加速板侧面
		//if (Args->Mesh2==&JSP)	Body = Args->Mesh1;
		//if (Body)
		//{
		//	if (Body->Handle)
		//	{
		//		UCHuman* Human = (UCHuman*)Body->Handle;

		//		UCPContacts	Contacts(Args);
		//		Contacts.Param.Mu = 0.0f;

		//		Contacts.Param.SoftERP = 0.8f;
		//		Contacts.Param.SoftCFM = 0.2f;

		//		if (Contacts.Size)
		//		{
		//			if (Human->Physics.State.IsRunning("超人"))
		//			{
		//				Contacts.Size = 0;
		//			}
		//			else
		//			{
		//				Human->Physics.RefreshWall(Args,Contacts);
		//			}
		//		}
		//	}
		//}
                Body = 0;
		if (Args->Mesh1==&JS)	Body = Args->Mesh2;
		if (Args->Mesh2==&JS)	Body = Args->Mesh1;
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
					Human->Physics.Center.LinearVel =Human->Physics.CenterBody.GetWay_Z() * 320.0f;
					Human->Physics.RefreshGround(Args,Contacts);
				}
			}
		}		
		Body = 0;
		if (Args->Mesh1==&Ground)	Body = Args->Mesh2;
		if (Args->Mesh2==&Ground)	Body = Args->Mesh1;
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
					Human->Physics.Head = Contacts.NormalAt(0);
					Human->Physics.RefreshGround(Args,Contacts);
				}
			}
		}
	}
};

#endif