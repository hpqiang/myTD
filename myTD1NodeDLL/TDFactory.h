#pragma once

template<class T>
class TDFactory
{
public:
	T* getInstance()
	{
		try
		{
			m_Instance = new T();
			//			cout << "type(T): " << type(*T) << endl;
			//			cout << "type_info(T)" << type_info(*m_Instance) << endl;
			return m_Instance;
		}
		catch (exception& e)
		{
			cout << "exception occured: " << e.what() << endl;
			m_Instance = nullptr;
		}
		//finally([&]() { cout << "finally" << endl; return m_Instance; });
		return nullptr;
	}
private:
	T* m_Instance;
};
