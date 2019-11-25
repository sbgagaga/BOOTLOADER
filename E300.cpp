#include "stdafx.h"
#include "E300.h"
#include "stdio.h"


E300::E300(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	LoadAPI();
	E300time = new QTimer;
	E300time->stop();
	E300time->setInterval(50);
	connect(E300time, SIGNAL(timeout()), this, SLOT(on_timer_timeout()));
}

void E300::LoadAPI()
{
	InitializePointers();
	if (!LoadDllHandle())
	{
		::MessageBox(nullptr, TEXT("没有找到.dll文件"), TEXT("Error！"), MB_ICONERROR);
		return;
	}
	// Loads API functions
	//
	m_pRemoveClient = (fpRemoveClient)GetFunction("LIN_RemoveClient");
	m_pResetClient = (fpResetClient)GetFunction("LIN_ResetClient");
	m_pConnectClient = (fpConnectClient)GetFunction("LIN_ConnectClient");
	m_pDisconnectClient = (fpDisconnectClient)GetFunction("LIN_DisconnectClient");
	m_pResetHardware = (fpResetHardware)GetFunction("LIN_ResetHardware");
	m_pSuspendKeepAlive = (fpSuspendKeepAlive)GetFunction("LIN_SuspendKeepAlive");
	m_pResumeKeepAlive = (fpResumeKeepAlive)GetFunction("LIN_ResumeKeepAlive");
	m_pResetHardwareConfig = (fpResetHardwareConfig)GetFunction("LIN_ResetHardwareConfig");
	m_pSuspendSchedule = (fpSuspendSchedule)GetFunction("LIN_SuspendSchedule");
	m_pResumeSchedule = (fpResumeSchedule)GetFunction("LIN_ResumeSchedule");
	m_pXmtWakeUp = (fpXmtWakeUp)GetFunction("LIN_XmtWakeUp");
	m_pDeleteSchedule = (fpDeleteSchedule)GetFunction("LIN_DeleteSchedule");
	m_pStartSchedule = (fpStartSchedule)GetFunction("LIN_StartSchedule");
	m_pRegisterClient = (fpRegisterClient)GetFunction("LIN_RegisterClient");
	m_pIdentifyHardware = (fpIdentifyHardware)GetFunction("LIN_IdentifyHardware");
	m_pSetClientParam = (fpSetClientParam)GetFunction("LIN_SetClientParam");
	m_pGetClientParam = (fpGetClientParam)GetFunction("LIN_GetClientParam");
	m_pSetClientFilter = (fpSetClientFilter)GetFunction("LIN_SetClientFilter");
	m_pGetClientFilter = (fpGetClientFilter)GetFunction("LIN_GetClientFilter");
	m_pRead = (fpRead)GetFunction("LIN_Read");
	m_pReadMulti = (fpReadMulti)GetFunction("LIN_ReadMulti");
	m_pWrite = (fpWrite)GetFunction("LIN_Write");
	m_pInitializeHardware = (fpInitializeHardware)GetFunction("LIN_InitializeHardware");
	m_pGetAvailableHardware = (fpGetAvailableHardware)GetFunction("LIN_GetAvailableHardware");
	m_pSetHardwareParam = (fpSetHardwareParam)GetFunction("LIN_SetHardwareParam");
	m_pGetHardwareParam = (fpGetHardwareParam)GetFunction("LIN_GetHardwareParam");
	m_pRegisterFrameId = (fpRegisterFrameId)GetFunction("LIN_RegisterFrameId");
	m_pSetFrameEntry = (fpSetFrameEntry)GetFunction("LIN_SetFrameEntry");
	m_pGetFrameEntry = (fpGetFrameEntry)GetFunction("LIN_GetFrameEntry");
	m_pUpdateByteArray = (fpUpdateByteArray)GetFunction("LIN_UpdateByteArray");
	m_pStartKeepAlive = (fpStartKeepAlive)GetFunction("LIN_StartKeepAlive");
	m_pSetSchedule = (fpSetSchedule)GetFunction("LIN_SetSchedule");
	m_pGetSchedule = (fpGetSchedule)GetFunction("LIN_GetSchedule");
	m_pSetScheduleBreakPoint = (fpSetScheduleBreakPoint)GetFunction("LIN_SetScheduleBreakPoint");
	m_pStartAutoBaud = (fpStartAutoBaud)GetFunction("LIN_StartAutoBaud");
	m_pGetStatus = (fpGetStatus)GetFunction("LIN_GetStatus");
	m_pCalculateChecksum = (fpCalculateChecksum)GetFunction("LIN_CalculateChecksum");
	m_pGetVersion = (fpGetVersion)GetFunction("LIN_GetVersion");
	m_pGetVersionInfo = (fpGetVersionInfo)GetFunction("LIN_GetVersionInfo");
	m_pGetErrorText = (fpGetErrorText)GetFunction("LIN_GetErrorText");
	m_pGetPID = (fpGetPID)GetFunction("LIN_GetPID");
	m_pGetTargetTime = (fpGetTargetTime)GetFunction("LIN_GetTargetTime");
	m_pSetResponseRemap = (fpSetResponseRemap)GetFunction("LIN_SetResponseRemap");
	m_pGetResponseRemap = (fpGetResponseRemap)GetFunction("LIN_GetResponseRemap");
	m_pGetSystemTime = (fpGetSystemTime)GetFunction("LIN_GetSystemTime");

	// Checks that all functions are loaded
	m_bWasLoaded = m_pRemoveClient && m_pResetClient && m_pConnectClient && m_pDisconnectClient &&
		m_pResetHardware && m_pSuspendKeepAlive && m_pResumeKeepAlive && m_pResetHardwareConfig &&
		m_pSuspendSchedule && m_pResumeSchedule && m_pXmtWakeUp && m_pDeleteSchedule && m_pStartSchedule &&
		m_pRegisterClient && m_pIdentifyHardware && m_pSetClientParam && m_pGetClientParam && m_pSetClientFilter &&
		m_pGetClientFilter && m_pRead && m_pReadMulti && m_pWrite && m_pInitializeHardware && m_pGetAvailableHardware &&
		m_pSetHardwareParam && m_pGetHardwareParam && m_pRegisterFrameId && m_pSetFrameEntry && m_pGetFrameEntry &&
		m_pUpdateByteArray && m_pStartKeepAlive && m_pSetSchedule && m_pGetSchedule && m_pSetScheduleBreakPoint &&
		m_pStartAutoBaud && m_pGetStatus && m_pCalculateChecksum && m_pGetVersion && m_pGetVersionInfo &&
		m_pGetErrorText && m_pGetPID && m_pGetTargetTime && m_pSetResponseRemap && m_pGetResponseRemap &&
		m_pGetSystemTime;

	// If the API was not loaded (Wrong version), an error message is shown.
	//
	if (m_bWasLoaded == NULL)
		::MessageBox(nullptr, TEXT("功能未加载成功"), TEXT("Error！"), MB_ICONERROR);
	else
	{
		Display(u8"加载成功");
	}
}

