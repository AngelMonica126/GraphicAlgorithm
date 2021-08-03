//--------------------------------------------------------------------------------------
// Copyright (c) Elay Pu. All rights reserved.
//--------------------------------------------------------------------------------------

#pragma once
#include <map>
#include <vector>
#include <iostream>
namespace ElayGraphics
{
	template <typename TDataType>
	class MVector
	{
	public:
		void	 push_back(const std::string &vDataName, const TDataType &vData);
		void	 push_back(const std::string &vDataName, const TDataType &vData, int vPointerFlag);
		template <typename Pred>
		void	 push_back(const std::string &vDataName, const TDataType &vData, Pred vPred);
		void	 reserve(size_t vCapacity);
		void	 clear();
		bool	 empty();
		bool	 empty() const;
		bool	 find(const std::string &vDataName);
		bool	 find(const std::string &vDataName) const;
		size_t	 size();
		size_t	 size() const;
		auto&	 operator[](int i);
		auto&	 operator[](const std::string &vDataName);
		auto&	 operator[](int i) const;

		auto	 begin();
		auto	 begin() const;
		auto	 end();
		auto	 end() const;

	private:
		std::vector<TDataType> m_DataSet;
		std::map<std::string, TDataType> m_DataMap;
	};
}

template <typename TDataType>
void ElayGraphics::MVector<TDataType>::push_back(const std::string &vDataName, const TDataType &vData)
{
	m_DataSet.insert(0,vData);
	sort(m_DataSet->begin(),m_DataSet->end(), [](const TDataType &vPass1, const TDataType &vPass2) {return vPass1 < vPass2; })
	m_DataMap[vDataName] = vData;
}

template <typename TDataType>
void ElayGraphics::MVector<TDataType>::push_back(const std::string &vDataName, const TDataType &vData, int vPointerFlag)
{
	m_DataSet.insert(std::lower_bound(m_DataSet.begin(), m_DataSet.end(), vData, [](const TDataType &vPass1, const TDataType &vPass2) {return *vPass1 < *vPass2; }), vData); 
	m_DataMap[vDataName] = vData;
}



template <typename TDataType>
template <typename Pred>
void ElayGraphics::MVector<TDataType>::push_back(const std::string &vDataName, const TDataType &vData, Pred vPred)
{
	m_DataSet.insert(std::lower_bound(m_DataSet.begin(), m_DataSet.end(), vData, vPred), vData);   
}

template <typename TDataType>
auto& ElayGraphics::MVector<TDataType>::operator [](int i)
{
	return const_cast<TDataType&>(static_cast<const MVector&>(*this)[i]);
}

template <typename TDataType>
auto& ElayGraphics::MVector<TDataType>::operator [](const std::string &vDataName)
{
	_ASSERT(!vDataName.empty() && m_DataMap.find(vDataName) != m_DataMap.end());
	return m_DataMap[vDataName];
}

template <typename TDataType>
auto& ElayGraphics::MVector<TDataType>::operator [](int i) const 
{
	_ASSERT(i < m_DataSet.size() && i >= 0);
	return m_DataSet[i];
}

template <typename TDataType>
auto ElayGraphics::MVector<TDataType>::begin()
{
	return m_DataSet.begin();
}

template <typename TDataType>
auto ElayGraphics::MVector<TDataType>::begin() const
{
	return m_DataSet.begin();
}

template <typename TDataType>
auto ElayGraphics::MVector<TDataType>::end()
{
	return m_DataSet.end();
}

template <typename TDataType>
auto ElayGraphics::MVector<TDataType>::end() const
{
	return m_DataSet.end();
}

template <typename TDataType>
bool ElayGraphics::MVector<TDataType>::empty()
{
	return m_DataSet.empty() || m_DataMap.empty();
}

template <typename TDataType>
bool ElayGraphics::MVector<TDataType>::empty() const
{
	return m_DataSet.empty() || m_DataMap.empty();
}

template <typename TDataType>
void ElayGraphics::MVector<TDataType>::clear()
{
	m_DataSet.clear();
	m_DataMap.clear();
}

template <typename TDataType>
size_t ElayGraphics::MVector<TDataType>::size()
{
	return static_cast<const MVector&>(*this).size();
}

template <typename TDataType>
size_t ElayGraphics::MVector<TDataType>::size() const
{
	return m_DataSet.size();
}

template <typename TDataType>
void ElayGraphics::MVector<TDataType>::reserve(size_t vCapacity)
{
	_ASSERT(vCapacity >= 0);
	m_DataSet.reserve(vCapacity);
}

template <typename TDataType>
bool ElayGraphics::MVector<TDataType>::find(const std::string &vDataName)
{
	return m_DataMap.find(vDataName) != m_DataMap.end();
}

template <typename TDataType>
bool ElayGraphics::MVector<TDataType>::find(const std::string &vDataName) const
{
	return m_DataMap.find(vDataName) != m_DataMap.end();
}