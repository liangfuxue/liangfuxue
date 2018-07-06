/*
	文件名称：C课程设计.cpp
	创建日期：2018-06-20
	作者:梁富学
	文件说明：数据操作类WagesManager和控制台界面操作
	最后修改日期：2018-06-20
	

	文件名称：Inc.h
	创建日期：2018-06-20
	作者：梁富学
	文件说明：一些字符串操作函数，全局函数
	最后修改日期：2018-06-20
	版本：0.1
*/
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
	//字符串切割的使用
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

	//去掉字符串的空格
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

	//判断一个字符串是否保存数字
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

	//判断文件是否存在
	inline bool IsExistFile(string csPath)
	{
		int nRet = _access(csPath.c_str(), 0);
		return 0 == nRet || EACCES == nRet;
	}

	//字符串转float
	inline float StringToFloat(string str)
	{
		return (float)atof(str.c_str());
	}
}
#endif // _INC_H_


//职工信息结构体：
//工号 + 姓名 + 岗位工资 + 薪级工资 + 职务津贴 + 绩效工资 + 应发工资 + 个人所得税 + 实发工资
struct WorkerInfo
{
  //这6个属性是要求用户输入的 
	string id;				//工号
	string name;			//姓名
	float postWages;		//岗位工资 
	float paySalary;		//薪级工资
	float jobAllowance;     //职务津贴
	float performancePay;   //绩效工资


	//这3个属性是根据上面的自动计算 
	float shouldPay;		//应发工资
	float tax;				//个人所得税
	float realWages;		//实发工资

	void Print()
	{
		cout << "-----------------------------------------------------------------------------\r\n";
		cout << "工号：" << id << " 姓名：" << name << endl;
		cout << "岗位工资：" << postWages << " 薪级工资：" << paySalary << " 职务津贴：" << jobAllowance << " 绩效工资：" << performancePay << endl;
		cout << "应发工资：" << shouldPay << " 个人所得税：" << tax << " 实发工资：" << realWages << endl;
		cout << "-----------------------------------------------------------------------------\r\n";
	}
};




//工资管理类
//只包含数据的查询、修改、添加、删除、保存、显示、输入数据
//不涉及与用户界面的操作  与界面高度分离
//对数据删除 添加 修改操作时及时更新数据到文件
class WagesManager
{
public:
	WagesManager()
	{
		this->Read();
	}

	//判断是否存在该员工  方便输入数据的时候判断使用
	bool IsExistWorker(string id)
	{
		return this->Get(id) != _works.end();
	}

	//4.1读取职工工资数据函数：read()
	void Read()
	{
		_works.clear();
		if (Utility::IsExistFile("gx.txt"))  //数据文件存在
		{
			fstream fs;
			fs.open("gx.txt", ios::in | ios::out);
			char buff[1024] = {0};
			while (fs.getline(buff, 1024))   //读取行
			{
				auto vec = Utility::SplitString(buff, "=");   //字符串分割
	
				WorkerInfo wi;
				wi.id = vec.at(0);								//工号
				wi.name = vec.at(1);							//姓名
				wi.postWages = Utility::StringToFloat(vec.at(2));			//岗位工资 
				wi.paySalary = Utility::StringToFloat(vec.at(3));			//薪级工资
				wi.jobAllowance = Utility::StringToFloat(vec.at(4));		//职务津贴
				wi.performancePay = Utility::StringToFloat(vec.at(5));    //绩效工资
				wi.shouldPay = Utility::StringToFloat(vec.at(6));			//应发工资
				wi.tax = Utility::StringToFloat(vec.at(7));				//个人所得税
				wi.realWages = Utility::StringToFloat(vec.at(8));			//实发工资

				_works.push_back(wi);
			}
			fs.close();
		}
	}

	//4.2保存职工工资数据函数：write()
	void Save()
	{
		ofstream fs("gx.txt");  //一次性写入到数据文件 会覆盖旧数据
		for (auto it = _works.begin(); it != _works.end(); it++)
		{
			fs << it->id << "=" << it->name << "=" << it->postWages << "=" << it->paySalary << "=" <<
				it->jobAllowance << "=" << it->performancePay << "=" << it->shouldPay << "=" << it->tax << "=" << it->realWages << endl;
		}
		fs.close();
	}