void E300::InitializePointers()
{
	m_pRemoveClient = nullptr;
	m_pResetClient = nullptr;
	m_pConnectClient = nullptr;
	m_pDisconnectClient = nullptr;
	m_pResetHardware = nullptr;
	m_pSuspendKeepAlive = nullptr;
	m_pResumeKeepAlive = nullptr;
	m_pResetHardwareConfig = nullptr;
	m_pSuspendSchedule = nullptr;
	m_pResumeSchedule = nullptr;
	m_pXmtWakeUp = nullptr;
	m_pDeleteSchedule = nullptr;
	m_pStartSchedule = nullptr;
	m_pRegisterClient = nullptr;
	m_pIdentifyHardware = nullptr;
	m_pSetClientParam = nullptr;
	m_pGetClientParam = nullptr;
	m_pSetClientFilter = nullptr;
	m_pGetClientFilter = nullptr;
	m_pRead = nullptr;
	m_pReadMulti = nullptr;
	m_pWrite = nullptr;
	m_pInitializeHardware = nullptr;
	m_pGetAvailableHardware = nullptr;
	m_pSetHardwareParam = nullptr;
	m_pGetHardwareParam = nullptr;
	m_pRegisterFrameId = nullptr;
	m_pSetFrameEntry = nullptr;
	m_pGetFrameEntry = nullptr;
	m_pUpdateByteArray = nullptr;
	m_pStartKeepAlive = nullptr;
	m_pSetSchedule = nullptr;
	m_pGetSchedule = nullptr;
	m_pSetScheduleBreakPoint = nullptr;
	m_pStartAutoBaud = nullptr;
	m_pGetStatus = nullptr;
	m_pCalculateChecksum = nullptr;
	m_pGetVersion = nullptr;
	m_pGetVersionInfo = nullptr;
	m_pGetErrorText = nullptr;
	m_pGetPID = nullptr;
	m_pGetTargetTime = nullptr;
	m_pSetResponseRemap = nullptr;
	m_pGetResponseRemap = nullptr;
	m_pGetSystemTime = nullptr;
}

BYTE E300::LoadDllHandle()
{
	m_hDll = LoadLibraryA("PLinApi");
	if (m_bWasLoaded)
		return true;
	return (m_hDll != nullptr);
}

E300::~E300()
{
	//delete ui;
}

void E300::Display(QString msg)
{
	ui.textBrowser->append(msg);
}

void E300::on_refresh_clicked()
{
	HLINHW* lwHwHandles;
	WORD lwBuffSize;
	QString str;
	int lwCount, lnHwType, lnDevNo, lnChannel, i;
	TLINError lLINErr;
	lwHwHandles = nullptr;
	HLINHW lwHw;
	lwCount = 0;
	lLINErr = GetAvailableHardware(lwHwHandles, 0, &lwCount);
	if (lwCount == 0)
	{
		lwCount = 16;
	}
	lwHwHandles = new HLINHW[lwCount];
	Hwtemp = new HLINHW[lwCount];
	lwBuffSize = (WORD)(lwCount * sizeof(HLINHW));
	lLINErr = GetAvailableHardware(lwHwHandles, lwBuffSize, &lwCount);
	if (lLINErr == errOK)
	{
		ui.selectHW->clear();
		if (lwCount == 0)
		{
			ui.selectHW->addItem(u8"没有找到硬件");
			return;
		}
		else
		{
			for (i = 0; i < lwCount; i++)
			{
				// Get the handle of the hardware.
				lwHw = lwHwHandles[i];
				// Read the type of the hardware with the handle lwHw.
				GetHardwareParam(lwHw, hwpType, &lnHwType, 0);
				// Read the device number of the hardware with the handle lwHw.
				GetHardwareParam(lwHw, hwpDeviceNumber, &lnDevNo, 0);
				// Read the channel number of the hardware with the handle lwHw.
				GetHardwareParam(lwHw, hwpChannelNumber, &lnChannel, 0);
				switch (lnHwType)
				{
				case LIN_HW_TYPE_USB_PRO:
					str.append("PCAN-USB Pro");
					break;
				case LIN_HW_TYPE_USB_PRO_FD:
					// Show as unknown hardware
					str.append("PCAN-USB Pro FD");
					break;
				case LIN_HW_TYPE_PLIN_USB:
					// Show as unknown hardware
					str.append("PLIN-USB");
					break;
				default:
					// Show as unknown hardware
					str.append("Unknown");
					break;
				}
				str.append(" - dev.");
				str.append(QString::number(lnDevNo));
				str.append(", chan.");
				str.append(QString::number(lnChannel));
				ui.selectHW->addItem(str);
				str.clear();
				Hwtemp[i] = lwHw;
			}
		}
	}
	else
	{
		Display(u8"加载失败");
		return;
	}
	delete lwHwHandles;
}

