#include "gtest/gtest.h"
#include "TransactionFactory.hpp"
#include "Date.hpp"
#include "Utility.hpp"

TEST(ContainerTests,canInitializeContainer)
{
  std::map<std::string,database::DataType> schema = {
    {"p_id",database::numeric},
    {"p_name",database::non_numeric},
    {"p_country",database::non_numeric},
    {"p_date_added",database::numeric}};
  database::Container container = database::TransactionFactory::ContainerWith_Name_Schema("Person",schema);
  std::hash<std::string> s_hash;
  std::size_t expected_container_id = s_hash("Person");
  ASSERT_TRUE(container.name() == "Person");
  ASSERT_TRUE(container.id() == expected_container_id);
  ASSERT_TRUE(container.schema() == schema);
}

TEST(ContainerTests, canStoreAndRetrieveAllData)
{
  std::map<std::string,database::DataType> schema = {
    {"p_id",database::non_numeric},
    {"p_name",database::non_numeric},
    {"p_country",database::non_numeric}};

  database::Container container = database::TransactionFactory::ContainerWith_Name_Schema("Person",schema);
  std::map<std::string,database::ComparableString> value1 = {
    {"p_id",(std::string)"p1"},
    {"p_name",(std::string)"John"},
    {"p_country",(std::string)"US"}};
  std::map<std::string,database::ComparableString> value2 = {
    {"p_id",(std::string)"p2"},
    {"p_name",(std::string)"Tim"},
    {"p_country",(std::string)"US"}};
    std::map<std::string,database::ComparableString> value3 = {
    {"p_id",(std::string)"p3"},
    {"p_name",(std::string)"Ulrike"},
    {"p_country",(std::string)"DE"}};
  database::TransactionFactory::InsertInto(container,value1);
  database::TransactionFactory::InsertInto(container,value2);
  database::TransactionFactory::InsertInto(container,value3);
  std::map<std::string,std::vector<database::ComparableString>> expected_result = {
    {"p_country",{(std::string)"US",(std::string)"US",(std::string)"DE"}},
    {"p_id",{(std::string)"p1",(std::string)"p2",(std::string)"p3"}},
    {"p_name",{(std::string)"John",(std::string)"Tim",(std::string)"Ulrike"}}
  };
  database::TransactionFactory::SelectAllFrom(container,[&](auto query_result){
    ASSERT_TRUE(query_result == expected_result);
  });
}

