
#include "stdafx.h"
#include "swordtest2.h"
#include "swordtest2Dlg.h"



void Cswordtest2Dlg::OnBnClickedButton1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString id;
	m_id.GetWindowTextW(id);
	printf("ID=%S\n", id);
	printf("Loading...");
	init(id);
}


void Cswordtest2Dlg::init(CString id)
{
	out = fopen("bin\\output.txt", "w");
	m_ctlStockCur.SetSingleData(0, (_variant_t)"J");
	m_ctlStockCur.SetSingleData(1, (_variant_t)id);
	m_ctlStockCur.RequestData((_variant_t)"SCPC2");
}


void Cswordtest2Dlg::ReceiveDataItgexpertctlctrl1()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	int cnt = m_ctlStockCur.GetMultiRecordCount(0);
	for (int i = 0; i < cnt; i++)
	{
		if (cnt % 10 == 0)
			printf(".");
		CString time, price, vol, rltv;
		CString askp, bidp;
		CString ctrt;
		time = (_variant_t)m_ctlStockCur.GetMultiData(0, i, 0, 0);
		price = (_variant_t)m_ctlStockCur.GetMultiData(0, i, 1, 0);
		vol = (_variant_t)m_ctlStockCur.GetMultiData(0, i, 7, 0);
		askp = (_variant_t)m_ctlStockCur.GetMultiData(0, i, 5, 0);
		bidp = (_variant_t)m_ctlStockCur.GetMultiData(0, i, 6, 0);
		rltv = (_variant_t)m_ctlStockCur.GetMultiData(0, i, 9, 0);
		ctrt = (_variant_t)m_ctlStockCur.GetMultiData(0, i, 4, 0);
		fprintf(out, "%S\t%S\t%S\t%S\t%S\t%S\t%S\n", time.GetBuffer(), price.GetBuffer(), vol.GetBuffer(), askp.GetBuffer(), bidp.GetBuffer(), rltv.GetBuffer(), ctrt.GetBuffer());
	}
	if (!reload())
	{
		fclose(out);
		printf("\nComplete!\n");
	}
}


bool Cswordtest2Dlg::reload() {
	bool next = m_ctlStockCur.IsMoreNextData();
	if (next)
	{
		m_ctlStockCur.RequestNextData((_variant_t)"SCPC2");
		return true;
	}
	else
		return false;

}