#pragma once

//Singleton

template<class T>
class TDSingleton
{
public:
	static T* getInstance()
	{
		if (m_Instance == nullptr)
		{
			try
			{
				m_Instance = new T();
				return m_Instance;
			}
			catch (exception& e)
			{
				cout << "exception: " << e.what() << endl;
			}
		}
		return m_Instance;
	}
private:
	//TDSingleton(const TDSingleton&) {}  //Prevent copy cstor from happening
	static T* m_Instance;
};

template<class T>
T* TDSingleton<T>::m_Instance = nullptr;

