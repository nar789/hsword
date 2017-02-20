
// swordtest2Dlg.cpp : ���� ����
//

#include "stdafx.h"
#include "swordtest2.h"
#include "swordtest2Dlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ���� ���α׷� ������ ���Ǵ� CAboutDlg ��ȭ �����Դϴ�.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

// �����Դϴ�.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// Cswordtest2Dlg ��ȭ ����



Cswordtest2Dlg::Cswordtest2Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_SWORDTEST2_DIALOG, pParent)
{
	system("title HSword");
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	server = NULL;
	ChkedInit();
	FILE *in = fopen("config.ini", "r");
	if (in) {
		fclose(in);
		LoadINI();
	}
	else {
		CreateINI();
		LoadINI();
	}
}

void Cswordtest2Dlg::ChkedInit() {
	chkedcnt = chkedprice = chkedid = "";
}

void Cswordtest2Dlg::CreateINI() {
	MessageBox( L"create file config.ini complete.", L"HUME");
	CString path = (_T(".\\config.ini"));
	CString section, key, value;
	section = _T("ACOUNT");
	key = _T("ACOUNT");
	value = _T("1234567801");
	WritePrivateProfileString(section, key, value, path);

	key = _T("PASSWORD");
	value = _T("1111");
	WritePrivateProfileString(section, key, value, path);

	key = _T("MONEY");
	value = _T("10000000");
	WritePrivateProfileString(section, key, value, path);
}

void Cswordtest2Dlg::LoadINI() {
	TCHAR buf[MAX_PATH] = { 0, };
	CString path = (_T(".\\config.ini"));
	CString sction, key, value;
	sction = _T("ACOUNT");
	
	key = _T("ACOUNT");
	GetPrivateProfileString(sction, key, _T(""), buf, MAX_PATH,path);
	value.Format(_T("%s"), buf);
	ACOUNT = value;

	key = _T("PASSWORD");
	GetPrivateProfileString(sction, key, _T(""), buf, MAX_PATH, path);
	value.Format(_T("%s"), buf);
	PASSWORD = value;

	key = _T("MONEY");
	GetPrivateProfileString(sction, key, _T(""), buf, MAX_PATH, path);
	value.Format(_T("%s"), buf);
	MONEY = _ttoi(value);

	printf("ACOUNT=%S\n",ACOUNT);
	//printf("PASSWORD=%S\n", PASSWORD);
	printf("MONEY=%d\n", MONEY);
}

void Cswordtest2Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ITGEXPERTCTLCTRL1, m_ctlStockCur);
	DDX_Control(pDX, IDC_buy, m_buy);
	DDX_Control(pDX, IDC_sell, m_sell);
	DDX_Control(pDX, IDC_EDIT1, m_id);
	DDX_Control(pDX, IDC_BUY_ID, m_buy_id);
	DDX_Control(pDX, IDC_BUY_PRICE, m_buy_price);
	DDX_Control(pDX, IDC_SELL_ID, m_sell_id);
	DDX_Control(pDX, IDC_SELL_PRICE, m_sell_price);
	DDX_Control(pDX, IDC_SCPC2, m_scpc2);
	DDX_Control(pDX, IDC_A, m_a);
	DDX_Control(pDX, IDC_B, m_b);
	DDX_Control(pDX, IDC_CNT, m_c);
	DDX_Control(pDX, IDC_AC, m_ac);
	DDX_Control(pDX, IDC_BC, m_bc);
	DDX_Control(pDX, IDC_FEE1, m_fee1);
	DDX_Control(pDX, IDC_FEE2, m_fee2);
	DDX_Control(pDX, IDC_TAX, m_tax);
	DDX_Control(pDX, IDC_PRO, m_pro);
	DDX_Control(pDX, IDC_FT, m_ft);
	DDX_Control(pDX, IDC_PRO2, m_pro2);
	DDX_Control(pDX, IDC_SDOC, m_sdoc);
}