TEST(ContainerTests,canStoreAndRetrieveDataBasedOnSingleCriterion)
{
  std::map<std::string,database::DataType> schema = {
    {"p_id",database::non_numeric},
    {"p_name",database::non_numeric},
    {"p_country",database::non_numeric}};

  database::Container container = database::TransactionFactory::ContainerWith_Name_Schema("Person",schema);
  std::map<std::string,database::ComparableString> value1 = {
    {"p_id",(std::string)"p1"},
    {"p_name",(std::string)"John"},
    {"p_country",(std::string)"US"}};
  std::map<std::string,database::ComparableString> value2 = {
    {"p_id",(std::string)"p2"},
    {"p_name",(std::string)"Tim"},
    {"p_country",(std::string)"US"}};
    std::map<std::string,database::ComparableString> value3 = {
    {"p_id",(std::string)"p3"},
    {"p_name",(std::string)"Ulrike"},
    {"p_country",(std::string)"DE"}};
    std::map<std::string,database::ComparableString> value4 = {
    {"p_id",(std::string)"p4"},
    {"p_name",(std::string)"Heinrich"},
    {"p_country",(std::string)"DE"}};
    std::map<std::string,database::ComparableString> value5 = {
    {"p_id",(std::string)"p5"},
    {"p_name",(std::string)"Dominik"},
    {"p_country",(std::string)"DE"}};
    std::map<std::string,database::ComparableString> value6 = {
    {"p_id",(std::string)"p6"},
    {"p_name",(std::string)"Mathias"},
    {"p_country",(std::string)"DE"}};
    std::map<std::string,database::ComparableString> value7 = {
    {"p_id",(std::string)"p7"},
    {"p_name",(std::string)"Steve"},
    {"p_country",(std::string)"NZ"}};
    std::map<std::string,database::ComparableString> value8 = {
    {"p_id",(std::string)"p8"},
    {"p_name",(std::string)"Mark"},
    {"p_country",(std::string)"AU"}};
    std::map<std::string,database::ComparableString> value9 = {
    {"p_id",(std::string)"p9"},
    {"p_name",(std::string)"Sam"},
    {"p_country",(std::string)"NZ"}};
  database::TransactionFactory::InsertInto(container,value1);
  database::TransactionFactory::InsertInto(container,value2);
  database::TransactionFactory::InsertInto(container,value3);
  database::TransactionFactory::InsertInto(container,value4);
  database::TransactionFactory::InsertInto(container,value5);
  database::TransactionFactory::InsertInto(container,value6);
  database::TransactionFactory::InsertInto(container,value7);
  database::TransactionFactory::InsertInto(container,value8);
  database::TransactionFactory::InsertInto(container,value9);

  
  database::TransactionFactory::SelectRawDataSetWithCriteriaFrom(container,
    {{"p_country",{std::make_pair<database::ComparableString,database::ComparisonType>((std::string)"DE",database::ComparisonType::equal_to),std::make_pair<database::ComparableString,database::ComparisonType>((std::string)"NZ",database::equal_to)}}},
    {},[&](auto query_result){
    
    std::map<std::string,std::vector<database::ComparableString>> expected_result1 = {
      {"p_country",{(std::string)"DE",(std::string)"DE",(std::string)"DE",(std::string)"DE",(std::string)"NZ",(std::string)"NZ"}},
      {"p_id",{(std::string)"p3",(std::string)"p4",(std::string)"p5",(std::string)"p6",(std::string)"p7",(std::string)"p9"}},
      {"p_name",{(std::string)"Ulrike",(std::string)"Heinrich",(std::string)"Dominik",(std::string)"Mathias",(std::string)"Steve",(std::string)"Sam"}}
    };
    ASSERT_TRUE(query_result == expected_result1);
  });

  database::TransactionFactory::SelectRawDataSetWithCriteriaFrom(container,
    {{"p_country",{std::make_pair<database::ComparableString,database::ComparisonType>((std::string)"DE",database::not_equal_to)}}},
    {},[&](auto query_result) {

    std::map<std::string,std::vector<database::ComparableString>> expected_result2 = {
      {"p_country",{(std::string)"US",(std::string)"US",(std::string)"NZ",(std::string)"AU",(std::string)"NZ"}},
      {"p_id",{(std::string)"p1",(std::string)"p2",(std::string)"p7",(std::string)"p8",(std::string)"p9"}},
      {"p_name",{(std::string)"John",(std::string)"Tim",(std::string)"Steve",(std::string)"Mark",(std::string)"Sam"}}
    };
    ASSERT_TRUE(query_result == expected_result2);
  });

  database::TransactionFactory::SelectRawDataSetWithCriteriaFrom(container,
    {{"p_country",{std::make_pair<database::ComparableString,database::ComparisonType>((std::string)"US",database::equal_to),std::make_pair<database::ComparableString,database::ComparisonType>((std::string)"AU",database::equal_to)}}},
    {"p_id","p_name"},[&](auto query_result) {
    
    std::map<std::string,std::vector<database::ComparableString>> expected_result3 = {
      {"p_id",{(std::string)"p1",(std::string)"p2",(std::string)"p8"}},
      {"p_name",{(std::string)"John",(std::string)"Tim",(std::string)"Mark"}}
    };
    ASSERT_TRUE(query_result == expected_result3);
  });

  std::map<std::string,std::vector<database::ComparableString>> expected_result4 = {
    {"p_name",{(std::string)"John",(std::string)"Tim",(std::string)"Ulrike",(std::string)"Heinrich",(std::string)"Dominik",(std::string)"Mathias",(std::string)"Steve",(std::string)"Mark",(std::string)"Sam"}},
    {"p_country",{(std::string)"US",(std::string)"US",(std::string)"DE",(std::string)"DE",(std::string)"DE",(std::string)"DE",(std::string)"NZ",(std::string)"AU",(std::string)"NZ"}}
  };
  database::TransactionFactory::SelectRawDataSetFrom(container,{"p_name","p_country"},[&](auto query_result) {
    ASSERT_TRUE(query_result == expected_result4);
  });
}

