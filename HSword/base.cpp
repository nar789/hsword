
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
	
	socket = NULL;
	server = NULL;
	serverrun = false;
	ChkedInit();
	FILE *in = fopen("config.ini", "r");
	if (in)
		fclose(in);
	else
		CreateINI();
	LoadINI();
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
	DDX_Control(pDX, IDC_PORT1, m_port1);
	DDX_Control(pDX, IDC_PORT2, m_port2);
	DDX_Control(pDX, IDC_SMCO, m_smco);
}

BEGIN_MESSAGE_MAP(Cswordtest2Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON1, &Cswordtest2Dlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BTN_BUY, &Cswordtest2Dlg::OnClickedBtnBuy)
	ON_BN_CLICKED(IDC_BTN_SELL, &Cswordtest2Dlg::OnBnClickedBtnSell)
	ON_BN_CLICKED(IDC_SERVER, &Cswordtest2Dlg::OnBnClickedServer)
	ON_BN_CLICKED(IDC_BTN_SDOC, &Cswordtest2Dlg::OnBnClickedBtnSdoc)
	ON_BN_CLICKED(IDC_BTN_SCPC2, &Cswordtest2Dlg::OnBnClickedBtnScpc2)
	ON_BN_CLICKED(IDC_SERVER_STOP, &Cswordtest2Dlg::OnBnClickedServerStop)
	ON_BN_CLICKED(IDC_CAL, &Cswordtest2Dlg::OnBnClickedCal)
	ON_BN_CLICKED(IDC_DEL, &Cswordtest2Dlg::OnBnClickedDel)
	ON_BN_CLICKED(IDC_CREATE1, &Cswordtest2Dlg::OnClickedCreate1)
	ON_BN_CLICKED(IDC_CREATE2, &Cswordtest2Dlg::OnClickedCreate2)
	ON_BN_CLICKED(IDC_S1, &Cswordtest2Dlg::OnClickedS1)
	ON_BN_CLICKED(IDC_S2, &Cswordtest2Dlg::OnClickedS2)
	ON_BN_CLICKED(IDC_ALLSERVERSTOP, &Cswordtest2Dlg::OnClickedAllserverstop)
	ON_BN_CLICKED(BTN_SMCO, &Cswordtest2Dlg::OnClickedBtnSmco)
END_MESSAGE_MAP()


BEGIN_EVENTSINK_MAP(Cswordtest2Dlg, CDialogEx)
	ON_EVENT(Cswordtest2Dlg, IDC_ITGEXPERTCTLCTRL1, 1, Cswordtest2Dlg::ReceiveDataItgexpertctlctrl1, VTS_NONE)
	ON_EVENT(Cswordtest2Dlg, IDC_buy, 1, Cswordtest2Dlg::OnReceivedataBuy, VTS_NONE)
	ON_EVENT(Cswordtest2Dlg, IDC_buy, 3, Cswordtest2Dlg::OnReceiveerrordataBuy, VTS_NONE)
	ON_EVENT(Cswordtest2Dlg, IDC_sell, 1, Cswordtest2Dlg::ReceiveDatasell, VTS_NONE)
	ON_EVENT(Cswordtest2Dlg, IDC_sell, 3, Cswordtest2Dlg::ReceiveErrorDatasell, VTS_NONE)
	ON_EVENT(Cswordtest2Dlg, IDC_SCPC2, 1, Cswordtest2Dlg::OnReceivedataScpc2, VTS_NONE)
	ON_EVENT(Cswordtest2Dlg, IDC_SCPC2, 3, Cswordtest2Dlg::OnReceiveerrordataScpc2, VTS_NONE)
	ON_EVENT(Cswordtest2Dlg, IDC_SDOC, 1, Cswordtest2Dlg::OnReceivedataSdoc, VTS_NONE)
	ON_EVENT(Cswordtest2Dlg, IDC_SMCO, 1, Cswordtest2Dlg::OnReceivedataSmco, VTS_NONE)
END_EVENTSINK_MAP()

// Cswordtest2Dlg �޽��� ó����




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


	m_id.SetWindowTextW(L"005930");
	m_buy_id.SetWindowTextW(L"005930");
	m_buy_price.SetWindowTextW(L"1800000");
	m_sell_id.SetWindowTextW(L"005930");
	m_sell_price.SetWindowTextW(L"100");
	m_port1.SetWindowTextW(L"1371");
	m_port2.SetWindowTextW(L"1372");
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