BEGIN_MESSAGE_MAP(Cswordtest2Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &Cswordtest2Dlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BTN_BUY, &Cswordtest2Dlg::OnClickedBtnBuy)
	ON_BN_CLICKED(IDC_BTN_SELL, &Cswordtest2Dlg::OnBnClickedBtnSell)
	ON_BN_CLICKED(IDC_SERVER, &Cswordtest2Dlg::OnBnClickedServer)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BTN_SDOC, &Cswordtest2Dlg::OnBnClickedBtnSdoc)
	ON_BN_CLICKED(IDC_BTN_SCPC2, &Cswordtest2Dlg::OnBnClickedBtnScpc2)
	ON_BN_CLICKED(IDC_SERVER_STOP, &Cswordtest2Dlg::OnBnClickedServerStop)
	ON_BN_CLICKED(IDC_CAL, &Cswordtest2Dlg::OnBnClickedCal)
	ON_BN_CLICKED(IDC_DEL, &Cswordtest2Dlg::OnBnClickedDel)
END_MESSAGE_MAP()


// Cswordtest2Dlg �޽��� ó����


void Cswordtest2Dlg::init(CString id)
{
	out = fopen("bin\\output.txt", "w");
	m_ctlStockCur.SetSingleData(0, (_variant_t)"J");
	m_ctlStockCur.SetSingleData(1, (_variant_t)id);
	m_ctlStockCur.RequestData((_variant_t)"SCPC2");
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
BOOL Cswordtest2Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// �ý��� �޴��� "����..." �޴� �׸��� �߰��մϴ�.

	// IDM_ABOUTBOX�� �ý��� ��� ������ �־�� �մϴ�.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// �� ��ȭ ������ �������� �����մϴ�.  ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	//  �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.

	// TODO: ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	//init();

	m_id.SetWindowTextW(L"005930");
	m_buy_id.SetWindowTextW(L"005930");
	m_buy_price.SetWindowTextW(L"1800000");
	m_sell_id.SetWindowTextW(L"005930");
	m_sell_price.SetWindowTextW(L"100");
	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
}


void Cswordtest2Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸�����
//  �Ʒ� �ڵ尡 �ʿ��մϴ�.  ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
//  �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void Cswordtest2Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ�Դϴ�.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Ŭ���̾�Ʈ �簢������ �������� ����� ����ϴ�.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �������� �׸��ϴ�.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// ����ڰ� �ּ�ȭ�� â�� ���� ���ȿ� Ŀ���� ǥ�õǵ��� �ý��ۿ���
//  �� �Լ��� ȣ���մϴ�.
HCURSOR Cswordtest2Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

BEGIN_EVENTSINK_MAP(Cswordtest2Dlg, CDialogEx)
	ON_EVENT(Cswordtest2Dlg, IDC_ITGEXPERTCTLCTRL1, 1, Cswordtest2Dlg::ReceiveDataItgexpertctlctrl1, VTS_NONE)
	ON_EVENT(Cswordtest2Dlg, IDC_ITGEXPERTCTLCTRL1, 3, Cswordtest2Dlg::ReceiveErrorDataItgexpertctlctrl1, VTS_NONE)
	ON_EVENT(Cswordtest2Dlg, IDC_ITGEXPERTCTLCTRL1, 2, Cswordtest2Dlg::ReceiveRealDataItgexpertctlctrl1, VTS_NONE)
	ON_EVENT(Cswordtest2Dlg, IDC_buy, 1, Cswordtest2Dlg::OnReceivedataBuy, VTS_NONE)
	ON_EVENT(Cswordtest2Dlg, IDC_buy, 3, Cswordtest2Dlg::OnReceiveerrordataBuy, VTS_NONE)
	ON_EVENT(Cswordtest2Dlg, IDC_sell, 1, Cswordtest2Dlg::ReceiveDatasell, VTS_NONE)
	ON_EVENT(Cswordtest2Dlg, IDC_sell, 3, Cswordtest2Dlg::ReceiveErrorDatasell, VTS_NONE)
	ON_EVENT(Cswordtest2Dlg, IDC_SCPC2, 1, Cswordtest2Dlg::OnReceivedataScpc2, VTS_NONE)
	ON_EVENT(Cswordtest2Dlg, IDC_SCPC2, 3, Cswordtest2Dlg::OnReceiveerrordataScpc2, VTS_NONE)	
	ON_EVENT(Cswordtest2Dlg, IDC_SDOC, 1, Cswordtest2Dlg::OnReceivedataSdoc, VTS_NONE)