void E300::on_connect_clicked()
{
	unsigned long long filter = 0xFFFFFFFFFFFFFFFF;
	HLINHW HW;
	QString str;
	if (ui.selectHW->currentText() == u8"没有找到硬件" || ui.selectHW->currentText().isEmpty())
	{
		Display(u8"请选择硬件");
		return;
	}
	else
	{
		HW = Hwtemp[ui.selectHW->currentIndex()];
		m_LastLINErr = RegisterClient("pin", NULL, &m_hClient);
		if (m_LastLINErr == errOK)
		{
			Display(u8"注册成功");
		}
		else
		{
			Display(u8"注册失败");
			return;
		}
		m_LastLINErr = ConnectClient(m_hClient, HW);
		if (m_LastLINErr == errOK)
		{
			m_hHw = HW;
			m_LastLINErr = InitializeHardware(m_hClient, m_hHw, modMaster, ui.selectbaud->currentText().toInt());
			SetClientFilter(m_hClient, m_hHw, filter);
			GetClientFilter(m_hClient, m_hHw, &filter);
		}
		else
		{
			Display(u8"连接失败");
			str = QString::number(m_LastLINErr);
			Display(u8"错误代码： " + str);
			return;
		}
	}
	ui.stop->setEnabled(TRUE);
	ui.connect->setEnabled(FALSE);
	ui.selectHW->setEnabled(FALSE);
	ui.selectbaud->setEnabled(FALSE);
	ui.refresh->setEnabled(FALSE);
	connectflag = 1;
	Display(u8"连接成功");
}

void E300::on_stop_clicked()
{
	if (m_hClient != NULL || m_hHw != NULL)
	{
		DisconnectClient(m_hClient, m_hHw);
		RemoveClient(m_hClient);
		m_hClient = NULL;
		m_hHw != NULL;
	}
	ui.stop->setEnabled(FALSE);
	ui.connect->setEnabled(TRUE);
	ui.selectHW->setEnabled(TRUE);
	ui.selectbaud->setEnabled(TRUE);
	ui.refresh->setEnabled(TRUE);
	connectflag = 0;
	Display(u8"断开连接");
}

void E300::on_send_clicked()
{

}

void E300::on_clear_clicked()
{
	ui.textBrowser->clear();
}

void E300::Write(BYTE id, BYTE dir, BYTE* buf)
{
	TLINMsg msg;
	if (!connectflag)
	{
		Display(u8"请连接硬件");
		return;
	}
	msg.FrameId = CalculatePID(id);
	msg.Direction = dir;
	msg.ChecksumType = cstEnhanced;
	msg.Length = 8;
	for (int i = 0; i < 8; i++)
	{
		msg.Data[i] = buf[i];
	}
	CalculateChecksum(&msg);
	Write(m_hClient, m_hHw, &msg);
}

void E300::Write3C(BYTE* buf)
{
	TLINMsg msg;
	if (!connectflag)
	{
		Display(u8"请连接硬件");
		return;
	}
	msg.FrameId = 0x3c;
	msg.Direction = Publisher;
	msg.ChecksumType = cstClassic;
	msg.Length = 8;
	for (int i = 0; i < 8; i++)
	{
		msg.Data[i] = buf[i];
	}
	CalculateChecksum(&msg);
	Write(m_hClient, m_hHw, &msg);
}

void E300::Write3DHead()
{
	TLINMsg msg;
	if (!connectflag)
	{
		Display(u8"请连接硬件");
		return;
	}
	msg.FrameId = 0x7D;
	msg.Direction = Subscriber;
	msg.ChecksumType = cstClassic;
	msg.Length = 8;
	for (int i = 0; i < 8; i++)
	{
		msg.Data[i] = 0;
	}
	CalculateChecksum(&msg);
	Write(m_hClient, m_hHw, &msg);
}

void E300::ReadMsg()
{
	TLINRcvMsg RcvMsg;
	TLINError Err = errOK;
	QString temp;
	while (!(Err & errRcvQueueEmpty))
	{
		Err = Read(m_hClient, &RcvMsg);
		if (Err != errOK)
		{
			continue;
		}
		if (RcvMsg.Type != mstStandard)
		{
			continue;
		}
		if (RcvMsg.ErrorFlags != 0)
		{
			continue;
		}
		temp.append("ID ");
		temp.append(QString::number(RcvMsg.FrameId & 0x3F, 16));
		temp.append(":");
		for (int i = 0; i < RcvMsg.Length; i++)
		{
			temp.append("0x");
			if (RcvMsg.Data[i] == 0)
			{
				temp.append("00");
			}
			else if (RcvMsg.Data[i] >> 4 == 0)
			{
				temp.append("0");
				temp.append(QString::number(RcvMsg.Data[i], 16).toUpper());
			}
			else
			{
				temp.append(QString::number(RcvMsg.Data[i], 16).toUpper());
			}
			temp.append(" ");
		}
		temp.append("\n");
		Display(temp);
	}
}

