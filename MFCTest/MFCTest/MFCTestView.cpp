
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

#include "MainFrm.h"

#define WM_SEEKUR_RET WM_USER+100		 //Seekur������Ϣ
#define WM_SEEKUR_MOVE WM_USER+101		//Seekur������Ϣ����ֵ��
//#define WM_SEEKUR_SUB WM_USER+102		//Seekur������Ϣ����ֵ��
#define WM_SEEKUR_END WM_USER+103		//Seekur������Ϣ��������

#define WM_TRACK_START WM_USER+104		//��ʼ׷��
#define WM_TRACK_STOP WM_USER+105		//ֹͣ׷��
#define WM_TRACK_END WM_USER+106		//����׷��

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define wm

// CMFCTestView

IMPLEMENT_DYNCREATE(CMFCTestView, CFormView)

BEGIN_MESSAGE_MAP(CMFCTestView, CFormView)
	ON_WM_CREATE()
	ON_COMMAND(ID_FILE_OPEN, &CMFCTestView::OnFileOpen)
//	ON_WM_SIZE()
ON_MESSAGE(WM_COMM_RXCHAR, &CMFCTestView::OnCommunication)
ON_BN_CLICKED(IDC_BUTTON1, &CMFCTestView::OnClickedButton1)
ON_COMMAND(ID_FILE_SAVE, &CMFCTestView::OnFileSave)
ON_BN_CLICKED(IDC_BUTTON2, &CMFCTestView::OnBnClickedButton2)
ON_MESSAGE(WM_SEEKUR_RET, &CMFCTestView::OnSeekur)
ON_BN_CLICKED(IDC_BUTTON3, &CMFCTestView::OnBnClickedButton3)
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
{
	// TODO:  �ڴ˴���ӹ������

}

CMFCTestView::~CMFCTestView()
{
	PostThreadMessage(seekur_thread->m_nThreadID, WM_SEEKUR_END, 0, 0);
	PostThreadMessage(track_thread->m_nThreadID, WM_TRACK_STOP, 0, 0);
}

