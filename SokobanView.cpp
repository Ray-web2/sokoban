#include<vector>
// SokobanView.cpp: CSokobanView 類別的實作
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS 可以定義在實作預覽、縮圖和搜尋篩選條件處理常式的
// ATL 專案中，並允許與該專案共用文件程式碼。
#ifndef SHARED_HANDLERS
#include "Sokoban.h"
#endif

#include "SokobanDoc.h"
#include "SokobanView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#include<vector>

int worker_spot;
int box_count = 0;
int dest_count = 0;
char map_num[16];

class grid
{
public:
	char type;
	int row;
	int colm;
	bool dest;

	grid() {}

	void exchange(Direction d);
	int find_grid(Direction d);
};

std::vector<grid> map;

int grid::find_grid(Direction d)
{
	int i = 0;
	switch (d)
	{
	case LEFT:
		while (map[i].row != row - 1 || map[i].colm != colm) { i++; }
		break;

	case RIGHT:
		while (map[i].row != row + 1 || map[i].colm != colm) { i++; }
		break;

	case UP:
		while (map[i].row != row || map[i].colm != colm - 1) { i++; }
		break;

	case DOWN:
		while (map[i].row != row || map[i].colm != colm + 1) { i++; }
		break;
	}
	return i;
}

void grid::exchange(Direction d)
{
	if (type == 'W' || type == 'M') worker_spot = find_grid(d);

	char temp;
	grid* target;
	target = &(map[find_grid(d)]);
	if (type == 'M' && target->type == ' ')
	{
		type = 'W';
		(*target).type = 'D';
	}
	else if (type == 'W' && target->type == 'D')
	{
		type = 'M';
		(*target).type = ' ';
	}
	else if (type == 'B' && target->type == 'D')
	{
		(*target).type = ' ';
		type = 'C';
		box_count = box_count + 1;
	}
	else if (type == 'C' && target->type == ' ')
	{
		type = 'B';
		(*target).type = 'D';
		box_count = box_count - 1;
	}

	temp = type;
	type = (*target).type;
	(*target).type = temp;

	return;
}
// CSokobanView

IMPLEMENT_DYNCREATE(CSokobanView, CView)

BEGIN_MESSAGE_MAP(CSokobanView, CView)
	// 標準列印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CSokobanView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// CSokobanView 建構/解構

CSokobanView::CSokobanView() noexcept
{
	// TODO: 在此加入建構程式碼
	arrival.LoadBitmap(IDB_BITarrival);
	blank.LoadBitmap(IDB_BITblank);
	box.LoadBitmap(IDB_BITbox);
	destination.LoadBitmap(IDB_BITdestination);
	wall.LoadBitmap(IDB_BITwall);
	worker.LoadBitmap(IDB_BITworker);
}

CSokobanView::~CSokobanView()
{
}

BOOL CSokobanView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此經由修改 CREATESTRUCT cs 
	// 達到修改視窗類別或樣式的目的

	return CView::PreCreateWindow(cs);
}

// CSokobanView 繪圖

void CSokobanView::OnDraw(CDC* pDC)
{
	CSokobanDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此加入原生資料的描繪程式碼
	readmap(pDoc->level);
	showMap();
	showlevel();
}


// CSokobanView 列印


void CSokobanView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CSokobanView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 預設的準備列印程式碼
	return DoPreparePrinting(pInfo);
}

void CSokobanView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 加入列印前額外的初始設定
}

void CSokobanView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 加入列印後的清除程式碼
}

void CSokobanView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CSokobanView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CSokobanView 診斷

#ifdef _DEBUG
void CSokobanView::AssertValid() const
{
	CView::AssertValid();
}

void CSokobanView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CSokobanDoc* CSokobanView::GetDocument() const // 內嵌非偵錯版本
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSokobanDoc)));
	return (CSokobanDoc*)m_pDocument;
}
#endif //_DEBUG


// CSokobanView 訊息處理常式


void CSokobanView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CSokobanDoc* pDoc = GetDocument();
	switch (nChar)
	{
	case 'H':
	case 'W':
	case VK_UP:
		if (moveworker(UP) != 1)
		{
			error_message("You can not go there!");
		}
		else showlevel();
		break;

	case 'J':
	case 'A':
	case VK_LEFT:
		if (moveworker(LEFT) != 1)
		{
			error_message("You can not go there!");
		}
		else showlevel();
		break;

	case 'K':
	case 'S':
	case VK_DOWN:
		if (moveworker(DOWN) != 1)
		{
			error_message("You can not go there!");
		}
		else showlevel();
		break;

	case 'L':
	case 'D':
	case VK_RIGHT:
		if (moveworker(RIGHT) != 1)
		{
			error_message("You can not go there!");
		}
		else showlevel();
		break;

	case VK_NEXT:
		pDoc->level = pDoc->level + 1;
		pDoc->UpdateAllViews(NULL);
		break;

	case VK_PRIOR:
		pDoc->level = pDoc->level - 1;
		pDoc->UpdateAllViews(NULL);
		break;

	case 'R':
		readmap(pDoc->level);
		showMap();
		break;
	}

	if (dest_count == box_count && dest_count != 0)
	{
		error_message("you are smart!!!!");
		pDoc->level = pDoc->level + 1;
		pDoc->UpdateAllViews(NULL);
	}
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