void E300::WriteHead(BYTE id, BYTE dir)
{
	TLINMsg msg;
	msg.FrameId = CalculatePID(id);
	msg.Direction = dir;
	msg.ChecksumType = cstEnhanced;
	msg.Length = 8;
	for (int i = 0; i < 8; i++)
	{
		msg.Data[i] = 0;
	}
	CalculateChecksum(&msg);
	Write(m_hClient, m_hHw, &msg);
}

void E300::ReadMsg(BYTE* buf)
{
	TLINRcvMsg RcvMsg;
	TLINError Err = errOK;
	QString temp = "";
	while (!(Err & errRcvQueueEmpty))
	{
		Err = Read(m_hClient, &RcvMsg);
		if (Err != 0)
		{
			continue;
		}
		if (RcvMsg.Type != mstStandard)
		{
			continue;
		}
		if (RcvMsg.ErrorFlags != 0)
		{
			continue;
		}
		temp.append("ID ");
		temp.append(QString::number(RcvMsg.FrameId & 0x3F, 16));
		temp.append(":");
		for (int i = 0; i < RcvMsg.Length; i++)
		{
			buf[i] = RcvMsg.Data[i];
			temp.append("0x");
			if (RcvMsg.Data[i] == 0)
			{
				temp.append("00");
			}
			else if (RcvMsg.Data[i] >> 4 == 0)
			{
				temp.append("0");
				temp.append(QString::number(RcvMsg.Data[i], 16).toUpper());
			}
			else
			{
				temp.append(QString::number(RcvMsg.Data[i], 16).toUpper());
			}
			temp.append(" ");
		}
		temp.append("\n");
		Display(temp);
	}
}

BYTE E300::CalculatePID(BYTE ID)
{
	int P0, P1;
	P0 = (ID & 0x01) ^ ((ID & 0x02) >> 1) ^ ((ID & 0x04) >> 2) ^ ((ID & 0x10) >> 4);
	P1 = (((ID & 0x02) >> 1) ^ ((ID & 0x08) >> 3) ^ ((ID & 0x10) >> 4) ^ ((ID & 0x20) >> 5));
	P1 = P1 ^ 0x01;
	ID = ID | (P0 << 6) | (P1 << 7);

	return ID;
}

void E300::on_E300_start_clicked()
{
	if (!connectflag)
	{
		Display(u8"请连接硬件");
		return;
	}
	ui.E300_start->setEnabled(FALSE);
	ui.E300_stop->setEnabled(TRUE);


	ui.Menu_short->setStyleSheet(off_led);
	ui.Menu_long->setStyleSheet(off_led);

	ui.Mode_short->setStyleSheet(off_led);
	ui.Mode_long->setStyleSheet(off_led);

	ui.ADAS_short->setStyleSheet(off_led);
	ui.ADAS_long->setStyleSheet(off_led);

	ui.Answer_short->setStyleSheet(off_led);
	ui.Answer_long->setStyleSheet(off_led);

	ui.Speech_short->setStyleSheet(off_led);
	ui.Speech_long->setStyleSheet(off_led);

	ui.DIST_short->setStyleSheet(off_led);
	ui.DIST_long->setStyleSheet(off_led);

	ui.RESPlus_short->setStyleSheet(off_led);
	ui.RESPlus_long->setStyleSheet(off_led);

	ui.Crusie_short->setStyleSheet(off_led);
	ui.Crusie_long->setStyleSheet(off_led);

	ui.SETReduce_short->setStyleSheet(off_led);
	ui.SETReduce_long->setStyleSheet(off_led);

	ui.Return_short->setStyleSheet(off_led);
	ui.Return_long->setStyleSheet(off_led);

	ui.Up_short->setStyleSheet(off_led);
	ui.Up_long->setStyleSheet(off_led);

	ui.Down_short->setStyleSheet(off_led);
	ui.Down_long->setStyleSheet(off_led);

	ui.SeekReduce_short->setStyleSheet(off_led);
	ui.SeekReduce_long->setStyleSheet(off_led);

	ui.OK_short->setStyleSheet(off_led);
	ui.OK_long->setStyleSheet(off_led);

	ui.SeekPlus_short->setStyleSheet(off_led);
	ui.SeekPlus_long->setStyleSheet(off_led);

	ui.VolPlus_short->setStyleSheet(off_led);
	ui.VolPlus_long->setStyleSheet(off_led);

	ui.Mute_short->setStyleSheet(off_led);
	ui.Mute_long->setStyleSheet(off_led);

	ui.VolReduce_short->setStyleSheet(off_led);
	ui.VolReduce_long->setStyleSheet(off_led);

	ui.DiagInfoSW_short->setStyleSheet(off_led);
	ui.DiagInfoSW_long->setStyleSheet(off_led);
	DIDdiagnose();
	E300time->start();

}

void E300::on_E300_stop_clicked()
{
	ui.E300_start->setEnabled(TRUE);
	ui.E300_stop->setEnabled(FALSE);
	E300time->stop();
}

