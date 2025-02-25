
// SokobanView.h: CSokobanView 類別的介面
//

#pragma once
enum Direction {LEFT,UP,DOWN,RIGHT};

class CSokobanView : public CView
{
protected: // 僅從序列化建立
	CSokobanView() noexcept;
	DECLARE_DYNCREATE(CSokobanView)

// 屬性
public:
	CSokobanDoc* GetDocument() const;

// 作業
public:

// 覆寫
public:
	virtual void OnDraw(CDC* pDC);  // 覆寫以描繪此檢視
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 程式碼實作
public:
	virtual ~CSokobanView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 產生的訊息對應函式
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	CBitmap arrival,blank, box, destination, wall, worker;
	void showMap();
	void showlevel();
	void error_message(char* s = "You can not go there!");
	int readmap(int n);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	bool MoveBox(Direction d);
	bool moveworker(Direction d);
	void clearmap();
	char ByWorker(Direction d);
	char NextByWorker(Direction d);
};

#ifndef _DEBUG  // 對 SokobanView.cpp 中的版本進行偵錯
inline CSokobanDoc* CSokobanView::GetDocument() const
   { return reinterpret_cast<CSokobanDoc*>(m_pDocument); }
#endif

