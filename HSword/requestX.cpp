

#include "stdafx.h"
#include "swordtest2.h"
#include "swordtest2Dlg.h"


void Cswordtest2Dlg::OnBnClickedBtnScpc2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_scpc2.SetSingleData(0, (variant_t)"J");
	m_scpc2.SetSingleData(1, (variant_t)"005930");
	m_scpc2.RequestData((variant_t)"SCPC");
	printf("SCPC request\n");
}

void Cswordtest2Dlg::RequestX(CString id) {

	m_scpc2.SetSingleData(0, (variant_t)"J");
	m_scpc2.SetSingleData(1, (variant_t)id);
	m_scpc2.RequestData((variant_t)"SCP");
	printf("SCPC request<%S>\n", id);
	
}

void Cswordtest2Dlg::OnReceivedataScpc2()
{
	const int prcidx = 11;
	const int ctrtidx = 14;
	const int hgpridx = 19;
	//const int midx = 4;
	//const int rltvidx = 5;

	CString prc = (variant_t)m_scpc2.GetMultiData(0, 0, prcidx, 0);
	CString ctrt = (variant_t)m_scpc2.GetMultiData(0, 0, ctrtidx, 0);
	CString hgpr = (variant_t)m_scpc2.GetMultiData(0, 0, hgpridx, 0);
	//CString m = (variant_t)m_scpc2.GetMultiData(0, 0, midx, 0);
	//CString rltv = (variant_t)m_scpc2.GetMultiData(0, 0, rltvidx, 0);
	
	lock.lock();
		sprintf(socket->servermsg, "%S %S %S", prc, ctrt, hgpr);
	lock.unlock();
}

void Cswordtest2Dlg::OnReceiveerrordataScpc2()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	printf("scpc error.\n");
}
