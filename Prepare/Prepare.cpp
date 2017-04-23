
// PrepareDlg.cpp : ���� ����
//

#include "stdafx.h"
#include "Prepare.h"
#include "PrepareDlg.h"
#include "afxdialogex.h"
#include "Regression.h"

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


// CPrepareDlg ��ȭ ����



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


// CPrepareDlg �޽��� ó����

BOOL CPrepareDlg::OnInitDialog()
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
	init();

	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
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

// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸�����
//  �Ʒ� �ڵ尡 �ʿ��մϴ�.  ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
//  �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void CPrepareDlg::OnPaint()
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