int CSokobanView::readmap(int n)
{
	CClientDC adc(this);
	map.clear();
	dest_count = 0;
	box_count = 0;
	int count = 0;
	char c;
	FILE* fp;

	if (n < 10)
	{
		sprintf_s(map_num, "map00%d.txt", n);
	}
	else if (n >= 10 && n < 100)
	{
		sprintf_s(map_num, "map0%d.txt", n);
	}
	else if (n >= 100)
	{
		sprintf_s(map_num, "map%d.txt", n);
	}

	errno_t err = fopen_s(&fp, map_num, "r");
	if (err != 0)
	{
		return 0;
	}
	else while ((c = fgetc(fp)) != EOF)
	{
		map.push_back(grid());
		(*(map.end() - 1)).type = c;

		if (c == 'W')
		{
			worker_spot = count;
		}

		if (c == 'M')
		{
			dest_count += 1;
		}
		if (c == 'D')
		{
			dest_count += 1;
		}
		if (c == 'C')
		{
			dest_count = dest_count + 1;
			box_count = box_count + 1;
		}

		count = count + 1;
	}
	fclose(fp);
	return 1;
}

void CSokobanView::showMap()
{
	clearmap();
	CSokobanDoc* pDoc = GetDocument();
	CClientDC aDC(this);
	CDC memDC;
	memDC.CreateCompatibleDC(&aDC);
	int max_x = 0;
	int max_y = 0;
	int x = 0;
	int y = 0;
	for (int i = 0; i < map.size(); i++)
	{
		if (map[i].type != '\n')
		{
			switch (map[i].type)
			{
			case 'H':
				memDC.SelectObject(&wall);
				aDC.BitBlt(x * 32, y * 32, 32, 32, &memDC, 0, 0, SRCCOPY);
				break;
			case 'B':
				memDC.SelectObject(&box);
				aDC.BitBlt(x * 32, y * 32, 32, 32, &memDC, 0, 0, SRCCOPY);
				break;
			case 'M': case 'W':
				memDC.SelectObject(&worker);
				aDC.BitBlt(x * 32, y * 32, 32, 32, &memDC, 0, 0, SRCCOPY);
				break;
			case 'D':
				memDC.SelectObject(&destination);
				aDC.BitBlt(x * 32, y * 32, 32, 32, &memDC, 0, 0, SRCCOPY);
				break;
			case 'C':
				memDC.SelectObject(&arrival);
				aDC.BitBlt(x * 32, y * 32, 32, 32, &memDC, 0, 0, SRCCOPY);
				break;
			case ' ':
				memDC.SelectObject(&blank);
				aDC.BitBlt(x * 32, y * 32, 32, 32, &memDC, 0, 0, SRCCOPY);
				break;
			}

			map[i].row = x;
			map[i].colm = y;
			x++;
		}

		else
		{
			if (max_x < x) max_x = x;
			y += 1;
			x = 0;
		}
	}
	pDoc->row = 32 * max_x;
	pDoc->colum = 32 * max_y;
}


void CSokobanView::error_message(char* s)
{
	CClientDC aDC(this);
	if (s == "")
	{
		aDC.TextOutW(600, 200, CString("                                            "));
	}
	else
	{
		aDC.TextOutW(600, 200, CString("You can not go there!"));
	}
}

void CSokobanView::showlevel()
{
	CSokobanDoc* pDoc = GetDocument();
	CString s;
	CClientDC aDC(this);
	s.Format(L"Level: %d", pDoc->level);
	aDC.TextOutW(pDoc->row + 50, 30, s);
	s.Format(L"Destinaton: %d", dest_count);
	aDC.TextOutW(pDoc->row + 50, 60, s);
	s.Format(L"Arrival: %d", box_count);
	aDC.TextOutW(pDoc->row + 50, 90, s);
	aDC.TextOutW(pDoc->row + 50, 120, L"Press r to retry");
}

char CSokobanView::ByWorker(Direction d)
{
	// TODO: 請在此新增您的實作程式碼.
	return map[map[worker_spot].find_grid(d)].type;
}


char CSokobanView::NextByWorker(Direction d)
{
	// TODO: 請在此新增您的實作程式碼.

	return map[map[map[worker_spot].find_grid(d)].find_grid(d)].type;
}

bool CSokobanView::MoveBox(Direction d)
{
	char ch;
	ch = NextByWorker(d);

	switch (ch)
	{
	case ' ': case 'D':
		map[map[worker_spot].find_grid(d)].exchange(d);
		map[worker_spot].exchange(d);
		showMap();
		return 1;

	case 'W': case'H': case'B':case'C':
		return 0;
	}
}

bool CSokobanView::moveworker(Direction d)
{
	char ch;
	ch = ByWorker(d);

	switch (ch)
	{
	case ' ': case 'D':
		map[worker_spot].exchange(d);
		showMap();
		error_message("");
		return 1;

	case 'H':
		return 0;

	default:
		return MoveBox(d);

	}
}

void CSokobanView::clearmap()
{
	CSokobanDoc* pDoc = GetDocument();
	CClientDC adc(this);
	adc.Rectangle(0, 0, pDoc->row, pDoc->colum);
}