void E300::DIDdiagnose()
{
	UINT8 buf[8] = {0},len=10;
	QString str="";
	ui.DIDHW->clear();
	ui.DIDSW->clear();
	if (!connectflag)
	{
		Display("请连接硬件");
		return;
	}
	buf[0] = 0x21;
	buf[1] = 0x03;
	buf[2] = 0x22;
	buf[3] = 0xFF;
	buf[4] = 0x00;
	Write3C(buf);
	Sleep(10);
	ReadMsg();
	Sleep(30);
	Write3DHead();
	Sleep(10);
	ReadMsg(buf);
	for (int i = 0; i < 2; i++)
	{
		str.append(buf[6 + i]);
	}
	len -= 2;
	while (buf[0] == 0x21)
	{
		Write3DHead();
		Sleep(10);
		buf[0] = 0;
		ReadMsg(buf);
		for (int i = 0; i < 6; i++)
		{
			str.append(buf[2 + i]);
			len--;
			if (len == 0)
			{
				break;
			}
		}
		if (len == 0)
		{
			break;
		}
	}
	ui.DIDSW->setText(str);

	str.clear();
	len = 10;

	buf[0] = 0x21;
	buf[1] = 0x03;
	buf[2] = 0x22;
	buf[3] = 0xFF;
	buf[4] = 0x01;
	Write3C(buf);
	Sleep(10);
	ReadMsg();
	Sleep(30);
	Write3DHead();
	Sleep(10);
	ReadMsg(buf);
	for (int i = 0; i < 2; i++)
	{
		str.append(buf[6 + i]);
	}
	len -= 2;
	while (buf[0] == 0x21)
	{
		Write3DHead();
		Sleep(10);
		buf[0] = 0;
		ReadMsg(buf);
		for (int i = 0; i < 6; i++)
		{
			str.append(buf[2 + i]);
			len--;
			if (len == 0)
			{
				break;
			}
		}
		if (len == 0)
		{
			break;
		}
	}
	ui.DIDHW->setText(str);

}