void CMFCTestView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT6, m_editHeading);
	DDX_Control(pDX, IDC_EDIT5, m_editDistance);
	DDX_Control(pDX, IDC_EDIT2, m_editLongitude);
	DDX_Control(pDX, IDC_EDIT1, m_editLatitude);
	DDX_Control(pDX, IDC_CHECK1, m_cPointLine);
	DDX_Control(pDX, IDC_EDIT4, m_editSeekurVel);
	DDX_Control(pDX, IDC_EDIT3, m_editSeekurHeading);
	DDX_Control(pDX, IDC_EDIT7, m_editKDis);
	DDX_Control(pDX, IDC_EDIT8, m_editKSubHead);
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
	CWnd* pWndCal=GetDlgItem(IDC_MAPCONTROL1);
	LPUNKNOWN pUnk=pWndCal->GetControlUnknown();
	pUnk->QueryInterface(IID_IMapControl2,(LPVOID *)&m_ipMapControl);

	CWnd* pWndCal2 = GetDlgItem(IDC_TOCCONTROL1);
	LPUNKNOWN pUnk2 = pWndCal2->GetControlUnknown();
	pUnk2->QueryInterface(IID_ITOCControl2, (LPVOID *)&m_ipTocControl);
	m_ipTocControl->SetBuddyControl(m_ipMapControl);

	//���ô��ڳ��򣬴���GPS��ͨ��
	CMainFrame *p = (CMainFrame*)AfxGetMainWnd();
	CMFCTestView *m_CView = (CMFCTestView *)p->GetActiveView();//��ȡView���ָ��
	if (serialPort.InitPort(m_CView, 2))
	{
		serialPort.StartMonitoring();
	}
	else
	{
		MessageBox(_T("GPS����ͨ��ʧ�ܣ�"), _T("��ʾ"), MB_OK);
	}

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
	
	track_thread = AfxBeginThread(
		TrackFuc,
		this->GetSafeHwnd()   // ���ݴ���ľ��
		);

	
}
//
//HRESULT CMFCTestView::CreateShapeFile(esriGeometryType type, CString layerPath, CString layerName, IFeatureClass** ppFeatureClass)
//{
//	if (!ppFeatureClass)
//		return E_POINTER;
//
//	IWorkspaceFactoryPtr ipWKSPFac(CLSID_ShapefileWorkspaceFactory);
//	IWorkspacePtr ipWKSP;
//	HRESULT hr = ipWKSPFac->OpenFromFile((CComBSTR)layerPath, 0, &ipWKSP);
//	if (FAILED(hr) || ipWKSP == 0)
//	{
//		AfxMessageBox(_T("�޷���Ŀ���ļ��У�"), MB_ICONINFORMATION);
//		return hr;
//	}
//	IFeatureWorkspacePtr ipFeatureWKSP(ipWKSP);
//
//	// Set up a simple fields collection
//	IFieldsPtr ipFields(CLSID_Fields);
//	IFieldsEditPtr ipFieldsEdit(ipFields);
//
//	// Make and add the shape field. It will need a geometry definition and a spatial reference
//	IFieldPtr ipField(CLSID_Field);
//	IFieldEditPtr ipFieldEdit(ipField);
//	hr = ipFieldEdit->put_Name(CComBSTR(L"Shape"));
//	hr = ipFieldEdit->put_Type(esriFieldTypeGeometry);
//
//	IGeometryDefPtr ipGeomDef(CLSID_GeometryDef);
//	IGeometryDefEditPtr ipGeomDefEdit(ipGeomDef);
//	hr = ipGeomDefEdit->put_GeometryType(type);
//	ISpatialReferencePtr ipUnkSpatial(CLSID_UnknownCoordinateSystem);
//	hr = ipGeomDefEdit->putref_SpatialReference(ipUnkSpatial);
//	ipFieldEdit->putref_GeometryDef(ipGeomDef);
//	ipFieldsEdit->AddField(ipField);
//
//	// Add another miscellanesous text field
//	IFieldPtr ipX(CLSID_Field);
//	IFieldPtr ipY(CLSID_Field);
//	IFieldPtr ipValue(CLSID_Field);
//	long len = 30;
//	ipFieldEdit = ipX;
//	ipFieldEdit->put_Length(len);
//	ipFieldEdit->put_Name(CComBSTR(L"X"));
//	ipFieldEdit->put_Type(esriFieldTypeDouble);
//	ipFieldsEdit->AddField(ipX);
//
//	ipFieldEdit = ipY;
//	ipFieldEdit->put_Length(len);
//	ipFieldEdit->put_Name(CComBSTR(L"Y"));
//	ipFieldEdit->put_Type(esriFieldTypeDouble);
//	ipFieldsEdit->AddField(ipY);
//	ipFieldEdit = ipValue;
//
//	ipFieldEdit->put_Length(len);
//	ipFieldEdit->put_Name(CComBSTR(L"Value"));
//	ipFieldEdit->put_Type(esriFieldTypeDouble);
//	ipFieldsEdit->AddField(ipValue);
//
//	// Create the shapefile (some parameters apply to geodatabase options and can be defaulted as NULL)
//	IFeatureClassPtr ipFeatClass;
//	hr = ipFeatureWKSP->CreateFeatureClass(CComBSTR(layerName), ipFields,
//		0, 0,
//		esriFTSimple,
//		CComBSTR(L"Shape"),
//		CComBSTR(L""),
//		&ipFeatClass);
//	*ppFeatureClass = ipFeatClass;
//	if (*ppFeatureClass)
//		(*ppFeatureClass)->AddRef();
//	else
//		return E_FAIL;
//
//	return hr;
//
//}

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


//void CMFCTestView::OnSize(UINT nType, int cx, int cy)
//{
//	CFormView::OnSize(nType, cx, cy);
//
//	// TODO:  �ڴ˴������Ϣ����������
//	CWnd *pMapCtl = GetDlgItem(IDC_MAPCONTROL1);
//	//CRect MapCtlrect;  
//	if (pMapCtl)
//	{
//		//pMapCtl->GetWindowRect( &MapCtlrect );  
//		pMapCtl->MoveWindow(0, 0, cx, cy);
//	}
//}

