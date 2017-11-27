#pragma once

template <typename T>
class CSingleton
{
public: 
	static T& Instance()
	{
		static T instance; 
		return instance;
	}

protected:

	// Prevents construction of Singleton interface
	explicit CSingleton<T>() { }

	// Ensures pointer deconstruction of dynamically allocated child to be 
	// preformed in child (if the deconstructor is explicitly overrided publically)
	// NOTE: this case should NEVER really happen. Singletons should never be dynamically allocated. (bad form)
	virtual ~CSingleton<T>() { }

};