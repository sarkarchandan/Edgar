#include "TransactionFactory.hpp"

database::Container database::TransactionFactory::ContainerWith_Name_Schema(const std::string& container_name, const std::map<std::string,std::string>& container_schema)
{
  database::Container container = {container_name,container_schema};
  return container;
}

void database::TransactionFactory::InsertInto(database::Container& container, const std::map<std::string,database::ComparableString>& values)
{
  container._InsertInto(values);
}

std::map<std::string,std::vector<database::ComparableString>> database::TransactionFactory::SelectAllFrom(const database::Container& container)
{
  std::map<std::string,std::vector<database::ComparableString>> result = container._SelectAll();
  return result;
}

// std::map<std::string,std::vector<database::ComparableString>> database::TransactionFactory::SelectAllWithCriteriaFrom(const database::Container& container,const std::map<std::string,database::ComparableString>& filter_criteria,const std::vector<database::ValueComparisonType>& filter_comparison_types)
// {
//   std::map<std::string,std::vector<database::ComparableString>> result = container._SelectAllWithCriteria(filter_criteria,filter_comparison_types);
//   return result;
// }