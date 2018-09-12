#include "stdafx.h"
#include <Windows.h>
#include <io.h>

/// <summary>
/// ��ȡ�ļ����ļ���·��
/// </summary>
/// <param name="dirPath">�ļ���·��</param>
/// <param name="files">���ҵ��ļ����ļ���·�����vector</param>
/// <param name="isRecur">�Ƿ������Ŀ¼��Ĭ�ϲ�����</param>
/// <param name="type">�������ͣ�0Ϊ�����ң�1Ϊ�ļ���2Ϊ�ļ��С�Ĭ��Ϊ�ļ�</param>
void getPath(string dirPath, vector<string>& files, bool isRecur = false, int type = 1) {
	//�ļ����
	long hFile = 0;
	//�ļ���Ϣ
	struct _finddata_t fileinfo;  //�����õ��ļ���Ϣ��ȡ�ṹ
	string p;  //string�������˼��һ����ֵ����:assign()���кܶ����ذ汾
	string st = p.assign(dirPath).append("\\*").c_str();
	if ((hFile = _findfirst(p.assign(dirPath).append("\\*").c_str(), &fileinfo)) != -1) {
		do {
			if (fileinfo.attrib & _A_SUBDIR) {  //�Ƚ��ļ������Ƿ����ļ���
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
				{
					if (type != 1)
					{
						files.push_back(p.assign(dirPath).append("\\").append(fileinfo.name));
					}
					if (isRecur)
					{
						getPath(p.assign(dirPath).append("\\").append(fileinfo.name), files, true);
					}
				}
			}
			else if (type != 2) {
				files.push_back(p.assign(dirPath).append("\\").append(fileinfo.name));
			}
		} while (_findnext(hFile, &fileinfo) == 0);  //Ѱ����һ�����ɹ�����0������-1
		_findclose(hFile);

	}

}

/// <summary>
/// �ϲ���ͬ�ļ��У���������������ļ�
/// </summary>
/// <param name="dir_path">�ļ���·��</param>
void MergeDirFiles(string dir_path){
	vector<string> files;

	getPath(dir_path, files, true);

	char szDrive[_MAX_DRIVE];   //������
	char szDir[_MAX_DIR];       //·����
	char szFname[_MAX_FNAME];   //�ļ���
	char szExt[_MAX_EXT];       //��׺��
	int num = 1;
	int per = 1;
	for (vector<string>::iterator iter = files.begin(); iter != files.end(); iter++)
	{
		cout << *iter << endl;
		string old_path = *iter;

		_splitpath_s(old_path.c_str(), szDrive, szDir, szFname, szExt); //�ֽ�·��
		//�ж��Ƿ�������ͼƬ�ļ�
		if (strcmp(szExt, ".jpg") == 0 || strcmp(szExt, ".JPG") == 0 || strcmp(szExt, ".jpeg") == 0 || strcmp(szExt, ".JPEG") == 0
			|| strcmp(szExt, ".png") == 0 || strcmp(szExt, ".PNG") == 0 || strcmp(szExt, ".gif") == 0 || strcmp(szExt, ".GIF") == 0
			|| strcmp(szExt, ".bmp") == 0 || strcmp(szExt, ".BMP") == 0 || strcmp(szExt, ".tif") == 0 || strcmp(szExt, ".TIF") == 0)
		{
			string newName;

			switch (num)
			{
			case 10:
				per = 2;
				break;
			case 100:
				per = 3;
				break;
			default:
				break;
			}

			switch (per)
			{
			case 1:
				newName = "00" + to_string(num);
				break;
			case 2:
				newName = "0" + to_string(num);
				break;
			case 3:
				newName = to_string(num);
				break;
			default:
				break;
			}

			int j = 0;
			while (szExt[j] != '\0')
			{
				newName += szExt[j];
				j++;
			}
			string new_path = dir_path + "\\" + newName;
			cout << new_path << endl;
			num++;
			MoveFile(old_path.c_str(), new_path.c_str());
			/*LPCWSTR lOld = stringToLPCWSTR(old_path);
			LPCWSTR lNew = stringToLPCWSTR(dir_path + "\\" + newName);*/
			/*int pos = old_path.find_last_of('\\');
			if (pos!=string::npos)
			{
			pos ++;
			string name = old_path.substr(pos, old_path.size() - pos);
			cout << dir_path+"\\"+name << endl;
			}*/
		}
		else
		{
			DeleteFile(old_path.c_str());
			continue;
		}


	}
}