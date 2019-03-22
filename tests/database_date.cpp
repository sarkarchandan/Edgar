#include "gtest/gtest.h"
#include "Date.hpp"

TEST(DateTests,canCreateDates)
{
  database::Date date1 = {2017,04,12};
  ASSERT_EQ(date1.day(),12);
  ASSERT_EQ(date1.month(),04);
  ASSERT_EQ(date1.year(),2017);
  ASSERT_EQ(date1.serialized_value(),20170412);

  database::Date date2 = {20031202};
  ASSERT_EQ(date2.day(),02);
  ASSERT_EQ(date2.month(),12);
  ASSERT_EQ(date2.year(),2003);
  ASSERT_EQ(date2.serialized_value(),20031202);
}

TEST(DateTests,canCompareDates)
{
  database::Date date1 = {2017,01,02};
  database::Date date2 = {20170103};
  ASSERT_TRUE(date1 < date2);
  ASSERT_TRUE(date2 > date1);
  
  database::Date date3 = {2016,02,18};
  database::Date date4 = {2016,01,19};
  ASSERT_TRUE(date4 < date3);
  ASSERT_TRUE(date3 > date4);

  database::Date date5 = {20180323};
  database::Date date6 = {2019,04,23};
  ASSERT_TRUE(date5 < date6);
  ASSERT_TRUE(date6 > date5);

  database::Date date7 = {20180212};
  database::Date date8 = {2018,02,12};
  ASSERT_TRUE(date7 == date8);

  database::Date date9 = {2019,02,23};
  database::Date date10 = {2018,02,23};
  ASSERT_TRUE(date9 != date10);
  ASSERT_TRUE(date9 > date10);

  database::Date date11 = {20171123};
  database::Date date12 = {20171023};
  ASSERT_TRUE(date11 != date12);
  ASSERT_TRUE(date12 < date11);

  database::Date date13 = {20170512};
  database::Date date14 = {2017,05,11};
  ASSERT_TRUE(date13 != date14);
  ASSERT_FALSE(date13 < date14);
}