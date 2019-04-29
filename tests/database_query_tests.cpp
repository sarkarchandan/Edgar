#include "gtest/gtest.h"
#include "Query.hpp"
#include "Utility.hpp"

TEST(QueryTests,canDeriveTransactionType)
{
  database::Query query_create_database = "create database company";
  database::Query query_create_container = "create container company.employee(employee_id integer,employee_name string,employee_status string)";
  database::Query query_insert_into = "insert into company.employee values(employee_id:1,employee_name:chandan,employee_status:fulltime)";
  database::Query query_select_all = "select * from company.employee";
  database::Query query_select_dataset = "select employee_id,employee_name from company.employee";
  database::Query query_select_dataset_with_criteria = "select employee_id,employee_name from company.employee where employee_id = 1";
  database::Query query_update = "update company.employee set employee_status = fulltime where employee_id = 1";
  database::Query query_truncate = "truncate container company.employee";
  // database::Query query_alter = "alter container company.employee delete column employee_status";
  // database::Query query_delete_from = "delete from company.employee where employee_id = 1";
  // database::Query query_drop_container = "drop container company.employee";
  // database::Query query_drop_database = "drop database company";

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

  ASSERT_TRUE(query_select_dataset_with_criteria.transactionType() == database::select_dataset);
  ASSERT_TRUE(query_select_dataset_with_criteria.transactionMetaType() == database::dml);

  ASSERT_TRUE(query_update.transactionType() == database::update);
  ASSERT_TRUE(query_update.transactionMetaType() == database::dml);

  ASSERT_TRUE(query_truncate.transactionType() == database::truncate);
  ASSERT_TRUE(query_truncate.transactionMetaType() == database::dml);

  // ASSERT_TRUE(query_alter.transactionType() == database::alter);
  // ASSERT_TRUE(query_alter.transactionMetaType() == database::ddl);

  // ASSERT_TRUE(query_delete_from.transactionType() == database::delete_from);
  // ASSERT_TRUE(query_delete_from.transactionMetaType() == database::dml);
  //
  // ASSERT_TRUE(query_drop_container.transactionType() == database::drop_container);
  // ASSERT_TRUE(query_drop_container.transactionMetaType() == database::ddl);
  //
  // ASSERT_TRUE(query_drop_database.transactionType() == database::drop_database);
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
  database::api_schema_type expected_schema = {
    {"employee_id",database::integer},
    {"employee_name",database::string},
    {"employee_status",database::string}
  };
  ASSERT_TRUE(query_create_container.schema() == expected_schema);
}

TEST(QueryTests_InsertInto,canDetermineSpecificationForInsertInto)
{
  database::Query query_insert_into = "insert into company.employee values(employee_id:1,employee_name:chandan,employee_status:fulltime)";
  ASSERT_TRUE(query_insert_into.transactionMetaType() == database::dml);
  ASSERT_TRUE(query_insert_into.databaseName() == "company");
  ASSERT_TRUE(query_insert_into.containerName() == "employee");
  std::map<std::string,std::string> values = {
    {"employee_id","1"},
    {"employee_name","chandan"},
    {"employee_status","fulltime"}
  };
  ASSERT_TRUE(query_insert_into.values() == values);
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
  database::Query query_select_dataset = "select employee_id,employee_name from company.employee";
  ASSERT_TRUE(query_select_dataset.transactionMetaType() == database::dml);
  ASSERT_TRUE(query_select_dataset.databaseName() == "company");
  ASSERT_TRUE(query_select_dataset.containerName() == "employee");
  std::vector<std::string> expected_dataset = {"employee_id","employee_name"};
  ASSERT_TRUE(query_select_dataset.dataset() == expected_dataset);
  database::api_filter_type expected_filter = {};
  ASSERT_TRUE(query_select_dataset.filter() == expected_filter);
}