/*
������GPS����ͨ�ź���
������
@ch�����ڴ��ݵĵ����ַ�
@ch���˿ں�
����ֵ��0
*/
afx_msg LRESULT CMFCTestView::OnCommunication(WPARAM ch, LPARAM portnum)
{
	//���յ����ַ�
	if (ch != 13 && ch != 10)
	{
		gpsStr += (char)ch;
	}
	//���յ�����һ��ʱ�����ַ���
	else if (gpsStr != "")
	{
		//�ж��Ƿ���Ҫ�ͷ�GPS����
		/*while (gpsInfos.size()>0)
		{
		GPSInfo *tmp;
		gpsInfos.pop_back(tmp);
		delete tmp;
		}*/
		if (gpsInfos.size() > 0)
		{
			for each (GPSInfo* info in gpsInfos)
			{
				delete info;
			}
			gpsInfos.clear();
		}
		gpsInfos = gpsTran.Tanslate(gpsStr);

		for each (GPSInfo* info in gpsInfos)
		{
			//�ж��ǲ���GPS��γ������
			if (info->InfoType == GGA)
			{
				/*m_editLongitude = (CEdit *)GetDlgItem(IDC_EDIT1);
				m_editLatitude = (CEdit *)GetDlgItem(IDC_EDIT2);*/
				CString x;
				x.Format(_T("%lf"), ((GGAInfo*)info)->Longitude);
				CString y;
				y.Format(_T("%lf"), ((GGAInfo*)info)->Latitude);
				m_editLongitude.SetWindowText(x);
				m_editLatitude.SetWindowText(y);

				myGPSInfo.Longitude = ((GGAInfo*)info)->Longitude;
				myGPSInfo.Latitude = ((GGAInfo*)info)->Latitude;

				//ˢ��GPSλ��
				IActiveViewPtr iActiveView;
				m_ipMapControl->get_ActiveView(&iActiveView);
				IPointPtr ipoint(CLSID_Point);
				if (ipoint == NULL)
					MessageBox(_T("�㴴������"));
				ipoint->PutCoords(myGPSInfo.Longitude*100, myGPSInfo.Latitude*100);

				//����
				//IGeometryPtr pGeometry(ipoint);
				IGeometryPtr pgeomln(ipoint);
				IGraphicsContainerPtr pgracont(iActiveView);
				//pgracont->DeleteAllElements();

				IMarkerElementPtr pmarkerelem(CLSID_MarkerElement);//����element������element
				if (pmarkerelem == NULL)
					MessageBox(_T("��ͼԪ�ش�������"));
				IMarkerSymbolPtr imarkerSymbol(m_isymbol);//��m_isymbol��ʼ��imarkerSymbol����symbol
				pmarkerelem->put_Symbol(imarkerSymbol);//��symbol��ӵ�element
				((IElementPtr)pmarkerelem)->put_Geometry(pgeomln);
				if (lastPointElement != NULL){
					pgracont->DeleteElement(lastPointElement);
				}
				lastPointElement = pmarkerelem;
				pgracont->AddElement((IElementPtr)pmarkerelem, 0);
				iActiveView->Refresh();
			}

			if (info->InfoType == PSAT_HPR)
			{
				CString heading;
				heading.Format(_T("%lf"), ((PSAT_HPRInfo*)info)->Heading);
				m_editSeekurVel.SetWindowText(heading);
				myGPSInfo.SpeedKm = ((VTGInfo*)info)->SpeedKm;
			}

			if (info->InfoType == VTG)
			{
				CString vel;
				vel.Format(_T("%lf"), ((VTGInfo*)info)->SpeedKm);
				m_editSeekurVel.SetWindowText(vel);
			}
			delete info;
		}
		gpsInfos.shrink_to_fit();
	}

	return 0;
}