	//4.3查询职工工资数据函数：find()
	void Find(const string &id)
	{
		auto result = this->Get(id);
		if (result != _works.end())
			result->Print();
	}

	//4.4浏览职工工资数据函数：list()
	void ListInfo()
	{
		for (auto it = _works.begin(); it != _works.end(); it++)
		{
			it->Print();
		}
	}
	//4.5修改职工工资数据函数：modify()
	//只需要修改这4个属性：岗位工资、薪级工资、职务津贴、绩效工资 即可
	//应发工资、个人所得税、实发工资会自动计算
	void Modify(const string &id, float postWages, float paySalary, float jobAllowance, float performancePay)
	{
		auto result = this->Get(id);
		if (result != _works.end())
		{
			result->postWages = postWages;
			result->paySalary = paySalary;
			result->jobAllowance = jobAllowance;
			result->performancePay = performancePay;
			CalcTax(id);  //重新计算应发工资、个人所得税、实发工资

			Save(); //对数据进行了修改操作 要及时保存到数据文件
		}
	}

	//4.6删除职工工资数据函数：del()
	void Delete(const string &id)
	{
		auto result = this->Get(id);
		if (result != _works.end())     //存在就执行双向链表的erase删除操作
		{
			_works.erase(result);

			Save(); //对数据进行了修改操作 要及时保存到数据文件
		}
	}

	//4.7添加职工工资数据函数：add()
	//// 工号  姓名 岗位工资   薪级工资 职务津贴 绩效工资)
	bool Add(const string &id, const string &name, float postWages, float paySalary, float jobAllowance, float performancePay)
	{
		if (!Utility::StringIsNumber(id))  //id里面保存了其他字符 不是全数字 
			return false;

		auto result = this->Get(id);
		if (result == _works.end())  //不存在这个ID才能添加，避免重复
		{
			WorkerInfo wi;
			wi.id = id;								//工号
			wi.name = name;							//姓名
			wi.postWages = postWages;				//岗位工资 
			wi.paySalary = paySalary;				//薪级工资
			wi.jobAllowance = jobAllowance;			//职务津贴
			wi.performancePay = performancePay;     //绩效工资

			_works.push_back(wi);			//插入到双向链表尾部
			this->CalcTax(id);				//计算税率等

			Save(); //对数据进行了添加操作 要及时保存到数据文件
			return true;
		}
		return false;
	}


	//4.8计算个人所得税函数：grsds()
	//以现行税率为标准
	//应纳个人所得税税额=应纳税所得额×适用税率

	/*级数	应纳税所得额(含税)						税率(%)	   
	1	不超过500元的部分						     5		
	2	超过500元至2,000元的部分				    10			
	3	超过2,000元至5,000元的部分				    15		
	4	超过5,000元至20,000元的部分				    20			
	5	超过20,000元至40,000元的部分				25
	6	超过40,000元至60,000元的部分		        30
    7	超过60,000元至80,000元的部分				35
	8	超过80,000元至100,000元的部分               40
	9	超过100,000元的部分							45			
	*/
	void CalcTax(const string &id)
	{
		auto m = this->Get(id);
		if (m != _works.end())
		{
			//应发工资 = 岗位工资 +薪级工资+职务津贴+绩效工资
			m->shouldPay = m->postWages + m->paySalary + m->jobAllowance + m->performancePay;

			float a = m->shouldPay;

			
			if (a <= 0)
			{
				m->tax = 0;
				m->realWages = m->shouldPay;
			}
			else
			{
				//应纳个人所得税税额=应纳税所得额×适用税率
				if (a > 0 && a <= 500)
					m->tax = a * 0.05f;
				else if (a > 500 && a <= 2000)
					m->tax = a * 0.1f;
				else if (a > 2000 && a <= 5000)
					m->tax = a * 0.15f;
				else if (a > 5000 && a <= 20000)
					m->tax = a * 0.20f;
				else if (a > 20000 && a <= 40000)
					m->tax = a * 0.25f;
				else if (a > 40000 && a <= 60000)
					m->tax = a * 0.30f;
				else if (a > 60000 && a <= 80000)
					m->tax = a * 0.35f;
				else if (a > 80000 && a <= 100000)
					m->tax = a * 0.40f;
				else
					m->tax = a * 0.45f;

				//实收工资 = 应付工资 - 税
				m->realWages = m->shouldPay - m->tax;
			}
		}
	}
private:
	//通过职工工号id返回双向链表迭代器指针
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


