#include "gtest/gtest.h"
#include "DatabaseEngine.hpp"

TEST(DatabaseEngineTests,canCreateDatabase)
{
  database::DatabaseEngine database_engine;
  database::Query query_create_database = "create database company";
  database_engine.ExecuteForDataDefinition(query_create_database,[&](bool database_created){
    ASSERT_TRUE(database_created);
  });
}

TEST(DatabaseEngineTests,canCreateContainer)
{
  database::DatabaseEngine database_engine;
  database::Query query_create_container = "create container company.employee(employee_id integer,employee_name string,employee_status string)";
  database_engine.ExecuteForDataDefinition(query_create_container,[&](auto container_created) {
    ASSERT_FALSE(container_created);
  });
  database::Query query_create_database = "create database company";
  database_engine.ExecuteForDataDefinition(query_create_database,[&](auto database_created) {
    ASSERT_TRUE(database_created);
  });
  database_engine.ExecuteForDataDefinition(query_create_container,[&](auto container_created) {
    ASSERT_TRUE(container_created);
  });
}

TEST(DatabaseEngineTests,canInsertDataIntoContainer)
{
  database::DatabaseEngine database_engine;
  database::Query query_create_database = "create database company";
  database::Query query_create_container = "create container company.employee(employee_id integer,employee_name string,employee_status string)";
  database::Query query_insert_into = "insert into company.employee values(employee_id:1,employee_name:chandan,employee_status:fulltime)";
  database_engine.ExecuteForDataDefinition(query_create_database,[&](auto database_created){ 
    ASSERT_TRUE(database_created);
  });
  database_engine.ExecuteForDataDefinition(query_create_container,[&](auto container_created){
    ASSERT_TRUE(container_created);
  });
  database_engine.ExecuteForDataManipulation(query_insert_into,[&](auto query_result) {
    std::map<std::string,std::vector<std::string>> expected_result = {
      {"employee_id",{"1"}},
      {"employee_name",{"chandan"}},
      {"employee_status",{"fulltime"}}
    };
    ASSERT_TRUE(query_result == expected_result);
  });
}
