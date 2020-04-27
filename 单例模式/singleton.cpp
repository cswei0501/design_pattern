#include <iostream>
#include <thread>
#include <mutex>
using namespace std;


std::mutex resource_mutex;

class CSingleton
{
private:
    CSingleton(){}	//私有化构造函数
	
private:
	static CSingleton *m_instance;	//静态成员变量
	
public:
    static CSingleton *GetInstance()
	{
		//双重锁定,提高效率
		//a)如果 (m_instance != NULL)  条件成立,则肯定表示m_instance已经被new过了
		//b)如果 (m_instance == NULL)  不代表m_instance一定没被new过了
		
		if(m_instance == NULL)	//双重锁定
		{
			std::unique_lock<std::mutex> mymutex(resource_mutex);
			if(m_instance == NULL)
			{
				m_instance = new CSingleton();
				static CGarhuishou cl;
			}
		}
		
		return m_instance;
	}
	
	class CGarhuishou	//类中套类,用来释放对象
	{
	public:
		~CGarhuishou()	//类的析构函数
		{
			if(CSingleton::m_instance)
			{
				delete CSingleton::m_instance;
				CSingleton::m_instance = NULL;
			}
		}
	};
	
	void func()
	{
		cout << "test func" << endl;
	}
}

void mythread()
{
	cout << "thread begin..." << endl;
	CSingleton *p_a = CSingleton::GetInstance();
	p_a->func();
	cout << "thread end" << endl;
}



//类静态变量初始化
CSingleton *CSingleton::m_instance = NULL;

int main()
{
    std::thread mytobj1(mythread);
	std::thread mytobj2(mythread);
	mytobj1.join();
	mytobj2.join();
}