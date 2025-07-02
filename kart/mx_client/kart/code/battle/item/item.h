#ifndef ITEM
#define ITEM

class UCFlyItem
{
public:
	UCMesh				Mesh;		//模型
	ucINT				Type;		//类型
	ucINT				Owner;		//主人
	ucINT				Index;		//编号
	ucINT				Target;		//目标
	ucINT				LifeTime;	//生命
	ucINT				Group;		//队伍
	UCPortalObject		Object;

	UCFlyItem(ucINT TypeIn, UCHuman* User, uc3dxVector3 v, UCMesh* Parent, UCPortalManage* Manage)//构造函数 
	{
		Type = TypeIn;
		Owner = User->ID;
		Index = User->ItemID;
		User->ItemID++;

		LifeTime = 0;

		Target = -1;

		Group = User->GroupID;

		//水球波及圈
		if (Type == 4)
		{
			Mesh.Name = UCString("res/model/action/bigwaterball.dat");
			Mesh.Scale = uc3dxVector3(28.0f, 28.0f, 28.0f);
			//Mesh.RenderMode =1;
			Mesh.PlayAction(0);
		}
		//跟踪弹
		else if (Type == 5)
		{
			Mesh.Name = UCString("res/model/action/genzongdan.dat");	//创建木桶模型
			Mesh.Scale = uc3dxVector3(3.0f, 3.0f, 3.0f);
			Mesh.PlayAction(0);
		}
		//飞弹(一定时间爆炸，产生水球波及圈)
		else if (Type == 6)
		{
			Mesh.Name = UCString("res/model/action/shuidan.dat");	//创建木桶模型//feidan.dat
			Mesh.Scale = uc3dxVector3(2.5f, 2.5f, 2.5f);
			Mesh.PlayAction(0);
		}
		//球形云层
		else if (Type == 7)
		{
			Mesh.Name = UCString("res/model/action/blindrog.dat");
			Mesh.Scale = uc3dxVector3(20.0f, 20.0f, 20.0f);
			Mesh.PlayAction(0);
		}
		//定时炸弹
		else if (Type == 8)
		{
			Mesh.Name = UCString("res/model/action/bigwaterball.dat");
			Mesh.Scale = uc3dxVector3(24.0f, 24.0f, 24.0f);
			//Mesh.RenderMode =1;
			Mesh.PlayAction(0);
		}
		//第一名爆炸
		else if (Type == 9)
		{
			Mesh.Name = UCString("res/model/action/bigwaterball.dat");
			Mesh.Scale = uc3dxVector3(18.0f, 18.0f, 18.0f);
			//Mesh.RenderMode =1;
			Mesh.PlayAction(0);
		}
		//跟踪雷
		else if (Type == 10)
		{
			if (Group == 11)//红队蝙蝠
			{
				Mesh.Name = UCString("res/model/action/followboomred.dat");
			}
			else//蓝队蝙蝠
			{
				Mesh.Name = UCString("res/model/action/followboomblue.dat");
			}
			Mesh.Scale = uc3dxVector3(8.0f, 8.0f, 8.0f);
			Mesh.PlayAction(0);
		}
		//定时水炸弹
		else if (Type == 11)
		{
			Mesh.Name = UCString("res/model/action/timeboom.dat");
			Mesh.Scale = uc3dxVector3(4.0f, 4.0f, 4.0f);
			//Mesh.RenderMode =1;
			Mesh.PlayAction(0);
			Mesh.SetActionSpeed(2.0f);
		}
		//定时火火弹，爆炸以后生成大火球
		else if (Type == 12)
		{
			Mesh.Name = UCString("res/model/action/timefireboom.dat");
			Mesh.Scale = uc3dxVector3(4.0f, 4.0f, 4.0f);
			Mesh.PlayAction(0);
			Mesh.SetActionSpeed(2.0f);
		}
		//大火球波及圈
		else if (Type == 13)
		{
			Mesh.Name = UCString("res/model/action/bigfireball.dat");
			Mesh.Scale = uc3dxVector3(28.0f, 28.0f, 28.0f);
			Mesh.PlayAction(0);
		}
		//拿锅铲的厨师
		else if (Type == 14)
		{
			Mesh.Name = UCString("res/model/action/chief.dat");
			Mesh.Scale = uc3dxVector3(8.0f, 8.0f, 8.0f);
			Mesh.PlayAction(0);
		}
		//减速棒
		else if (Type == 15)
		{
			Mesh.Name = UCString("res/model/action/speeddownstick.dat");
			Mesh.Scale = uc3dxVector3(4.0f, 4.0f, 4.0f);
			//Mesh.RenderMode = 1;
			Mesh.PlayAction(0);
		}
		//闪光波
		else if (Type == 16)
		{
			Mesh.Name = UCString("res/model/action/shineshakeball.dat");
			Mesh.Scale = uc3dxVector3(40.0f, 40.0f, 40.0f);
			//Mesh.RenderMode = 1;
			Mesh.PlayAction(0);
		}
		//闪光弹(一定时间爆炸，产生闪光波波及圈)
		else if (Type == 17)
		{
			Mesh.Name = UCString("res/model/action/shineshakeboom.dat");
			Mesh.Scale = uc3dxVector3(2.0f, 2.0f, 2.0f);
			//Mesh.RenderMode = 1;
			Mesh.PlayAction(0);
		}
		//超级障碍机器(爆炸出一堆障碍雷，以及两个大火球)
		else if (Type == 18)
		{
			Mesh.Name = UCString("res/model/action/superboom.dat");
			Mesh.Scale = uc3dxVector3(7.0f, 7.0f, 7.0f);
			//Mesh.RenderMode = 1;
			Mesh.PlayAction(0);
		}
		//恶魔
		else if (Type == 19)
		{
			Mesh.Name = UCString("res/model/action/devil.dat");
			Mesh.Scale = uc3dxVector3(6.0f, 6.0f, 6.0f);
			//Mesh.RenderMode = 1;
			Mesh.PlayAction(0);
		}
		//飞行牢笼
		else if (Type == 20)
		{
			Mesh.Name = UCString("res/model/action/alarm.dat");
			Mesh.Scale = uc3dxVector3(6.0f, 6.0f, 6.0f);
			//Mesh.RenderMode = 1;
			Mesh.PlayAction(0);
		}
		Mesh.Pos = v;
		if (Type == 15 || Type == 19)
		{

		}
		else
			Mesh.Rot = User->Physics.CenterBody.Rot.Value();
		Mesh.Parent = Parent;

		Object.Bind(Manage);
		Object.Locate(&User->Physics.PortalObject);
	}
};

