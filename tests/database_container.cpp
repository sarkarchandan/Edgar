#include "gtest/gtest.h"
#include "TransactionFactory.hpp"
#include "Utility.hpp"

TEST(ContainerTests,canInitializeContainer)
{
  std::map<std::string,std::string> schema = {
    {"p_id","std::size_t"},
    {"p_name","database::ComparableString"},
    {"p_country","database::ComparableString"},
    {"p_date_added","database::ComparableString"}};
  database::Container container = database::TransactionFactory::ContainerWith_Name_Schema("Person",schema);
  std::hash<std::string> s_hash;
  std::size_t expected_container_id = s_hash("Person");
  ASSERT_TRUE(container.name() == "Person");
  ASSERT_TRUE(container.id() == expected_container_id);
  ASSERT_TRUE(container.schema() == schema);
}

TEST(ContainerTests, canStoreAndRetrieveAllData)
{
  std::map<std::string,std::string> schema = {
    {"p_id","std::size_t"},
    {"p_name","database::ComparableString"},
    {"p_country","database::ComparableString"}};

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

TEST(ContainerTests,canStoreAndRetrieveAllDataBasedOnSingleCriterion)
{
  std::map<std::string,std::string> schema = {
    {"p_id","std::size_t"},
    {"p_name","database::ComparableString"},
    {"p_country","database::ComparableString"}};

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
  database::TransactionFactory::InsertInto(container,value1);
  database::TransactionFactory::InsertInto(container,value2);
  database::TransactionFactory::InsertInto(container,value3);
  database::TransactionFactory::InsertInto(container,value4);
  database::TransactionFactory::InsertInto(container,value5);
  database::TransactionFactory::InsertInto(container,value6);
  std::map<std::string,std::vector<database::ComparableString>> expected_result = {
    {"p_country",{(std::string)"US",(std::string)"US"}},
    {"p_id",{(std::string)"p1",(std::string)"p2"}},
    {"p_name",{(std::string)"John",(std::string)"Tim"}}
  };
  database::TransactionFactory::SelectWithCriteriaFrom(container,{{"p_country",(std::string)"DE"}},{database::ValueComparisonType::equal_to},{},[&](auto query_result){
    std::cout << "Obtained result: " << "\n" << query_result << "\n";
    ASSERT_TRUE(query_result == expected_result);
  });
}
