
// MFCTestView.cpp : CMFCTestView ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#ifndef SHARED_HANDLERS
#include "MFCTest.h"
#endif

#include "MFCTestDoc.h"
#include "MFCTestView.h"
#include <math.h>
#include <stdio.h>
#include "MainFrm.h"
#include "ObstacleAvoid.h"

#define WM_SEEKUR_RET WM_USER+100		 //Seekur������Ϣ
#define WM_SEEKUR_MOVE WM_USER+101		//Seekur������Ϣ����ʼ��
//#define WM_SEEKUR_SUB WM_USER+102		//Seekur������Ϣ����ֵ��
#define WM_SEEKUR_END WM_USER+103		//Seekur������Ϣ��������
#define WM_SEEKUR_STOP WM_USER+104		//Seekur������Ϣ��ֹͣ��

#define WM_TRACK_START WM_USER+104		//��ʼ׷��
#define WM_TRACK_STOP WM_USER+105		//ֹͣ׷��
#define WM_TRACK_END WM_USER+106		//����׷��

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define wm

#define PI 3.1415926
#define EARTH_RADIUS 6378.137

double rad(double d)
{
	return d * PI / 180.0;
}

double GetDistance(double lat1, double lng1, double lat2, double lng2)
{
	double radLat1 = rad(lat1);
	double radLat2 = rad(lat2);
	double a = radLat1 - radLat2;
	double b = rad(lng1) - rad(lng2);
	double s = 2 * asin(sqrt(pow(sin(a / 2), 2) +
		cos(radLat1) * cos(radLat2) * pow(sin(b / 2), 2)));
	s = s * EARTH_RADIUS;
	s = round(s * 10000) / 10000;
	return s;
}

unsigned long HextoDec(const unsigned char *hex, int length)
{
	int i;
	unsigned long rslt = 0;
	for (i = 0; i < length; i++)
	{
		rslt += (unsigned long)(hex[i]) << (8 * (length - 1 - i));

	}
	return rslt;
}

int HexToDem(string str)
{
	int dem = 0;
	for (int i = 0; i < str.size(); i++)
	{
		dem = dem * 16;
		if ((str[i] <= '9') && (str[i] >= '0'))        //0~9֮����ַ�
			dem += str[i] - '0';
		else if ((str[i] <= 'F') && (str[i] >= 'A'))   //A~F֮����ַ�
			dem += str[i] - 'A' + 10;
		else if ((str[i] <= 'f') && (str[i] >= 'a'))   //a~f֮����ַ�
			dem += str[i] - 'a' + 10;
		else
			return -1;                          //����ʱ����-1
	}
	return dem;
}
// CMFCTestView

IMPLEMENT_DYNCREATE(CMFCTestView, CFormView)

BEGIN_MESSAGE_MAP(CMFCTestView, CFormView)
	ON_WM_CREATE()
	ON_COMMAND(ID_FILE_OPEN, &CMFCTestView::OnFileOpen)
	//	ON_WM_SIZE()
	ON_MESSAGE(WM_COMM_RXCHAR, &CMFCTestView::OnSerialPortCallback)
	ON_BN_CLICKED(IDC_BUTTON1, &CMFCTestView::OnBtnSavePath)
	ON_COMMAND(ID_FILE_SAVE, &CMFCTestView::OnFileSave)
	ON_BN_CLICKED(IDC_BUTTON2, &CMFCTestView::OnBtnMoveSeekur)
	ON_MESSAGE(WM_SEEKUR_RET, &CMFCTestView::OnSeekur)
	ON_BN_CLICKED(IDC_BUTTON3, &CMFCTestView::OnBtnTrack)
	ON_BN_CLICKED(IDC_BUTTON4, &CMFCTestView::OnBtnPathAdd)
END_MESSAGE_MAP()


BEGIN_EVENTSINK_MAP(CMFCTestView, CFormView)
	//{{AFX_EVENTSINK_MAP(CFieldNetView_Map)
	ON_EVENT(CMFCTestView, IDC_MAPCONTROL1, 1 /* OnMouseDown */, OnMouseDownMapcontrol1, VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_R8 VTS_R8)
	//ON_EVENT(CMFCTestView, IDC_MAPCONTROL1, 4 /* OnDoubleClick */, OnDoubleClickMapcontrol1, VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_R8 VTS_R8)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()


// CMFCTestView ����/����

CMFCTestView::CMFCTestView()
: CFormView(CMFCTestView::IDD)
, m_velocity(0)
{
	// TODO:  �ڴ˴���ӹ������

}

CMFCTestView::~CMFCTestView()
{
	PostThreadMessage(seekur_thread->m_nThreadID, WM_SEEKUR_END, 0, 0);
	//PostThreadMessage(track_thread->m_nThreadID, WM_TRACK_STOP, 0, 0);
}

void CMFCTestView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT6, m_editMoveHeading);
	DDX_Control(pDX, IDC_EDIT5, m_editMoveDistance);
	DDX_Control(pDX, IDC_EDIT2, m_editLongitude);
	DDX_Control(pDX, IDC_EDIT1, m_editLatitude);
	DDX_Control(pDX, IDC_CHECK1, m_cUseID);
	DDX_Control(pDX, IDC_EDIT4, m_editSeekurVel);
	DDX_Control(pDX, IDC_EDIT3, m_editSeekurHeading);
	DDX_Control(pDX, IDC_EDIT7, m_editKp);
	DDX_Control(pDX, IDC_EDIT8, m_editKinter);
	DDX_Control(pDX, IDC_BUTTON3, m_btnTrack);
	DDX_Control(pDX, IDC_EDIT10, m_editSatNum);
	DDX_Control(pDX, IDC_EDIT9, m_editGPSStaus);
	DDX_Control(pDX, IDC_CHECK2, m_checkShowBJ54);
	DDX_Control(pDX, IDC_EDIT11, m_editVelocity);
	DDX_Control(pDX, IDC_BUTTON4, m_btnPathSelect);
	DDX_Control(pDX, IDC_EDIT12, m_editSubHeading);
	DDX_Control(pDX, IDC_EDIT13, m_editDis);
	DDX_Control(pDX, IDC_EDIT14, m_editKi);
	DDX_Control(pDX, IDC_EDIT15, m_editKd);
	DDX_Control(pDX, IDC_EDIT17, m_editBJ54_x);
	DDX_Control(pDX, IDC_EDIT16, m_editBJ54_y);
}

BOOL CMFCTestView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO:  �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ
	return CFormView::PreCreateWindow(cs);
}

