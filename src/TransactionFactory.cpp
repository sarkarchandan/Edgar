#include "TransactionFactory.hpp"

database::Container database::TransactionFactory::ContainerWith_Name_Schema(const std::string& container_name, const std::map<std::string,database::DataType>& container_schema)
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

void database::TransactionFactory::SelectRawWithCriteriaFrom(const database::Container& container,const std::map<std::string,std::vector<database::ComparableString>>& filter_criteria,const std::map<std::string,std::vector<database::ComparisonType>>& filter_comparison_params,const std::map<std::string,std::vector<database::AssociationType>>& filter_association_params,const std::vector<std::string>& dataset,const std::function<void(const std::map<std::string,std::vector<database::ComparableString>>&)>& lambda)
{
  container._SelectRawWithCriteria(filter_criteria,filter_comparison_params,filter_association_params,dataset,lambda);
}