UINT CMFCTestView::SeekurFuc(LPVOID lParam){
	//��ȡexeִ���ļ�·��
	HMODULE module = GetModuleHandle(0);
	TCHAR exeFullPath[MAX_PATH]; // MAX_PATH
	GetModuleFileName(NULL, exeFullPath, MAX_PATH);
	CString fileName(exeFullPath);
	int argc = 3;
	char *argv[3];
	char rp[MAX_PATH];
#if defined(UNICODE) 
	WideCharToMultiByte(CP_ACP, 0, exeFullPath, -1, rp, wcslen(exeFullPath), 0, 0);
	rp[wcslen(exeFullPath)] = 0;
#else 
	lstrcpy((PTSTR)rp, (PTSTR)pFileName);
#endif 
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
	// �첽���л����˴���ѭ��
	robot->enableMotors();
	robot->runAsync(true);
	BaseAction action(robot);

	MSG msg;
	while (true)
	{
		GetMessage(&msg, NULL, 0, 0);
		seekurParaPtr pPara = (seekurParaPtr)msg.wParam;
		if (msg.message==WM_SEEKUR_MOVE)
		{
			//ֱ���ƶ�����
			if (pPara->distance>0)
				action.Move(pPara->distance);
			//ת���Ƕ�
			if (pPara->heading>0)
				action.SetDeltaHeading(pPara->heading);
		}
		if (msg.message==WM_SEEKUR_END)
		{
			break;
		}
		::PostMessage((HWND)lParam,WM_SEEKUR_RET, 0, 0);//�����Զ�����Ϣ
		delete pPara;
	}
	//robot->comInt(ArCommands::ENABLE, 1);
	robot->disconnect();
	Aria::exit();

	return 0;
}

