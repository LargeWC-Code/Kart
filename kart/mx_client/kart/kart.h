#ifndef _kart_H_
#define _kart_H_
#define SCREEN_CX	540
#define SCREEN_CY	960

//网关
#include "../../mx_server/container/public/common/game_gatebase.h"
//账号服务
#include "../../mx_server/container/public/common/game_accountbase.h"
//大厅服务 - 数据从homebase间接连接
#include "../../mx_server/container/private/common/game_homebase.h"
//匹配服务
#include "../../mx_server/container/private/common/game_matchbase.h"
//游戏服务
#include "../../mx_server/container/private/common/game_logicbase.h"

#include "code/login/ucregister.h"

#include "code/login/uclogin.h"

#include "code/home/uchome.h"

#include "code/battle/ucbattle.h"

class Ckart : public UCControl
{
public:
	UCRContainer_NET	RContainer_NET;
private:
	UCRegister*			m_pRegister;
	UCLogin*			m_pLogin;

	UCHome*				m_pHome;

	UCBattle*			m_pBattle;

	UCString			GateUrl;

	ucINT				HomeID;
	ucINT				MatchID;
private:
	UCRObjGameGate				RObjGameGate;

	UCRObjGameHome				RObjGameHome;
	UCRObjGameMatch				RObjGameMatch;

	UCRObjGameBattle			RObjGameBattle;

