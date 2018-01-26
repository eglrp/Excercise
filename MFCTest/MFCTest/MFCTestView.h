
// MFCTestView.h : CMFCTestView ��Ľӿ�
//

#pragma once

#include "resource.h"

#include "GPSTranslate.h"
#include "SerialPort.h"
#include <vector>


class CMFCTestView : public CFormView
{
protected: // �������л�����
	CMFCTestView();
	DECLARE_DYNCREATE(CMFCTestView)

public:
	enum{ IDD = IDD_MFCTEST_FORM };

// ����
public:
	CMFCTestDoc* GetDocument() const;

// ����
public:

// ��д
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual void OnInitialUpdate(); // ������һ�ε���

// ʵ��
public:
	virtual ~CMFCTestView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

//�ؼ�����
protected:
	IMapControl2Ptr m_ipMapControl;/*ָ��ǰ��ͼ�ĵ���ָ��*/
	//ITOCControl2Ptr m_ipTocControl;/*ָ��ǰ��ͼ�ĵ���ָ��*/
	CEdit *m_editLongitude;		//�����ı���
	CEdit *m_editLatitude;		//γ���ı���
	INewLineFeedbackPtr m_pNewLineFeedback;
	IGeometryPtr m_pgeometry;

//��Ա����
protected:
	CSerialPort serialPort;		//����ͨ����
	GPSTranslate gpsTran;	/*GPS��������*/
	string gpsStr;	//��ȡ����������һ��GPS���
	vector<GPSInfo*> gpsInfos;		//�������GPS�����
	MyGPSInfo myGPSInfo;		//��ʵ��ʹ�õ�GPS��Ϣ
	bool testBool;

private:
	void AddCreateElement(IGeometryPtr pgeomln, IActiveViewPtr iactiveview);
	void OnTestMarkerStyle();
	IPoint* geoToProj(IPoint* point/*��Ҫ��������ϵ�ĵ�*/, long fromProjType = 3857, long toGeoType = 4326);
	ISymbolPtr m_isymbol;
	
// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
	DECLARE_EVENTSINK_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnFileOpen();
//	afx_msg void OnSize(UINT nType, int cx, int cy);
protected:
	/*
	������GPS����ͨ�ź���
	������
	@ch�����ڴ��ݵĵ����ַ�
	@ch���˿ں�
	����ֵ��0
	*/
	afx_msg LRESULT OnCommunication(WPARAM ch, LPARAM portnum);
public:
//	afx_msg void OnBnClickedButton1();
	afx_msg void OnClickedButton1();
	afx_msg void OnMouseDownMapcontrol1(long button, long shift, long x, long y, double mapX, double mapY);
	afx_msg void OnDoubleClickMapcontrol1(long button, long shift, long X, long Y, double mapX, double mapY);
	afx_msg	void OnMouseMoveMapcontrol1(long button, long shift, long X, long Y, double mapX, double mapY);
	afx_msg void OnFileSave();
};

#ifndef _DEBUG  // MFCTestView.cpp �еĵ��԰汾
inline CMFCTestDoc* CMFCTestView::GetDocument() const
   { return reinterpret_cast<CMFCTestDoc*>(m_pDocument); }
#endif

