
// swordtest9Dlg.cpp : ���� ����
//

#include "stdafx.h"
#include "swordtest9.h"
#include "swordtest9Dlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Cswordtest9Dlg ��ȭ ����



Cswordtest9Dlg::Cswordtest9Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_SWORDTEST9_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void Cswordtest9Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SCPC, scpc);
	DDX_Control(pDX, IDC_ITGRANK, itgrank);
}

BEGIN_MESSAGE_MAP(Cswordtest9Dlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_UP, &Cswordtest9Dlg::OnClickedUp)
	ON_BN_CLICKED(IDC_RANK, &Cswordtest9Dlg::OnClickedRank)
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(Cswordtest9Dlg, CDialogEx)
	ON_EVENT(Cswordtest9Dlg, IDC_SCPC, 1, Cswordtest9Dlg::OnReceivedataScpc, VTS_NONE)
	ON_EVENT(Cswordtest9Dlg, IDC_ITGRANK, 1, Cswordtest9Dlg::OnReceivedataItgrank, VTS_NONE)
END_EVENTSINK_MAP()


// Cswordtest9Dlg �޽��� ó����

BOOL Cswordtest9Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// �� ��ȭ ������ �������� �����մϴ�.  ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	//  �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.

	ArguProcessor();

	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
}

void Cswordtest9Dlg::ArguProcessor() {
	printf("UP v1.0.3.10\n");
	env = getenv("HSWORD_HOME");	
	CString arg = __targv[1];
	if (arg == "-r")
	{
		/*
		while (true) {
			time_t t = time(NULL);
			tm* cur;
			cur = localtime(&t);
			if (cur->tm_hour >= 9)
				break;
		}*/
		OnClickedRank();
	}
}

// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸�����
//  �Ʒ� �ڵ尡 �ʿ��մϴ�.  ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
//  �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void Cswordtest9Dlg::OnPaint()
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
HCURSOR Cswordtest9Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void Cswordtest9Dlg::OnClickedUp()
{
	char c[7];
	sprintf(c, "%06d", code);
	scpc.SetSingleData(0,(_variant_t)"J");
	scpc.SetSingleData(1, (_variant_t)c);
	scpc.RequestData((_variant_t)"SCP");
}



void Cswordtest9Dlg::OnReceivedataScpc()
{
	const int prcidx = 11;
	const int ratioidx = 14;
	const int typeidx = 2;

	CString prc = (_variant_t)scpc.GetSingleData(prcidx,0);
	CString type = (_variant_t)scpc.GetSingleData(typeidx, 0);
	if (prc != "0" && ( type=="KOSPI" || type=="KOSPI200" )) { //include kospi, kosdaq
		CString ratio = (_variant_t)scpc.GetSingleData(ratioidx, 0);
		printf("%06d %S %S %S\n", code, prc, ratio,type);
		FILE *out = fopen("output.txt", "a");
		fprintf(out, "%06d\n", code);
		fclose(out);
	}
	if (code/100000<3) {
		if (code>=100000 && code % 100000 == 0)
			printf("[%d]\n",code/100000);
		code++;
		OnClickedUp();
	}
	else
	{
		printf("Complete!\n");
	}

}


void Cswordtest9Dlg::OnClickedRank()
{
	char file[255];
	strcpy(file, env);
	strcat(file, "\\code.txt");
	FILE *in = fopen(file, "r");
	codecnt = 0;
	codeidx = 0;
	topratio = 0;
	strcpy(topcode, "");


	if (in) {
		while (fscanf(in, "%s", curcode[codecnt++]) != -1) {
		}
		fclose(in);
	}
	printf("[%d]Upload on memory.\n",codecnt);
	itgrank.SetSingleData(0, (_variant_t)"J");
	itgrank.SetSingleData(1, (_variant_t)curcode[0]);
	itgrank.RequestData((_variant_t)"SCP");
}


void Cswordtest9Dlg::OnReceivedataItgrank()
{	
	const int ratioidx = 14;
	CString ratio = (_variant_t)itgrank.GetSingleData(ratioidx, 0);
	float f = _ttof(ratio);
	if (f > topratio)
	{
		topratio = f;
		strcpy(topcode, curcode[codeidx]);
	}
	if (++codeidx < codecnt-1) {
		if (codeidx % 5 == 0)printf(".");
		itgrank.SetSingleData(0, (_variant_t)"J");
		itgrank.SetSingleData(1, (_variant_t)curcode[codeidx]);
		itgrank.RequestData((_variant_t)"SCP");
	}
	else {
		char file[255];
		strcpy(file, env);
		strcat(file, "\\up.txt");
		FILE *out = fopen(file, "w");
		if (out)
		{
			fprintf(out, "%s", topcode);
			fclose(out);
		}
		printf("\n%s %.1f\n", topcode,topratio);
		OnClickedRank();
	}
}
