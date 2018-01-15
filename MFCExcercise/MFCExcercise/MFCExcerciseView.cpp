
// MFCExcerciseView.cpp : CMFCExcerciseView ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#ifndef SHARED_HANDLERS
#include "MFCExcercise.h"
#endif

#include "MFCExcerciseDoc.h"
#include "MFCExcerciseView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMFCExcerciseView

IMPLEMENT_DYNCREATE(CMFCExcerciseView, CView)

BEGIN_MESSAGE_MAP(CMFCExcerciseView, CView)
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CMFCExcerciseView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CMFCExcerciseView ����/����

CMFCExcerciseView::CMFCExcerciseView()
{
	// TODO:  �ڴ˴���ӹ������

}

CMFCExcerciseView::~CMFCExcerciseView()
{
}

BOOL CMFCExcerciseView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO:  �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// CMFCExcerciseView ����

void CMFCExcerciseView::OnDraw(CDC* /*pDC*/)
{
	CMFCExcerciseDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO:  �ڴ˴�Ϊ����������ӻ��ƴ���
}


// CMFCExcerciseView ��ӡ


void CMFCExcerciseView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CMFCExcerciseView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void CMFCExcerciseView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO:  ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void CMFCExcerciseView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO:  ��Ӵ�ӡ����е��������
}

void CMFCExcerciseView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CMFCExcerciseView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CMFCExcerciseView ���

#ifdef _DEBUG
void CMFCExcerciseView::AssertValid() const
{
	CView::AssertValid();
}

void CMFCExcerciseView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMFCExcerciseDoc* CMFCExcerciseView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMFCExcerciseDoc)));
	return (CMFCExcerciseDoc*)m_pDocument;
}
#endif //_DEBUG


// CMFCExcerciseView ��Ϣ�������
