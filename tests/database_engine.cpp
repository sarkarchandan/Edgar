#include "gtest/gtest.h"
#include "DatabaseEngine.hpp"
#include "Utility.hpp"

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

TEST(DatabaseEngineTests,canSelectAllDataFromContainer)
{
  database::DatabaseEngine database_engine;
  database::Query query_create_database = "create database company";
  database::Query query_create_container = "create container company.employee(employee_id integer,employee_name string,employee_status string)";
  std::vector<database::Query> insert_queries = {
    "insert into company.employee values(employee_id:1,employee_name:Dominik_Oeh,employee_status:fulltime)",
    "insert into company.employee values(employee_id:2,employee_name:Heinrich_Graser,employee_status:parttime)",
    "insert into company.employee values(employee_id:3,employee_name:Mathias_Renner,employee_status:parttime)",
    "insert into company.employee values(employee_id:4,employee_name:Marcus_Kalb,employee_status:fulltime)"
  };

  database_engine.ExecuteForDataDefinition(query_create_database,[&](auto database_created){ 
    ASSERT_TRUE(database_created);
  });
  database_engine.ExecuteForDataDefinition(query_create_container,[&](auto container_created){
    ASSERT_TRUE(container_created);
  });
  std::for_each(insert_queries.begin(),insert_queries.end(),[&](auto query) {
    database_engine.ExecuteForDataManipulation(query,[&](auto query_result) { /*pass*/ });
  });

  database::Query query_select_all = "select * from company.employee";
  database_engine.ExecuteForDataManipulation(query_select_all,[&](auto query_result) {
    std::map<std::string,std::vector<std::string>> expected_result = {
      {"employee_id",{"1","2","3","4"}},
      {"employee_name",{"Dominik_Oeh","Heinrich_Graser","Mathias_Renner","Marcus_Kalb"}},
      {"employee_status",{"fulltime","parttime","parttime","fulltime"}}
    };
    std::cout << "Result Set: " << "\n" << query_result << "\n";
    ASSERT_TRUE(query_result == expected_result);
  });
}
