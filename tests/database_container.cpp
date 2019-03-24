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

TEST(ContainerTests,canStoreAndRetrieveAllDataFromContainer)
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
  std::map<std::string,std::vector<database::ComparableString>> query_result = database::TransactionFactory::SelectAllFrom(container);
  std::map<std::string,std::vector<database::ComparableString>> expected_result = {
    {"p_country",{(std::string)"US",(std::string)"US",(std::string)"DE"}},
    {"p_id",{(std::string)"p1",(std::string)"p2",(std::string)"p3"}},
    {"p_name",{(std::string)"John",(std::string)"Tim",(std::string)"Ulrike"}}
  };
  ASSERT_TRUE(query_result == expected_result);
}
