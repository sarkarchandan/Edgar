#include "gtest/gtest.h"
#include "TransactionFactory.hpp"
#include "Utility.hpp"

TEST(SampleTest, canPerformSampleTest)
{
  std::string columns[] = {"person_id","person_name","person_country"};
  database::Table person = database::TransactionFactory::Create("Person",columns,3);
  std::unordered_map<std::string,std::string> entry1 = {{"person_id","1"},{"person_name","John"},{"person_country","USA"}};
  std::unordered_map<std::string,std::string> entry2 = {{"person_id","2"},{"person_name","James"},{"person_country","DE"}};
  std::unordered_map<std::string,std::string> entry3 = {{"person_id","3"},{"person_name","Ulrike"},{"person_country","USA"}};
  database::TransactionFactory::Insert(person,entry1);
  database::TransactionFactory::Insert(person,entry2);
  database::TransactionFactory::Insert(person,entry3);

  std::unordered_map<std::string,std::vector<std::string>> result = database::TransactionFactory::Read(person,{{"person_country","USA"}});

  ASSERT_EQ(result["person_id"].size(),2);
  ASSERT_EQ(result["person_name"].size(),2);
  ASSERT_EQ(result["person_country"].size(),2);
}

