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
			return m_Instance;
		}
		catch (exception& e)
		{
			cout << "exception occured: " << e.what() << endl;
			m_Instance = nullptr;
		}
		return nullptr;
	}
private:
	T* m_Instance;
};