	UCRObjGameUserData			RObjGameUserData;
	UCRObjGameUserData_Ext		RObjGameUserData_Ext;
public:
	Ckart()
	{
		UCRunNewFiber(UCEvent(this, OnFiberInit), ucNULL);
	}
	~Ckart()
	{
		delete m_pBattle;
		delete m_pHome;

		delete m_pLogin;
		delete m_pRegister;
	}
	ucVOID OnFiberInit(UCObject* Object, UCEventArgs* Args)
	{
		Anchor = 15;
		Size = UCSize(SCREEN_CX, SCREEN_CY);

		HomeID = -1;
		MatchID = -1;

		UCGetApp()->Owner = UCString("Kart-Monitor");

		RContainer_NET.WS_Run(1);
		RContainer_NET.RunNewFiber(UCString("client"), UCGetTime(), ucTRUE, ucFALSE, 100);

		RContainer_NET.AppendClient(UCString("local//UCRObjGameGate.obj"), typeof(UCRObjGameGate), &RObjGameGate);

		RContainer_NET.AppendClient(UCString("local//UCRObjGameHome.obj"), typeof(UCRObjGameHome), &RObjGameHome);
		RContainer_NET.AppendClient(UCString("local//UCRObjGameMatch.obj"), typeof(UCRObjGameMatch), &RObjGameMatch);

		RContainer_NET.AppendClient(UCString("local//UCRObjGameBattle.obj"), typeof(UCRObjGameBattle), &RObjGameBattle);

		RContainer_NET.AppendClient(UCString("local//UCRObjGameUserData.obj"), typeof(UCRObjGameUserData), &RObjGameUserData);
		RContainer_NET.AppendClient(UCString("local//UCRObjGameUserData_Ext.obj"), typeof(UCRObjGameUserData_Ext), &RObjGameUserData_Ext);		

		GateUrl = UCString("ws://127.0.0.1:6800");

		m_pRegister = new UCRegister(&RObjGameHome);
		m_pRegister->OnRegisterSucceed = UCEvent(this, OnRegisterSucceed);

		m_pLogin = new UCLogin(&RObjGameHome);
		m_pLogin->OnRegister = UCEvent(this, OnRegister);
		m_pLogin->OnLoginSucceed = UCEvent(this, OnLoginSucceed);
		m_pLogin->Enable = 1;
		m_pLogin->Visible = 1;
		m_pLogin->OnLoginPrepare = UCEvent(this, OnLoginPrepare);
		m_pLogin->SelectUrl.OnSet += UCEvent(this, OnLoginSelectUrl);

		m_pHome = new UCHome(&RObjGameHome, &RObjGameMatch, &RObjGameBattle, &RObjGameUserData, &RObjGameUserData_Ext);
		m_pHome->Enter();
		m_pHome->OnLogout = UCEvent(this, OnLogout);
		m_pHome->OnMatchSucceed = UCEvent(this, OnMatchSucceed);

		m_pBattle = new UCBattle(&RObjGameBattle);
		m_pBattle->OnExit = UCEvent(this, OnBattleExit);

		AddControl(m_pRegister);
		AddControl(m_pLogin);

		AddControl(m_pHome);
		AddControl(m_pBattle);

		AutoUpdate(ucTRUE);
	}
	ucBOOL InitGateObj(ucCONST UCString& GateUrl)
	{
		if (!RObjGameGate.Linked)
		{
			ucINT Code = RObjGameGate.Link(GateUrl + UCString("//UCRObjGameGate.obj"));
			if (Code != 0)
			{
				WBox(UCString("RObjGameGate Link失败，code = ") + ITOS(Code));
				return ucFALSE;
			}

			HomeID = -1;
			UCString strRet = RObjGameGate.GetHomeID(HomeID);
			if (strRet != UCString("succeed"))
				return ucFALSE;
		}
		return ucTRUE;
	}
	ucVOID OnLoginSelectUrl(UCObject* Sender, UCEventArgs* e)
	{
		GateUrl = m_pLogin->SelectUrl.Value;
	}
	ucVOID OnLoginPrepare(UCObject* Sender, UCEventArgs* e)
	{
		InitHomeObj(GateUrl);
	}
	ucBOOL InitHomeObj(ucCONST UCString& GateUrl)
	{
		if (!InitGateObj(GateUrl))
			return ucFALSE;

		if (!RObjGameHome.Linked)
		{
			UCString GameHomeUrl = ITOS(HomeID, UCString("sm://home:%d"));
			UCString strGateGameHomeUrl = GateUrl + UCString("//") + GameHomeUrl;

			ucINT Code = RObjGameHome.Link(strGateGameHomeUrl + UCString("//UCRObjGameHome.obj"));
			if (Code != 0)
			{
				WBox(UCString("RObjGameHome Link失败，code = ") + ITOS(Code));
				return ucFALSE;
			}
		}

		return ucTRUE;
	}
	ucBOOL InitUserObj(ucCONST UCString& GateUrl, ucCONST UCGameUserID& GameUserID)
	{
		if (!RObjGameUserData.Linked || !RObjGameUserData_Ext.Linked)
		{
			UCString GameHomeUrl = ITOS(HomeID, UCString("sm://home:%d"));
			UCString strGateGameHomeUrl = GateUrl + UCString("//") + GameHomeUrl;

			ucINT Code = RObjGameUserData.Link(strGateGameHomeUrl + UCString("//UCRObjGameUserData/") + GameUserID.ToString() + UCString(".obj"), UCROBJECT_LINK_STABLE);
			if (Code != 0)
			{
				WBox(UCString("RObjGameUserData Link失败，code = ") + ITOS(Code));
				return ucFALSE;
			}

			Code = RObjGameUserData_Ext.Link(strGateGameHomeUrl + UCString("//UCRObjGameUserData/UCRObjGameUserData_Ext/") + GameUserID.ToString() + UCString(".obj"), UCROBJECT_LINK_STABLE);
			if (Code != 0)
			{
				WBox(UCString("UserData_Ext_RDObj Link失败，code = ") + ITOS(Code));
				return ucFALSE;
			}
		}

		if (!RObjGameMatch.Linked)
		{
			MatchID = -1;
			UCString strRet = RObjGameGate.GetMatchID(MatchID);
			if (strRet != UCString("succeed"))
				return ucFALSE;

			UCString GameMatchUrl = ITOS(HomeID, UCString("sm://match:%d"));
			UCString strGateGameMatchUrl = GateUrl + UCString("//") + GameMatchUrl;
			ucINT Code = RObjGameMatch.Link(strGateGameMatchUrl + UCString("//UCRObjGameMatch.obj"));
			if (Code != 0)
			{
				WBox(UCString("RObjGameMatch Link失败，code = ") + ITOS(Code));
				return ucFALSE;
			}
		}

		return ucTRUE;
	}
	ucVOID OnRegister(UCObject*, UCEventArgs*)
	{
		if (!InitHomeObj(GateUrl))
			return;

		m_pLogin->HideUI();
		m_pRegister->ShowUI();
	}
	ucVOID OnRegisterSucceed(UCObject*, UCEventArgs*)
	{
		m_pRegister->HideUI();
		m_pLogin->m_tbUsername.Text = m_pRegister->m_tbUsername.Text;
		m_pLogin->m_tbPassword.Text = m_pRegister->m_tbPassword.Text;

		m_pLogin->ShowUI();
	}
	ucVOID OnLoginSucceed(UCObject*, UCEventArgs*)
	{
		m_pLogin->HideUI();

		if (InitUserObj(GateUrl, m_pLogin->GameUserID))
		{
			//如果已经在服务器，直接进场内
			if (RObjGameUserData_Ext.BattleInfo.LogicID != -1 &&
				RObjGameUserData_Ext.BattleInfo.BattleID != -1 &&
				RObjGameUserData_Ext.BattleInfo.MatchID != -1)
				OnMatchSucceed(0, 0);
			else
				m_pHome->ShowUI(m_pLogin->GameUserID, GateUrl, m_pLogin->Token);
		}
	}
	ucVOID OnLogout(UCObject*, UCEventArgs*)
	{
		RObjGameHome.Dislink();
		RObjGameMatch.Dislink();

		RObjGameBattle.Dislink();

		RObjGameUserData.Dislink();
		RObjGameUserData_Ext.Dislink();

		RObjGameGate.Dislink();

		m_pLogin->ShowUI();
	}
	ucVOID OnMatchSucceed(UCObject*, UCEventArgs*)
	{
		ucINT Code = RObjGameBattle.Link(GateUrl + UCString("//sm://logic:") +
			ITOS(RObjGameUserData_Ext.BattleInfo.LogicID) + UCString("//UCRObjGameBattle/") +
			ITOS(RObjGameUserData_Ext.BattleInfo.BattleID) + UCString(".obj"));
		if (Code != 0)
		{
			WBox(UCString("Battle_RCObj Link失败，错误 = ") + ITOS(Code));
			return;
		}

		m_pHome->HideUI();
		m_pBattle->ShowUI(m_pLogin->GameUserID, m_pLogin->Token);
	}
	ucVOID OnBattleExit(UCObject*, UCEventArgs*)
	{
		m_pBattle->HideUI();
		m_pHome->ShowUI(m_pLogin->GameUserID, GateUrl, m_pLogin->Token);
	}
};

#endif	//_kart_H_