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

void database::TransactionFactory::SelectAllFrom(const database::Container& container,const std::function<void(const std::map<std::string,std::vector<database::ComparableString>>&)>& lambda)
{
  container._SelectAll(lambda);
}

void database::TransactionFactory::SelectWithCriteriaFrom(const database::Container& container,const std::map<std::string,database::ComparableString>& filter_criteria,const std::vector<database::ValueComparisonType>& filter_comparison_types,const std::vector<std::string>& required_columns,const std::function<void(const std::map<std::string,std::vector<database::ComparableString>>&)>& lambda)
{
  container._SelectWithCriteria(filter_criteria,filter_comparison_types,required_columns,lambda);
}