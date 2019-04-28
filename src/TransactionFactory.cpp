#include "TransactionFactory.hpp"

database::Container database::TransactionFactory::ContainerWith_Name_Schema(const std::string& container_name, const database::impl_schema_type& container_schema)
{
  database::Container container = {container_name,container_schema};
  return container;
}

void database::TransactionFactory::InsertInto(database::Container& container, const database::impl_insert_update_type& values)
{
  container._InsertInto(values);
}

void database::TransactionFactory::SelectAllFrom(const database::Container& container,const std::function<void(const database::impl_dataset_type&)>& lambda)
{
  container._SelectAll(lambda);
}

void database::TransactionFactory::SelectRawDataSetFrom(const database::Container& container, const std::vector<std::string>& dataset,const std::function<void(const database::impl_dataset_type&)>& lambda)
{
  container._SelectRawDataSet(dataset,lambda);
}

void database::TransactionFactory::SelectRawDataSetWithCriteriaFrom(const database::Container& container,const database::impl_filter_type& filter_criteria,const std::vector<std::string>& dataset,const std::function<void(const database::impl_dataset_type&)>& lambda)
{
  container._SelectRawDataSetWithCriteria(filter_criteria,dataset,lambda);
}

void database::TransactionFactory::Update(database::Container& container,const database::impl_filter_type& filter_criteria,const database::impl_insert_update_type& new_value)
{
  container._Update(filter_criteria,new_value);
}

void database::TransactionFactory::DeleteFrom(database::Container& container,const database::impl_filter_type& filter_criteria)
{
  container._DeleteFrom(filter_criteria);
}

void database::TransactionFactory::Truncate(database::Container& container)
{
  container._Truncate();
}