END_EVENTSINK_MAP()


void Cswordtest2Dlg::ReceiveDataItgexpertctlctrl1()
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	int cnt = m_ctlStockCur.GetMultiRecordCount(0);
	for (int i = 0; i < cnt;i++)
	{
		if(cnt%10==0)
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
		fprintf(out,"%S\t%S\t%S\t%S\t%S\t%S\t%S\n",time.GetBuffer(),price.GetBuffer(),vol.GetBuffer(),askp.GetBuffer(),bidp.GetBuffer(),rltv.GetBuffer(),ctrt.GetBuffer());
	}
	if (!reload())
	{
		fclose(out);
		printf("\nComplete!\n");
	}
}


void Cswordtest2Dlg::ReceiveErrorDataItgexpertctlctrl1()
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	printf("error occur!\n");
}


void Cswordtest2Dlg::ReceiveRealDataItgexpertctlctrl1()
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	printf("real data\n");
}



void Cswordtest2Dlg::OnBnClickedButton1()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CString id;
	m_id.GetWindowTextW(id);
	printf("ID=%S\n",id);
	printf("Loading...");
	init(id);
}


void Cswordtest2Dlg::OnClickedBtnBuy()
{
	CString id,price;
	m_buy_id.GetWindowTextW(id);
	m_buy_price.GetWindowTextW(price);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	//CString acnt = (CString)"8816597701";
	CString acnt = ACOUNT;
	CString pass=(variant_t)m_buy.GetEncryptPassword((variant_t)PASSWORD);
	m_buy.SetSingleData(0, (variant_t)acnt.Left(8));
	m_buy.SetSingleData(1, (variant_t)acnt.Mid(8));
	m_buy.SetSingleData(2, (variant_t)pass);
	m_buy.SetSingleData(3, (variant_t)id);
	m_buy.SetSingleData(4, (variant_t)"00");
	m_buy.SetSingleData(5, (variant_t)"1");
	m_buy.SetSingleData(6, (variant_t)price);
	m_buy.SetSingleData(7, (variant_t)"");
	m_buy.RequestData((_variant_t)"SCABO");
	printf("Buy Request Send! <%S><%S>\n ",id,price);
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
	printf("Buy Request Send! <%S><%S><%S>\n",id,price,cnt);

	m_sell_id.SetWindowTextW(id);
	m_sell_price.SetWindowTextW(cnt);
}

void Cswordtest2Dlg::Sell(CString id, CString price, CString cnt) {
	
	chkedid = id;
	chkedprice = price;
	chkedcnt = cnt;
	OnBnClickedBtnSdoc();
}

void Cswordtest2Dlg::ChkedSell(CString id, CString price, CString cnt)
{
	FILE *log = fopen("bin\\log.txt", "a");
	CTime t = CTime::GetCurrentTime();
	CString time = t.Format(L"%Y-%m-%d %H:%M:%S");
	fprintf(log, "S %S %S %S ", id, price, cnt);
	fwprintf(log, L"%s\n", time.GetBuffer(time.GetLength()));
	fclose(log);
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
	printf("Sell Request Send! <%S><%S><%S>\n", id, price, cnt);
}



void Cswordtest2Dlg::OnReceivedataBuy()
{
	printf("buy request receive start.\n");
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	CString get[3];
	for (int i = 0; i < 3; i++) {
		get[i] = (_variant_t)m_buy.GetSingleData(i, 0);
	}
	printf("�ѱ��ŷ��������ֹ�������ȣ : %d\n",get[0]);
	printf("�ֹ���ȣ : %d\n", get[1]);
	printf("�ֹ��ð� : %d\n", get[2]);
}


void Cswordtest2Dlg::OnReceiveerrordataBuy()
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	printf("buy error.\n");
}


void Cswordtest2Dlg::ReceiveDatasell()
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	printf("Sell request receive start.\n");
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	CString get[3];
	for (int i = 0; i < 3; i++) {
		get[i] = (_variant_t)m_sell.GetSingleData(i, 0);
	}
	printf("�ѱ��ŷ��������ֹ�������ȣ : %d\n", get[0]);
	printf("�ֹ���ȣ : %d\n", get[1]);
	printf("�ֹ��ð� : %d\n", get[2]);
}


