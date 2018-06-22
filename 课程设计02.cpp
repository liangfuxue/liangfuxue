
#ifndef _INC_H_
#define _INC_H_
#include <string>
#include <iostream>
#include <vector>
#include <io.h>
#include <iostream>
#include <list>
#include <fstream>
using namespace std;


namespace Utility
{
	//�ַ����и�
	inline std::vector<std::string> SplitString(const std::string &str, const std::string &pattern)
	{
		std::vector<std::string> vec;
		std::string strs = str + pattern;

		size_t pos = strs.find(pattern);
		size_t size = strs.size();

		while (pos != std::string::npos)
		{
			std::string x = strs.substr(0, pos);
			vec.push_back(x);
			strs = strs.substr(pos + 1, size);
			pos = strs.find(pattern);
		}
		return vec;
	}

	//ȥ���ַ����Ŀո�
	inline std::string StringTrim(std::string str)
	{
		std::string temp;
		for (size_t i = 0; i < str.length(); i++)
		{
			if (str.at(i) != ' ' && str.at(i) != '\t')
			{
				temp += str.at(i);
			}
		}
		return temp;
	}

	//�ж�һ���ַ����Ƿ񱣴�����
	inline bool StringIsNumber(std::string str)
	{
		for (size_t i = 0; i < str.length(); i++)
		{
			if (str.at(i) < '0' || str.at(i) > '9')
			{
				return false;
			}
		}
		return true;
	}

	//�ж��ļ��Ƿ����
	inline bool IsExistFile(string csPath)
	{
		int nRet = _access(csPath.c_str(), 0);
		return 0 == nRet || EACCES == nRet;
	}

	//�ַ���תfloat
	inline float StringToFloat(string str)
	{
		return (float)atof(str.c_str());
	}
}
#endif // _INC_H_


//ְ����Ϣ�ṹ��
//���� + ���� + ��λ���� + н������ + ְ����� + ��Ч���� + Ӧ������ + ��������˰ + ʵ������
struct WorkerInfo
{
	//��6��������Ҫ���û������ 
	string id;				//����
	string name;			//����
	float postWages;		//��λ���� 
	float paySalary;		//н������
	float jobAllowance;     //ְ�����
	float performancePay;   //��Ч����


	//��3�������Ǹ���������Զ����� 
	float shouldPay;		//Ӧ������
	float tax;				//��������˰
	float realWages;		//ʵ������

	void add()
	{
		cout << "-----------------------------------------------------------------------------\r\n";
		cout << "���ţ�" << id << " ������" << name << endl;
		cout << "��λ���ʣ�" << postWages << " н�����ʣ�" << paySalary << " ְ�������" << jobAllowance << " ��Ч���ʣ�" << performancePay << endl;
		cout << "Ӧ�����ʣ�" << shouldPay << " ��������˰��" << tax << " ʵ�����ʣ�" << realWages << endl;
		cout << "-----------------------------------------------------------------------------\r\n";
	}
};




//���ʹ�����
//ֻ�������ݵĲ�ѯ���޸ġ���ӡ�ɾ�������桢��ʾ����������
//���漰���û�����Ĳ���  �����߶ȷ���
//������ɾ�� ��� �޸Ĳ���ʱ��ʱ�������ݵ��ļ�
class WagesManager
{
public:
	WagesManager()
	{
		this->Read();
	}

	//�Ƿ���ڸ�Ա��  �����������ݵ�ʱ���ж�ʹ��
	bool IsExistWorker(string id)
	{
		return this->Get(id) != _works.end();
	}

	//4.1��ȡְ���������ݺ�����read()
	void Read()
	{
		_works.clear();
		if (Utility::IsExistFile("gx.txt"))  //�����ļ�����
		{
			fstream fs;
			fs.open("gx.txt", ios::in | ios::out);
			char buff[1024] = {0};
			while (fs.getline(buff, 1024))   //��ȡ��
			{
				auto vec = Utility::SplitString(buff, "=");   //�ַ����ָ�
	
				WorkerInfo wi;
				wi.id = vec.at(0);								//����
				wi.name = vec.at(1);							//����
				wi.postWages = Utility::StringToFloat(vec.at(2));			//��λ���� 
				wi.paySalary = Utility::StringToFloat(vec.at(3));			//н������
				wi.jobAllowance = Utility::StringToFloat(vec.at(4));		//ְ�����
				wi.performancePay = Utility::StringToFloat(vec.at(5));    //��Ч����
				wi.shouldPay = Utility::StringToFloat(vec.at(6));			//Ӧ������
				wi.tax = Utility::StringToFloat(vec.at(7));				//��������˰
				wi.realWages = Utility::StringToFloat(vec.at(8));			//ʵ������

				_works.push_back(wi);
			}
			fs.close();
		}
	}

	//4.2����ְ���������ݺ�����write()
	void Save()
	{
		ofstream fs("gx.txt");  //һ����д�뵽�����ļ� �Ḳ�Ǿ�����
		for (auto it = _works.begin(); it != _works.end(); it++)
		{
			fs << it->id << "=" << it->name << "=" << it->postWages << "=" << it->paySalary << "=" <<
				it->jobAllowance << "=" << it->performancePay << "=" << it->shouldPay << "=" << it->tax << "=" << it->realWages << endl;
		}
		fs.close();
	}


