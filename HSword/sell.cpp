
#include "stdafx.h"
#include "swordtest2.h"
#include "swordtest2Dlg.h"

#define FIRSTSELL true
#define SECONDSELL false

bool sellStatus = FIRSTSELL;
CString ORGN_ODNO;
CString KRX_FWDG_ORD_ORGNO;

void Cswordtest2Dlg::Sell(CString id, CString price, CString cnt) {

	chkedid = id;
	chkedprice = price;
	chkedcnt = cnt;

	if (sellStatus == FIRSTSELL) {
		OnBnClickedBtnSdoc();
		sellStatus = SECONDSELL;
	}
	else if (sellStatus == SECONDSELL) {
		OnClickedBtnSmco();
	}
}


void Cswordtest2Dlg::OnClickedBtnSmco()
{
	CString pass = (variant_t)m_sdoc.GetEncryptPassword((variant_t)PASSWORD);
	CString acnt = ACOUNT;
	m_smco.SetSingleData(0, (_variant_t)acnt.Left(8));
	m_smco.SetSingleData(1, (_variant_t)acnt.Right(2));
	m_smco.SetSingleData(2, (_variant_t)pass);
	m_smco.SetSingleData(3, (_variant_t)KRX_FWDG_ORD_ORGNO);
	m_smco.SetSingleData(4, (_variant_t)ORGN_ODNO);
	m_smco.SetSingleData(5, (_variant_t)"01");
	m_smco.SetSingleData(6, (_variant_t)"01");
	m_smco.SetSingleData(7, (_variant_t)chkedcnt);
	m_smco.SetSingleData(8, (_variant_t)chkedprice);
	m_smco.SetSingleData(9, (_variant_t)"Y");
	m_smco.SetSingleData(10, (_variant_t)"01036906736");

	m_smco.RequestData((_variant_t)"SMCO");
}

void Cswordtest2Dlg::OnReceivedataSmco()
{
	printf("SMCO Request Receive Start.\n");
	
	CString get[3];
	for (int i = 0; i < 3; i++) {
		get[i] = (_variant_t)m_smco.GetSingleData(i, 0);
	}
	printf("한국거래소전송주문조직번호 : %d\n", get[0]);
	printf("주문번호 : %d\n", get[1]);
	printf("주문시각 : %d\n", get[2]);
}


void Cswordtest2Dlg::OnBnClickedBtnSdoc()
{
	time_t t = time(NULL);
	tm* today;
	today = localtime(&t);
	char date[9];
	sprintf(date, "%d%02d%02d", (int)today->tm_year + 1900, (int)today->tm_mon + 1, (int)today->tm_mday);
	CString pass = (variant_t)m_sdoc.GetEncryptPassword((variant_t)PASSWORD);
	CString acnt = ACOUNT;
	m_sdoc.SetSingleData(0, (_variant_t)acnt.Left(8));
	m_sdoc.SetSingleData(1, (_variant_t)acnt.Right(2));
	m_sdoc.SetSingleData(2, (_variant_t)pass);
	m_sdoc.SetSingleData(3, (_variant_t)date);
	m_sdoc.SetSingleData(4, (_variant_t)date);
	m_sdoc.SetSingleData(5, (_variant_t)"02");
	m_sdoc.SetSingleData(6, (_variant_t)"00");
	m_sdoc.SetSingleData(7, (_variant_t)"");
	m_sdoc.SetSingleData(8, (_variant_t)"01");
	m_sdoc.SetSingleData(9, (_variant_t)"");
	m_sdoc.SetSingleData(10, (_variant_t)"");
	m_sdoc.SetSingleData(11, (_variant_t)"00");
	m_sdoc.SetSingleData(12, (_variant_t)"");
	m_sdoc.SetSingleData(13, (_variant_t)"");
	m_sdoc.SetSingleData(14, (_variant_t)"");
	m_sdoc.RequestData((_variant_t)"SDOC");
	//printf("SDOC request\n");
}

void Cswordtest2Dlg::OnReceivedataSdoc()
{
	int mc = m_sdoc.GetMultiRecordCount(0);
	CString cnt;
	for (int i = 0; i < mc; i++) {
		cnt = (_variant_t)m_sdoc.GetMultiData(0, i, 12, 0);
	}
	//printf("ChkedCnt:%S\n", cnt);
	if (cnt == chkedcnt) {
		ChkedSell(chkedid, chkedprice, chkedcnt);
	}
	else {
		OnBnClickedBtnSdoc();
	}
}

void Cswordtest2Dlg::ChkedSell(CString id, CString price, CString cnt)
{
	FILE *log = fopen("bin\\log.txt", "a");
	if (log) {
		CTime t = CTime::GetCurrentTime();
		CString time = t.Format(L"%Y-%m-%d %H:%M:%S");
		fprintf(log, "S %S %S %S ", id, price, cnt);
		fwprintf(log, L"%s\n", time.GetBuffer(time.GetLength()));
		fclose(log);
	}
	//CString acnt = (CString)"8816597701";
	CString acnt = ACOUNT;
	CString pass = (variant_t)m_sell.GetEncryptPassword((variant_t)PASSWORD);
	m_sell.SetSingleData(0, (variant_t)acnt.Left(8));
	m_sell.SetSingleData(1, (variant_t)acnt.Mid(8));
	m_sell.SetSingleData(2, (variant_t)pass);
	m_sell.SetSingleData(3, (variant_t)id);
	m_sell.SetSingleData(4, (variant_t)"01");
	m_sell.SetSingleData(5, (variant_t)"00");
	m_sell.SetSingleData(6, (variant_t)cnt);
	m_sell.SetSingleData(7, (variant_t)price);
	m_sell.SetSingleData(8, (variant_t)"01036906736");
	m_sell.RequestData((_variant_t)"SCAAO");
	printf("Sell Request Send! <%S><%S><%S>\n", id, price, cnt);
}


void Cswordtest2Dlg::ReceiveDatasell()
{

	printf("Sell request receive start.\n");
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CString get[3];
	for (int i = 0; i < 3; i++) {
		get[i] = (_variant_t)m_sell.GetSingleData(i, 0);
	}
	printf("한국거래소전송주문조직번호 : %d\n", get[0]);
	printf("주문번호 : %d\n", get[1]);
	printf("주문시각 : %d\n", get[2]);

	ORGN_ODNO = get[1];
	KRX_FWDG_ORD_ORGNO = get[0];
}


void Cswordtest2Dlg::ReceiveErrorDatasell()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	printf("sell error.\n");
}


void Cswordtest2Dlg::OnBnClickedBtnSell()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString id, cnt;
	m_sell_id.GetWindowTextW(id);
	m_sell_price.GetWindowTextW(cnt);//매도 수량
									 //CString acnt = (CString)"8816597701";
	CString acnt = ACOUNT;
	CString pass = (variant_t)m_sell.GetEncryptPassword((variant_t)PASSWORD);
	m_sell.SetSingleData(0, (variant_t)acnt.Left(8));
	m_sell.SetSingleData(1, (variant_t)acnt.Mid(8));
	m_sell.SetSingleData(2, (variant_t)pass);
	m_sell.SetSingleData(3, (variant_t)id);
	m_sell.SetSingleData(4, (variant_t)"01");
	m_sell.SetSingleData(5, (variant_t)"01");
	m_sell.SetSingleData(6, (variant_t)cnt);
	m_sell.SetSingleData(7, (variant_t)"");
	m_sell.SetSingleData(8, (variant_t)"01036906736");
	m_sell.RequestData((_variant_t)"SCAAO");
	printf("Sell Request Send! <%S><%S>\n", id, cnt);
}

