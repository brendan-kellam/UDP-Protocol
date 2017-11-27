#pragma once

template <typename T>
struct GenericUInt32Comparator
{
	typedef uint32_t (T::*GETTER)() const;
	GETTER m_getterFunc;
	uint32_t m_data;
	GenericUInt32Comparator(GETTER getterFunc, uint32_t data)
	{
		m_getterFunc = getterFunc; 
		m_data = data;
	}

	bool operator()(const T& obj)
	{
		if ((obj.*m_getterFunc)() == m_data)
		{
			return true;
		}
		else
		{
			return false;
		}
	}


};

template <typename T>
struct GenericUInt16Comparator
{
	typedef uint16_t(T::*GETTER)() const;
	GETTER m_getterFunc;
	uint16_t m_data;
	GenericUInt16Comparator(GETTER getterFunc, uint16_t data)
	{
		m_getterFunc = getterFunc;
		m_data = data;
	}

	bool operator()(const T& obj)
	{
		if ((obj.*m_getterFunc)() == m_data)
		{
			return true;
		}
		else
		{
			return false;
		}
	}


};

template <typename T, typename G>
struct GenericComparator
{
	typedef G(T::*GETTER)() const;
	GETTER m_getterFunc;
	G m_data;
	
	GenericComparator(GETTER getterFunc, G data)
	{
		m_getterFunc = getterFunc;
		m_data = data;
	}

	bool operator()(const T& obj)
	{
		if ((obj.*m_getterFunc)() == m_data)
		{
			return true;
		}
		else
		{
			return false;
		}
	}


};