TEST(ContainerTests,canUpdateDataBasedOnSingleCriterion)
{
  std::map<std::string,database::DataType> schema = {
    {"emp_id",database::non_numeric},
    {"emp_name",database::non_numeric},
    {"emp_status",database::non_numeric},
    {"emp_start_date",database::non_numeric}
  };
  database::Container container = database::TransactionFactory::ContainerWith_Name_Schema("Employee",schema);
  std::map<std::string,database::ComparableString> value1 = {
    {"emp_id",(std::string)"emp1"},
    {"emp_name",(std::string)"John"},
    {"emp_status",(std::string)"PARTTIME"},
    {"emp_start_date",database::Date::With_ddmmyyyy(11012018).string_value()}
  };
  std::map<std::string,database::ComparableString> value2 = {
    {"emp_id",(std::string)"emp2"},
    {"emp_name",(std::string)"James"},
    {"emp_status",(std::string)"PARTTIME"},
    {"emp_start_date",database::Date::With_ddmmyyyy(11022018).string_value()}
  };
  std::map<std::string,database::ComparableString> value3 = {
    {"emp_id",(std::string)"emp3"},
    {"emp_name",(std::string)"Tim"},
    {"emp_status",(std::string)"PARTTIME"},
    {"emp_start_date",database::Date::With_ddmmyyyy(11032018).string_value()}
  };
  std::map<std::string,database::ComparableString> value4 = {
    {"emp_id",(std::string)"emp4"},
    {"emp_name",(std::string)"Ulrike"},
    {"emp_status",(std::string)"PARTTIME"},
    {"emp_start_date",database::Date::With_ddmmyyyy(11042018).string_value()}
  };
  std::map<std::string,database::ComparableString> value5 = {
    {"emp_id",(std::string)"emp5"},
    {"emp_name",(std::string)"Markus"},
    {"emp_status",(std::string)"PARTTIME"},
    {"emp_start_date",database::Date::With_ddmmyyyy(11052018).string_value()}
  };
  std::map<std::string,database::ComparableString> value6 = {
    {"emp_id",(std::string)"emp6"},
    {"emp_name",(std::string)"Udo"},
    {"emp_status",(std::string)"PARTTIME"},
    {"emp_start_date",database::Date::With_ddmmyyyy(11062018).string_value()}
  };
  database::TransactionFactory::InsertInto(container,value1);
  database::TransactionFactory::InsertInto(container,value2);
  database::TransactionFactory::InsertInto(container,value3);
  database::TransactionFactory::InsertInto(container,value4);
  database::TransactionFactory::InsertInto(container,value5);
  database::TransactionFactory::InsertInto(container,value6);
  database::TransactionFactory::Update(container,
    {{"emp_start_date",{std::make_pair<database::ComparableString,database::ComparisonType>(database::Date::With_ddmmyyyy(11042018).string_value(),database::lesser_or_equal_to)}}},
    {{"emp_status",(std::string)"FULLTIME"}}
  );
  std::map<std::string,std::vector<database::ComparableString>> expected_result1 = {
    {"emp_name",{(std::string)"John",(std::string)"James",(std::string)"Tim",(std::string)"Ulrike",(std::string)"Markus",(std::string)"Udo"}},
    {"emp_status",{(std::string)"FULLTIME",(std::string)"FULLTIME",(std::string)"FULLTIME",(std::string)"FULLTIME",(std::string)"PARTTIME",(std::string)"PARTTIME"}}
  };
  database::TransactionFactory::SelectRawDataSetFrom(container,{"emp_name","emp_status"},[&](auto query_result){
    ASSERT_TRUE(query_result == expected_result1);
  });
}
