#ifndef AVATAR
#define AVATAR

#define BOY_PATH		UCString("res/model/avatar/boy")
#define GIRL_PATH		UCString("res/model/avatar/girl")

#define CAR_PATH		UCString("res/model/avatar/car")

#define ACTION_PATH		UCString("res/model/avatar/action")

class UCAvatar : public UCMesh
{
public:
	UCMesh		Face;
	UCMesh		Hair;

	UCMesh		Body;
	UCMesh		Hand;

	UCMesh		Pant;
	UCMesh		Shoe;

	UCMesh		CarBody;

	UCMesh		Wheel[4];
	UCAvatar()
	{
		Face.Parent = &Body;
		Hair.Parent = &Body;

		Hand.Parent = &Body;

		Pant.Parent = &Body;
		Shoe.Parent = &Body;

		Body.Parent = this;

		CarBody.Parent = this;

		for (ucINT i = 0; i < 4; i++)
			Wheel[i].Parent = &CarBody;

		Body.Rot.y = 0.5f;
		Body.Pos.x = 18.0f;

		CarBody.Pos.z = 36.0f;
		CarBody.Rot.y = 0.5f;
	}
	ucVOID	ChangeModel(UCMaterial* MtlObject, ucINT SexID, ucCONST UCRProAvatarData& AvatarData)
	{
		UCString MeshPath;
		UCString SexPath;
		if (SexID == 0)
		{
			MeshPath = BOY_PATH;
			SexPath = UCString("boy");
		}
		else
		{
			MeshPath = GIRL_PATH;
			SexPath = UCString("girl");
		}

		UCRProAvatarData AvatarDataTemp = AvatarData;

		if (AvatarDataTemp.FaceID < 0)
			AvatarDataTemp.FaceID = 0;
		if (AvatarDataTemp.HairID < 0)
			AvatarDataTemp.HairID = 23;

		if (AvatarDataTemp.BodyID < 0)
			AvatarDataTemp.BodyID = 23;
		if (AvatarDataTemp.HandID < 0)
			AvatarDataTemp.HandID = 23;

		if (AvatarDataTemp.PantID < 0)
			AvatarDataTemp.PantID = 23;
		if (AvatarDataTemp.ShoeID < 0)
			AvatarDataTemp.ShoeID = 23;

		if (AvatarDataTemp.CarID < 0)
			AvatarDataTemp.CarID = 7020;

		Face.Name = MeshPath + UCString("/face/saiche_") + SexPath + UCString("_basicface_") + ITOS(AvatarDataTemp.FaceID) + UCString(".dat");
		Hair.Name = MeshPath + UCString("/hair/saiche_") + SexPath + UCString("_hair_") + ITOS(AvatarDataTemp.HairID) + UCString(".dat");
		
		Body.Name = MeshPath + UCString("/body/saiche_") + SexPath + UCString("_body_") + ITOS(AvatarDataTemp.BodyID) + UCString(".dat");
		Hand.Name = MeshPath + UCString("/hand/saiche_") + SexPath + UCString("_hand_") + ITOS(AvatarDataTemp.HandID) + UCString(".dat");
		
		Pant.Name = MeshPath + UCString("/pant/saiche_") + SexPath + UCString("_pant_") + ITOS(AvatarDataTemp.PantID) + UCString(".dat");
		Shoe.Name = MeshPath + UCString("/shoe/saiche_") + SexPath + UCString("_shose_") + ITOS(AvatarDataTemp.ShoeID) + UCString(".dat");

		CarBody.Name = CAR_PATH + UCString("/car") + ITOS(7000 + AvatarDataTemp.CarID) + UCString(".dat");

		for (ucINT i = 0; i < 4; i++)
			Wheel[i].Name = CAR_PATH + UCString("/luntai/car") + ITOS(7000 + AvatarDataTemp.CarID) + UCString("_luntai.dat");

		Face.SetMaterial(MtlObject, 1);
		Hair.SetMaterial(MtlObject, 1);

		Body.SetMaterial(MtlObject, 1);
		Hand.SetMaterial(MtlObject, 1);

		Pant.SetMaterial(MtlObject, 1);
		Shoe.SetMaterial(MtlObject, 1);

		CarBody.SetMaterial(MtlObject, 1);
		for (ucINT i = 0; i < 4; i++)
		{
			Wheel[i].SetMaterial(MtlObject, 1);
			Wheel[i].Visible = 0;
		}

		Face.AddActionFile(ACTION_PATH + UCString("/actionshow.dat"));
		Hair.AddActionFile(ACTION_PATH + UCString("/actionshow.dat"));

		Body.AddActionFile(ACTION_PATH + UCString("/actionshow.dat"));
		Hand.AddActionFile(ACTION_PATH + UCString("/actionshow.dat"));

		Pant.AddActionFile(ACTION_PATH + UCString("/actionshow.dat"));
		Shoe.AddActionFile(ACTION_PATH + UCString("/actionshow.dat"));

		Face.PlayAction(2);
		Hair.PlayAction(2);

		Body.PlayAction(2);
		Hand.PlayAction(2);

		Pant.PlayAction(2);
		Shoe.PlayAction(2);
	}
};

#endif