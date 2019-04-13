#include "gtest/gtest.h"
#include "Query.hpp"

TEST(QueryTests,canDeriveTransactionType)
{
  database::Query query_create_database = "create database company";
  database::Query query_create_container = "create container company.employee(employee_id int,employee_name string,employee_status string)";
  database::Query query_insert_into = "insert into company.employee values(employee_id:1,employee_name:chandan,employee_status:fulltime)";
  database::Query query_select_all = "select * from companye.employee";
  database::Query query_select_dataset = "select employee_id,employee_name from company.employee where employee_id = 1";
  database::Query query_update = "update company.employee set employee_status = fulltime where employee_id = 1";
  database::Query query_truncate = "truncate container company.employee";
  database::Query query_alter = "alter container company.employee delete column employee_status";
  database::Query query_delete_from = "delete from company.employee where employee_id = 1";
  database::Query query_drop_container = "drop container company.employee";
  database::Query query_drop_database = "drop database company";

  ASSERT_TRUE(query_create_database.m_transaction_type == database::TransactionType::create_database);
  ASSERT_TRUE(query_create_container.m_transaction_type == database::TransactionType::create_container);
  ASSERT_TRUE(query_insert_into.m_transaction_type == database::TransactionType::insert_into);
  ASSERT_TRUE(query_select_all.m_transaction_type == database::TransactionType::select_all);
  ASSERT_TRUE(query_select_dataset.m_transaction_type == database::TransactionType::select_dataset);
  ASSERT_TRUE(query_update.m_transaction_type == database::TransactionType::update);
  ASSERT_TRUE(query_truncate.m_transaction_type == database::TransactionType::truncate);
  ASSERT_TRUE(query_alter.m_transaction_type == database::TransactionType::alter);
  ASSERT_TRUE(query_delete_from.m_transaction_type == database::TransactionType::delete_from);
  ASSERT_TRUE(query_drop_container.m_transaction_type == database::TransactionType::drop_container);
  ASSERT_TRUE(query_drop_database.m_transaction_type == database::TransactionType::drop_database);
}

TEST(QueryTests_CreateDatabase,canDetermineSpecificationForCreateDatabase)
{
  database::Query query_create_database = "create database company";
  ASSERT_TRUE(query_create_database.m_database_name == "company");
}

TEST(QueryTests_CreateContainer,canDetermineSpecificationForCreateContainer)
{
  database::Query query_create_container = "create container company.employee(employee_id int,employee_name string,employee_status string)";
  ASSERT_TRUE(query_create_container.m_database_name == "company");
  ASSERT_TRUE(query_create_container.m_container_name == "employee");
  std::map<std::string,std::string> expectedSchema = {
    {"employee_id","int"},
    {"employee_name","string"},
    {"employee_status","string"}
  };
  ASSERT_TRUE(query_create_container.m_container_schema == expectedSchema);
}

TEST(QueryTests_InsertInto,canDetermineSpecificationForInsertInto)
{
  database::Query query_insert_into = "insert into company.employee values(employee_id:1,employee_name:chandan,employee_status:fulltime)";
  ASSERT_TRUE(query_insert_into.m_database_name == "company");
  ASSERT_TRUE(query_insert_into.m_container_name == "employee");
  std::map<std::string,std::string> expectedDataSet = {
    {"employee_id","1"},
    {"employee_name","chandan"},
    {"employee_status","fulltime"}
  };
  ASSERT_TRUE(query_insert_into.m_insert_dataset == expectedDataSet);
}

TEST(QueryTests_SelectAll,canDetermineSpecificationForSelectAll)
{
  database::Query query_select_all = "select * from company.employee";
  ASSERT_TRUE(query_select_all.m_database_name == "company");
  ASSERT_TRUE(query_select_all.m_container_name == "employee");
}

TEST(QueryTests_SelectDataSet,canDetermineSpecificationForSelectDataSet)
{
  database::Query query_select_dataset = "select employee_id,employee_name from company.employee where employee_id = 1";
  ASSERT_TRUE(query_select_dataset.m_database_name == "company");
  ASSERT_TRUE(query_select_dataset.m_container_name == "employee");
  std::vector<std::string> expected_dataset = {"employee_id","employee_name"};
  ASSERT_TRUE(query_select_dataset.m_select_dataset == expected_dataset);
  std::map<std::string,std::string> expected_conditions = {
    {"employee_id","1"}
  };
  ASSERT_TRUE(query_select_dataset.m_select_conditions == expected_conditions);
}

TEST(QueryTests_Update,canDetermineSpecificationForUpdate)
{
  database::Query query_update = "update company.employee set employee_name = Tim,employee_status = fulltime where employee_id = 1";
  ASSERT_TRUE(query_update.m_database_name == "company");
  ASSERT_TRUE(query_update.m_container_name == "employee");
  std::map<std::string,std::string> update_data = {
    {"employee_name","Tim"},
    {"employee_status","fulltime"}
  };
  std::map<std::string,std::string> update_conditions = {
    {"employee_id","1"}
  };
  ASSERT_TRUE(query_update.m_update_data == update_data);
  ASSERT_TRUE(query_update.m_update_conditions == update_conditions);
}