void Cswordtest2Dlg::ReceiveErrorDatasell()
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	printf("sell error.\n");
}


void Cswordtest2Dlg::OnBnClickedBtnSell()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CString id, cnt;
	m_sell_id.GetWindowTextW(id);
	m_sell_price.GetWindowTextW(cnt);//�ŵ� ����
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


void Cswordtest2Dlg::OnBnClickedServer()
{
	if (!serverrun) {
		serverrun = true;
		server = new thread(&Cswordtest2Dlg::ServerThread, this);
	}
	else {
		printf("Already server is running.\n");
	}
}

void Cswordtest2Dlg::ServerThread() {
	socket.SetDlg(this);
	socket.ptr_buy = &Cswordtest2Dlg::Buy;
	socket.ptr_sell = &Cswordtest2Dlg::Sell;
	socket.ptr_x = &Cswordtest2Dlg::RequestX;
	while (!socket.run()) {
		if (!serverrun)
			break;
	}
}



void Cswordtest2Dlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	if (server != NULL)
		server->join();
}

void Cswordtest2Dlg::OnBnClickedBtnSdoc()
{
	time_t t = time(NULL);
	tm* today;
	today = localtime(&t);
	char date[9];
	sprintf(date,"%d%02d%02d", (int)today->tm_year+1900, (int)today->tm_mon+1, (int)today->tm_mday);
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


void Cswordtest2Dlg::OnReceivedataScpc2()
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	CString prc = (variant_t)m_scpc2.GetSingleData(11, 0);
	CString ctrt = (variant_t)m_scpc2.GetSingleData(14, 0);
	CString m= (variant_t)m_scpc2.GetSingleData(16, 0);
	CString sprc = (variant_t)m_scpc2.GetSingleData(18, 0);
	sprintf(socket.servermsg, "%S %S %S %S", prc, ctrt, m, sprc);
}


void Cswordtest2Dlg::OnReceiveerrordataScpc2()
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	printf("scp error.\n");
}


void Cswordtest2Dlg::OnBnClickedBtnScpc2()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_scpc2.SetSingleData(0, (variant_t)"J");
	m_scpc2.SetSingleData(1, (variant_t)"005930");
	m_scpc2.RequestData((variant_t)"SCP");
	printf("SCP request\n");
}
void Cswordtest2Dlg::RequestX(CString id) {
	m_scpc2.SetSingleData(0, (variant_t)"J");
	m_scpc2.SetSingleData(1, (variant_t)id);
	m_scpc2.RequestData((variant_t)"SCP");
	printf("SCP request<%S>\n",id);
}


void Cswordtest2Dlg::OnBnClickedServerStop()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if (server != NULL) {
		this->serverrun = false;
		server = NULL;
		printf(">Stop listening.\n");
	}
}

void Cswordtest2Dlg::OnBnClickedCal()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CString sa,sb,sc;
	int a,b,c;
	m_a.GetWindowTextW(sa);
	m_b.GetWindowTextW(sb);
	m_c.GetWindowTextW(sc);
	a = _ttoi(sa);
	b = _ttoi(sb);
	c = _ttoi(sc);
	
	if (!a) {
		MessageBox(L"�ż����� �Է����ּ���.",L"HUME");
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

	CString ca,cb;
	ca.Format(_T("%d"), a*c);
	cb.Format(_T("%d"), b*c);
	m_ac.SetWindowTextW(ca);
	m_bc.SetWindowTextW(cb);

	double fa = (a*c*0.00127296) + 1500;
	double fb = (b*c*0.00127296) + 1500;
	double ftax = (b*c*0.003);
	CString csfa, csfb,cst;
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
	CString cspro,cspro2;
	cspro.Format(_T("%d"), pro);
	cspro2.Format(_T("%d"), pro2);
	m_pro.SetWindowTextW(cspro);
	m_pro2.SetWindowTextW(cspro2);

}

void Cswordtest2Dlg::OnBnClickedDel()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	WinExec("bin\\clean.bat", SW_HIDE);
	printf(">Clean data.\n");
}


