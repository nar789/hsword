
#include "stdafx.h"
#include "swordtest2.h"
#include "swordtest2Dlg.h"



void Cswordtest2Dlg::OnClickedS1()
{
	if (server != NULL) {
		printf("Already server is running.\n");
		return;
	}
	CString p;
	m_port1.GetWindowTextW(p);
	sprintf(port, "%S", p);
	serverrun = true;
	server = new thread(&Cswordtest2Dlg::ServerThread, this);

}


void Cswordtest2Dlg::OnClickedS2()
{
	if (server != NULL) {
		printf("Already server is running.\n");
		return;
	}
	CString p;
	m_port2.GetWindowTextW(p);
	sprintf(port, "%S", p);
	serverrun = true;
	server = new thread(&Cswordtest2Dlg::ServerThread, this);
}



void Cswordtest2Dlg::OnBnClickedServer()
{
	if (!serverrun) {
		CString p;
		m_port1.GetWindowTextW(p);
		sprintf(port, "%S", p);
		serverrun = true;
		server = new thread(&Cswordtest2Dlg::ServerThread, this);
	}
	else {
		printf("Already all server is running.\n");
	}
}

void Cswordtest2Dlg::ServerThread() {
	socket = new HSOCKET();
	socket->SetPort(port);
	socket->SetDlg(this);
	socket->ptr_buy = &Cswordtest2Dlg::Buy;
	socket->ptr_sell = &Cswordtest2Dlg::Sell;
	socket->ptr_x = &Cswordtest2Dlg::RequestX;
	socket->lock = &lock;
	while (!socket->run()) {
		Sleep(1);
		if (!serverrun)
			break;
	}
}

void Cswordtest2Dlg::OnBnClickedServerStop()
{
	lock.lock();
		if (socket != NULL)
		{
			socket = NULL;
		}
		if (server != NULL) {
			this->serverrun = false;
			server = NULL;
			printf(">Stop listening.\n");
		}
	lock.unlock();

}

void Cswordtest2Dlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	if (server != NULL)
		server->join();
}



void Cswordtest2Dlg::OnBnClickedCal()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString sa, sb, sc;
	int a, b, c;
	m_a.GetWindowTextW(sa);
	m_b.GetWindowTextW(sb);
	m_c.GetWindowTextW(sc);
	a = _ttoi(sa);
	b = _ttoi(sb);
	c = _ttoi(sc);

	if (!a) {
		MessageBox(L"매수금을 입력해주세요.", L"HUME");
		return;
	}
	if (b < 10)
	{
		double fb = a*(1 + (double)b / 100);
		int ifb = (int)fb;
		CString csfb;
		csfb.Format(_T("%d"), ifb);
		m_b.SetWindowTextW(csfb);
		b = ifb;
	}
	if (!c)
	{
		c = MONEY / a;
		CString csc;
		csc.Format(_T("%d"), c);
		m_c.SetWindowTextW(csc);
	}

	CString ca, cb;
	ca.Format(_T("%d"), a*c);
	cb.Format(_T("%d"), b*c);
	m_ac.SetWindowTextW(ca);
	m_bc.SetWindowTextW(cb);

	double fa = (a*c*0.00127296) + 1500;
	double fb = (b*c*0.00127296) + 1500;
	double ftax = (b*c*0.003);
	CString csfa, csfb, cst;
	csfa.Format(_T("%f"), fa);
	csfb.Format(_T("%f"), fb);
	cst.Format(_T("%f"), ftax);
	m_fee1.SetWindowTextW(csfa);
	m_fee2.SetWindowTextW(csfb);
	m_tax.SetWindowTextW(cst);

	double ft = fa + fb + ftax;
	int intft = (int)ft;
	CString csft;
	csft.Format(_T("%d"), intft);
	m_ft.SetWindowTextW(csft);

	int pro = (b*c) - (a*c);
	int pro2 = pro - intft;
	CString cspro, cspro2;
	cspro.Format(_T("%d"), pro);
	cspro2.Format(_T("%d"), pro2);
	m_pro.SetWindowTextW(cspro);
	m_pro2.SetWindowTextW(cspro2);

}

void Cswordtest2Dlg::OnBnClickedDel()
{
	char cmd[255];
	CString port1, port2;
	m_port1.GetWindowTextW(port1);
	m_port2.GetWindowTextW(port2);
	sprintf(cmd, "bin\\del.bat %S", port1);
	system(cmd);
	sprintf(cmd, "bin\\del.bat %S", port2);
	system(cmd);
}




void Cswordtest2Dlg::OnClickedCreate1()
{
	char cmd[255];
	CString port;
	m_port1.GetWindowTextW(port);

	sprintf(cmd, "bin\\copy.bat %S", port);
	system(cmd);
}


void Cswordtest2Dlg::OnClickedCreate2()
{
	char cmd[255];
	CString port;
	m_port2.GetWindowTextW(port);

	sprintf(cmd, "bin\\copy.bat %S", port);
	system(cmd);
}

void Cswordtest2Dlg::StopAllServer()
{
	/*
	CString port[2];
	char cmd[255];
	m_port1.GetWindowTextW(port[0]);
	m_port2.GetWindowTextW(port[1]);
	for (int i = 0; i < 2; i++) {
		sprintf(cmd, "del %S\\hstcp.exe", port[i]);
		system(cmd);
		sprintf(cmd, "del %S\\up.py", port[i]);
		system(cmd);
	}*/
}


void Cswordtest2Dlg::OnClickedAllserverstop()
{
	StopAllServer();
}