/*
地雷类道具

因为只判断自己的情况，需要做出同步删除
*/
class UCLandMine
{
public:
	UCMesh				Mesh;		//模型
	ucINT				Type;		//类型
	ucINT				Owner;		//主人
	ucINT				Index;		//编号
	ucINT				Time;		//还有多久消失
	ucINT				OwnerGroup; //所属组别

	UCLandMine(ucINT TypeIn, UCHuman* User, uc3dxVector3 v, UCMesh* Parent)//构造函数 
	{
		Type = TypeIn;
		Owner = User->ID;
		OwnerGroup = User->GroupID;
		Index = User->ItemID;
		User->ItemID++;

		Time = 3200;

		//假道具箱
		if (Type == 2)
		{
			Mesh.Name = UCString("res/model/action/xiangzi.dat");
			Mesh.Scale = uc3dxVector3(8.0f, 8.0f, 8.0f);
		}
		//香蕉
		else if (Type == 3)
		{
			Mesh.Name = UCString("res/model/action/xiangjiao.dat");
			Mesh.Scale = uc3dxVector3(1.25f, 1.25f, 1.25f);
			Mesh.PlayAction(0);
		}
		else if (Type == 6)//超级障碍雷
		{
			Mesh.Name = UCString("res/model/action/blockxiangzi.dat");
			Mesh.Scale = uc3dxVector3(1.3f, 1.3f, 1.3f);
			Mesh.PlayAction(0);
		}
		else if (Type == 7)//枷锁
		{
			Mesh.Name = UCString("res/model/action/jiasuo.dat");
			Mesh.Scale = uc3dxVector3(5.0f, 5.0f, 5.0f);
			Mesh.PlayAction(0);
			Time = 5000;
		}

		Mesh.Pos = v;
		Mesh.Parent = Parent;
	}
};
class UCLandOil
{
public:
	UCMesh				Mesh;		//模型
	ucINT				Type;		//类型
	ucINT				Owner;		//道具主人
	ucINT				LandGroup;	//道具存放主人所属队伍
	ucINT				Index;		//编号
	ucINT				Time;		//还有多久消失
	ucINT				OwnerGroup; //道具主人所属组别

	UCLandOil(ucINT TypeIn, UCHuman* User, UCHuman* IUser, uc3dxVector3 v, UCMesh* Parent)//构造函数 
	{
		Type = TypeIn;
		Owner = User->ID;
		LandGroup = IUser->GroupID;
		OwnerGroup = User->GroupID;
		Index = User->ItemID;
		User->ItemID++;

		if (Type == 1)
		{
			if (LandGroup == OwnerGroup)	//加速站自己人		
				Mesh.Name = UCString("res/model/action/oilstationred.dat");
			else//加速站敌人
				Mesh.Name = UCString("res/model/action/oilstationbad.dat");
			Mesh.Scale = uc3dxVector3(15.0f, 15.0f, 15.0f);
			Mesh.PlayAction(0);
			Time = 2000;
		}

		Mesh.Pos = v;
		Mesh.Parent = Parent;
	}
};

//道具箱
class UCPropBox
{
public:
	UCMesh				Mesh;		//模型
	ucINT				Time;		//还有多久重生

	//构造函数 
	UCPropBox(uc3dxVector3 v, UCMesh* Parent)
	{
		Mesh.Name = UCString("res/model/action/xiangzi.dat");
		Mesh.PlayAction(0,0.85f,0);
		Mesh.Scale = uc3dxVector3(10.0f, 10.0f, 10.0f);
		Mesh.Pos = v;
		Mesh.Parent = Parent;
	}

	void Unvisible()
	{
		Mesh.Visible = 0;
		Time = 240;		//五秒钟重生
	}

	void Update()
	{
		if (Mesh.Visible.Value == 0)
		{
			Time--;
			if (Time == 0)
			{
				Mesh.Visible = 1;
			}
		}
	}
};

#endif