void E300::on_timer_timeout()
{
	UINT8 Menu, Mode, ADAS, Answer, Speech, DIST, RESPlus, Crusie, SETReduce, Return, \
		Up, Down, SeekReduce, OK, SeekPlus, VolPlus, Mute, VolReduce, DiagInfoSW;
	uint8_t buf[8] = {0};
	WriteHead(0x19, Subscriber);
	Sleep(10);
	ReadMsg(buf);
	Return = buf[0] & 0x03;
	Menu = (buf[0] & 0x0c) >> 4;
	Up = (buf[0] & 0x30) >> 8;
	Down = (buf[0] & 0xC0) >> 12;

	OK = buf[1] & 0x03;
	Speech = (buf[1] & 0x0c) >> 4;
	Mode = (buf[1] & 0x30) >> 8;

	VolPlus = (buf[2] & 0x0c) >> 4;
	VolReduce = (buf[2] & 0x30) >> 8;
	Mute = (buf[2] & 0xC0) >> 12;

	SeekPlus = buf[3] & 0x03;
	SeekReduce = (buf[3] & 0x0c) >> 4;
	Crusie = (buf[3] & 0x30) >> 8;

	RESPlus = buf[4] & 0x03;
	SETReduce = (buf[4] & 0x0c) >> 4;
	DIST = (buf[4] & 0x30) >> 8;

	ADAS = (buf[6] & 0x0c) >> 4;

	DiagInfoSW = buf[7] & 0x07;
	Answer= (buf[7] & 0x18) >> 3;

	switch (Menu)
	{
	case 0:ui.Menu_short->setStyleSheet(off_led); ui.Menu_long->setStyleSheet(off_led);break;
	case 1:ui.Menu_short->setStyleSheet(off_led); ui.Menu_long->setStyleSheet(on_led); break;
	case 2:ui.Menu_short->setStyleSheet(on_led); ui.Menu_long->setStyleSheet(off_led); break;
	}

	switch (Mode)
	{
	case 0:ui.Mode_short->setStyleSheet(off_led); ui.Mode_long->setStyleSheet(off_led); break;
	case 1:ui.Mode_short->setStyleSheet(off_led); ui.Mode_long->setStyleSheet(on_led); break;
	case 2:ui.Mode_short->setStyleSheet(on_led); ui.Mode_long->setStyleSheet(off_led); break;
	}

	switch (ADAS)
	{
	case 0:ui.ADAS_short->setStyleSheet(off_led); ui.ADAS_long->setStyleSheet(off_led); break;
	case 1:ui.ADAS_short->setStyleSheet(off_led); ui.ADAS_long->setStyleSheet(on_led); break;
	case 2:ui.ADAS_short->setStyleSheet(on_led); ui.ADAS_long->setStyleSheet(off_led); break;
	}

	switch (Answer)
	{
	case 0:ui.Answer_short->setStyleSheet(off_led); ui.Answer_long->setStyleSheet(off_led); break;
	case 1:ui.Answer_short->setStyleSheet(off_led); ui.Answer_long->setStyleSheet(on_led); break;
	case 2:ui.Answer_short->setStyleSheet(on_led); ui.Answer_long->setStyleSheet(off_led); break;
	}

	switch (Speech)
	{
	case 0:ui.Speech_short->setStyleSheet(off_led); ui.Speech_long->setStyleSheet(off_led); break;
	case 1:ui.Speech_short->setStyleSheet(off_led); ui.Speech_long->setStyleSheet(on_led); break;
	case 2:ui.Speech_short->setStyleSheet(on_led); ui.Speech_long->setStyleSheet(off_led); break;
	}

	switch (DIST)
	{
	case 0:ui.DIST_short->setStyleSheet(off_led); ui.DIST_long->setStyleSheet(off_led); break;
	case 1:ui.DIST_short->setStyleSheet(off_led); ui.DIST_long->setStyleSheet(on_led); break;
	case 2:ui.DIST_short->setStyleSheet(on_led); ui.DIST_long->setStyleSheet(off_led); break;
	}

	switch (RESPlus)
	{
	case 0:ui.RESPlus_short->setStyleSheet(off_led); ui.RESPlus_long->setStyleSheet(off_led); break;
	case 1:ui.RESPlus_short->setStyleSheet(off_led); ui.RESPlus_long->setStyleSheet(on_led); break;
	case 2:ui.RESPlus_short->setStyleSheet(on_led); ui.RESPlus_long->setStyleSheet(off_led); break;
	}

	switch (Crusie)
	{
	case 0:ui.Crusie_short->setStyleSheet(off_led); ui.Crusie_long->setStyleSheet(off_led); break;
	case 1:ui.Crusie_short->setStyleSheet(off_led); ui.Crusie_long->setStyleSheet(on_led); break;
	case 2:ui.Crusie_short->setStyleSheet(on_led); ui.Crusie_long->setStyleSheet(off_led); break;
	}

	switch (SETReduce)
	{
	case 0:ui.SETReduce_short->setStyleSheet(off_led); ui.SETReduce_long->setStyleSheet(off_led); break;
	case 1:ui.SETReduce_short->setStyleSheet(off_led); ui.SETReduce_long->setStyleSheet(on_led); break;
	case 2:ui.SETReduce_short->setStyleSheet(on_led); ui.SETReduce_long->setStyleSheet(off_led); break;
	}

	switch (Return)
	{
	case 0:ui.Return_short->setStyleSheet(off_led); ui.Return_long->setStyleSheet(off_led); break;
	case 1:ui.Return_short->setStyleSheet(off_led); ui.Return_long->setStyleSheet(on_led); break;
	case 2:ui.Return_short->setStyleSheet(on_led); ui.Return_long->setStyleSheet(off_led); break;
	}

	switch (Up)
	{
	case 0:ui.Up_short->setStyleSheet(off_led); ui.Up_long->setStyleSheet(off_led); break;
	case 1:ui.Up_short->setStyleSheet(off_led); ui.Up_long->setStyleSheet(on_led); break;
	case 2:ui.Up_short->setStyleSheet(on_led); ui.Up_long->setStyleSheet(off_led); break;
	}

	switch (Down)
	{
	case 0:ui.Down_short->setStyleSheet(off_led); ui.Up_long->setStyleSheet(off_led); break;
	case 1:ui.Down_short->setStyleSheet(off_led); ui.Up_long->setStyleSheet(on_led); break;
	case 2:ui.Up_short->setStyleSheet(on_led); ui.Up_long->setStyleSheet(off_led); break;
	}

	switch (SeekReduce)
	{
	case 0:ui.SeekReduce_short->setStyleSheet(off_led); ui.SeekReduce_long->setStyleSheet(off_led); break;
	case 1:ui.SeekReduce_short->setStyleSheet(off_led); ui.SeekReduce_long->setStyleSheet(on_led); break;
	case 2:ui.SeekReduce_short->setStyleSheet(on_led); ui.SeekReduce_long->setStyleSheet(off_led); break;
	}

	switch (OK)
	{
	case 0:ui.OK_short->setStyleSheet(off_led); ui.OK_long->setStyleSheet(off_led); break;
	case 1:ui.OK_short->setStyleSheet(off_led); ui.OK_long->setStyleSheet(on_led); break;
	case 2:ui.OK_short->setStyleSheet(on_led); ui.OK_long->setStyleSheet(off_led); break;
	}

	switch (SeekPlus)
	{
	case 0:ui.SeekPlus_short->setStyleSheet(off_led); ui.SeekPlus_long->setStyleSheet(off_led); break;
	case 1:ui.SeekPlus_short->setStyleSheet(off_led); ui.SeekPlus_long->setStyleSheet(on_led); break;
	case 2:ui.SeekPlus_short->setStyleSheet(on_led); ui.SeekPlus_long->setStyleSheet(off_led); break;
	}

	switch (VolPlus)
	{
	case 0:ui.VolPlus_short->setStyleSheet(off_led); ui.VolPlus_long->setStyleSheet(off_led); break;
	case 1:ui.VolPlus_short->setStyleSheet(off_led); ui.VolPlus_long->setStyleSheet(on_led); break;
	case 2:ui.VolPlus_short->setStyleSheet(on_led); ui.VolPlus_long->setStyleSheet(off_led); break;
	}

	switch (Mute)
	{
	case 0:ui.Mute_short->setStyleSheet(off_led); ui.Mute_long->setStyleSheet(off_led); break;
	case 1:ui.Mute_short->setStyleSheet(off_led); ui.Mute_long->setStyleSheet(on_led); break;
	case 2:ui.Mute_short->setStyleSheet(on_led); ui.Mute_long->setStyleSheet(off_led); break;
	}

	switch (VolReduce)
	{
	case 0:ui.VolReduce_short->setStyleSheet(off_led); ui.VolReduce_long->setStyleSheet(off_led); break;
	case 1:ui.VolReduce_short->setStyleSheet(off_led); ui.VolReduce_long->setStyleSheet(on_led); break;
	case 2:ui.VolReduce_short->setStyleSheet(on_led); ui.VolReduce_long->setStyleSheet(off_led); break;
	}

	switch (DiagInfoSW)
	{
	case 0:ui.DiagInfoSW_short->setStyleSheet(off_led); ui.DiagInfoSW_long->setStyleSheet(off_led); break;
	case 2:ui.DiagInfoSW_short->setStyleSheet(off_led); ui.DiagInfoSW_long->setStyleSheet(on_led); break;
	case 3:ui.DiagInfoSW_short->setStyleSheet(on_led); ui.DiagInfoSW_long->setStyleSheet(off_led); break;
	}
}























































FARPROC E300::GetFunction(char* szName)
{
	// There is no DLL loaded
	//
	if (m_hDll == nullptr)
		return nullptr;

	// Gets the address of the given function in the loeaded DLL
	//
	return GetProcAddress(m_hDll, szName);
}

//	Public API functions
//
TLINError E300::RegisterClient(LPSTR strName, DWORD hWnd, HLINCLIENT* hClient)
{
	if (!m_bWasLoaded)
		return errUnknown;

	return m_pRegisterClient(strName, hWnd, hClient);
}