	//4.3��ѯְ���������ݺ�����find()
	void Find(const string &id)
	{
		auto result = this->Get(id);
		if (result != _works.end())
			result->add();
	}

	//4.4���ְ���������ݺ�����list()
	void ListInfo()
	{
		for (auto it = _works.begin(); it != _works.end(); it++)
		{
			it->add();
		}
	}
	//4.5�޸�ְ���������ݺ�����modify()
	//ֻ��Ҫ�޸���4�����ԣ���λ���ʡ�н�����ʡ�ְ���������Ч���� ����
	//Ӧ�����ʡ���������˰��ʵ�����ʻ��Զ�����
	void Modify(const string &id, float postWages, float paySalary, float jobAllowance, float performancePay)
	{
		auto result = this->Get(id);
		if (result != _works.end())
		{
			result->postWages = postWages;
			result->paySalary = paySalary;
			result->jobAllowance = jobAllowance;
			result->performancePay = performancePay;
			CalcTax(id);  //���¼���Ӧ�����ʡ���������˰��ʵ������

			Save(); //�����ݽ������޸Ĳ��� Ҫ��ʱ���浽�����ļ�
		}
	}

	

	
private:
	//ͨ��ְ������id����˫�����������ָ��
	list<WorkerInfo>::iterator Get(string id)
	{
		list<WorkerInfo>::iterator result = _works.end();
		for (auto it = _works.begin(); it != _works.end(); it++)
		{
			if (id == it->id)
			{
				result = it;
				break;
			}
		}
		return result;
	}

	//˫��������ְ����Ϣ
	list<WorkerInfo> _works;  
};







//�������û��������  �����ûʲô��������  ���������������
void ShowMainMenu()
{
	system("cls");
	static string items[8] = { "......................................1����ѯ...................................." , "......................................2���޸�...................................." , "......................................3�����...................................." , "......................................4��ɾ��...................................." , "......................................5������...................................." , "......................................6�����....................................."  ,"......................................7���˳�...................................","..................................��������Ӧ��ָ��.............................." };
	for (int i = 0; i < 8; i++)
		cout << items[i] << endl;
}

int main()
{
	system("color F4"); //����������ɫ�ͱ�����ɫ
	WagesManager wm;	//��������ʱ���ù��캯���Զ���ȡ�����ļ�
	while (true)
	{
		ShowMainMenu();
		string id;				//����
		string name;			//����
		float postWages;		//��λ���� 
		float paySalary;		//н������
		float jobAllowance;     //ְ�����
		float performancePay;   //��Ч����

		string isDelete;		//ȷ���Ƿ�ɾ��

		int flag = -1;
		cin >> flag;
		switch (flag)
		{
		case 1:			//1����ѯ
			cout << "�������ѯ�Ĺ���" << endl;
			cin >> id;
			if (!wm.IsExistWorker(id))
			{
				cout << "�����ڸ�Ա��,��ѯʧ��" << endl;
				break;
			}
			wm.Find(id);
			break;
		case 2:			//2���޸�
			cout << "�������޸ĵĹ���" << endl;
			cin >> id;
			if (!wm.IsExistWorker(id))
			{
				cout << "�����ڸ�Ա��,�޸�ʧ��" << endl;
				break;
			}
			cout << "�����������λ����" << endl;
			cin >> postWages;
			cout << "����������н������" << endl;
			cin >> paySalary;
			cout << "����������ְ�����" << endl;
			cin >> jobAllowance;
			cout << "���������뼨Ч����" << endl;
			cin >> performancePay;
			wm.Modify(id, postWages, paySalary, jobAllowance, performancePay);
			break;
		case 3:			//3�����
			cout << "��������ӵĹ���" << endl;
			cin >> id;
			if (wm.IsExistWorker(id))
			{
				cout << "���ʧ��,Ա��ID�Ѿ�����" << endl;
				break;
			}
			cout << "��������ӵ�����" << endl;
			cin >> name;
			cout << "�������λ����" << endl;
			cin >> postWages;
			cout << "������н������" << endl;
			cin >> paySalary;
			cout << "������ְ�����" << endl;
			cin >> jobAllowance;
			cout << "�����뼨Ч����" << endl;
			cin >> performancePay;

			wm.Add(id, name, postWages, paySalary, jobAllowance, performancePay);
			break;
		case 4:		//4��ɾ��
			cout << "������ɾ���Ĺ���" << endl;
			cin >> id;
			if (wm.IsExistWorker(id))
			{
				cout << "ȷ��ɾ����Y\tȡ��ɾ��N" << endl;
				cin >> isDelete;
				if (isDelete == "Y" || isDelete == "y")
				{
					wm.Delete(id);
					cout << "ɾ���ɹ�" << endl;
				}
			}
			else
				cout << "ɾ��ʧ�ܣ������ڸ�Ա��" << endl;

			break;
		case 5:		//5������
			wm.Save();
			break;
		case 6:		//6�����������Ϣ
			wm.ListInfo();
			break;
		case 7:		//7���˳�����
			return 0;
		default:
			break;
		}
		system("pause");
	}
	return 0;
}