void CMFCTestView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();

	//��ʼ���ؼ�
	//MapControl�ؼ�
	CWnd* pWndCal = GetDlgItem(IDC_MAPCONTROL1);
	LPUNKNOWN pUnk = pWndCal->GetControlUnknown();
	pUnk->QueryInterface(IID_IMapControl2, (LPVOID *)&m_ipMapControl);

	CWnd* pWndCal2 = GetDlgItem(IDC_TOCCONTROL1);
	LPUNKNOWN pUnk2 = pWndCal2->GetControlUnknown();
	pUnk2->QueryInterface(IID_ITOCControl2, (LPVOID *)&m_ipTocControl);
	m_ipTocControl->SetBuddyControl(m_ipMapControl);

	//���������߳�
	//myEvent = ::CreateEvent(
	//	NULL,    // �¼������ԣ�
	//	FALSE,    // �Զ�
	//	FALSE,    // ��ʼ����û���źŵ�
	//	_T("e_seekur")    // ��������
	//	);
	seekur_thread = AfxBeginThread(
		SeekurFuc,
		this->GetSafeHwnd()   // ���ݴ���ľ��
		);

	isGPSEnd = false;
	m_GPSport = 6;
	//���ô��ڳ��򣬴���GPS��ͨ��
	CMainFrame *p = (CMainFrame*)AfxGetMainWnd();
	CMFCTestView *m_CView = (CMFCTestView *)p->GetActiveView();//��ȡView���ָ��
	//if (serialPort_gps.InitPort(m_CView, m_GPSport))
	//{
	//	serialPort_gps.StartMonitoring();
	//}
	//else
	//{
	//	MessageBox(_T("GPS����ͨ��ʧ�ܣ�"), _T("��ʾ"), MB_OK);
	//}
	laserBufPos = 0;
	laserCheckBufPos = 0;
	//LASER_HEADER = { 0x02, 0x80, 0x6E, 0x01, 0xB0, 0xB5, 0x00 };
	/*LASER_HEADER[0] = '0x02';
	LASER_HEADER[1] = 0x80;
	LASER_HEADER[2] = 0x6E;
	LASER_HEADER[3] = 0x01;
	LASER_HEADER[4] = 0xB0;
	LASER_HEADER[5] = 0xB5;
	LASER_HEADER[6] = 0x00;*/
	gpsStr = "";
	testStr = "";
	m_laserport = 3;
	if (serialPort_laser.InitPort(m_CView, m_laserport, 9600))
	{
		serialPort_laser.StartMonitoring();
	}
	else
	{
		MessageBox(_T("���⴮��ͨ��ʧ�ܣ�"), _T("��ʾ"), MB_OK);
	}


	m_editKp.SetWindowTextW(_T("0.25"));
	m_editKinter.SetWindowTextW(_T("2"));
	m_editKi.SetWindowTextW(_T("0"));
	m_editKd.SetWindowTextW(_T("0"));
	m_cUseID.SetCheck(1);
	m_editVelocity.SetWindowTextW(_T("500"));
	isTracked = false;
	isGPSEnd = false;
	isPathExist = false;
	isAvoid = false;
	pathLayerName = _T("path");
}

// CMFCTestView ���

#ifdef _DEBUG
void CMFCTestView::AssertValid() const
{
	CFormView::AssertValid();
}

void CMFCTestView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CMFCTestDoc* CMFCTestView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMFCTestDoc)));
	return (CMFCTestDoc*)m_pDocument;
}
#endif //_DEBUG


// CMFCTestView ��Ϣ�������


int CMFCTestView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFormView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  �ڴ������ר�õĴ�������

	//��ʼ�����
	::CoInitialize(NULL);
	ArcGISVersionLib::IArcGISVersionPtr ipVer(__uuidof(ArcGISVersionLib::VersionManager));
	VARIANT_BOOL succeeded;
	if (FAILED(ipVer->LoadVersion(ArcGISVersionLib::esriArcGISEngine, L"10.2", &succeeded))) //10.2 �汾 
		return -1;
	IAoInitializePtr m_AoInit;//(CLSID_AoInitialize); 
	m_AoInit.CreateInstance(CLSID_AoInitialize);
	esriLicenseStatus ls;
	HRESULT h = m_AoInit->Initialize(esriLicenseProductCode::esriLicenseProductCodeEngineGeoDB, &ls);



	return 0;
}


#pragma region ��Ա����
void CMFCTestView::OnFileOpen()
{
	// TODO:  �ڴ���������������
	//char sfileter[]="Shape files(*.Shp)|*.Shp|mxd�ĵ�(*.mxd)|*.mxd|�����ļ�(*.*)";  
	CFileDialog dlg(TRUE, //TRUEΪOPEN�Ի���FALSEΪSAVE AS�Ի���  
		NULL,
		NULL,
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		(LPCTSTR)_TEXT("Shape files(*.shp)|*.shp|mxd�ĵ�(*.mxd)|*.mxd|"),
		NULL);
	CString m_strFileName;
	if (dlg.DoModal() == IDOK)
	{
		//m_MapControl=new CMapControl2();  


		m_strFileName = dlg.GetPathName();//ȫ·����  
		//CString filepath=dlg.GetFolderPath();//·�����ƣ������ļ���  
		//CString filename=dlg.GetFileName();//�ļ���������·��  
		CString strExt = dlg.GetFileExt();//��׺����������  
		if (strExt == "shp")
		{
			CComBSTR MX_DATAFILE;
			//MX_DATAFILE = "F:\\����mxd�ĵ�\\shenzhen.mxd" ;  
			MX_DATAFILE = dlg.GetPathName();
			BSTR filePath = dlg.GetFolderPath().AllocSysString();
			BSTR fileName = dlg.GetFileName().AllocSysString();
			m_ipMapControl->AddShapeFile(filePath, fileName);

			//�����ص���·��ͼ�㣬���ȡ��ͼ�㣬������·����ʼ����
			CString cLayerName = dlg.GetFileTitle();
			if (!pathLayerName.CompareNoCase(cLayerName))
			{
				isPathExist = true;
				//��ȡ·��ͼ��
				ILayerPtr pLayer;
				long num;
				m_ipMapControl->get_LayerCount(&num);
				for (long i = 0; i < num; i++)
				{
					BSTR name;
					m_ipMapControl->get_Layer(i, &pLayer);
					pLayer->get_Name(&name);
					//CString cName(name);
					if (!pathLayerName.CompareNoCase(name))
					{
						//��ȡ·��
						IFeatureLayerPtr pFeatLayer = pLayer;
						IFeatureClassPtr pFeatCla;
						pFeatLayer->get_FeatureClass(&pFeatCla);
						IFeaturePtr pFeature;
						pFeatCla->GetFeature(0, &pFeature);
						IGeometryPtr pGeometry;
						pFeature->get_Shape(&pGeometry);
						m_trackPath = pGeometry;

						//������ʼ·���ͺ���
						ISegmentCollectionPtr pSegCol = (ISegmentCollectionPtr)m_trackPath;	//ֱ�߼���
						pSegCol->get_SegmentCount(&segNum);
						ISegmentPtr pSeg;	//��ǰ����·��
						pSegCol->get_Segment(0, &pSeg);
						pNowPath = pSeg;
						double angle;	//ֱ����X��нǣ��Ի���Ϊ��λ
						pNowPath->get_Angle(&angle);
						lineHeading = angle * 180 / PI;
						//��Ϊ��ǰ�Ƕ�����������XΪ������ʱ��Ƕȣ���Ӧ���ǵ�ͼ����
						//����Ҫת���Ա�Ϊ����˳ʱ��Ƕ�
						if (lineHeading <= 90)
						{
							//1��3��4����
							lineHeading = 90 - lineHeading;
						}
						else
						{
							//2����
							lineHeading = 360 + (90 - lineHeading);
						}
						break;
					}
				}
			}
			
			
		}
		else if (strExt == "mxd")
		{
			CComBSTR MX_DATAFILE;
			//MX_DATAFILE = "F:\\����mxd�ĵ�\\shenzhen.mxd" ;  
			MX_DATAFILE = dlg.GetPathName();
			VARIANT_BOOL bValidDoc;
			//m_MapControlView.CheckMxFile( MX_DATAFILE );  
			m_ipMapControl->CheckMxFile(MX_DATAFILE, &bValidDoc);
			//VARIANT vt = 0;  
			if (bValidDoc)
				m_ipMapControl->LoadMxFile(MX_DATAFILE);
		}
		else
		{
			AfxMessageBox(_T("��ѡ����ʵ��ļ�!"));
			return;
		}
		m_ipMapControl->Refresh(esriViewAll);
	}
}

