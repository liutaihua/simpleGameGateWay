#pragma once

#ifndef SDHASHMAP_H
#define SDHASHMAP_H
/**
* @file sdhashmap.h
* @author lw
* @brief hashmap类
*
**/
/**
* @defgroup grouphamap hashmap(接口同map)
* @ingroup  Crown
* @{
*/

#include <functional>
#include <unordered_map>
#include <set>
#include <unordered_set>

template <typename K, typename V>
class CnHashMap : public std::unordered_map<K, V>
{
public:
	typedef typename CnHashMap<K, V>::const_iterator const_iterator;
	typedef typename CnHashMap<K, V>::iterator iterator;

public:

	V get(const K& key, V defaultValue) const
	{
		const_iterator itor = this->find(key);
		if (itor == this->end())
			return defaultValue;
		return itor->second;
	}

	bool has(const K& key) const
	{
		const_iterator itor = this->find(key);
		return (itor != this->end());
	}

	typedef std::function< bool(const V&) > value_predicator;

	const_iterator find_value(value_predicator predicator) const
	{
		for (auto itor = this->begin(); itor != this->end(); ++itor)
		{
			if (predicator(itor->second))
			{
				return itor;
			}
		}
		return this->end();
	}

	iterator find_value(value_predicator predicator)
	{
		for (auto itor = this->begin(); itor != this->end(); ++itor)
		{
			if (predicator(itor->second))
			{
				return itor;
			}
		}
		return this->end();
	}
};

template <typename V>
class CnHashSet : public std::unordered_set<V>
{
public:
	typedef typename CnHashSet<V>::const_iterator const_iterator;
	typedef typename CnHashSet<V>::iterator iterator;

public:

	bool has(const V elements) const
	{
		const_iterator itor = this->find(elements);
		return itor != this->end();
	}

};

template <typename V>
class CnSet : public std::set<V>
{
public:
	typedef typename CnSet<V>::const_iterator const_iterator;
	typedef typename CnSet<V>::iterator iterator;

public:
	bool has(const V elements) const
	{
		const_iterator itor = this->find(elements);
		return itor != this->end();
	}

};

#endif
