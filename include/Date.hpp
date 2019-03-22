#ifndef DATE_H
#define DATE_H
#include <iostream>

namespace database
{
  struct Date
  {
    #pragma mark Member properties
    private:
    int32_t m_day;
    int32_t m_month;
    int32_t m_year;
    int32_t m_serialized_value;

    #pragma mark Initializers and accessors
    public:
    Date(const int32_t& year, const int32_t& month, const int32_t& day)
    :m_year(year),m_month(month),m_day(day)
    {
      m_serialized_value = (m_year * 10000) + (m_month * 100) + m_day;
    }
    Date(const int32_t& serialized_value)
    :m_serialized_value(serialized_value)
    {
      m_year = serialized_value / 10000;
      int32_t year_component_remainder = serialized_value % 10000;
      m_month = year_component_remainder / 100;
      m_day = year_component_remainder % 100;
    }
    ~Date(){}
    int32_t day() const { return m_day; }
    int32_t year() const { return m_year; }
    int32_t month() const {return m_month; }
    int32_t serialized_value() const { return m_serialized_value; }
    
    bool operator == (const database::Date& date) const
    {
      return this -> m_year == date.m_year && this -> m_month == date.m_month && this -> m_day == date.m_day;
    }
    bool operator != (const database::Date& date) const
    {
      return !(*this == date);
    }
    bool operator < (const database::Date& date) const
    {
      if(*this == date) return false;
      else
      {
        if(this -> m_year <= date.m_year)
        {
          if(this -> m_year < date.m_year) return true;
          else //Same year
          {
            if(this -> m_month <= date.m_month)
            {
              if(this -> m_month < date.m_month) return true;
              else //Same month
              {
                if(this -> m_day <= date.m_day)
                {
                  if(this -> m_day < date.m_day) return true;
                  else return false;
                }else return false;
              }
            }else return false;
          }
        }
        else return false;
      }
    }
    bool operator > (const database::Date& date) const
    {
      return (*this != date) && !(*this < date);
    }
  };
}

// unsigned int32_t operator - (const database& Date& lhs, const database::Date& rhs);

#endif //DATE_H