#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include <map>
#include <queue>
#include <set>

template <class T>
using Vector = std::vector<T>;

template<class T, int N>
using Array = std::array<T, N>;

template<class T>
using Queue = std::queue<T>;

using String = std::string;

template <class T, class U>
using Map = std::map<T, U>;

template <class T, class U>
using UnorderedMap = std::unordered_map<T, U>;

template <class T>
using Set = std::set<T>;
