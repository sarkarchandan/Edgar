#include "DatabaseEngine.hpp"
#include "TransactionFactory.hpp"

#pragma mark Implementation layer convenient methods
database::impl_filter_type Transform(const database::api_filter_type& api_filter_params)
{
  database::impl_filter_type impl_filter_params;
  std::for_each(api_filter_params.begin(),api_filter_params.end(),[&](auto map_level_pair) {
    database::impl_compare_type buffer;
    std::for_each(map_level_pair.second.begin(),map_level_pair.second.end(),[&](auto vector_level_pair) {
      std::pair<database::ComparableString,database::ComparisonType> impl_comp_pair = {vector_level_pair.first,vector_level_pair.second};
      buffer.emplace_back(impl_comp_pair);
    });
    impl_filter_params[map_level_pair.first] = buffer;
  });
  return impl_filter_params;
}

database::impl_schema_type PrepareSchema(const database::api_schema_type& raw_schema)
{
  database::impl_schema_type schema;
  std::for_each(raw_schema.begin(),raw_schema.end(),[&](auto pair) {
    switch (pair.second)
    {
      case database::integer: schema[pair.first] = database::DataType::quantifiable; break;
      case database::string: schema[pair.first] = database::DataType::non_quantifiable; break;
      case database::boolean: schema[pair.first] = database::DataType::non_quantifiable; break;
      case database::date: schema[pair.first] = database::non_quantifiable; break;
      default: break;
    }
  });
  return schema;
}

#pragma mark Implementation for data definition
void database::DatabaseEngine::CreateDatabase(const std::string& database_name,const std::function<void(bool)>& completion)
{
  if(m_databases.find(database_name) != m_databases.end()) completion(false);
  else
  {
    database::Database database = database_name;
    m_databases[database_name] = database;
    completion(true);
  }
}

void database::DatabaseEngine::CreateContainer(const std::string& database_name, const std::string& container_name, const database::api_schema_type& container_schema,const std::function<void(bool)>& completion)
{
  if(m_databases.find(database_name) != m_databases.end())
  {
    if(m_databases[database_name].m_containers.find(container_name) != m_databases[database_name].m_containers.end()) completion(false);
    else
    {
      auto schema = PrepareSchema(container_schema);
      database::Container container = database::TransactionFactory::ContainerWith_Name_Schema(container_name,schema);
      m_databases[database_name].m_containers[container_name] = container;
      completion(true);
    }
  }
  else completion(false);
}

void database::DatabaseEngine::InsertIntoContainer(const std::string& database_name, const std::string& container_name, const database::api_insert_update_type& values,const std::function<void(const database::api_dataset_type&)>& result)
{
  if(m_databases.find(database_name) != m_databases.end())
  {
    if(m_databases[database_name].m_containers.find(container_name) != m_databases[database_name].m_containers.end())
    {
      std::map<std::string,database::ComparableString> comparable_keyvalues;
      std::for_each(values.begin(),values.end(),[&](auto pair){ comparable_keyvalues[pair.first] = pair.second; });
      database::TransactionFactory::InsertInto(m_databases[database_name].m_containers[container_name],comparable_keyvalues);

      std::map<std::string,std::vector<std::string>> query_result;
      std::for_each(values.begin(),values.end(),[&](auto pair){
        query_result[pair.first] = {pair.second};
      });
      result(query_result);
    }
    else result({});
  }
  else result({});
}

void database::DatabaseEngine::SelectAllFromContainer(const std::string& database_name,const std::string& container_name,const std::function<void(const database::api_dataset_type&)>& result)
{
  if(m_databases.find(database_name) != m_databases.end())
  {
    if(m_databases[database_name].m_containers.find(container_name) != m_databases[database_name].m_containers.end())
    {
      std::map<std::string,std::vector<std::string>> query_result;
      database::TransactionFactory::SelectAllFrom(m_databases[database_name].m_containers[container_name],[&](auto result){
        std::for_each(result.begin(),result.end(),[&](auto pair){
          std::vector<std::string>values;
          std::transform(pair.second.begin(),pair.second.end(),std::back_inserter(values),[&](auto value){
            return value.m_string;
          });
          query_result[pair.first] = values;
        });
      });
      result(query_result);
    }
    else result({});
  }
  else result({});
}

