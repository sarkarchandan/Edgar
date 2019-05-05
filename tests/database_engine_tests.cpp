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
    std::cout << "Result set for inserted record: " << "\n" << query_result << "\n";
    ASSERT_TRUE(query_result == expected_result);
  });
}

TEST(DatabaseEngineTests,canSelectAllDataFromContainer)
{
  database::DatabaseEngine database_engine;
  database::Query query_create_database = "create database company";
  database::Query query_create_container = "create container company.employee(employee_id integer,employee_name string,employee_status string)";
  std::vector<database::Query> insert_queries = {
    "insert into company.employee values(employee_id:1,employee_name:Dominik_Schmidt,employee_status:fulltime)",
    "insert into company.employee values(employee_id:2,employee_name:Mathias_Heinrich,employee_status:parttime)",
    "insert into company.employee values(employee_id:3,employee_name:Andreas_Renner,employee_status:parttime)",
    "insert into company.employee values(employee_id:4,employee_name:Marcus_Kaiser,employee_status:fulltime)"
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
      {"employee_name",{"Dominik_Schmidt","Mathias_Heinrich","Andreas_Renner","Marcus_Kaiser"}},
      {"employee_status",{"fulltime","parttime","parttime","fulltime"}}
    };
    std::cout << "Result Set for all records: " << "\n" << query_result << "\n";
    ASSERT_TRUE(query_result == expected_result);
  });
}

TEST(DatabaseEngineTests,canSelectDatasetFromContainer)
{
  database::DatabaseEngine database_engine;
  database::Query query_create_database = "create database company";
  database::Query query_create_container = "create container company.employee(employee_id integer,employee_name string,employee_status string)";
  std::vector<database::Query> insert_queries = {
    "insert into company.employee values(employee_id:1,employee_name:Dominik_Schimdt,employee_status:fulltime)",
    "insert into company.employee values(employee_id:2,employee_name:Heinrich_Keil,employee_status:parttime)",
    "insert into company.employee values(employee_id:3,employee_name:Mathias_Rheinlein,employee_status:parttime)",
    "insert into company.employee values(employee_id:4,employee_name:Marcus_Eisermann,employee_status:fulltime)"
  };

  database_engine.ExecuteForDataDefinition(query_create_database,[&](auto database_created){
    ASSERT_TRUE(database_created);
  });
  database_engine.ExecuteForDataDefinition(query_create_container,[&](auto container_created){
    ASSERT_TRUE(container_created);
  });
  std::for_each(insert_queries.begin(),insert_queries.end(),[&](auto query) {
    database_engine.ExecuteForDataManipulation(query,[&](auto query_result) {
      ASSERT_FALSE(query_result.empty());
    });
  });

  database::Query query_select_dataset1 = "select employee_id,employee_name from company.employee";
  database_engine.ExecuteForDataManipulation(query_select_dataset1,[&](auto query_result) {
    // std::cout <<  "Query: " << query_select_dataset1 << "\n";
    std::map<std::string,std::vector<std::string>> expected_result = {
      {"employee_id",{"1","2","3","4"}},
      {"employee_name",{"Dominik_Schimdt","Heinrich_Keil","Mathias_Rheinlein","Marcus_Eisermann"}}
    };
    std::cout << "Result Set for selected dataset without criteria: " << "\n" << query_result << "\n";
    ASSERT_TRUE(query_result == expected_result);
  });

  database::Query query_select_dataset2 = "select employee_id from company.employee";
  database_engine.ExecuteForDataManipulation(query_select_dataset2,[&](auto query_result) {
    std::map<std::string,std::vector<std::string>> expected_result = {
      {"employee_id",{"1","2","3","4"}}
    };
    std::cout << "Result Set for selected dataset without criteria: " << "\n" << query_result << "\n";
    ASSERT_TRUE(query_result == expected_result);
  });
}


TEST(DatabaseEngineTests,canSelectDatasetFromContainerWithCriteria)
{
  database::Query query_create_database = "create database university";
  database::Query query_create_container = "create container university.student (student_id integer,student_name string,student_dob string,student_gender string";
  std::vector<database::Query> insert_queries = {
    "insert into university.student values(student_id:1,student_name:Markus_Eisermann,student_dob:19920312,student_gender:male)",
    "insert into university.student values(student_id:2,student_name:Ulrike_Von_Stryk,student_dob:19871112,student_gender:female)",
    "insert into university.student values(student_id:3,student_name:Dominik_Vlad,student_dob:19900821,student_gender:male)",
    "insert into university.student values(student_id:4,student_name:Mathias_Zeug,student_dob:19840918,student_gender:male)",
    "insert into university.student values(student_id:5,student_name:Kerstin_Peh,student_dob:19930611,student_gender:female)",
    "insert into university.student values(student_id:6,student_name:Tanja_Schimdt,student_dob:19951201,student_gender:female)",
    "insert into university.student values(student_id:7,student_name:Anna_Graser,student_dob:19890401,student_gender:female)",
    "insert into university.student values(student_id:8,student_name:Martin_Kaiser,student_dob:19840310,student_gender:male)",
  };
  database::DatabaseEngine database_engine;
  database_engine.ExecuteForDataDefinition(query_create_database,[&](auto database_created) {
    ASSERT_TRUE(database_created);
  });
  database_engine.ExecuteForDataDefinition(query_create_container,[&](auto container_created) {
    ASSERT_TRUE(container_created);
  });
  std::for_each(insert_queries.begin(),insert_queries.end(),[&](auto query) {
    database_engine.ExecuteForDataManipulation(query,[&](auto query_result) {
      ASSERT_FALSE(query_result.empty());
    });
  });

  database::Query select_query1 = "select * from university.student where student_gender = female";
  database::Query select_query2 = "select student_id,student_name from university.student where student_dob > 19840310 and student_dob < 19930611";

  database::api_filter_type filter2 = {
    {"student_dob",{{"19840310",database::greater_than},{"19930611",database::lesser_than}}}
  };
  ASSERT_TRUE(select_query2.transactionType() == database::select_dataset);
  ASSERT_TRUE(select_query2.filter() == filter2);

  database_engine.ExecuteForDataManipulation(select_query1,[&](auto query_result) {
    database::api_dataset_type expected_result = {
      {"student_id",{"2","5","6","7"}},
      {"student_name",{"Ulrike_Von_Stryk","Kerstin_Peh","Tanja_Schimdt","Anna_Graser"}},
      {"student_dob",{"19871112","19930611","19951201","19890401"}},
      {"student_gender",{"female","female","female","female"}}
    };
    ASSERT_TRUE(expected_result == query_result);
    std::cout << "Result set for all dataset with criteria: " << "\n" << query_result << "\n";
  });

  FAIL() << "Evaluation of more than one criterion - Not Implemented" << "\n";
  // database_engine.ExecuteForDataManipulation(select_query2,[&](auto query_result) {
  //   database::api_dataset_type expected_result = {
  //     {"student_id",{"1","2","3","4","7"}},
  //     {"student_name",{"Markus_Eisermann","Ulrike_Von_Stryk","Dominik_Vlad","Mathias_Zeug","Anna_Graser"}}
  //   };
  //   std::cout << "Result set for specific dataset with criteria: " << "\n" << query_result << "\n";
  //   ASSERT_TRUE(expected_result == query_result);
  //   // std::cout << "Result set for specific dataset with criteria: " << "\n" << query_result << "\n";
  // });
}