/*
������GPS����ͨ�ź���
������
@ch�����ڴ��ݵĵ����ַ�
@portnum���˿ں�
����ֵ��0
*/
afx_msg LRESULT CMFCTestView::OnSerialPortCallback(WPARAM ch, LPARAM portnum)
{
	//GPS����
	if (portnum == m_GPSport){
		//���յ����ַ�
		if (ch != 13 && ch != 10)
		{
			gpsStr += (char)ch;
		}
		//���յ�����һ��ʱ�����ַ���
		else if (gpsStr != "")
		{
			OnGPSAnalysis(gpsStr);
			gpsStr = "";
		}
	}

	//��������
	if (portnum == m_laserport){
		char a = 0x00;
		char b = '0x80';
		unsigned char cha = (unsigned char)ch;
		int i;
		char c16 = ch & 0xFF;
		if (c16==a)
		{
			int dsdf = 324;
		}
		char ctest[3] = { 0 };
		sprintf(ctest, "%02X", cha);
		string cstr = ctest;
		//sscanf(&cha, "%x", ctest);
		//CString str;
		//str.Format(_T("%02X"), cha);//�����յ���BYTE������ת��Ϊ��Ӧ��ʮ������  
		////char szTemp[4];
		////wchar_t *wChar = str.GetBuffer(str.GetLength());
		////str.ReleaseBuffer();
		////// ���õ���wchar* ����תΪ char*����
		////size_t len = wcslen(wChar) + 1;
		////size_t converted = 0;
		////char *cChar;
		////cChar = (char*)malloc(len*sizeof(char));
		////wcstombs_s(&converted, cChar, len, wChar, _TRUNCATE);
		//RecText.Append(str);
		gpsStr += cstr + " ";
		//У���Ƿ�Ϊ�������ݵ���ʼ
		if (LASER_HEADER[laserCheckBufPos] == cstr)
		{
			if (laserCheckBufPos>0)
			{
				int ef = 234;
			}
			laserCheckBuf[laserCheckBufPos] = cstr;
			if (laserCheckBufPos == 6)
			{
				laserDataLength = HexToDem(laserCheckBuf[6] + laserCheckBuf[5]);//����8λ����λ��λ���ϳ�16λ����
				//��һ�����ȫ��������󣬶��ڽ��н���
				if (laserBufPos == laserDataLength*2)
				{
					OnLaserAnalysis(laserBuf, laserDataLength);
				}
			}
			laserCheckBufPos++;
		}
		else
		{
			laserCheckBufPos = 0;
			laserCheckBuf[0] = '\0';
		}

		//����ͬһ��ʱ��������д������
		if (laserBufPos<laserDataLength*2)
		{
			laserBuf[laserBufPos] = cstr;
			laserBufPos++;
		}
	}

	return 0;
}



