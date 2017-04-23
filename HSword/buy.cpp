

#include "stdafx.h"
#include "swordtest2.h"
#include "swordtest2Dlg.h"


void Cswordtest2Dlg::OnClickedBtnBuy()
{
	CString id, price;
	m_buy_id.GetWindowTextW(id);
	m_buy_price.GetWindowTextW(price);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//CString acnt = (CString)"8816597701";
	CString acnt = ACOUNT;
	CString pass = (variant_t)m_buy.GetEncryptPassword((variant_t)PASSWORD);
	m_buy.SetSingleData(0, (variant_t)acnt.Left(8));
	m_buy.SetSingleData(1, (variant_t)acnt.Mid(8));
	m_buy.SetSingleData(2, (variant_t)pass);
	m_buy.SetSingleData(3, (variant_t)id);
	m_buy.SetSingleData(4, (variant_t)"00");
	m_buy.SetSingleData(5, (variant_t)"1");
	m_buy.SetSingleData(6, (variant_t)price);
	m_buy.SetSingleData(7, (variant_t)"");
	m_buy.RequestData((_variant_t)"SCABO");
	printf("Buy Request Send! <%S><%S>\n ", id, price);
}

void Cswordtest2Dlg::Buy(CString id, CString price, CString cnt) {
	FILE *log = fopen("bin\\log.txt", "a");
	CTime t = CTime::GetCurrentTime();
	CString time = t.Format(L"%Y-%m-%d %H:%M:%S");
	fprintf(log, "B %S %S %S ", id, price, cnt);
	fwprintf(log, L"%s\n", time.GetBuffer(time.GetLength()));
	fclose(log);

	CString acnt = ACOUNT;
	CString pass = (variant_t)m_buy.GetEncryptPassword((variant_t)PASSWORD);
	m_buy.SetSingleData(0, (variant_t)acnt.Left(8));
	m_buy.SetSingleData(1, (variant_t)acnt.Mid(8));
	m_buy.SetSingleData(2, (variant_t)pass);
	m_buy.SetSingleData(3, (variant_t)id);
	m_buy.SetSingleData(4, (variant_t)"00");
	m_buy.SetSingleData(5, (variant_t)cnt);
	m_buy.SetSingleData(6, (variant_t)price);
	m_buy.SetSingleData(7, (variant_t)"01036906736");
	m_buy.RequestData((_variant_t)"SCABO");
	printf("Buy Request Send! <%S><%S><%S>\n", id, price, cnt);

	m_sell_id.SetWindowTextW(id);
	m_sell_price.SetWindowTextW(cnt);
}


void Cswordtest2Dlg::OnReceivedataBuy()
{
	printf("buy request receive start.\n");
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CString get[3];
	for (int i = 0; i < 3; i++) {
		get[i] = (_variant_t)m_buy.GetSingleData(i, 0);
	}
	printf("한국거래소전송주문조직번호 : %d\n", get[0]);
	printf("주문번호 : %d\n", get[1]);
	printf("주문시각 : %d\n", get[2]);
}


void Cswordtest2Dlg::OnReceiveerrordataBuy()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	printf("buy error.\n");
}