UINT CMFCTestView::TrackFuc(LPVOID lParam){
	
	MSG msg;
	bool isStart = false;
	while (true){
		bool isGet = PeekMessage(&msg, NULL, 0, 0, PM_REMOVE);
		//�ж��ǿ�ʼ����ֹͣ׷��
		if (isGet){
			if (msg.message = WM_TRACK_START){
				isStart = true;
			}
			else if (msg.message = WM_TRACK_STOP){
				isStart = false;
			}
			else if (msg.message = WM_TRACK_END){
				break;
			}
		}

		if (isStart)
		{
			CMainFrame*   pFrame = (CMainFrame*)AfxGetMainWnd();
			CMFCTestView* pView = (CMFCTestView*)pFrame->GetActiveView();
			IPolylinePtr pPolyline;
			pPolyline = pView->m_trackPath;

			//��ȡ�����
			IPointPtr pNearestPoint;	//�����
			IPointPtr pSeekurPoint;		//Seekur���ڵ�
			pView->lastPointElement->get_Geometry(&(IGeometryPtr)pSeekurPoint);

			double distAlongCurveFrom = 0; //������ʵ�㵽����㲿�ֵĳ���
			double distFromCurve = 0;//����㵽���ߵ���̾���
			VARIANT_BOOL isRightSide = false;//������Ƿ������ߵ��ұ�
			pPolyline->QueryPointAndDistance(esriNoExtension, pSeekurPoint, false, pNearestPoint, &distAlongCurveFrom, &distFromCurve, &isRightSide);

			//�ж��������Seekur����߻����ұߣ���ȡ
			double nearestX, seekurX;
			pNearestPoint->get_X(&nearestX);
			pSeekurPoint->get_X(&seekurX);
			IPointCollectionPtr pPointCol = (IPointCollectionPtr)pPolyline;

			IPointPtr pNextPoint;	//�������ڽ��㣬������Seekur������߶�
			//������ѯ
			long pointNum;	//������
			double nearestY;	//�����Y����
			pNearestPoint->get_Y(&nearestY);
			pPointCol->get_PointCount(&pointNum);
			if (nearestX < seekurX){
				for (long i = 0; i < pointNum; i++)
				{
					IPointPtr pTmpPoint;
					pPointCol->get_Point(i, &pTmpPoint);
					double tmpX, tmpY;
					pTmpPoint->get_X(&tmpX);
					pTmpPoint->get_Y(&tmpY);
					if (tmpX == nearestX&&tmpY == nearestY){
						if (i == pointNum - 1)
							pPointCol->get_Point(i - 1, &pNextPoint);
						else
							pPointCol->get_Point(i + 1, &pNextPoint);
						break;
					}
				}
			}
			else{
				for (long i = pointNum - 1; i >= 0; i--)
				{
					IPointPtr pTmpPoint;
					pPointCol->get_Point(i, &pTmpPoint);
					double tmpX, tmpY;
					pTmpPoint->get_X(&tmpX);
					pTmpPoint->get_Y(&tmpY);
					if (tmpX == nearestX&&tmpY == nearestY){
						if (i == 0)
							pPointCol->get_Point(i + 1, &pNextPoint);
						else
							pPointCol->get_Point(i - 1, &pNextPoint);
						break;
					}
				}
			}

			double lineHeading;
			double nextX, nextY;
			pNextPoint->get_X(&nextX);
			pNextPoint->get_Y(&nextY);
			if (nearestX > nextX)
			{
				lineHeading = 90 - (nearestY - nextY) / (nearestX - nextX);
			}
			else{
				lineHeading = 90 - (nextY - nearestY) / (nextX - nearestX);
			}

			CString cKDis;
			pView->m_editKDis.GetWindowTextW(cKDis);
			CString cKSubHead;
			pView->m_editKSubHead.GetWindowTextW(cKSubHead);
			double kDis = _ttof(cKDis);
			double kSubHead = _ttof(cKSubHead);

			double turnHeading = kDis*distFromCurve + kSubHead*(lineHeading - pView->myGPSInfo.Heading);

			seekurParaPtr pPara = new seekurPara();
			pPara->distance = distFromCurve;
			pPara->heading = turnHeading;

			PostThreadMessage(pView->seekur_thread->m_nThreadID, WM_SEEKUR_MOVE, (UINT)pPara, NULL);

			::PostMessage((HWND)lParam, WM_SEEKUR_RET, 0, 0);//�����Զ�����Ϣ
			delete pPara;
		}
		Sleep(1000);

	}
	return 0;
}
#pragma endregion

void CMFCTestView::OnClickedButton1()
{
	IPointPtr point1;
	HRESULT hr1 = point1.CreateInstance(CLSID_Point);
	point1->PutCoords(114.068188,22.531326);
	//point1->PutCoords(12698012.6530, 2575437.9373);

	point1 = geoToProj(point1);
	double x, y;
	point1->get_X(&x);
	point1->get_Y(&y);
	CString str;
	str.Format(_T("X=%lf,Y=%lf"), x, y);
	MessageBox(str);
	
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
	m_cPointLine.SetCheck(1);

	if (m_cPointLine.GetCheck())
	{
		//����
		//IGeometryPtr pGeometry(ipoint);
		AddCreateElement((IGeometryPtr)ipoint, iActiveView);
		iActiveView->Refresh();
	}
	else{
		//����
		HRESULT hr;
		if (m_pNewLineFeedback == NULL)
		{
			CoCreateInstance(CLSID_NewLineFeedback,
				NULL, CLSCTX_ALL, IID_INewLineFeedback,
				(void**)&m_pNewLineFeedback);
			ISymbolPtr isymbol;
			if (m_pNewLineFeedback == NULL) return;
			m_pNewLineFeedback->get_Symbol(&isymbol);
			ISimpleLineSymbolPtr pslnsym(isymbol);
			if (pslnsym == NULL) return;
			IRgbColorPtr prgb(CLSID_RgbColor);
			prgb->put_Red(0);
			prgb->put_Green(205);
			prgb->put_Blue(0);
			IColorPtr icolor(prgb);

			pslnsym->put_Color(icolor);
			pslnsym->put_Style(esriSLSDot);

			IScreenDisplayPtr pScrD;
			iActiveView->get_ScreenDisplay(&pScrD);

			m_pNewLineFeedback->putref_Display(pScrD);
			m_pNewLineFeedback->Start(ipoint);
		}
		else
			hr = m_pNewLineFeedback->AddPoint(ipoint);
	}
	

	
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
void CMFCTestView::OnBnClickedButton2()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	//��ȡҪ�ƶ��ľ���
	CString distance;
	m_editDistance.GetWindowTextW(distance);
	CString heading;
	m_editHeading.GetWindowTextW(heading);
	double dis = _ttof(distance);
	double head = _ttof(heading);
	seekurParaPtr pPara = new seekurPara();
	pPara->distance = dis;
	pPara->heading = head;

	PostThreadMessage(seekur_thread->m_nThreadID, WM_SEEKUR_MOVE, (UINT)pPara, NULL);
	//PostThreadMessage(seekur_thread->m_nThreadID, WM_SEEKUR_MOVE, dis, head);
	
}


