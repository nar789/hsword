
// swordtest9Dlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "swordtest9.h"
#include "swordtest9Dlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Cswordtest9Dlg 대화 상자



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


// Cswordtest9Dlg 메시지 처리기

BOOL Cswordtest9Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	ArguProcessor();

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
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

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void Cswordtest9Dlg::OnPaint()
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