TEST(QueryTests_SelectDataSetWithCriteria,canDetermineSpecificationForSelectDataSetWithCriteria)
{
  database::Query query_select_dataset_with_criteria1 = "select employee_id,employee_name from company.employee where employee_id = 1";
  ASSERT_TRUE(query_select_dataset_with_criteria1.transactionMetaType() == database::dml);
  ASSERT_TRUE(query_select_dataset_with_criteria1.databaseName() == "company");
  ASSERT_TRUE(query_select_dataset_with_criteria1.containerName() == "employee");
  std::vector<std::string> expected_dataset1 = {"employee_id","employee_name"};
  ASSERT_TRUE(query_select_dataset_with_criteria1.dataset() == expected_dataset1);
  database::api_filter_type filter1 = {
    {"employee_id",{{"1",database::equal_to}}}
  };
  ASSERT_TRUE(query_select_dataset_with_criteria1.filter() == filter1);

  database::Query query_select_dataset_with_criteria2 = "select employee_name from company.employee where employee_status = fulltime and employee_department <> sales";
  ASSERT_TRUE(query_select_dataset_with_criteria2.transactionType() == database::select_dataset);
  ASSERT_TRUE(query_select_dataset_with_criteria2.transactionMetaType() == database::dml);
  ASSERT_TRUE(query_select_dataset_with_criteria2.databaseName() == "company");
  ASSERT_TRUE(query_select_dataset_with_criteria2.containerName() == "employee");
  std::vector<std::string> expected_dataset2 = {"employee_name"};
  ASSERT_TRUE(query_select_dataset_with_criteria2.dataset() == expected_dataset2);
  database::api_filter_type filter2 = {
    {"employee_status",{{"fulltime",database::equal_to}}},
    {"employee_department",{{"sales",database::not_equal_to}}}
  };
  ASSERT_TRUE(query_select_dataset_with_criteria2.filter() == filter2);

  database::Query query_select_dataset_with_criteria3 = "select employee_id,employee_name,employee_status from company.employee where employee_join_date >= 20170912 and employee_join_date <= 20180101";
  ASSERT_TRUE(query_select_dataset_with_criteria3.transactionType() == database::select_dataset);
  ASSERT_TRUE(query_select_dataset_with_criteria3.transactionMetaType() == database::dml);
  ASSERT_TRUE(query_select_dataset_with_criteria3.databaseName() == "company");
  ASSERT_TRUE(query_select_dataset_with_criteria3.containerName() == "employee");
  std::cout << "Expected dataset: " << query_select_dataset_with_criteria3.dataset() << "\n";
  std::vector<std::string> expected_dataset3 = {"employee_id","employee_name","employee_status"};
  ASSERT_TRUE(query_select_dataset_with_criteria3.dataset() == expected_dataset3);
  database::api_filter_type filter3 = {
    {"employee_join_date",{{"20170912",database::greater_or_equal_to},{"20180101",database::lesser_or_equal_to}}}
  };
  ASSERT_TRUE(query_select_dataset_with_criteria3.filter() == filter3);
}

TEST(QueryTests_Update,canDetermineSpecificationForUpdate)
{
  database::Query query_update1 = "update company.employee set employee_name = Tim,employee_status = fulltime where employee_id = 1";
  ASSERT_TRUE(query_update1.transactionType() == database::update);
  ASSERT_TRUE(query_update1.transactionMetaType() == database::dml);
  ASSERT_TRUE(query_update1.databaseName() == "company");
  ASSERT_TRUE(query_update1.containerName() == "employee");

  database::api_insert_update_type update_data1 = {
    {"employee_name","Tim"},
    {"employee_status","fulltime"}
  };
  database::api_filter_type filter1 = {
    {"employee_id",{{"1",database::equal_to}}}
  };
  ASSERT_TRUE(query_update1.values() == update_data1);
  ASSERT_TRUE(query_update1.filter() == filter1);

  database::Query query_update2 = "update company.employee set employee_status = fulltime where where employee_join_date >= 20180301 and employee_join_date <= 20190101 and employee_status = parttime";
  ASSERT_TRUE(query_update2.transactionType() == database::update);
  ASSERT_TRUE(query_update2.transactionMetaType() == database::dml);
  ASSERT_TRUE(query_update2.databaseName() == "company");
  ASSERT_TRUE(query_update2.containerName() == "employee");
  database::api_insert_update_type update_data2 = {
    {"employee_status","fulltime"}
  };
  database::api_filter_type filter2 = {
    {"employee_join_date",{{"20180301",database::greater_or_equal_to},{"20190101",database::lesser_or_equal_to}}},
    {"employee_status",{{"parttime",database::equal_to}}}
  };
  ASSERT_TRUE(query_update2.values() == update_data2);
  ASSERT_TRUE(query_update2.filter() == filter2);
}

TEST(QueryTests_Truncate,canDetermineSpecificationForTruncate)
{
  database::Query query_truncate = "truncate container company.employee";
  ASSERT_TRUE(query_truncate.transactionMetaType() == database::dml);
  ASSERT_TRUE(query_truncate.databaseName() == "company");
  ASSERT_TRUE(query_truncate.containerName() == "employee");
}

// TEST(QueryTests_DeleteFrom,canDetermineSpecificationForDeleteFrom)
// {
//   database::Query query_delete_from = "delete from company.employee where employee_id = 1";
//   ASSERT_TRUE(query_delete_from.transactionMetaType() == database::dml);
//   ASSERT_TRUE(query_delete_from.databaseName() == "company");
//   ASSERT_TRUE(query_delete_from.containerName() == "employee");
//   std::map<std::string,std::string> delete_conditions = {
//     {"employee_id","1"}
//   };
//   ASSERT_TRUE(query_delete_from.deleteConditions() == delete_conditions);
// }
//
// TEST(QueryTests_DropContainer,canDetermineSpecificationForDropContainer)
// {
//   database::Query query_drop_container = "drop container company.employee";
//   ASSERT_TRUE(query_drop_container.transactionMetaType() == database::ddl);
//   ASSERT_TRUE(query_drop_container.databaseName() == "company");
//   ASSERT_TRUE(query_drop_container.containerName() == "employee");
// }
//
// TEST(QueryTests_DropDatabase,canDetermineSpecificationForDropDatabase)
// {
//   database::Query query_drop_database = "drop database company";
//   ASSERT_TRUE(query_drop_database.transactionMetaType() == database::ddl);
//   ASSERT_TRUE(query_drop_database.databaseName() == "company");
// }