void database::DatabaseEngine::SelectRawDataSetFromContainer(const std::string& database_name,const std::string& container_name,const std::vector<std::string>& data_set,const std::function<void(const database::api_dataset_type&)>& result)
{
  if(m_databases.find(database_name) != m_databases.end())
  {
    if(m_databases[database_name].m_containers.find(container_name) != m_databases[database_name].m_containers.end())
    {
      std::map<std::string,std::vector<std::string>> query_result;
      database::TransactionFactory::SelectRawDataSetFrom(m_databases[database_name].m_containers[container_name],data_set,[&](auto result){
        std::for_each(result.begin(),result.end(),[&](auto pair){
          std::vector<std::string>values;
          std::transform(pair.second.begin(),pair.second.end(),std::back_inserter(values),[&](auto value){
            return value.m_string;
          });
          query_result[pair.first] = values;
        });
      });
      result(query_result);
    }
    else result({});
  }
  else result({});
}

void database::DatabaseEngine::SelectRawDataSetFromContainerWithCriteria(const std::string& database_name,const std::string& container_name,const database::api_filter_type& filter_criteria,const std::vector<std::string>& data_set,const std::function<void(const database::api_dataset_type&)>& result)
{
  if(m_databases.find(database_name) != m_databases.end())
  {
    if(m_databases[database_name].m_containers.find(container_name) != m_databases[database_name].m_containers.end())
    {
      std::map<std::string,std::vector<std::string>> query_result;
      database::TransactionFactory::SelectRawDataSetWithCriteriaFrom(m_databases[database_name].m_containers[container_name],Transform(filter_criteria),data_set,[&](auto result){
        std::for_each(result.begin(),result.end(),[&](auto pair){
          std::vector<std::string>values;
          std::transform(pair.second.begin(),pair.second.end(),std::back_inserter(values),[&](auto value){
            return value.m_string;
          });
          query_result[pair.first] = values;
        });
      });
      result(query_result);
    }
    else result({});
  }
  else result({});
}

#pragma mark Implementation for data manipulation
void database::DatabaseEngine::ExecuteForDataDefinition(const database::Query& query,const std::function<void(bool)>& completion)
{
  if(query.transactionMetaType() != database::ddl)
    throw std::runtime_error("Inappropriate query attempted. DDL queries expected");
  switch (query.transactionType())
  {
    case database::create_database:
      CreateDatabase(query.databaseName(),completion);
      break;
    case database::create_container:
      CreateContainer(query.databaseName(),query.containerName(),query.schema(),completion);
      break;
    // case database::alter:
    //   #pragma mark TODO
    //   break;
    case database::drop_container:
      #pragma mark TODO
      break;
    case database::drop_database:
      #pragma mark TODO
      break;
    default:
      break;
  }
}

void database::DatabaseEngine::ExecuteForDataManipulation(const database::Query& query,const std::function<void(const database::api_dataset_type&)>& result)
{
  if(query.transactionMetaType() != database::dml)
    throw std::runtime_error("Inappropriate query attempted. DML queries expected");
  switch (query.transactionType())
  {
    case database::insert_into:
      InsertIntoContainer(query.databaseName(),query.containerName(),query.values(),result);
      break;
    case database::select_all:
      if(query.filter().empty())
        SelectAllFromContainer(query.databaseName(),query.containerName(),result);
      else
        SelectRawDataSetFromContainerWithCriteria(query.databaseName(),query.containerName(),query.filter(),{},result);
      break;
    case database::select_dataset:
      if(query.filter().empty())
        SelectRawDataSetFromContainer(query.databaseName(),query.containerName(),query.dataset(),result);
      else
        SelectRawDataSetFromContainerWithCriteria(query.databaseName(),query.containerName(),query.filter(),query.dataset(),result);
      break;
    case database::update:
      #pragma mark TODO
      break;
    case database::truncate:
      #pragma mark TODO
      break;
    case database::delete_from:
      #pragma mark TODO
      break;
    default:
      break;
  }
}
