#include "gtest/gtest.h"
#include "Query.hpp"

TEST(QueryTests,canDeriveTransactionType)
{
  database::Query query_create_database = "create database company";
  database::Query query_create_container = "create container company.employee(employee_id integer,employee_name string,employee_status string)";
  database::Query query_insert_into = "insert into company.employee values(employee_id:1,employee_name:chandan,employee_status:fulltime)";
  database::Query query_select_all = "select * from company.employee";
  database::Query query_select_dataset = "select employee_id,employee_name from company.employee where employee_id = 1";
  database::Query query_update = "update company.employee set employee_status = fulltime where employee_id = 1";
  database::Query query_truncate = "truncate container company.employee";
  database::Query query_alter = "alter container company.employee delete column employee_status";
  database::Query query_delete_from = "delete from company.employee where employee_id = 1";
  database::Query query_drop_container = "drop container company.employee";
  database::Query query_drop_database = "drop database company";

  ASSERT_TRUE(query_create_database.transactionType() == database::create_database);
  ASSERT_TRUE(query_create_database.transactionMetaType() == database::ddl);

  ASSERT_TRUE(query_create_container.transactionType() == database::create_container);
  ASSERT_TRUE(query_create_container.transactionMetaType() == database::ddl);

  ASSERT_TRUE(query_insert_into.transactionType() == database::insert_into);
  ASSERT_TRUE(query_insert_into.transactionMetaType() == database::dml);

  ASSERT_TRUE(query_select_all.transactionType() == database::select_all);
  ASSERT_TRUE(query_select_all.transactionMetaType() == database::dml);

  ASSERT_TRUE(query_select_dataset.transactionType() == database::select_dataset);
  ASSERT_TRUE(query_select_dataset.transactionMetaType() == database::dml);

  ASSERT_TRUE(query_update.transactionType() == database::update);
  ASSERT_TRUE(query_update.transactionMetaType() == database::dml);

  ASSERT_TRUE(query_truncate.transactionType() == database::truncate);
  // ASSERT_TRUE(query_truncate.transactionMetaType() == database::dml);

  ASSERT_TRUE(query_alter.transactionType() == database::alter);
  // ASSERT_TRUE(query_alter.transactionMetaType() == database::ddl);

  ASSERT_TRUE(query_delete_from.transactionType() == database::delete_from);
  // ASSERT_TRUE(query_delete_from.transactionMetaType() == database::dml);

  ASSERT_TRUE(query_drop_container.transactionType() == database::drop_container);
  // ASSERT_TRUE(query_drop_container.transactionMetaType() == database::ddl);

  ASSERT_TRUE(query_drop_database.transactionType() == database::drop_database);
  // ASSERT_TRUE(query_drop_database.transactionMetaType() == database::ddl);
}

TEST(QueryTests_CreateDatabase,canDetermineSpecificationForCreateDatabase)
{
  database::Query query_create_database = "create database company";
  ASSERT_TRUE(query_create_database.transactionMetaType() == database::ddl);
  ASSERT_TRUE(query_create_database.databaseName() == "company");
}

TEST(QueryTests_CreateContainer,canDetermineSpecificationForCreateContainer)
{
  database::Query query_create_container = "create container company.employee(employee_id integer,employee_name string,employee_status string)";
  ASSERT_TRUE(query_create_container.transactionMetaType() == database::ddl);
  ASSERT_TRUE(query_create_container.databaseName() == "company");
  ASSERT_TRUE(query_create_container.containerName() == "employee");
  std::map<std::string,database::QueryDataType> expectedSchema = {
    {"employee_id",database::integer},
    {"employee_name",database::string},
    {"employee_status",database::string}
  };
  ASSERT_TRUE(query_create_container.containerSchema() == expectedSchema);
}

TEST(QueryTests_InsertInto,canDetermineSpecificationForInsertInto)
{
  database::Query query_insert_into = "insert into company.employee values(employee_id:1,employee_name:chandan,employee_status:fulltime)";
  ASSERT_TRUE(query_insert_into.transactionMetaType() == database::dml);
  ASSERT_TRUE(query_insert_into.databaseName() == "company");
  ASSERT_TRUE(query_insert_into.containerName() == "employee");
  std::map<std::string,std::string> expectedDataSet = {
    {"employee_id","1"},
    {"employee_name","chandan"},
    {"employee_status","fulltime"}
  };
  ASSERT_TRUE(query_insert_into.insertDataset() == expectedDataSet);
}

TEST(QueryTests_SelectAll,canDetermineSpecificationForSelectAll)
{
  database::Query query_select_all = "select * from company.employee";
  ASSERT_TRUE(query_select_all.transactionMetaType() == database::dml);
  ASSERT_TRUE(query_select_all.databaseName() == "company");
  ASSERT_TRUE(query_select_all.containerName() == "employee");
}

TEST(QueryTests_SelectDataSet,canDetermineSpecificationForSelectDataSet)
{
  database::Query query_select_dataset = "select employee_id,employee_name from company.employee where employee_id = 1";
  ASSERT_TRUE(query_select_dataset.transactionMetaType() == database::dml);
  ASSERT_TRUE(query_select_dataset.databaseName() == "company");
  ASSERT_TRUE(query_select_dataset.containerName() == "employee");
  std::vector<std::string> expected_dataset = {"employee_id","employee_name"};
  ASSERT_TRUE(query_select_dataset.selectDataset() == expected_dataset);
  std::map<std::string,std::string> expected_conditions = {
    {"employee_id","1"}
  };
  ASSERT_TRUE(query_select_dataset.selectConditions() == expected_conditions);
}

TEST(QueryTests_Update,canDetermineSpecificationForUpdate)
{
  database::Query query_update = "update company.employee set employee_name = Tim,employee_status = fulltime where employee_id = 1";
  ASSERT_TRUE(query_update.transactionMetaType() == database::dml);
  ASSERT_TRUE(query_update.databaseName() == "company");
  ASSERT_TRUE(query_update.containerName() == "employee");
  std::map<std::string,std::string> update_data = {
    {"employee_name","Tim"},
    {"employee_status","fulltime"}
  };
  std::map<std::string,std::string> update_conditions = {
    {"employee_id","1"}
  };
  ASSERT_TRUE(query_update.updateDataset() == update_data);
  ASSERT_TRUE(query_update.updateConditions() == update_conditions);
}
