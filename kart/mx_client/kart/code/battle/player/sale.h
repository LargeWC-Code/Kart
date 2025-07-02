#ifndef _Sale_12_3_2007_14_18
#define _Sale_12_3_2007_14_18

int GetRemindSeconds(UCTime nBuyDay,int Seconds)
{
	UCTime CT;
	UCTimeSpan SP(0,0,0,Seconds);

	nBuyDay = nBuyDay+SP;

	UCTimeSpan sp;
	sp = nBuyDay-CT;
	return sp.GetTotalSeconds();
}

struct cVentInfo
{
	int ItemID;
	UCString VentFire;
	UCString VentSmallFire;
};
class UCVentData
{
public:
	UCIntVoidMap Items;
	UCString VentFire;
	UCString VentSmallFire;
	UCVentData()
	{
		UCSlk slk;
		if(slk.ReadFromFile("magicxbig/res/UpdateSlk/Setting/VentData.slk"))
		{
			UCString sID;
			int i=0;
			slk.GetString(3+i,1,sID);
			while (!sID.IsEmpty())
			{
				cVentInfo* pNew = new cVentInfo;
				pNew->ItemID = STOI(sID);

				slk.GetString(i+3,2,pNew->VentFire);
				slk.GetString(i+3,3,pNew->VentSmallFire);				

				Items.Add(pNew->ItemID,pNew);

				sID = UCString("");
				i++;
				slk.GetString(3+i,1,sID);

			}

		}
	}
	void Init(int ID)
	{		
		cVentInfo* p = GetVentInfo(ID);
		if(p)
		{
			VentFire = p->VentFire;
			VentSmallFire = p->VentSmallFire;
		}
		else
		{
			VentFire = UCString("");
			VentSmallFire = UCString("");
		}
	}
	cVentInfo* GetVentInfo(int ID)
	{
		int pos = Items.FindKey(ID);
		if(pos>=0)
		{
			return  (cVentInfo*)Items.GetValueAt(pos);
			
		}
		return 0;

	}
	
	~UCVentData()
	{
		for (int i=Items.GetSize()-1;i>=0;i--)
		{
			cVentInfo* p = (cVentInfo*)Items.GetValueAt(i);
			delete p;


		}
		Items.RemoveAll();

	}
};
UCVentData VentThings;
/*
struct ShopSample
{
public:
	ShopSample()
	{
		Material.Name = UCString("magicxbig/res/shader/humancartoon.material");
		SelectedRow = 0;
	}
	void Init(UCSceneCtl* Scene)
	{
		BodyMesh.Parent = Scene->Scene;
		BlankFace.Parent = Scene->Scene;
		for (int i=0;i<MAXS;i++)
		{
			WheelMesh[i].Parent = &BodyMesh;
			WheelMesh[i].MipLevel = 2;

		}

		BodyMesh.MipLevel = 2;
		BlankFace.MipLevel = 2;
	}
	void Init(UCItemIno* pItemInfo)
	{
		if(pItemInfo)
		{
			ProductType =pItemInfo->ItemType;
			ProductLocaleID = pItemInfo->ItemID;
			ProductExp = pItemInfo->ItemDescribe;
			BuyByScore = pItemInfo->ByScore;
			BuyLevel = pItemInfo->BuyLevel;
			ProductName = pItemInfo->ItemName;

		}
		
		
	}

	void Reset(UCItemIno* pItemInfo)
	{						
		if(pItemInfo)
		{
			
			BodyMesh.Name = pItemInfo->MeshPath;
			BodyMesh.Scale = uc3dxVector3(pItemInfo->SellScale,pItemInfo->SellScale,pItemInfo->SellScale);
			BodyMesh.PlayAction(0);

			BlankFace.Name=UCString("");
			BlankFace.Visible=0;
			if(ProductType==MeshHair||ProductType==MeshSuit)
			{
				if(pItemInfo->SexOn==0)
					BlankFace.Name=UCString("magicxbig/res/Model/AvtaMesh/boy/Face/saiche_boy_basicface_0.dat");
				else
					BlankFace.Name=UCString("magicxbig/res/Model/AvtaMesh/girl/Face/saiche_girl_basicface_0.dat");

				BlankFace.Visible=1;
			}
			SetWheelVisible();

		}
		else
		{
			BodyMesh.Name = UCString("");
			BlankFace.Name=UCString();
			for(int i=0;i<MAXS;i++)
			{
				WheelMesh[i].Visible = 0;
			}

		}
		
	}


	void SetMaterial()
	{
		BodyMesh.SetMaterial(&Material);
		BlankFace.SetMaterial(&Material);
	}

	void SetVisible(int Visible)
	{
		BodyMesh.Visible = Visible;
		BlankFace.Visible  = Visible;

	}
	void SetWheelVisible()
	{
		if(ProductType==MeshCar)
		{
			CarThings.Init(ProductLocaleID);
			for (int j=0;j<MAXS;j++)
			{
				WheelMesh[j].Name = CarThings.WheelName[j];
				WheelMesh[j].Pos = CarThings.WheelPosition[j];

				WheelMesh[j].SetMaterial(&Material);
				WheelMesh[j].Visible =1;
			}

		}
		else
		{
			for(int i=0;i<MAXS;i++)
			{
				WheelMesh[i].Visible = 0;
			}

		}		

	}



	UCMaterial	Material;

	UCMesh		BodyMesh;
	UCMesh		BlankFace;
	UCMesh		WheelMesh[MAXS];

	
	UCString	ProductName;
	UCString	ProductExp;
	cSaleInfo	Product_ServerID;
	int			SelectedRow;
	int			ProductType;
	int			ProductLocaleID;
	int			BuyByScore;
	int			BuyLevel;
};
*/
#endif

