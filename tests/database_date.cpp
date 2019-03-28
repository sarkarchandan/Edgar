#include "gtest/gtest.h"
#include "Date.hpp"
#include "Utility.hpp"

TEST(DateTests,canCreateDates)
{
  database::Date date1 = database::Date::With_yyyy_mm_dd(2017,04,12);
  ASSERT_EQ(date1.day(),12);
  ASSERT_EQ(date1.month(),04);
  ASSERT_EQ(date1.year(),2017);
  ASSERT_EQ(date1.serialized_value(),20170412);

  database::Date date2 = database::Date::With_yyyymmdd(20031202);
  ASSERT_EQ(date2.day(),02);
  ASSERT_EQ(date2.month(),12);
  ASSERT_EQ(date2.year(),2003);
  ASSERT_EQ(date2.serialized_value(),20031202);

  database::Date date3 = database::Date::With_mm_dd_yyyy(10,05,1998);
  ASSERT_EQ(date3.day(),05);
  ASSERT_EQ(date3.month(),10);
  ASSERT_EQ(date3.year(),1998);
  ASSERT_EQ(date3.serialized_value(),19981005);

  database::Date date4 = database::Date::With_mmddyyyy(7281986);
  ASSERT_EQ(date4.day(),28);
  ASSERT_EQ(date4.month(),7);
  ASSERT_EQ(date4.year(),1986);
  ASSERT_EQ(date4.serialized_value(),19860728);

  database::Date date5 = database::Date::With_dd_mm_yyyy(1,11,2001);
  ASSERT_EQ(date5.day(),1);
  ASSERT_EQ(date5.month(),11);
  ASSERT_EQ(date5.year(),2001);
  ASSERT_EQ(date5.serialized_value(),20011101);

  database::Date date6 = database::Date::With_ddmmyyyy(19022014);
  ASSERT_EQ(date6.day(),19);
  ASSERT_EQ(date6.month(),02);
  ASSERT_EQ(date6.year(),2014);
  ASSERT_EQ(date6.serialized_value(),20140219);

  database::Date date7 = database::Date::With_yyyymmdd((std::string)"20031202");
  database::Date date8 = database::Date::With_yyyy_mm_dd(2003,12,02);
  ASSERT_TRUE(date7 == date8);
  ASSERT_TRUE(date7.string_value() == date8.string_value());
  ASSERT_TRUE(date7.serialized_value() == date8.serialized_value());

  database::Date date9 = database::Date::With_mmddyyyy((std::string)"7281986");
  database::Date date10 = database::Date::With_mm_dd_yyyy(07,28,1986);
  ASSERT_TRUE(date9 == date10);
  ASSERT_TRUE(date9.string_value() == date10.string_value());
  ASSERT_TRUE(date9.serialized_value() == date10.serialized_value());

  database::Date date11 = database::Date::With_ddmmyyyy((std::string)"19022014");
  database::Date date12 = database::Date::With_dd_mm_yyyy(19,02,2014);
  ASSERT_TRUE(date11 == date12);
  ASSERT_TRUE(date11.string_value() == date12.string_value());
  ASSERT_TRUE(date11.serialized_value() == date12.serialized_value());
}

TEST(DateTests_ExceptionTests,canValidate)
{
  EXPECT_THROW(database::Date::With_ddmmyyyy(32012017),std::invalid_argument);
  EXPECT_THROW(database::Date::With_mmddyyyy(13052008),std::invalid_argument);
  EXPECT_THROW(database::Date::With_yyyymmdd(20030631),std::invalid_argument);
}

TEST(DateTests,canCompareDates)
{
  database::Date date1 = database::Date::With_yyyy_mm_dd(2017,01,02);
  database::Date date2 = database::Date::With_yyyymmdd(20170103);
  ASSERT_TRUE(date1 < date2);
  ASSERT_TRUE(date1 <= date2);
  ASSERT_TRUE(date2 > date1);
  ASSERT_TRUE(date2 >= date1);

  database::Date date3 = database::Date::With_yyyy_mm_dd(2016,02,18);
  database::Date date4 = database::Date::With_ddmmyyyy(19012016);
  ASSERT_TRUE(date4 < date3);
  ASSERT_TRUE(date4 <= date3);
  ASSERT_TRUE(date3 > date4);
  ASSERT_TRUE(date3 >= date4);

  database::Date date5 = database::Date::With_ddmmyyyy(23032018);
  database::Date date6 = database::Date::With_yyyymmdd(20190423);
  ASSERT_TRUE(date5 < date6);
  ASSERT_TRUE(date5 <= date6);
  ASSERT_TRUE(date6 > date5);
  ASSERT_TRUE(date6 >= date5);

  database::Date date7 = database::Date::With_yyyymmdd(20181202);
  database::Date date8 = database::Date::With_mmddyyyy(12022018);
  ASSERT_TRUE(date7 == date8);
  ASSERT_TRUE(date7 <= date8);
  ASSERT_TRUE(date7 >= date8);

  database::Date date9 = database::Date::With_mm_dd_yyyy(02,23,2019);
  database::Date date10 = database::Date::With_dd_mm_yyyy(23,02,2018);
  ASSERT_TRUE(date9 != date10);
  ASSERT_TRUE(date9 > date10);
  ASSERT_TRUE(date9 >= date10);

  database::Date date11 = database::Date::With_yyyymmdd(20171123);
  database::Date date12 = database::Date::With_ddmmyyyy(23102017);
  ASSERT_TRUE(date11 != date12);
  ASSERT_TRUE(date12 < date11);

  database::Date date13 = database::Date::With_yyyymmdd(20170512);
  database::Date date14 = database::Date::With_yyyy_mm_dd(2017,05,11);
  ASSERT_TRUE(date13 != date14);
  ASSERT_FALSE(date13 < date14);
  ASSERT_TRUE(date13 >= date14);
}

TEST(DateTests,canConvertFromDateToComparableString)
{
  FAIL() << "Not Implemented";
}

TEST(DateTests,canConvertFromComparableStringToDate)
{
  FAIL() << "Not Implemented";
}
