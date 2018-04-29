
// MFCTestView.h : CMFCTestView ��Ľӿ�
//

#pragma once

#include "resource.h"

#include "GPSTranslate.h"
#include "SerialPort.h"
#include <vector>
#include "BaseAction.h"
#include "afxwin.h"
#include <windows.h>

//seekur���Ʋ���
typedef struct seekurPara{
	double distance;	//����
	double heading;		//����
	double veloctiy;	//�ٶ�
}*seekurParaPtr;


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
public:
	IMapControl2Ptr m_ipMapControl;/*ָ��ǰ��ͼ�ĵ���ָ��*/
	ITOCControl2Ptr m_ipTocControl;/*ָ��ǰ��ͼ�����ָ��*/
	//CEdit m_editLongitude;		//�����ı���
	//CEdit m_editLatitude;		//γ���ı���
	INewLineFeedbackPtr m_pNewLineFeedback;
	IPolylinePtr m_trackPath;	//׷��·��
	bool isTracked;		//�Ƿ�����׷��
	MyGPSInfo myGPSInfo;		//��ʵ��ʹ�õ�GPS��Ϣ

//��Ա����
protected:
	CSerialPort serialPort;		//����ͨ����
	GPSTranslate gpsTran;	/*GPS��������*/
	string gpsStr;	//��ȡ����������һ��GPS���
	GPSInfo* gpsInfo;		//�������GPS���ָ��
	CWinThread* seekur_thread;		//Seekur�߳̾��
	CWinThread* track_thread;		//׷���߳̾��

	//ILayerPtr m_currentLayer;		//��ǰͼ��
	//IMapPtr m_map;		//��ͼ�ؼ��еĵ�ͼ
	//IFeaturePtr m_editFeature;	//�༭�е�Ҫ��
	//IDisplayFeedbackPtr m_feedback;		//���ڵ�ͼ��ʾ
	//bool m_bInUse;		//�ж��Ƿ�����ʹ��
	//IPointCollectionPtr m_pointCollection;	//��ǰҪ�صĵ㼯
	IPolylinePtr pPath;			//������·��

	IElementPtr lastPointElement;

private:
	void AddCreateElement(IGeometryPtr pgeomln, IActiveViewPtr iactiveview);
	void OnTestMarkerStyle();
	IPoint* geoToProj(IPoint* point/*��Ҫ��������ϵ�ĵ�*/, long fromProjType = 3857, long toGeoType = 4326);
	ISymbolPtr m_isymbol;
	static UINT SeekurFuc(LPVOID lParam);		//Seekur�����̺߳���
	//HRESULT CreateShapeFile(esriGeometryType type, CString layerPath, CString layerName, IFeatureClass** ppFeatureClass);
	void CreateShapeFile();
	static UINT TrackFuc(LPVOID lParam);		//׷���̺߳���

	
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
	afx_msg void OnBnClickedButton2();
private:
	// �����̵߳Ľ���
protected:
	afx_msg LRESULT OnSeekur(WPARAM wParam, LPARAM lParam);
public:
	// �����ı���
	CEdit m_editHeading;
	// �����ı���
	CEdit m_editDistance;
	// γ���ı���
	CEdit m_editLongitude;
	// �����ı���
	CEdit m_editLatitude;
	afx_msg void OnBnClickedButton3();
	// �����л�����
	CButton m_cPointLine;
	// Seekur��ǰ�ٶ�
	CEdit m_editSeekurVel;
	// Seekur��ǰ����
	CEdit m_editSeekurHeading;
	// P���Ʒ�K1������������й�
	CEdit m_editKDis;
	// P���Ʒ�K2�������뺽���ֵ���
	CEdit m_editKSubHead;
	// ׷�ٿ�ʼ/ֹͣ��ť
	CButton m_btnTrack;
	// ����54��X����
	CEdit m_editBJ54_x;
	// ����54��Y����
	CEdit m_editBJ54_y;
	// �Ƿ��ڵ�ͼ����ʾGPS����
	CButton m_checkShowGPS;
	// Seekur�˶��ٶ�
	CEdit m_editVelocity;
	// ׷��ʱ���ٶ�
	double m_velocity;
	// ·��ѡ��ť
	CButton m_btnPathSelect;
	afx_msg void OnBtnPathCreate();
	// �����ֵ
	CEdit m_editSubHeading;
	// �����ֵ
	CEdit m_editDis;
};

#ifndef _DEBUG  // MFCTestView.cpp �еĵ��԰汾
inline CMFCTestDoc* CMFCTestView::GetDocument() const
   { return reinterpret_cast<CMFCTestDoc*>(m_pDocument); }
#endif

