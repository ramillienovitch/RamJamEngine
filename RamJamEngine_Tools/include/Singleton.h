#pragma once

namespace RJE
{
	template <typename T>
	class Singleton
	{
	private:
		Singleton(const Singleton<T>&);
		Singleton& operator=(const Singleton<T>&);
		Singleton()	{ mInstance = this; };

	protected:
		static T* mInstance;

	public:
		static T& getInstance()		{ return *mInstance; }
		static T* getInstancePtr()	{ return mInstance; }
	};
	template <typename T> T* Singleton<T>::mInstance;
}