/*
������GPS�������ݽ���
������
@spData��GPS���
����ֵ��
*/
void CMFCTestView::OnGPSAnalysis(string spData)
{
	CString cTp;
	GPSInfo* gpsInfo = gpsTran.Tanslate(spData);

	//�ж��ǲ���GPS��γ������
	if (gpsInfo->InfoType == GGA)
	{
		double longtitude = ((GGAInfo*)gpsInfo)->Longitude;
		double latitude = ((GGAInfo*)gpsInfo)->Latitude;
		//ˢ�´�����ʾ
		cTp.Format(_T("%lf"), longtitude);
		m_editLongitude.SetWindowText(cTp);
		cTp.Format(_T("%lf"), latitude);
		m_editLatitude.SetWindowTextW(cTp);

		myGPSInfo.Longitude = longtitude;
		myGPSInfo.Latitude = latitude;

		// ���������
		IPointPtr pShowPoint;
		IPointPtr pGpsPoint;
		HRESULT hr1 = pGpsPoint.CreateInstance(CLSID_Point);
		pGpsPoint->PutCoords(myGPSInfo.Longitude, myGPSInfo.Latitude);

		// ˢ������״̬
		cTp.Format(_T("%d"), ((GGAInfo*)gpsInfo)->UseSatelliteNum);
		m_editSatNum.SetWindowText(cTp);
		cTp.Format(_T("%d"), ((GGAInfo*)gpsInfo)->GPSStatus);
		m_editGPSStaus.SetWindowText(cTp);

		// �ж��Ƿ�Ҫת��Ϊ����54����
		if (m_checkShowBJ54.GetCheck()){
			IPointPtr pBJ54Point = geoToProj(pGpsPoint);
			double bj_x, bj_y;
			pBJ54Point->get_X(&bj_x);
			pBJ54Point->get_Y(&bj_y);
			myGPSInfo.BJ54_X = bj_x;
			myGPSInfo.BJ54_Y = bj_y;
			cTp.Format(_T("%lf"), bj_x);
			m_editBJ54_x.SetWindowText(cTp);
			cTp.Format(_T("%lf"), bj_y);
			m_editBJ54_y.SetWindowText(cTp);

			/*ISpatialReferenceFactory2Ptr ipSpaRefFact2(CLSID_SpatialReferenceEnvironment);
			IGeographicCoordinateSystemPtr ipGeoCoordSys;
			ipSpaRefFact2->CreateGeographicCoordinateSystem(esriSRGeoCS_Beijing1954, &ipGeoCoordSys);

			ISpatialReferencePtr ipSRef;
			ipSRef = ipGeoCoordSys;


			IPointPtr point2(CLSID_Point);
			point1->putref_SpatialReference(ipSRef);
			point2->PutCoords(bj_x, bj_y);*/

			pShowPoint = pBJ54Point;
		}
		else
		{
			pShowPoint = pGpsPoint;
		}
		//ˢ��GPSλ��
		IActiveViewPtr iActiveView;
		m_ipMapControl->get_ActiveView(&iActiveView);
		/*IPointPtr ipoint(CLSID_Point);
		if (ipoint == NULL)
		MessageBox(_T("�㴴������"));
		ipoint->PutCoords(myGPSInfo.Longitude*100, myGPSInfo.Latitude*100);*/

		//����
		IGeometryPtr pgeomln(pGpsPoint);
		IGraphicsContainerPtr pgracont(iActiveView);
		//pgracont->DeleteAllElements();

		IMarkerElementPtr pmarkerelem(CLSID_MarkerElement);//����element������element
		if (pmarkerelem == NULL)
			MessageBox(_T("��ͼԪ�ش�������"));
		//IMarkerSymbolPtr imarkerSymbol(m_isymbol);//��m_isymbol��ʼ��imarkerSymbol����symbol
		//pmarkerelem->put_Symbol(imarkerSymbol);//��symbol��ӵ�element
		((IElementPtr)pmarkerelem)->put_Geometry(pgeomln);
		if (lastPointElement != NULL){
			pgracont->DeleteElement(lastPointElement);
		}
		lastPointElement = pmarkerelem;
		pgracont->AddElement((IElementPtr)pmarkerelem, 0);
		iActiveView->Refresh();

		isGPSEnd = false;
	}
	else if (gpsInfo->InfoType == PSAT_HPR)
	{
		CString heading;
		heading.Format(_T("%lf"), ((PSAT_HPRInfo*)gpsInfo)->Heading);
		m_editSeekurHeading.SetWindowText(heading);
		myGPSInfo.Heading = ((PSAT_HPRInfo*)gpsInfo)->Heading;
		isGPSEnd = true;
	}

	//��·���Ѽ����Ҹ�����һ��GPS����ʱ����״̬
	if (isPathExist&&isGPSEnd)
	{
		//���㵱ǰ�����ͺ����
		//::CoInitialize(NULL);

		//��ȡ�����
		IPointPtr pNearestPoint(CLSID_Point);	//�����
		IPointPtr pSeekurPoint(CLSID_Point);		//Seekur���ڵ�
		IGeometryPtr pGeo;
		lastPointElement->get_Geometry(&pGeo);
		pSeekurPoint = (IPointPtr)pGeo;
		double x;
		pSeekurPoint->get_X(&x);
		double y;
		pSeekurPoint->get_Y(&y);
		//pSeekurPoint->PutCoords(12943887.463977, 4857805.079523);

		double distAlongCurveFrom = 0; //������ʵ�㵽����㲿�ֵĳ���
		double distFromCurve = 0;//����㵽���ߵ���̾���
		VARIANT_BOOL isRightSide = false;//������Ƿ������ߵ��ұ�
		m_trackPath->QueryPointAndDistance(esriNoExtension, pSeekurPoint, false, pNearestPoint, &distAlongCurveFrom, &distFromCurve, &isRightSide);

		//�ж��������Seekur����߻����ұߣ���ȡ
		double nearestX, nearestY, seekurX, seekurY;
		pNearestPoint->get_X(&nearestX);
		pNearestPoint->get_Y(&nearestY);
		pSeekurPoint->get_X(&seekurX);
		pSeekurPoint->get_Y(&seekurY);

		//����������Ƿ��ڵ�ǰ·��
		ITopologicalOperatorPtr topo = (ITopologicalOperatorPtr)pNearestPoint;
		/*IGeometryPtr buffer;
		topo->Buffer(0.001, &buffer);
		topo = (ITopologicalOperatorPtr)buffer;*/
		IGeometryPtr pTpGeo;
		HRESULT hr = topo->Intersect((IGeometryPtr)pNowPath, esriGeometry0Dimension, &pTpGeo);
		if (FAILED(hr))
		{
			MessageBox(_T("����������"));
		}
		VARIANT_BOOL isEmpty;	//�����Ƿ�Ϊ��
		pTpGeo->get_IsEmpty(&isEmpty);
		//���ж��Ƿ��ڵ�ǰ·���ϣ����������¼�������·��
		if (isEmpty){
			ISegmentCollectionPtr pSegCol = (ISegmentCollectionPtr)m_trackPath;	//·������
			for (long i = nowPathPos + 1; i < segNum; i++)
			{
				ISegmentPtr pSeg;	//��ǰ����·��
				//ISegmentPtr pSeg(CLSID_Line);	//��ǰ����·��
				pSegCol->get_Segment(i, &pSeg);

				//����������Ƿ��ڵ�ǰ·��
				hr = topo->Intersect((IGeometryPtr)pSeg, esriGeometry0Dimension, &pTpGeo);
				if (FAILED(hr))
				{
					MessageBox(_T("����������"));
				}
				pTpGeo->get_IsEmpty(&isEmpty);
				if (!isEmpty)
				{
					pNowPath = pSeg;
					double angle;	//ֱ����X��нǣ��Ի���Ϊ��λ
					pNowPath->get_Angle(&angle);
					lineHeading = angle * 180 / PI;
					//��Ϊ��ǰ�Ƕ�����������XΪ������ʱ��Ƕȣ���Ӧ���ǵ�ͼ����
					//����Ҫת���Ա�Ϊ����˳ʱ��Ƕ�
					if (lineHeading <= 90)
					{
						//1��3��4����
						lineHeading = 90 - lineHeading;
					}
					else
					{
						//2����
						lineHeading = 360 + (90 - lineHeading);
					}
					nowPathPos = i;
					break;
				}
			}
		}

		distFromCurve *= 100000 * 100;	//cm
		double dis = GetDistance(seekurY, seekurX, nearestY, nearestX) * 1000 * 100;	//Seekur��·�����루cm��
		dis = distFromCurve;
		double subHeading = myGPSInfo.Heading - lineHeading;	//Seekur����·�뾶�����ֵ
		//·��������1��4���ޣ�Seekur������2��3���޵����
		if (lineHeading<180 && myGPSInfo.Heading>180 + lineHeading)
		{
			subHeading = myGPSInfo.Heading - 360 - lineHeading;
		}
		//Seekur������1��4���ޣ�·��������2��3���޵����
		if (lineHeading > 180 && myGPSInfo.Heading < lineHeading - 180)
		{
			subHeading = myGPSInfo.Heading + 360 - lineHeading;
		}
		if (!isRightSide){
			dis = -dis;
		}
		err = dis + kinter*subHeading;		// ������ǰ״̬
		

		CString cTp;
		cTp.Format(_T("%lf"), subHeading);
		m_editSubHeading.SetWindowText(cTp);
		cTp.Format(_T("%lf"), dis);
		m_editDis.SetWindowText(cTp);

		//��׷��״̬�Ҳ�Ϊ����״̬ʱ�����˶�ָ��
		if (isTracked&&!isAvoid)
		{
			double turnHeading = 0;	//ת���
			//P����
			turnHeading = kp*err;

			if (m_cUseID.GetCheck()){
				//50����������I����
				if (dis < 5)
				{
					integral += err;
					turnHeading += ki*integral;
				}

				//D����
				turnHeading += kd*(err - err_last);
				err_last = err;
			}

			double maxTrun = 60;	//���ת���
			/*if (dis < 5)
			{
			maxTrun = 10;
			}
			else if (dis < 10)
			{
			maxTrun = 20;
			}
			else if (dis < 20)
			{
			maxTrun = 30;
			}*/
			//�жϻ�������·���Ҳ໹�����
			if (isRightSide)
			{
				//����ת�ǣ�ʹת�ǲ�����ת���ĺ����ֵ�������ת���
				if (turnHeading - subHeading >= maxTrun)
				{
					turnHeading = maxTrun + subHeading;
				}
				////��ֹ���Ҳ�ʱ��ƫ
				//if ((subHeading + turnHeading) <= -maxTrun)
				//{
				//	turnHeading = -maxTrun - subHeading;
				//}
			}
			else
			{
				maxTrun = -maxTrun;
				//����ת�ǣ�ʹת�ǲ�����ת���ĺ����ֵ�������ת���
				if (turnHeading - subHeading <= maxTrun)
				{
					turnHeading = maxTrun + subHeading;
				}
				////��ֹ�����ʱ��ƫ
				//if ((subHeading + turnHeading) >= maxTrun)
				//{
				//	turnHeading = maxTrun - subHeading;
				//}
			}
			cTp.Format(_T("%lf"), turnHeading);
			m_editSeekurVel.SetWindowText(cTp);

			//���Ϳ���ָ��
			seekurParaPtr pPara = new seekurPara();
			pPara->distance = 0;
			pPara->veloctiy = 0;
			pPara->heading = turnHeading;

			PostThreadMessage(seekur_thread->m_nThreadID, WM_SEEKUR_MOVE, (UINT)pPara, 0);
		}
		else{
			integral = 0;
			err_last = 0;
		}
	}

	delete gpsInfo;
}


