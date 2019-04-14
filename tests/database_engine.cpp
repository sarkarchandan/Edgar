#include "gtest/gtest.h"
#include "DatabaseEngine.hpp"

TEST(DatabaseEngineTests,canCreateDatabase)
{
  database::DatabaseEngine databaseEngine;
  database::Query query_create_database = "create database company";
  databaseEngine.ExecuteForDataDefintion(query_create_database,[&](bool created){
    ASSERT_TRUE(created);
  });
}
