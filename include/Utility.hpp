#ifndef UTILITY_H
#define UTILITY_H
#include<iostream>
#include<unordered_map>
#include<vector>
#include "ComparableString.hpp"

template<typename Type>
std::ostream& operator <<(std::ostream& stream, const std::vector<Type>& vector)
{
  stream << "{  ";
  for(Type value: vector)
    stream << value << "\t";
  stream << "}";
  return stream;
}

std::ostream& operator <<(std::ostream& stream, std::map<std::string,std::vector<database::ComparableString>>& structure)
{
  std::for_each(structure.begin(),structure.end(),[&](auto pair){
    stream << pair.first << "\t" << pair.second << "\n";
  });
  return stream;
}

#endif //UTILITY_H