/*
���������⴮�����ݽ���
������
@buf��16���Ƽ��������
@len�����ݳ���
����ֵ��
*/
void CMFCTestView::OnLaserAnalysis(string *buf, int len){
	for (int i = 0; i < len; i++)
	{
		//only upper 12 bits of upper byte are used
		laserData[i] = HexToDem(buf[2 * i + 1] + buf[2 * i]);
		//intBuf[i] = buf[2 * i] | ((buf[2 * i + 1] & 0x1f) << 8);
	}
	//�������ݣ��ж��Ƿ���Ҫ����
	if (isTracked)
	{
		obsAvoid.Initialize(10 * 1000, 90, 90);
		bool isObs = obsAvoid.SetObstaclePosture(laserData, len);
		if (isObs)
		{
			isAvoid = true;
			float turnHeading = obsAvoid.ComputeAvoidHeading();
			//���Ϳ���ָ��
			seekurParaPtr pPara = new seekurPara();
			pPara->distance = 0;
			pPara->veloctiy = 0;
			pPara->heading = turnHeading;

			PostThreadMessage(seekur_thread->m_nThreadID, WM_SEEKUR_MOVE, (UINT)pPara, 0);
		}
		else
		{
			isAvoid = false;
		}
		
	}
}

UINT CMFCTestView::SeekurFuc(LPVOID lParam){
	//��ȡexeִ���ļ�·��
	HMODULE module = GetModuleHandle(0);
	TCHAR exeFullPath[MAX_PATH]; // MAX_PATH
	GetModuleFileName(NULL, exeFullPath, MAX_PATH);
	CString fileName(exeFullPath);
	int argc = 3;
	char rp[MAX_PATH];
#if defined(UNICODE) 
	WideCharToMultiByte(CP_ACP, 0, exeFullPath, -1, rp, wcslen(exeFullPath), 0, 0);
	rp[wcslen(exeFullPath)] = 0;
#else 
	lstrcpy((PTSTR)rp, (PTSTR)pFileName);
#endif 
	char *argv[3];
	/*string path1 = rp;*/
	argv[0] = rp;
	argv[1] = "-rp";
	argv[2] = "com3";

	Aria::init();
	ArArgumentParser argParser(&argc, argv);
	argParser.loadDefaultArguments();
	ArRobot* robot = new ArRobot();
	ArRobotConnector con(&argParser, robot);

	//�������������еĲ���
	if (!Aria::parseArgs()){
		Aria::logOptions();
	}
	//���ӻ�����
	if (!con.connectRobot()){
		ArLog::log(ArLog::Normal, "�޷����ӻ�����");
		if (argParser.checkHelpAndWarnUnparsed()){
			ArLog::log(ArLog::Terse, "�޷����ӻ�����");
		}
	}

	//robot->com2Bytes(116, 6, 1);
	//robot->com2Bytes(116, 7, 1);
	//robot->com2Bytes(116, 28, 1);
	//robot->com2Bytes(116, 12, 1);


	// �첽���л����˴���ѭ��
	robot->enableMotors();
	robot->runAsync(true);
	BaseAction action(robot);

	MSG msg;
	while (true)
	{
		GetMessage(&msg, NULL, 0, 0);
		if (msg.message == WM_SEEKUR_MOVE)
		{
			seekurParaPtr pPara = (seekurParaPtr)msg.wParam;
			//ֱ���ƶ�����
			if (pPara->distance != 0)
				action.Move(pPara->distance);
			//ת���Ƕ�
			if (pPara->heading != 0)
				action.SetDeltaHeading(pPara->heading);
			//ֱ���ƶ�����
			if (pPara->veloctiy != 0)
				action.SetVelocity(pPara->veloctiy);
			delete pPara;
		}

		if (msg.message == WM_SEEKUR_STOP)
		{
			action.Stop();
		}
		if (msg.message == WM_SEEKUR_END)
		{
			action.Stop();
			break;
		}
		//::PostMessage((HWND)lParam,WM_SEEKUR_RET, 0, 0);//�����Զ�����Ϣ
	}
	//robot->comInt(ArCommands::ENABLE, 1);
	robot->disconnect();
	Aria::exit();

	return 0;
}

