#ifndef COMPARABLE_STRING_H
#define COMPARABLE_STRING_H
#include <string>
#include <algorithm>
#include <iostream>

namespace database
{
  struct ComparableString
  {
    std::string m_string;
    ComparableString(const std::string& string)
    :m_string(string){}
    ~ComparableString(){}
  
    inline bool operator == (const database::ComparableString& string) const { return this -> m_string == string.m_string; }
    inline bool operator != (const database::ComparableString& string) const { return (!(*this == string)); }
    inline bool operator < (const database::ComparableString& string) const
    {
      const char* lhs = this -> m_string.c_str();
      std::size_t lhs_length = this -> m_string.length();
      const char* rhs = string.m_string.c_str();
      std::size_t rhs_length = string.m_string.length();
      if(std::lexicographical_compare(lhs,lhs + lhs_length,rhs,rhs + rhs_length))
        return true;
      else
        return false;
    }
    inline bool operator > (const database::ComparableString& string) const { return (*this != string) && !(*this < string); }
    inline bool operator <= (const database::ComparableString& string) const { return (*this < string) || (*this == string); }
    inline bool operator >= (const database::ComparableString& string) const { return (*this > string) || (*this == string); }
  };

  std::ostream& operator <<(std::ostream& stream, const database::ComparableString& string)
  {
    stream << string.m_string;
    return stream;
  }
}

#endif //COMPARABLE_STRING_H