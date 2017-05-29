
#include "stdafx.h"
#include "swordtest2.h"
#include "swordtest2Dlg.h"


CString msg;

void Cswordtest2Dlg::OnBnClickedButton1()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CString id;
	m_id.GetWindowTextW(id);
	printf("ID=%S\n", id);
	printf("Loading...");
	msg = "SCPC";
	init(id);
}

void Cswordtest2Dlg::OnClickedReqScpc2()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CString id;
	m_id.GetWindowTextW(id);
	printf("ID=%S\n", id);
	printf("Loading...");
	msg = "SCPC2";
	init(id);
}



void Cswordtest2Dlg::init(CString id)
{
	out = fopen("bin\\output.txt", "w");
	m_ctlStockCur.SetSingleData(0, (_variant_t)"J");
	m_ctlStockCur.SetSingleData(1, (_variant_t)id);
	m_ctlStockCur.RequestData((_variant_t)msg);
}


void Cswordtest2Dlg::ReceiveDataItgexpertctlctrl1()
{

	int cnt = m_ctlStockCur.GetMultiRecordCount(0);
	for (int i = 0; i < cnt; i++)
	{
		if (cnt % 10 == 0)
			printf(".");
		CString time, price, vol, rltv;
		CString askp, bidp;
		CString ctrt;
		if (msg == "SCPC") {
			time = (_variant_t)m_ctlStockCur.GetMultiData(0, i, 0, 0);
			price = (_variant_t)m_ctlStockCur.GetMultiData(0, i, 1, 0);
			vol = (_variant_t)m_ctlStockCur.GetMultiData(0, i, 4, 0);
			askp = (_variant_t)m_ctlStockCur.GetMultiData(0, i, 2, 0);
			bidp = (_variant_t)m_ctlStockCur.GetMultiData(0, i, 3, 0);
			rltv = (_variant_t)m_ctlStockCur.GetMultiData(0, i, 5, 0);
			ctrt = (_variant_t)m_ctlStockCur.GetMultiData(0, i, 6, 0);
		}
		else if (msg == "SCPC2") {
			time = (_variant_t)m_ctlStockCur.GetMultiData(0, i, 0, 0);
			price = (_variant_t)m_ctlStockCur.GetMultiData(0, i, 1, 0);
			vol = (_variant_t)m_ctlStockCur.GetMultiData(0, i, 8, 0);
			askp = (_variant_t)m_ctlStockCur.GetMultiData(0, i, 5, 0);
			bidp = (_variant_t)m_ctlStockCur.GetMultiData(0, i, 6, 0);
			rltv = (_variant_t)m_ctlStockCur.GetMultiData(0, i, 9, 0);
			ctrt = (_variant_t)m_ctlStockCur.GetMultiData(0, i, 4, 0);
		}
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
		m_ctlStockCur.RequestNextData((_variant_t)msg);
		return true;
	}
	else
		return false;

}