afx_msg LRESULT CMFCTestView::OnSeekur(WPARAM wParam, LPARAM lParam)
{
	//MessageBox(_T("����ͨ��"));
	/*m_editLongitude = (CEdit *)GetDlgItem(IDC_EDIT1);
	m_editLongitude->SetWindowTextW(_T("������Ϣ"));*/
	return 0;
}

//����׷��
void CMFCTestView::OnBnClickedButton3()
{
	//��ȡ·��ͼ��
	ILayerPtr pLayer;
	IFeatureLayerPtr pFeatLayer;
	long num;
	m_ipMapControl->get_LayerCount(&num);
	BSTR pathName = ::SysAllocString(L"Path");	//·������ͼ������
	for (long i = 0; i < num; i++)
	{
		BSTR name;
		m_ipMapControl->get_Layer(i, &pLayer);
		pLayer->get_Name(&name);
		if (name == pathName)
		{
			pFeatLayer = pLayer;
			break;
		}
	}
	if (!pFeatLayer){
		MessageBox(_T("��·����"));
	}
	else{
		//��ȡ·��
		IFeatureClassPtr pFeatCla;
		pFeatLayer->get_FeatureClass(&pFeatCla);
		IFeaturePtr pFeature;
		pFeatCla->GetFeature(0, &pFeature);
		IGeometryPtr pGeometry;
		pFeature->get_Shape(&pGeometry);
		m_trackPath = pGeometry;

		if (!isTracked)
		{
			//֪ͨ�߳̿�ʼ׷��
			PostThreadMessage(track_thread->m_nThreadID, WM_TRACK_START, 0, 0);
		}
		else{
			PostThreadMessage(track_thread->m_nThreadID, WM_TRACK_STOP, 0, 0);
		}
	}
	//// TODO:  �ڴ���ӿؼ�֪ͨ����������
	//IFeatureLayer pFeatLayer = m_ipMapControl->get_Layer(0) as IFeatureLayer;
	//IFeatureClass pFeatCla = pFeatLayer.FeatureClass;

	//IPoint pnt = new PointClass();
	//pnt.X = 113.301;
	//pnt.Y = 23.113;
	//GISTool.CreateTimePoint(pFeatCla, pnt, 100.1);

	//axMapControl1.ActiveView.Refresh();

	/*ILayerPtr pLayer;
	IFeatureLayerPtr pFeatLayer;
	m_ipMapControl->get_Layer(0, &pLayer);
	pFeatLayer = pLayer;
	IFeatureClassPtr pFeatCla;
	pFeatLayer->get_FeatureClass(&pFeatCla);
	IPointPtr ipoint(CLSID_Point);
	ipoint->PutCoords(290, 480);*/

	
}