	list<WorkerInfo> _works;  
};







//下面是用户界面操作  这底下没什么技术含量  核心在上面这个类
void ShowMainMenu()
{
	system("cls");
	static string items[15] = {"                                                                                " ,
		                       "                                                                                " ,
		                       "         ###   欢迎使用广西民族大学软件与信息安全学院职工工资管理系统  ###      " ,
		                       "                                                                                " ,
							   "         请选择<1 - 7>:                                                         " ,
		                       "         =================================================================      " ,
							   "        ┇          1、查询职工工资记录                                 ┇      " ,
		                       "        ┇          2、修改职工工资记录                                 ┇      " ,
							   "        ┇          3、添加职工工资记录                                 ┇      " ,
							   "        ┇          4、删除职工工资记录                                 ┇      " ,
							   "        ┇          5、保存数据到文件                                   ┇      " ,
							   "        ┇          6、浏览职工记录                                     ┇      " ,
							   "        ┇          7、退出系统                                         ┇      " ,
							   "         =================================================================      " ,
							   "         你的选择是:                                                            "  };
	for (int i = 0; i < 15; i++)
		cout << items[i] << endl;
}

int main()
{

	WagesManager wm;	//创建对象时调用构造函数自动读取数据文件
	while (true)
	{
		ShowMainMenu();
		string id;				//工号
		string name;			//姓名
		float postWages;		//岗位工资 
		float paySalary;		//薪级工资
		float jobAllowance;     //职务津贴
		float performancePay;   //绩效工资

		string inDelete;		//确认是否删除

		int flag = -1;
		cin >> flag;
		switch (flag)
		{
		case 1:			//1、查询
			cout << "请输入查询的工号" << endl;
			cin >> id;
			if (!wm.IsExistWorker(id))
			{
				cout << "不存在该员工,查询失败" << endl;
				break;
			}
			wm.Find(id);
			break;

		case 2:			//2、修改
			cout << "请输入修改的工号" << endl;	
			cin >> id;
			if (!wm.IsExistWorker(id))
			{
				cout << "不存在该员工,修改失败" << endl;
				break;
			}
			cout << "请重新输入岗位工资" << endl;
			cin >> postWages;
			cout << "请重新输入薪级工资" << endl;
			cin >> paySalary;
			cout << "请重新输入职务津贴" << endl;
			cin >> jobAllowance;
			cout << "请重新输入绩效工资" << endl;
			cin >> performancePay;
			wm.Modify(id, postWages, paySalary, jobAllowance, performancePay);
			break;

		case 3:			//3、添加
			cout << "请输入添加的工号" << endl;
			cin >> id;
			if (wm.IsExistWorker(id))
			{
				cout << "添加失败,员工ID已经存在" << endl;
				break;
			}
			cout << "请输入添加的姓名" << endl;
			cin >> name;
			cout << "请输入岗位工资" << endl;
			cin >> postWages;
			cout << "请输入薪级工资" << endl;
			cin >> paySalary;
			cout << "请输入职务津贴" << endl;
			cin >> jobAllowance;
			cout << "请输入绩效工资" << endl;
			cin >> performancePay;

			wm.Add(id, name, postWages, paySalary, jobAllowance, performancePay);
			break;

		case 4:		//4、删除
			cout << "请输入删除的工号" << endl;
			cin >> id;
			if (wm.IsExistWorker(id))
			{
				cout << "确认删除：T\t取消删除F" << endl;
				cin >> inDelete;
				if (inDelete == "T" || inDelete == "T")
				{
					wm.Delete(id);
					cout << "删除成功" << endl;
				}
			}
			else
				cout << "删除失败，不存在该员工" << endl;

			break;

		case 5:		//5、保存
			wm.Save();
			break;

		case 6:		//6、浏览所有信息
			wm.ListInfo();
			break;

		case 7:		//7、退出程序
			return 0;
		default:
			break;
		}
		system("pause");
	}
	return 0;
}