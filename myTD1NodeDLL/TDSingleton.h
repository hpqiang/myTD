#pragma once

//Singleton
template<class T>
class TDSingleton
{
public:
	// Refer to: http://stackoverflow.com/questions/1724036/splitting-templated-c-classes-into-hpp-cpp-files-is-it-possible
	// A function for template class has to be here, cannot be moved to corresponding .cpp file
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
	TDSingleton(const TDSingleton&) {}  //Prevent copy cstor from happening
	~TDSingleton() {}  //Don't allow others to call dstor
	static T* m_Instance;
};

template<class T>
T* TDSingleton<T>::m_Instance = nullptr;

