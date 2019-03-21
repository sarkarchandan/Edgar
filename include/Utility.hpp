#ifndef UTILITY_H
#define UTILITY_H
#include<iostream>
#include<unordered_map>
#include<vector>

template<typename Type>
std::ostream& operator <<(std::ostream& stream, const std::vector<Type>& vector)
{
  stream << "{  ";
  for(Type value: vector)
    stream << value << "\t";
  stream << "}";
  return stream;
}

std::ostream& operator <<(std::ostream& stream, const std::unordered_map<std::string,std::vector<std::string>>& dictionary)
{
  for(auto it = dictionary.begin(); it != dictionary.end(); ++it)
    stream << it -> first << "\t" << it -> second << "\n";
  return stream;
}


#endif //UTILITY_H