UINT CMFCTestView::TrackFuc(LPVOID lParam){

	MSG msg;
	//bool isStart = false;
	bool isFirst = true;	//�Ƿ�Ϊ�տ�ʼ׷��
	GetMessage(&msg, NULL, 0, 0);
	CMFCTestView* pView = (CMFCTestView*)msg.wParam;

	::CoInitialize(NULL);
	//��ȡ·��������ֱ��
	IPolylinePtr pPolyline;
	pPolyline = pView->m_trackPath;
	ISegmentCollectionPtr pSegCol = (ISegmentCollectionPtr)pPolyline;	//ֱ�߼���
	long num;
	pSegCol->get_SegmentCount(&num);
	IPointPtr pStartPoint(CLSID_Point);		//��ǰ·����ʼ��
	IPointPtr pEndPoint(CLSID_Point);		//��ǰ·��������
	IPolylinePtr pNowPath(CLSID_Polyline);	//��ǰ·��
	int nowPathPos = -1;		//��ǰ·����ֱ�߼����е�λ��
	double lineHeading;		//·������
	//pNowPath = pPolyline;

	while (pView->isTracked){
		//��ȡ�����
		IPointPtr pNearestPoint(CLSID_Point);	//�����
		IPointPtr pSeekurPoint(CLSID_Point);		//Seekur���ڵ�
		IGeometryPtr pGeo;
		pView->lastPointElement->get_Geometry(&pGeo);
		pSeekurPoint = (IPointPtr)pGeo;
		double x;
		pSeekurPoint->get_X(&x);
		double y;
		pSeekurPoint->get_Y(&y);
		//pSeekurPoint->PutCoords(12943887.463977, 4857805.079523);

		double distAlongCurveFrom = 0; //������ʵ�㵽����㲿�ֵĳ���
		double distFromCurve = 0;//����㵽���ߵ���̾���
		VARIANT_BOOL isRightSide = false;//������Ƿ������ߵ��ұ�
		pPolyline->QueryPointAndDistance(esriNoExtension, pSeekurPoint, false, pNearestPoint, &distAlongCurveFrom, &distFromCurve, &isRightSide);

		//�ж��������Seekur����߻����ұߣ���ȡ
		double nearestX, nearestY, seekurX, seekurY;
		pNearestPoint->get_X(&nearestX);
		pNearestPoint->get_Y(&nearestY);
		pSeekurPoint->get_X(&seekurX);
		pSeekurPoint->get_Y(&seekurY);

		//����������Ƿ��ڵ�ǰ·��
		ITopologicalOperatorPtr topo = (ITopologicalOperatorPtr)pNearestPoint;
		/*IGeometryPtr buffer;
		topo->Buffer(0.001, &buffer);
		topo = (ITopologicalOperatorPtr)buffer;*/
		IGeometryPtr pTpGeo;
		HRESULT hr = topo->Intersect((IGeometryPtr)pNowPath, esriGeometry0Dimension, &pTpGeo);
		if (FAILED(hr))
		{
			pView->MessageBox(_T("����������"));
		}
		VARIANT_BOOL isEmpty;	//�����Ƿ�Ϊ��
		pTpGeo->get_IsEmpty(&isEmpty);
		//���ж��Ƿ��ڵ�ǰ·���ϣ����������¼�������·��
		if (isEmpty){
			for (long i = nowPathPos + 1; i < num; i++)
			{
				ISegmentPtr pSeg;	//��ǰ����·��
				//ISegmentPtr pSeg(CLSID_Line);	//��ǰ����·��
				pSegCol->get_Segment(i, &pSeg);

				//����polyline·��
				IPolylinePtr ptpPath(CLSID_Polyline);
				ISegmentCollectionPtr ptpSegCol = (ISegmentCollectionPtr)ptpPath;
				ptpSegCol->AddSegment(pSeg);


				//����������Ƿ��ڵ�ǰ·��
				hr = topo->Intersect((IGeometryPtr)ptpPath, esriGeometry0Dimension, &pTpGeo);
				if (FAILED(hr))
				{
					pView->MessageBox(_T("����������"));
				}
				pTpGeo->get_IsEmpty(&isEmpty);
				if (!isEmpty)
				{
					pNowPath = ptpPath;
					pSeg->get_FromPoint(&pStartPoint);
					pSeg->get_ToPoint(&pEndPoint);
					//����·������
					double startX, startY;
					pStartPoint->get_X(&startX);
					pStartPoint->get_Y(&startY);

					double endX, endY;
					pEndPoint->get_X(&endX);
					pEndPoint->get_Y(&endY);

					lineHeading = atan2(endY - startY, endX - startX) * 180 / PI;
					if (lineHeading > 0)
					{
						lineHeading = 90 - lineHeading;
					}
					else
					{
						lineHeading = -lineHeading;
					}
					nowPathPos = i;
					break;
				}
			}
		}

		CString cKDis;
		pView->m_editKp.GetWindowTextW(cKDis);
		CString cKSubHead;
		pView->m_editKinter.GetWindowTextW(cKSubHead);
		double kDis = _ttof(cKDis);
		double kSubHead = _ttof(cKSubHead);

		distFromCurve *= 100000;
		double dis = GetDistance(seekurY, seekurX, nearestY, nearestX) * 1000;	//Seekur��·�����루�ף�
		double subHeading = lineHeading - pView->myGPSInfo.Heading;	//·��������Seekur�����ֵ

		double turnHeading = 0;	//ת���
		double maxTrun = 45;	//���ת���
		if (dis > 2){
			maxTrun = 45;
		}
		else if (dis > 1){
			maxTrun = 30;
		}
		else if (dis>0.5)
		{
			maxTrun = 20;
		}
		else
		{
			maxTrun = 10;
		}


		/*if (turnHeading > maxTrun){
			int dfsds = 23;
		}*/
		//�жϻ�������·���Ҳ໹�����
		if (isRightSide)
		{
			//dis += 0.1;
			turnHeading = kDis*dis - kSubHead*subHeading;
			//����ת�ǣ�ʹת�ǲ�����ת���ĺ����ֵ�������ת���
			if ((subHeading + turnHeading) >= maxTrun)
			{
				turnHeading = maxTrun - subHeading;
			}
			//��ֹ���Ҳ�ʱ��ƫ
			if ((subHeading + turnHeading) <= -maxTrun)
			{
				turnHeading = -maxTrun - subHeading;
			}

			////���Ҳ�ʱ�����ֵ���ڵ������ת���ʱ����ת��
			//if (subHeading >= maxTrun)
			//{
			//	turnHeading= 0;
			//}
			/*if (dis <= diffDis&&subHeading >= (maxTrun / 2))
			{
			turnHeading = turnHeading/ - 2;
			}*/
		}
		else
		{
			dis = -dis;
			//dis += 0.1;
			turnHeading = kDis*dis + kSubHead*subHeading;
			//����ת�ǣ�ʹת�ǲ�����ת���ĺ����ֵ�������ת���
			if ((subHeading + turnHeading) <= -maxTrun)
			{
				turnHeading = -maxTrun - subHeading;
			}


			//��ֹ���Ҳ�ʱ��ƫ
			if ((subHeading + turnHeading) >= maxTrun)
			{
				turnHeading = maxTrun - subHeading;
			}
			////�����ʱ�����ֵС�ڵ��ڸ������ת���ʱ����ת��
			//if (subHeading <= -maxTrun)
			//{
			//	turnHeading = 0;
			//}
			//if (dis <= diffDis&&subHeading <=( -maxTrun / 2))
			//{
			//	turnHeading = turnHeading /- 2;
			//}
			//turnHeading = -turnHeading;
		}

		CString str;
		str.Format(_T("%lf"), subHeading);
		pView->m_editSubHeading.SetWindowText(str);
		str.Format(_T("%lf"), dis);
		pView->m_editDis.SetWindowText(str);
		CString vel;
		vel.Format(_T("%lf"), turnHeading);
		pView->m_editSeekurVel.SetWindowText(vel);

		seekurParaPtr pPara = new seekurPara();
		pPara->distance = 0;
		pPara->heading = turnHeading;

		if (isFirst)
		{
			CString cVel;
			pView->m_editVelocity.GetWindowTextW(cVel);
			double vel = _ttof(cVel);
			pPara->veloctiy = pView->m_velocity;
			isFirst = false;
		}

		PostThreadMessage(pView->seekur_thread->m_nThreadID, WM_SEEKUR_MOVE, (UINT)pPara, 0);

		//::PostMessage((HWND)lParam, WM_SEEKUR_RET, 0, 0);//�����Զ�����Ϣ
		Sleep(20);
	}
	PostThreadMessage(pView->seekur_thread->m_nThreadID, WM_SEEKUR_STOP, 0, 0);
	return 0;
}
#pragma endregion

