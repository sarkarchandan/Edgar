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
    ComparableString():m_string(""){}
    ComparableString(const std::string& string)
    :m_string(string){}
    ~ComparableString(){}
    ComparableString(const database::ComparableString& string) { this -> m_string = string.m_string; }
    ComparableString& operator =(const database::ComparableString& string) 
    {
      this -> m_string = string.m_string;
      return *this;
    }
    inline bool operator == (const database::ComparableString& string) const { return _ToLowerCase(this -> m_string) == _ToLowerCase(string.m_string); }
    inline bool operator != (const database::ComparableString& string) const { return (!(*this == string)); }
    inline bool operator < (const database::ComparableString& string) const
    {
      const char* lhs = _ToLowerCase(this -> m_string).c_str();
      std::size_t lhs_length = this -> m_string.length();
      const char* rhs = _ToLowerCase(string.m_string).c_str();
      std::size_t rhs_length = string.m_string.length();
      if(std::lexicographical_compare(lhs,lhs + lhs_length,rhs,rhs + rhs_length))
        return true;
      else
        return false;
    }
    inline bool operator > (const database::ComparableString& string) const { return (*this != string) && !(*this < string); }
    inline bool operator <= (const database::ComparableString& string) const { return (*this < string) || (*this == string); }
    inline bool operator >= (const database::ComparableString& string) const { return (*this > string) || (*this == string); }
    
    private:
    std::string _ToLowerCase(const std::string& string) const
    {
      std::string _string = string;
      std::transform(_string.begin(),_string.end(),_string.begin(),::tolower);
      return _string;
    }
  };
}

#endif //COMPARABLE_STRING_H