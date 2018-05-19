
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
	bool isTracked;		//�Ƿ�����׷��
	MyGPSInfo myGPSInfo;		//��ʵ��ʹ�õ�GPS��Ϣ

//��Ա����
protected:
	CSerialPort serialPort;		//����ͨ����
	GPSTranslate gpsTran;	/*GPS��������*/
	string gpsStr;	//��ȡ����������һ��GPS���
	CWinThread* seekur_thread;		//Seekur�߳̾��
	//CWinThread* track_thread;		//׷���߳̾��
	bool isGPSEnd;		//����GPS������Ƿ��������
	IPolylinePtr pPath;			//������·��
	IElementPtr lastPointElement;	// Seekur��һ�ε�����
	CString pathLayerName;		//·��ͼ������
	bool isPathExist;	//�Ƿ��Ѽ���·��ͼ��



private:
	void AddCreateElement(IGeometryPtr pgeomln, IActiveViewPtr iactiveview);
	void OnTestMarkerStyle();
	IPoint* geoToProj(IPoint* point/*��Ҫ��������ϵ�ĵ�*/, long fromProjType = 3857, long toGeoType = 4326);
	ISymbolPtr m_isymbol;
	static UINT SeekurFuc(LPVOID lParam);		//Seekur�����̺߳���
	//void CreateShapeFile();
	static UINT TrackFuc(LPVOID lParam);		//׷���̺߳���

	
// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
	DECLARE_EVENTSINK_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnFileOpen();
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
	afx_msg void OnMouseDownMapcontrol1(long button, long shift, long x, long y, double mapX, double mapY);
	afx_msg void OnDoubleClickMapcontrol1(long button, long shift, long X, long Y, double mapX, double mapY);
	afx_msg	void OnMouseMoveMapcontrol1(long button, long shift, long X, long Y, double mapX, double mapY);
	afx_msg void OnFileSave();
	afx_msg void OnBtnMoveSeekur();
private:
	// �����̵߳Ľ���
protected:
	afx_msg LRESULT OnSeekur(WPARAM wParam, LPARAM lParam);
public:
	// ת����ԽǶ��ı���
	CEdit m_editMoveHeading;
	// �ƶ���Ծ����ı���
	CEdit m_editMoveDistance;
	// γ���ı���
	CEdit m_editLongitude;
	// �����ı���
	CEdit m_editLatitude;
	afx_msg void OnBtnTrack();
	// Seekur��ǰ�ٶ��ı���
	CEdit m_editSeekurVel;
	// Seekur��ǰ�����ı���
	CEdit m_editSeekurHeading;
	// ׷�ٿ�ʼ/ֹͣ��ť
	CButton m_btnTrack;
	// GPS���������ı���
	CEdit m_editSatNum;
	// GPS״ָ̬ʾλ�ı���
	CEdit m_editGPSStaus;
	// �Ƿ�ת������54����
	CButton m_checkShowBJ54;
	// Seekur�˶��ٶ��ı���
	CEdit m_editVelocity;
	// ׷��ʱ���ٶ�
	double m_velocity;
	// ·��ѡ��ť
	CButton m_btnPathSelect;
	afx_msg void OnBtnPathAdd();
	afx_msg void OnBtnSavePath();
	// �����ֵ
	CEdit m_editSubHeading;
	// �����ֵ
	CEdit m_editDis;
#pragma region PID������ؿؼ�
	// P�����ı���
	CEdit m_editKp;
	// I�����ı���
	CEdit m_editKi;
	// d�����ı���
	CEdit m_editKd;
	// ������뺽����Ӧ����ֵ�ı���
	CEdit m_editKinter;
	IPointPtr pStartPoint;		//��ǰ·����ʼ��
	IPointPtr pEndPoint;		//��ǰ·��������
	ILinePtr pNowPath;	//��ǰ·��
	int nowPathPos;		//��ǰ·����·�������е�λ�ã�����������׷��·��
	double lineHeading;		//·������
	long segNum;	//·��������
	IPolylinePtr m_trackPath;	//׷��·��
	double err;		//��ǰƫ��ֵ
	double err_last;	//��һ�ε�ƫ��ֵ
	double integral;	//����ֵ
	double kp;	//P���Ʊ���ϵ��
	double ki;	//I���Ʊ���ϵ��
	double kd;	//D���Ʊ���ϵ��
	double kinter;	//�����ͺ����ı���ϵ��
	// ����ID����
	CButton m_cUseID;
#pragma endregion
	// ����54 x����
	CEdit m_editBJ54_x;
	// ����54 y����
	CEdit m_editBJ54_y;
};

#ifndef _DEBUG  // MFCTestView.cpp �еĵ��԰汾
inline CMFCTestDoc* CMFCTestView::GetDocument() const
   { return reinterpret_cast<CMFCTestDoc*>(m_pDocument); }
#endif