void CMFCTestView::OnBtnSavePath()
{
	//�ֽ��ļ�·��������
	CString strFolder = _T("D:\\arcMap");
	CString strFile = _T("path.shp");

	//���·���Ѽ��أ�ɾ������ͼ���Դ�ļ�
	long num;
	m_ipMapControl->get_LayerCount(&num);
	ILayerPtr pLayer;
	for (int i = 0; i < num; i++)
	{
		BSTR name;
		m_ipMapControl->get_Layer(i, &pLayer);
		pLayer->get_Name(&name);
		CString cName(name);
		if (!pathLayerName.CompareNoCase(cName))
		{
			m_ipMapControl->DeleteLayer(i);
			CFileFind finder;
			BOOL bWorking = finder.FindFile(_T("F:\\ArcMap\\path.*"));
			while (bWorking)
			{
				bWorking = finder.FindNextFile();
				if (bWorking)
				{
					CString filePath = finder.GetFilePath();
					DeleteFile(filePath);
				}
			}
			break;
		}
	}
	isPathExist = false;

	//Ϊ���SHP�ļ�������������
	IWorkspaceFactoryPtr ipWorkFact(CLSID_ShapefileWorkspaceFactory);
	IWorkspacePtr ipWork;
	HRESULT hr = ipWorkFact->OpenFromFile(CComBSTR(strFolder), 0, &ipWork);
	if (FAILED(hr) || ipWork == 0)
	{
		AfxMessageBox(_T("�޷���Ŀ���ļ��У�"), MB_ICONINFORMATION);
		/*if (FAILED(hr))
		return hr;
		else
		return E_FAIL;*/
	}
	IFeatureWorkspacePtr ipFeatWork(ipWork);

	// Set up fields
	IFieldsPtr ipFields(CLSID_Fields);
	IFieldsEditPtr ipFieldsEdit;
	ipFieldsEdit = ipFields; //QI
	// Geometry and spatial reference
	IFieldPtr ipField(CLSID_Field);
	IFieldEditPtr ipFieldEdit;
	ipFieldEdit = ipField; //QI
	ipFieldEdit->put_Name(CComBSTR("Shape"));
	ipFieldEdit->put_Type(esriFieldTypeGeometry);

	//�����ռ���������
	IGeometryDefPtr ipGeoDef(CLSID_GeometryDef);
	IGeometryDefEditPtr ipGeoDefEdit;
	ipGeoDefEdit = ipGeoDef; //QI
	ipGeoDefEdit->put_GeometryType(esriGeometryPolyline);

	//��������ϵ
	ISpatialReferenceFactory2Ptr ipSpaRefFact2(CLSID_SpatialReferenceEnvironment);
	IGeographicCoordinateSystemPtr ipGeoCoordSys;
	ipSpaRefFact2->CreateGeographicCoordinateSystem(esriSRGeoCS_Beijing1954, &ipGeoCoordSys);
	//ipSpaRefFact2->CreateGeographicCoordinateSystem(esriSRGeoCS_WGS1984, &ipGeoCoordSys);

	ISpatialReferencePtr ipSRef;
	ipSRef = ipGeoCoordSys;
	ipGeoDefEdit->putref_SpatialReference(ipSRef);
	ipFieldEdit->putref_GeometryDef(ipGeoDef);
	ipFieldsEdit->AddField(ipField);

	// Create the shapefile
	IFeatureClassPtr pFeatureClass;
	hr = ipFeatWork->CreateFeatureClass(CComBSTR(strFile), ipFields, 0, 0,
		esriFTSimple, CComBSTR("Shape"), 0, &pFeatureClass);

	IFeaturePtr pFeature;
	pFeatureClass->CreateFeature(&pFeature);



	pFeature->putref_Shape(pPath);
	pFeature->Store();

	CString str(_T("·����ʼ��"));
	m_btnPathSelect.SetWindowTextW(str);
	pPath->Release();
}

void CMFCTestView::OnMouseDownMapcontrol1(long button, long shift, long X, long Y, double mapX, double mapY)
{
	//IActiveViewPtr iActiveView(m_ipMapControl);

	//��ȡ��ǰ�ҳ�漰������ڵ�
	IActiveViewPtr iActiveView;
	m_ipMapControl->get_ActiveView(&iActiveView);
	IPointPtr ipoint(CLSID_Point);

	if (ipoint == NULL) return;
	ipoint->PutCoords(mapX, mapY);
	AddCreateElement((IGeometryPtr)ipoint, iActiveView);
	iActiveView->Refresh();

	//m_cUseID.SetCheck(1);

	//if (m_cUseID.GetCheck())
	//{
	//	//����
	//	//IGeometryPtr pGeometry(ipoint);
	//	AddCreateElement((IGeometryPtr)ipoint, iActiveView);
	//	iActiveView->Refresh();
	//}
	//else{
	//	//����
	//	HRESULT hr;
	//	if (m_pNewLineFeedback == NULL)
	//	{
	//		CoCreateInstance(CLSID_NewLineFeedback,
	//			NULL, CLSCTX_ALL, IID_INewLineFeedback,
	//			(void**)&m_pNewLineFeedback);
	//		ISymbolPtr isymbol;
	//		if (m_pNewLineFeedback == NULL) return;
	//		m_pNewLineFeedback->get_Symbol(&isymbol);
	//		ISimpleLineSymbolPtr pslnsym(isymbol);
	//		if (pslnsym == NULL) return;
	//		IRgbColorPtr prgb(CLSID_RgbColor);
	//		prgb->put_Red(0);
	//		prgb->put_Green(205);
	//		prgb->put_Blue(0);
	//		IColorPtr icolor(prgb);

	//		pslnsym->put_Color(icolor);
	//		pslnsym->put_Style(esriSLSDot);

	//		IScreenDisplayPtr pScrD;
	//		iActiveView->get_ScreenDisplay(&pScrD);

	//		m_pNewLineFeedback->putref_Display(pScrD);
	//		m_pNewLineFeedback->Start(ipoint);
	//	}
	//	else
	//		hr = m_pNewLineFeedback->AddPoint(ipoint);
	//}



}

void CMFCTestView::OnTestMarkerStyle()
{
	ISymbolSelectorPtr pSymbolSelector(CLSID_SymbolSelector);//symbol select
	ISimpleMarkerSymbolPtr psimpleMarksb(CLSID_SimpleMarkerSymbol);//simple marker
	VARIANT_BOOL bOK;
	if (pSymbolSelector == NULL) return;
	pSymbolSelector->AddSymbol((ISymbolPtr)psimpleMarksb, &bOK);//��simple marker��ӵ�symbol select��
	HRESULT hr;
	hr = pSymbolSelector->SelectSymbol(0, &bOK);
	if (FAILED(hr)) return;
	if (bOK)
		pSymbolSelector->GetSymbolAt(0, &m_isymbol);

}

void CMFCTestView::AddCreateElement(IGeometryPtr pgeomln, IActiveViewPtr iactiveview)
{
	IGraphicsContainerPtr pgracont(iactiveview);
	//pgracont->DeleteAllElements();

	IMarkerElementPtr pmarkerelem(CLSID_MarkerElement);//����element������element
	if (pmarkerelem == NULL) return;
	IMarkerSymbolPtr imarkerSymbol(m_isymbol);//��m_isymbol��ʼ��imarkerSymbol����symbol
	pmarkerelem->put_Symbol(imarkerSymbol);//��symbol��ӵ�element
	((IElementPtr)pmarkerelem)->put_Geometry(pgeomln);
	if (lastPointElement != NULL){
		pgracont->DeleteElement(lastPointElement);
	}
	lastPointElement = pmarkerelem;
	pgracont->AddElement((IElementPtr)pmarkerelem, 0);
}


IPoint* CMFCTestView::geoToProj(IPoint* point/*��Ҫ��������ϵ�ĵ�*/, long fromProjType, long toGeoType)
{
	long geoType = toGeoType;//4326;
	IPoint* points = point;
	ISpatialReference* spatialRf;
	ISpatialReference* spatialRf1;
	IGeographicCoordinateSystem* geograpicsys;
	IProjectedCoordinateSystem*projCoordSystem;
	ISpatialReferenceFactoryPtr originalSpecialReference;
	ISpatialReferenceFactoryPtr newReferenceSystem;

	HRESULT hr = originalSpecialReference.CreateInstance(CLSID_SpatialReferenceEnvironment);

	HRESULT hr1 = originalSpecialReference->CreateProjectedCoordinateSystem(fromProjType, &projCoordSystem);
	spatialRf = (ISpatialReference*)projCoordSystem;
	//HRESULT hr2 = points->putref_SpatialReference(spatialRf);


	newReferenceSystem.CreateInstance(CLSID_SpatialReferenceEnvironment);
	newReferenceSystem->CreateGeographicCoordinateSystem(geoType, &geograpicsys);
	spatialRf1 = (ISpatialReference*)geograpicsys;
	points->putref_SpatialReference(spatialRf1);//����ԭʼ�ռ�ο�

	points->Project(spatialRf);
	return points;
};