TLINError E300::RemoveClient(HLINCLIENT hClient)
{
	if (!m_bWasLoaded)
		return errUnknown;

	return m_pRemoveClient(hClient);
}

TLINError E300::ConnectClient(HLINCLIENT hClient, HLINHW hHw)
{
	if (!m_bWasLoaded)
		return errUnknown;

	return m_pConnectClient(hClient, hHw);
}

TLINError E300::DisconnectClient(HLINCLIENT hClient, HLINHW hHw)
{
	if (!m_bWasLoaded)
		return errUnknown;

	return m_pDisconnectClient(hClient, hHw);
}

TLINError E300::ResetClient(HLINCLIENT hClient)
{
	if (!m_bWasLoaded)
		return errUnknown;

	return m_pResetClient(hClient);
}

TLINError E300::SetClientParam(HLINCLIENT hClient, WORD wParam, DWORD dwValue)
{
	if (!m_bWasLoaded)
		return errUnknown;

	return m_pSetClientParam(hClient, wParam, dwValue);
}

TLINError E300::GetClientParam(HLINCLIENT hClient, WORD wParam, void* pBuff, WORD wBuffSize)
{
	if (!m_bWasLoaded)
		return errUnknown;

	return m_pGetClientParam(hClient, wParam, pBuff, wBuffSize);
}

TLINError E300::SetClientFilter(HLINCLIENT hClient, HLINHW hHw, unsigned __int64 iRcvMask)
{
	if (!m_bWasLoaded)
		return errUnknown;

	return m_pSetClientFilter(hClient, hHw, iRcvMask);
}

TLINError E300::GetClientFilter(HLINCLIENT hClient, HLINHW hHw, unsigned __int64* pRcvMask)
{
	if (!m_bWasLoaded)
		return errUnknown;

	return m_pGetClientFilter(hClient, hHw, pRcvMask);
}

TLINError E300::Read(HLINCLIENT hClient, TLINRcvMsg* pMsg)
{
	if (!m_bWasLoaded)
		return errUnknown;

	return m_pRead(hClient, pMsg);
}

TLINError E300::ReadMulti(HLINCLIENT hClient, TLINRcvMsg* pMsgBuff, int iMaxCount, int* pCount)
{
	if (!m_bWasLoaded)
		return errUnknown;

	return m_pReadMulti(hClient, pMsgBuff, iMaxCount, pCount);
}

TLINError E300::Write(HLINCLIENT hClient, HLINHW hHw, TLINMsg* pMsg)
{
	if (!m_bWasLoaded)
		return errUnknown;

	return m_pWrite(hClient, hHw, pMsg);
}

TLINError E300::InitializeHardware(HLINCLIENT hClient, HLINHW hHw, BYTE bMode, WORD wBaudrate)
{
	if (!m_bWasLoaded)
		return errUnknown;

	return m_pInitializeHardware(hClient, hHw, bMode, wBaudrate);
}

TLINError E300::GetAvailableHardware(HLINHW* pBuff, WORD wBuffSize, int* pCount)
{
	if (!m_bWasLoaded)
		return errUnknown;

	return m_pGetAvailableHardware(pBuff, wBuffSize, pCount);
}

TLINError E300::SetHardwareParam(HLINCLIENT hClient, HLINHW hHw, WORD wParam, void* pBuff, WORD wBuffSize)
{
	if (!m_bWasLoaded)
		return errUnknown;

	return m_pSetHardwareParam(hClient, hHw, wParam, pBuff, wBuffSize);
}

TLINError E300::GetHardwareParam(HLINHW hHw, WORD wParam, void* pBuff, WORD wBuffSize)
{
	if (!m_bWasLoaded)
		return errUnknown;

	return m_pGetHardwareParam(hHw, wParam, pBuff, wBuffSize);
}

TLINError E300::ResetHardware(HLINCLIENT hClient, HLINHW hHw)
{
	if (!m_bWasLoaded)
		return errUnknown;

	return m_pResetHardware(hClient, hHw);
}

TLINError E300::ResetHardwareConfig(HLINCLIENT hClient, HLINHW hHw)
{
	if (!m_bWasLoaded)
		return errUnknown;

	return m_pResetHardwareConfig(hClient, hHw);
}

TLINError E300::IdentifyHardware(HLINHW hHw)
{
	if (!m_bWasLoaded)
		return errUnknown;

	return m_pIdentifyHardware(hHw);
}

TLINError E300::RegisterFrameId(HLINCLIENT hClient, HLINHW hHw, BYTE bFromFrameId, BYTE bToFrameId)
{
	if (!m_bWasLoaded)
		return errUnknown;

	return m_pRegisterFrameId(hClient, hHw, bFromFrameId, bToFrameId);
}

TLINError E300::SetFrameEntry(HLINCLIENT hClient, HLINHW hHw, TLINFrameEntry* pFrameEntry)
{
	if (!m_bWasLoaded)
		return errUnknown;

	return m_pSetFrameEntry(hClient, hHw, pFrameEntry);
}

TLINError E300::GetFrameEntry(HLINHW hHw, TLINFrameEntry* pFrameEntry)
{
	if (!m_bWasLoaded)
		return errUnknown;

	return m_pGetFrameEntry(hHw, pFrameEntry);
}

TLINError E300::UpdateByteArray(HLINCLIENT hClient, HLINHW hHw, BYTE bFrameId, BYTE bIndex, BYTE bLen, BYTE* pData)
{
	if (!m_bWasLoaded)
		return errUnknown;

	return m_pUpdateByteArray(hClient, hHw, bFrameId, bIndex, bLen, pData);
}

