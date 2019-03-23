#include "gtest/gtest.h"
#include "TransactionFactory.hpp"

TEST(ContainerTests,canInitializeContainer)
{
  std::map<std::string,std::string> schema = {{"p_id","std::size_t"},{"p_name","std::string"},{"p_country","std::strng"},{"p_date_added","std::string"}};
  database::Container container = database::TransactionFactory::ContainerWith_Name_Schema("Person",schema);
  std::hash<std::string> s_hash;
  std::size_t expected_container_id = s_hash("Person");
  ASSERT_TRUE(container.name() == "Person");
  ASSERT_TRUE(container.id() == expected_container_id);
  ASSERT_TRUE(container.schema() == schema);
}