void CreateShapeFile(){
	//�ֽ��ļ�·��������
	CString strFolder = _T("F:\\arcMap");
	CString strFile = _T("path.shp");

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
	ipSpaRefFact2->CreateGeographicCoordinateSystem(esriSRGeoCS_WGS1984, &ipGeoCoordSys);

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

	IPolylinePtr pPolyline(CLSID_Polyline);
	IPointCollectionPtr pPtclo = (IPointCollectionPtr)pPolyline;

	IPointPtr point(CLSID_Point);
	point->PutCoords(11616.61347551, 3956.63814757);
	pPtclo->AddPoint(point);

	point = new IPointPtr(CLSID_Point);
	point->PutCoords(11516.61347551, 3856.63814757);
	pPtclo->AddPoint(point);


	pFeature->putref_Shape(pPolyline);
	pFeature->Store();


#pragma region ����������ʽ
	////����ͼ��
	//IFeatureClassPtr pFeatClass;
	//IFeatureLayerPtr pFeatLayer(CLSID_FeatureLayer);
	//ISpatialReferencePtr ipSpatialReference;

	////��������ϵ�����弸������
	//ISpatialReferenceFactoryPtr spatialReferenceFactory;
	//HRESULT hr = spatialReferenceFactory.CreateInstance(CLSID_SpatialReferenceEnvironment);

	//IGeographicCoordinateSystem* pGCS;
	//pGCS = spatialReferenceFactory.CreateGeographicCoordinateSystem((int)esriSRGeoCSType.esriSRGeoCS_WGS1984);

	//HRESULT hr = ipSpatialReference.CreateInstance(CLSID_UnknownCoordinateSystem);
	//if (FAILED(hr)) return;
	//hr = m_ipMapControl->get_SpatialReference(&ipSpatialReference);
	//if (FAILED(hr))
	//	return;
	//hr = pFeatLayer->putref_SpatialReference(ipSpatialReference);
	//if (FAILED(hr))
	//	return;
	//hr = pFeatLayer->put_Name((CComBSTR)(L"SeekurGPS"));
	//if (FAILED(hr))
	//	return;
	//


	//m_currentLayer = pFeatLayer;
	//m_ipMapControl->get_Map(&m_map);
	////��ȡ�༭��Ҫ����
	//IDatasetPtr pDataset;
	//pDataset = pFeatClass;
	//IWorkspaceEditPtr pWorkSpaceEdit;
	//IWorkspacePtr pWorkspace;
	//pDataset->get_Workspace(&pWorkspace);
	//pWorkSpaceEdit = pWorkspace;
	//VARIANT_BOOL isStart;
	//pWorkSpaceEdit->IsBeingEdited(&isStart);
	//if (!isStart)
	//{
	//	pWorkSpaceEdit->StartEditing(true);
	//}

	//IFeaturePtr pFeature;
	//pFeatClass->CreateFeature(&pFeature);
	//IPointPtr point(CLSID_Point);
	//point->PutCoords(452499, 4450252);
	//pFeature->putref_Shape(point);
	//pFeature->Store();
	////m_ipTocControl->Update();
	//pWorkSpaceEdit->StopEditing(isStart);

	//hr = pFeatLayer->putref_FeatureClass(pFeatClass);
	//if (FAILED(hr))
	//	return;

	//hr = m_ipMapControl->AddLayer(pFeatLayer, 1);
	//if (FAILED(hr))
	//	return;

	//IActiveViewPtr iActiveView;
	//m_ipMapControl->get_ActiveView(&iActiveView);
	//IPointPtr ipoint(CLSID_Point);

	////�㻭��
	//IGeometryPtr pGeometry;
	//pFeature->get_Shape(&pGeometry);
	//AddCreateElement(pGeometry, iActiveView);
	//iActiveView->Refresh();
	//
	////INewMultiPointFeedbackPtr pMultiPointFeed;
	////CoCreateInstance(CLSID_NewMultiPointFeedback,
	////	NULL, CLSCTX_ALL, IID_INewMultiPointFeedback,
	////	(void**)&pMultiPointFeed);
	////IActiveViewPtr pActiveView;
	////pActiveView = m_map;

	////IPointPtr point(CLSID_Point);
	////m_ipMapControl->ToMapPoint(290, 680, &point);
	////CreateFeature(point);
#pragma endregion
}
