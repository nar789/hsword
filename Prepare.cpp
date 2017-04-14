
// PrepareDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "Prepare.h"
#include "PrepareDlg.h"
#include "afxdialogex.h"
#include "Regression.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
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


// CPrepareDlg 대화 상자



CPrepareDlg::CPrepareDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_PREPARE_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPrepareDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SCPC, m_scpc);
}

BEGIN_MESSAGE_MAP(CPrepareDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(CPrepareDlg, CDialogEx)
	ON_EVENT(CPrepareDlg, IDC_SCPC, 1, CPrepareDlg::OnReceivedataScpc, VTS_NONE)
END_EVENTSINK_MAP()


// CPrepareDlg 메시지 처리기

BOOL CPrepareDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
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

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	init();

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CPrepareDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CPrepareDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CPrepareDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CPrepareDlg::init() {
	char codepath[255];
	r.init();
	sprintf(codepath, "%s\\origin.txt",getenv("HSWORD_HOME"));
	sprintf(writepath, "%s\\code.txt", getenv("HSWORD_HOME"));
	sprintf(blackpath, "%s\\black.txt", getenv("HSWORD_HOME"));
	FILE *in = fopen(codepath, "r");
	
	int cnt = 0;
	if (in) {
		while (fscanf(in, "%s", code[cnt++]) != -1);
		printf("loaded ok!\n");
		fclose(in);
	}
	codecnt = cnt;
	curidx = 0;
	Next();
}

void CPrepareDlg::RequestScpc(char* code) {
	m_scpc.SetSingleData(0, (_variant_t)"J");
	m_scpc.SetSingleData(1, (_variant_t)code);
	m_scpc.RequestData((_variant_t)"SCPC2");
}

int CPrepareDlg::hourToSec(int& hour) {
	int sec = hour % 100;
	hour /= 100;
	sec += (hour % 100) * 60;
	hour /= 100;
	sec += (hour % 100) * 3600;
	return sec;
}


void CPrepareDlg::OnReceivedataScpc()
{
	int rc = m_scpc.GetMultiRecordCount(0);
	const int feild_no_hour = 0;
	const int feild_no_curPrice = 1;
	const int feild_no_vol = 8;

	bool  smallvol = false;
	int ivol = 0;
	for (int i = 0; i < rc; i++) {
		//CString hour = (_variant_t)m_scpc.GetMultiData(0, i, feild_no_hour, 0);
		//CString price = (_variant_t)m_scpc.GetMultiData(0, i, feild_no_curPrice, 0);
		CString vol = (_variant_t)m_scpc.GetMultiData(0, i, feild_no_vol, 0);

		//int isec = _ttoi(hour);
		//int iprice = _ttoi(price);
		ivol = _ttoi(vol);

		if (ivol <= 50000) {
			smallvol = true;
			break;
		}

		//isec = hourToSec(isec);
		
		//r.save(isec, iprice);
	}
	
	//double v = r.calculate();
	
	if (!smallvol) {
		FILE *out = fopen(writepath, "a");
		if (out) {
			fprintf(out,"%s\n",code[curidx]);
			fclose(out);
		}
	}
	else {
		FILE *out = fopen(blackpath, "a");
		if (out) {
			fprintf(out, "%s %d\n", code[curidx],ivol);
			fclose(out);
		}
	}
	Next();
}

void CPrepareDlg::Next() {
	printf("%d\t%s\n",curidx,code[curidx]);
	if (curidx < codecnt - 1) {
		RequestScpc(code[++curidx]);
	}
	else
	{
		printf("Finish!\n");
		exit(0);
	}
}
