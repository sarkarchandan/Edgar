#ifndef DATE_H
#define DATE_H
#include <cstdlib>
#include <exception>
#include <unordered_map>

namespace database
{
  struct Date
  {
    #pragma mark Private member properties
    private:
    int32_t m_day;
    int32_t m_month;
    int32_t m_year;
    int32_t m_serialized_value;
    std::unordered_map<int32_t,int32_t> m_days_of_months;

    #pragma mark Private Initializer and implementation layer functions
    private:
    inline bool _IsLeapYear(const int32_t& year) const
    {
      if((year % 400) == 0) return true;
      else if((year % 100) == 0) return false;
      else if((year % 4) == 0) return true;
      else return false;
    }
    void _Validate(const int32_t& year, const int32_t& month, const int32_t& day) const
    {
      if(!(month >= 1 && month <= 12))
        throw std::invalid_argument("Invalid month value");
      
      if(month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12) //31 raw_year_days case
      {
        if(!(day >= 1 && day <= 31))
          throw std::invalid_argument("Invalid day value for the given calendar month");
      }
      else if(month == 4 || month == 6 || month == 9 || month == 11) //30 raw_year_days case
      {
        if(!(day >= 1 && day <= 30))
          throw std::invalid_argument("Invalid day value for the given calendar month");
      }
      else // 28 or 29 raw_year_days case
      {
        if(_IsLeapYear(year))
        {
          if(!(day >= 1 && day <= 29))
            throw std::invalid_argument("Invalid day value for the given calendar month of leap year");
        }else
          if(!(day >= 1 && day <= 28))
            throw std::invalid_argument("Invalid day value for the given calendar month");
      }
    }
    void _PopulateDaysOfMonths()
    {
      for(int32_t i = 1; i <= 12; i += 1)
      {
        if(i == 1 || i == 3 || i == 5 || i == 7 || i == 8 || i == 10 || i == 12) m_days_of_months[i] = 31;
        else if(i == 4 || i == 6 || i == 9 || i == 11) m_days_of_months[i] = 30;
        else
        {
          if(_IsLeapYear(year())) m_days_of_months[i] = 29;
          else m_days_of_months[i] = 28; 
        }
      }
    }
    // int32_t _DaysBetweenYears(const int32_t& lhs, const int32_t& rhs) const
    // {      
    //   if(lhs == rhs) return 0;
    //   int32_t raw_year_days = 0;
    //   if(lhs < rhs)
    //   {
    //     for(int32_t i = lhs; i < rhs; i += 1)
    //     {
    //       if(_IsLeapYear(i)) raw_year_days += 366;
    //       else raw_year_days += 365;
    //     }
    //   }else
    //   {
    //     for(int32_t i = rhs; i < lhs; i += 1)
    //     {
    //       if(_IsLeapYear(i)) raw_year_days += 366;
    //       else raw_year_days += 365;
    //     }
    //   }
    //   return raw_year_days;
    // }
    Date(const int32_t& year, const int32_t& month, const int32_t& day)
    {
      _Validate(year,month,day);
      m_year = year;
      m_month = month;
      m_day = day;
      m_serialized_value = (m_year * 10000) + (m_month * 100) + m_day;
      _PopulateDaysOfMonths();
    }

    public:
    Date() = delete;
    ~Date(){}
    int32_t day() const { return m_day; }
    int32_t year() const { return m_year; }
    int32_t month() const {return m_month; }
    int32_t serialized_value() const { return m_serialized_value; }
    
    inline bool operator == (const database::Date& date) const
    {
      return this -> m_year == date.m_year && this -> m_month == date.m_month && this -> m_day == date.m_day;
    }
    inline bool operator != (const database::Date& date) const
    {
      return !(*this == date);
    }
    inline bool operator < (const database::Date& date) const
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
                  else return false; //Same day
                }else return false;
              }
            }else return false;
          }
        }
        else return false;
      }
    }
    inline bool operator > (const database::Date& date) const
    {
      return (*this != date) && !(*this < date);
    }

    // int32_t Days_Until(const database::Date& later_date) const
    // {
    //   if(later_date < *this)
    //     throw std::invalid_argument("Argument has an earlier date");
    //   if(later_date == *this) return 0;
    //   else
    //   {
    //     int32_t raw_month_days = 0;
    //     int32_t index = month();

    //     while()
    //     {
    //       while(index != later_date.month())
    //       {
    //         std::cout << "Index: " << index << "\n";
    //         if(index != 0) raw_month_days += m_days_of_months.at(index);
    //         index = (index + 1) % 13;
    //       }
    //     }

    //     std::cout << "Raw days difference in months: " << raw_difference_months << "\n";

    //     if(month() < later_date.month())
    //       raw_difference_years += raw_difference_months;
    //     else 
    //       raw_difference_years -= raw_difference_months;
        
    //     int32_t raw_difference_days = day() - later_date.day();
    //     return raw_difference_years + raw_difference_days;
    //   }
    // }

    // int32_t Days_From(const database::Date& earlier_date) const;

    #pragma mark Static methods to create date
    //This is because with the default initializers, the relative positions for the components are ambiguous.
    static Date With_yyyy_mm_dd(const int32_t& year, const int32_t& month, const int32_t& day)
    { return Date(year,month,day); }
    static Date With_dd_mm_yyyy(const int32_t& day, const int32_t& month, const int32_t& year)
    { return Date(year,month,day); }
    static Date With_mm_dd_yyyy(const int32_t& month, const int32_t& day, const int32_t& year)
    { return Date(year,month,day); }
    static Date With_yyyymmdd(const int32_t& serialized_value)
    {
      int32_t year = serialized_value / 10000;
      int32_t year_component_remainder = serialized_value % 10000;
      int32_t month = year_component_remainder / 100;
      int32_t day = year_component_remainder % 100;
      return Date(year,month,day);
    }
    static Date With_ddmmyyyy(const int32_t& serialized_value)
    {
      int32_t year = serialized_value % 10000;
      int32_t year_component_remainder = serialized_value / 10000;
      int32_t month = year_component_remainder % 100;
      int32_t day = year_component_remainder / 100;
      return Date(year,month,day);
    }
    static Date With_mmddyyyy(const int32_t& serialized_value)
    {
      int32_t year = serialized_value % 10000;
      int32_t year_component_remainder = serialized_value / 10000;
      int32_t month = year_component_remainder / 100;
      int32_t day = year_component_remainder % 100;
      return Date(year,month,day);
    }
  };
}

// int32_t operator - (const database::Date& lhs, const database::Date& rhs);

#endif //DATE_H