void CMFCTestView::OnDoubleClickMapcontrol1(long button, long shift, long X, long Y, double mapX, double mapY)
{
	// TODO: �ڴ˴������Ϣ����������
	IActiveViewPtr iActiveView;
	m_ipMapControl->get_ActiveView(&iActiveView);
	IPolylinePtr ipolyline;
	if (m_pNewLineFeedback == NULL) return;
	if (m_isymbol != NULL) m_pNewLineFeedback->putref_Symbol(m_isymbol);
	m_pNewLineFeedback->Stop(&ipolyline);

	//m_pgeometry = ipolyline;
	if (ipolyline != NULL)
	{
		//AddCreateElement(m_pgeometry, iActiveView);


		IGraphicsContainerPtr pgracont(iActiveView);
		ILineElementPtr pLineElement(CLSID_LineElement);
		//pLineElement->put_Symbol(m_isymbol);

		IElementPtr pElement = (IElementPtr)pLineElement;
		pElement->put_Geometry(ipolyline);
		pgracont->AddElement((IElementPtr)pElement, 0);
	}
	m_pNewLineFeedback = NULL;
	iActiveView->Refresh();

}

void CMFCTestView::OnMouseMoveMapcontrol1(long button, long shift, long X, long Y, double mapX, double mapY)
{
	//MapComposer mapComposer;
	/*mapComposer.m_ipMap = m_ipMapControl->get_Map();
	mapComposer.ShowMouseCoord(mapX, mapY);
	IPointPtr pPoint(CLSID_Point);
	pPoint->PutCoords(mapX, mapY);*/
	//switch (m_operateType)
	//{
	//case 1:
	//	m_ctrlMap.put_MousePointer(2);//esriPointerCrosshair == 2
	//	if (m_pNewLineFeedback != NULL)
	//		m_pNewLineFeedback->MoveTo(pPoint);
	//	break;
	//}
	//// TODO: �ڴ˴������Ϣ����������
}


void CMFCTestView::OnFileSave()
{
	// TODO:  �ڴ���������������
	CFileDialog dlg(FALSE, //TRUEΪOPEN�Ի���FALSEΪSAVE AS�Ի���  
		_T("shp"),
		_T("newFile"),
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		(LPCTSTR)_TEXT("Shape files(*.shp)|*.shp|mxd�ĵ�(*.mxd)|*.mxd|"),
		NULL);
	CString m_strFileName;
	if (dlg.DoModal() == IDOK)
	{
		//m_MapControl=new CMapControl2();  


		m_strFileName = dlg.GetPathName();//ȫ·����  
		//CString filepath=dlg.GetFolderPath();//·�����ƣ������ļ���  
		//CString filename=dlg.GetFileName();//�ļ���������·��  
		CString strExt = dlg.GetFileExt();//��׺����������  
		if (strExt == "shp")
		{
			CComBSTR MX_DATAFILE;
			MX_DATAFILE = dlg.GetPathName();
			BSTR filePath = dlg.GetFolderPath().AllocSysString();
			BSTR fileName = dlg.GetFileName().AllocSysString();
			m_ipMapControl->AddShapeFile(filePath, fileName);
		}
		else if (strExt == "mxd")
		{
			CComBSTR MX_DATAFILE;
			MX_DATAFILE = dlg.GetPathName();
			VARIANT_BOOL bValidDoc;
			//m_MapControlView.CheckMxFile( MX_DATAFILE );  
			m_ipMapControl->CheckMxFile(MX_DATAFILE, &bValidDoc);
			//VARIANT vt = 0;  
			if (bValidDoc)
				m_ipMapControl->LoadMxFile(MX_DATAFILE);
		}
		else
		{
			AfxMessageBox(_T("��ѡ����ʵ��ļ�!"));
			return;
		}
		m_ipMapControl->Refresh(esriViewAll);
	}
}

//�����˶���ť
void CMFCTestView::OnBtnMoveSeekur()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	//��ȡҪ�ƶ��ľ���
	CString cTp;
	m_editMoveDistance.GetWindowTextW(cTp);
	double dis = _ttof(cTp) * 1000;
	m_editMoveHeading.GetWindowTextW(cTp);
	double head = _ttof(cTp);

	//��������ȷ�ϲ����Ƿ�����
	CString alert;
	alert.Format(_T("ȷ��Ϊ���²�����\n���룺%lf\n�Ƕȣ�%lf"), dis, head);
	int result = AfxMessageBox(alert, MB_OKCANCEL);
	if (result==IDOK)
	{
		seekurParaPtr pPara = new seekurPara();
		pPara->distance = dis;
		pPara->heading = head;

		PostThreadMessage(seekur_thread->m_nThreadID, WM_SEEKUR_MOVE, (UINT)pPara, NULL);
		//PostThreadMessage(seekur_thread->m_nThreadID, WM_SEEKUR_MOVE, dis, head);

		m_editMoveDistance.SetWindowTextW(_T(""));
		m_editMoveHeading.SetWindowTextW(_T(""));
	}

}


afx_msg LRESULT CMFCTestView::OnSeekur(WPARAM wParam, LPARAM lParam)
{
	//MessageBox(_T("����ͨ��"));
	/*m_editLongitude = (CEdit *)GetDlgItem(IDC_EDIT1);
	m_editLongitude->SetWindowTextW(_T("������Ϣ"));*/
	return 0;
}

//����׷��
void CMFCTestView::OnBtnTrack()
{
	if (!isPathExist){
		MessageBox(_T("��·����"));
	}
	else{
		if (!isTracked)
		{
			//����׷��
			isTracked = true;
			
			//��ʼ������
			nowPathPos = 0;
			CString cTp;	//��ʱCstring�࣬���ڽ��ı�������תΪ������ʽ
			m_editKp.GetWindowTextW(cTp);
			kp = _ttof(cTp);
			m_editKinter.GetWindowTextW(cTp);
			kinter = _ttof(cTp);
			m_editKi.GetWindowTextW(cTp);
			ki = _ttof(cTp);
			m_editKd.GetWindowTextW(cTp);
			kd = _ttof(cTp);
			integral = 0;
			err_last = 0;

			//��Seekur��ʼ�˶�
			seekurParaPtr pPara = new seekurPara();
			pPara->distance = 0;
			pPara->heading = 0;
			m_editVelocity.GetWindowTextW(cTp);
			double vel = _ttof(cTp);
			pPara->veloctiy = vel;
			PostThreadMessage(seekur_thread->m_nThreadID, WM_SEEKUR_MOVE, (UINT)pPara, 0);
			m_btnTrack.SetWindowTextW(_T("ֹͣ׷��"));
		}
		else{
			//PostThreadMessage(track_thread->m_nThreadID, WM_TRACK_STOP, 0, 0);
			isTracked = false;
			PostThreadMessage(seekur_thread->m_nThreadID, WM_SEEKUR_STOP, 0, 0);
			m_btnTrack.SetWindowTextW(_T("��ʼ׷��"));
		}
	}

}

/*
��·������ӵ㣬��·��ʱ�򴴽�·��
*/
void CMFCTestView::OnBtnPathAdd()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	//·��Ϊ��ʱ����·��
	if (!pPath)
	{
		HRESULT hr1 = pPath.CreateInstance(CLSID_Polyline);
		CString str(_T("��һ��"));
		m_btnPathSelect.SetWindowTextW(str);
	}

	IPointCollectionPtr pPtclo = (IPointCollectionPtr)pPath;
	IPointPtr point(CLSID_Point);
	point->PutCoords(myGPSInfo.Longitude, myGPSInfo.Latitude);
	pPtclo->AddPoint(point);
	//point->putref_SpatialReference()
}