TLINError E300::StartKeepAlive(HLINCLIENT hClient, HLINHW hHw, BYTE bFrameId, WORD wPeriod)
{
	if (!m_bWasLoaded)
		return errUnknown;

	return m_pStartKeepAlive(hClient, hHw, bFrameId, wPeriod);
}

TLINError E300::SuspendKeepAlive(HLINCLIENT hClient, HLINHW hHw)
{
	if (!m_bWasLoaded)
		return errUnknown;

	return m_pSuspendKeepAlive(hClient, hHw);
}

TLINError E300::ResumeKeepAlive(HLINCLIENT hClient, HLINHW hHw)
{
	if (!m_bWasLoaded)
		return errUnknown;

	return m_pResumeKeepAlive(hClient, hHw);
}

TLINError E300::SetSchedule(HLINCLIENT hClient, HLINHW hHw, int iScheduleNumber, TLINScheduleSlot* pSchedule, int iSlotCount)
{
	if (!m_bWasLoaded)
		return errUnknown;

	return m_pSetSchedule(hClient, hHw, iScheduleNumber, pSchedule, iSlotCount);
}

TLINError E300::GetSchedule(HLINHW hHw, int iScheduleNumber, TLINScheduleSlot* pScheduleBuff, int iMaxSlotCount, int* pSlotCount)
{
	if (!m_bWasLoaded)
		return errUnknown;

	return m_pGetSchedule(hHw, iScheduleNumber, pScheduleBuff, iMaxSlotCount, pSlotCount);
}

TLINError E300::DeleteSchedule(HLINCLIENT hClient, HLINHW hHw, int iScheduleNumber)
{
	if (!m_bWasLoaded)
		return errUnknown;

	return m_pDeleteSchedule(hClient, hHw, iScheduleNumber);
}

TLINError E300::SetScheduleBreakPoint(HLINCLIENT hClient, HLINHW hHw, int iBreakPointNumber, DWORD dwHandle)
{
	if (!m_bWasLoaded)
		return errUnknown;

	return m_pSetScheduleBreakPoint(hClient, hHw, iBreakPointNumber, dwHandle);
}

TLINError E300::StartSchedule(HLINCLIENT hClient, HLINHW hHw, int iScheduleNumber)
{
	if (!m_bWasLoaded)
		return errUnknown;

	return m_pStartSchedule(hClient, hHw, iScheduleNumber);
}

TLINError E300::SuspendSchedule(HLINCLIENT hClient, HLINHW hHw)
{
	if (!m_bWasLoaded)
		return errUnknown;

	return m_pSuspendSchedule(hClient, hHw);
}
TLINError E300::ResumeSchedule(HLINCLIENT hClient, HLINHW hHw)
{
	if (!m_bWasLoaded)
		return errUnknown;

	return m_pResumeSchedule(hClient, hHw);
}

TLINError E300::XmtWakeUp(HLINCLIENT hClient, HLINHW hHw)
{
	if (!m_bWasLoaded)
		return errUnknown;

	return m_pXmtWakeUp(hClient, hHw);
}

TLINError E300::StartAutoBaud(HLINCLIENT hClient, HLINHW hHw, WORD wTimeOut)
{
	if (!m_bWasLoaded)
		return errUnknown;

	return m_pStartAutoBaud(hClient, hHw, wTimeOut);
}

TLINError E300::GetStatus(HLINHW hHw, TLINHardwareStatus* pStatusBuff)
{
	if (!m_bWasLoaded)
		return errUnknown;

	return m_pGetStatus(hHw, pStatusBuff);
}

TLINError E300::CalculateChecksum(TLINMsg* pMsg)
{
	if (!m_bWasLoaded)
		return errUnknown;

	return m_pCalculateChecksum(pMsg);
}

TLINError E300::GetVersion(TLINVersion* pVerBuff)
{
	if (!m_bWasLoaded)
		return errUnknown;

	return m_pGetVersion(pVerBuff);
}

TLINError E300::GetVersionInfo(LPSTR strTextBuff, WORD wBuffSize)
{
	if (!m_bWasLoaded)
		return errUnknown;

	return m_pGetVersionInfo(strTextBuff, wBuffSize);
}

TLINError E300::GetErrorText(DWORD dwError, BYTE bLanguage, LPSTR strTextBuff, WORD wBuffSize)
{
	if (!m_bWasLoaded)
		return errUnknown;

	return m_pGetErrorText(dwError, bLanguage, strTextBuff, wBuffSize);
}

TLINError E300::GetPID(BYTE* pFrameId)
{
	if (!m_bWasLoaded)
		return errUnknown;

	return m_pGetPID(pFrameId);
}

TLINError E300::GetTargetTime(HLINHW hHw, unsigned __int64* pTargetTime)
{
	if (!m_bWasLoaded)
		return errUnknown;

	return m_pGetTargetTime(hHw, pTargetTime);
}

TLINError E300::SetResponseRemap(HLINCLIENT hClient, HLINHW hHw, BYTE* pRemapTab)
{
	if (!m_bWasLoaded)
		return errUnknown;

	return m_pSetResponseRemap(hClient, hHw, pRemapTab);
}


TLINError E300::GetResponseRemap(HLINHW hHw, BYTE* pRemapTab)
{
	if (!m_bWasLoaded)
		return errUnknown;

	return m_pGetResponseRemap(hHw, pRemapTab);
}

TLINError E300::GetSystemTime(unsigned __int64* pSystemTime)
{
	if (!m_bWasLoaded)
		return errUnknown;

	return m_pGetSystemTime(pSystemTime);
}