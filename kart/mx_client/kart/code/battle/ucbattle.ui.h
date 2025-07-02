/********************************************************************
修改时间：	2021/04/09 11:08:55
文 件 名：	ucbattle.ui.h
编辑警告：	这是由界面编辑器自动生成的代码文件
			手动修改不保证编辑器能正确识别和恢复
*********************************************************************/
#ifndef _UCBattle_UI_H_
#define _UCBattle_UI_H_

class UCBattle_UI : public UCSceneCtl
{
public:
	UCImage m_imgWheel;
public:
	UCBattle_UI()	//构造函数
	{
		//自己的初始化
		{
			UIName = "界面编辑器类：UCBattle";
			SetNewRealSize(540.000000,960.000000);
			Anchor = 15;
		}

		//UCImage m_imgWheel;
		{
			m_imgWheel.UIName = "UCBattle::m_imgWheel";
			m_imgWheel.RealLocation = UCFPoint(14, 0);
			m_imgWheel.SetNewRealSize(512.000000,280.000000);
			m_imgWheel.Anchor = 8;
			m_imgWheel.Picture.Name = "res/ui/battle/wheel.tga";
			m_imgWheel.Alpha = 64;
		}

		//父子关系结构
		{
			this->AddControl(&m_imgWheel);
		}
	}
	~UCBattle_UI()	//析构函数
	{
	}
};

#endif //_UCBattle_UI_H_