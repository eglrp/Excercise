#include "stdafx.h"
#include <Windows.h>
#include <string>
#include <io.h>

/// <summary>
/// �ϲ���ͬ�ļ��У���������������ļ�
/// </summary>
/// <param name="dir_path">�ļ���·��</param>
void MergeDirFiles(string dir_path);
#define N 6

struct TreeNode {
	int val;
	TreeNode *left;
	TreeNode *right;
	TreeNode(int x) : val(x), left(NULL), right(NULL) {}
};

void createdTree(TreeNode* pre, int i, int a[10], int exist[10]){
	if (i == 10)
	{
		return;
	}
	TreeNode *left = new TreeNode(a[i]);
	createdTree(left, i + 1, a, exist);
	pre->left = left;
	if (exist[i] == 0)
	{
		TreeNode *right = new TreeNode(a[i]);
		createdTree(right, i + 1, a, exist);
		pre->right = right;
	}
}

void showTree(TreeNode* node, string base,int type){
	if (node->val==8)
	{
		int sdf = 34;
	}
	string next = "";
	if (type == 1)
	{
		cout << node->val;
		next = to_string(node->val);
	}
	//Ҷ�ӽڵ�
	if (node->left->left == NULL)
	{
		if (node->right != NULL)
		{
			cout << endl;
		}
		if (node->left != NULL)
		{
			cout << base;
			cout << next;
			cout << node->left->val << endl;
		}
	}
	else
	{

		if (node->left != NULL)
		{
			showTree(node->left, base + next, 1);

		}
		if (node->right != NULL)
		{
			cout << base;
			showTree(node->right, base, 0);
		}
	}

}


int main()
{
	int a[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	int exist[10] = { 0 };
	
	for (size_t i = 0; i < 10; i++)
	{
		cin >> exist[i];
	}
	TreeNode* root = new TreeNode(-1);
	createdTree(root, 0, a, exist);
	showTree(root, "",0);
	system("pause");
	return 0;
}




class Solution {
public:
	TreeNode* reConstructBinaryTree(vector<int> pre, vector<int> vin) {
		//��ȡ������������λ��
		TreeNode* root = reConstructBinaryTree(pre, 0, pre.size() - 1, vin, 0, vin.size() - 1);
		return root;
	}

	TreeNode*reConstructBinaryTree(vector<int> pre, int preStart, int preEnd, vector<int> vin, int vinStart, int vinEnd){
		if (preStart > preEnd || vinStart > vinEnd){
			return NULL;
		}
		TreeNode* root = new TreeNode(pre[preStart]);
		for (size_t i = vinStart; i < vinEnd; i++)
		{
			if (vin[vinStart] == pre[preStart]){
				root->left = reConstructBinaryTree(pre, preStart + 1, preStart + i, vin, vinStart, i - 1);
				root->right = reConstructBinaryTree(pre, i + 1, preEnd, vin, i + 1, vinEnd);
				break;
			}
		}
		return root;
	}
};

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
						getPath(p.assign(dirPath).append("\\").append(fileinfo.name), files,true);
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
/// Strings ת LPCWSTR.
/// </summary>
/// <param name="orig">ԭ�ַ���</param>
/// <returns>LPCWSTR����</returns>
LPCWSTR stringToLPCWSTR(std::string orig)
{
	LPCWSTR s = stringToLPCWSTR(orig);
	size_t size = orig.size();
	wchar_t *buffer = new wchar_t[size + 1];
	MultiByteToWideChar(CP_ACP, 0, orig.c_str(), size, buffer, size * sizeof(wchar_t));
	buffer[size] = 0;  //ȷ���� '\0' ��β
	return buffer;

	/*size_t origsize = orig.length() + 1;
	const size_t newsize = 100;
	size_t convertedChars = 0;
	wchar_t *wcstring = (wchar_t *)malloc(sizeof(wchar_t)*(orig.length() - 1));
	mbstowcs_s(&convertedChars, wcstring, origsize, orig.c_str(), _TRUNCATE);*/
	//return wcstring;
}

/// <summary>
/// �ϲ���ͬ�ļ��У���������������ļ�
/// </summary>
/// <param name="dir_path">�ļ���·��</param>
void MergeDirFiles(string dir_path){
	vector<string> files;

	getPath(dir_path, files,true);

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



