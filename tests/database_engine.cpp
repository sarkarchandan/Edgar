#include "gtest/gtest.h"
#include "DatabaseEngine.hpp"

TEST(DatabaseEngineTests,canCreateDatabase)
{
  database::DatabaseEngine databaseEngine;
  database::Query query_create_database = "create database company";
  databaseEngine.ExecuteForDataDefintion(query_create_database,[&](bool database_created){
    ASSERT_TRUE(database_created);
  });
}

TEST(DatabaseEngineTests,canCreateContainer)
{
  database::DatabaseEngine databaseEngine;
  database::Query query_create_container = "create container company.employee(employee_id integer,employee_name string,employee_status string)";
  databaseEngine.ExecuteForDataDefintion(query_create_container,[&](auto container_created) {
    ASSERT_FALSE(container_created);
  });
  database::Query query_create_database = "create database company";
  databaseEngine.ExecuteForDataDefintion(query_create_database,[&](auto database_created) {
    ASSERT_TRUE(database_created);
  });
  databaseEngine.ExecuteForDataDefintion(query_create_container,[&](auto container_created) {
    ASSERT_TRUE(container_created);
  });
}
