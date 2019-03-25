#ifndef UTILITY_H
#define UTILITY_H
#include "ComparableString.hpp"
#include<vector>
#include<map>


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

std::ostream& operator <<(std::ostream& stream, const database::ComparableString& string)
{
  stream << string.m_string;
  return stream;
}


#endif //UTILITY_H