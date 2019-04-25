#include "DatabaseEngine.hpp"
#include "TransactionFactory.hpp"

#pragma mark Implementation layer convenient methods
std::map<std::string,std::vector<database::ComparableString>> _Transform(const std::map<std::string,std::vector<std::string>>& params)
{
  std::map<std::string,std::vector<database::ComparableString>> modified_params;
  std::for_each(params.begin(),params.end(),[&](auto pair) {
    std::vector<database::ComparableString> buffer;
    std::transform(pair.second.begin(),pair.second.end(),std::back_inserter(buffer),[&](auto value) { return value; });
    modified_params[pair.first] = buffer;
  });
  return modified_params;
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

std::map<std::string,database::DataType> PrepareSchema(const std::map<std::string,database::QueryDataType>& raw_schema)
{
  std::map<std::string,database::DataType> schema;
  std::for_each(raw_schema.begin(),raw_schema.end(),[&](auto pair) {
    switch (pair.second)
    {
      case database::integer: schema[pair.first] = database::DataType::quantifiable; break;
      case database::string: schema[pair.first] = database::DataType::non_quantifiable; break;
      case database::boolean: schema[pair.first] = database::DataType::non_quantifiable; break;
      default: break;
    }
  });
  return schema;
}

void database::DatabaseEngine::CreateContainer(const std::string& database_name, const std::string& container_name, const std::map<std::string,database::QueryDataType>& container_schema,const std::function<void(bool)>& completion)
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

void database::DatabaseEngine::InsertIntoContainer(const std::string& database_name, const std::string& container_name, const std::map<std::string,std::string>& values,const std::function<void(const std::map<std::string,std::vector<std::string>>&)>& result)
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

void database::DatabaseEngine::SelectAllFromContainer(const std::string& database_name,const std::string& container_name,const std::function<void(const std::map<std::string,std::vector<std::string>>&)>& result)
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

void database::DatabaseEngine::SelectRawDataSetFromContainer(const std::string& database_name,const std::string& container_name,const std::vector<std::string>& data_set,const std::function<void(const std::map<std::string,std::vector<std::string>>&)>& result)
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

void database::DatabaseEngine::SelectRawDataSetFromContainerWithCriteria(const std::string& database_name,const std::string& container_name,const std::map<std::string,std::vector<std::string>>& filter_criteria,const std::map<std::string,std::vector<database::ComparisonType>>& filter_comparison_params,const std::vector<std::string>& data_set,const std::function<void(const std::map<std::string,std::vector<std::string>>&)>& result)
{
  if(m_databases.find(database_name) != m_databases.end())
  {
    if(m_databases[database_name].m_containers.find(container_name) != m_databases[database_name].m_containers.end())
    {
      std::map<std::string,std::vector<std::string>> query_result;
      database::TransactionFactory::SelectRawDataSetWithCriteriaFrom(m_databases[database_name].m_containers[container_name],_Transform(filter_criteria),filter_comparison_params,data_set,[&](auto result){
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
      CreateContainer(query.databaseName(),query.containerName(),query.containerSchema(),completion);
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

void database::DatabaseEngine::ExecuteForDataManipulation(const database::Query& query,const std::function<void(const std::map<std::string,std::vector<std::string>>&)>& result)
{
  if(query.transactionMetaType() != database::dml)
    throw std::runtime_error("Inappropriate query attempted. DML queries expected");
  switch (query.transactionType())
  {
    case database::insert_into:
      InsertIntoContainer(query.databaseName(),query.containerName(),query.insertDataset(),result);
      break;
    case database::select_all:
      SelectAllFromContainer(query.databaseName(),query.containerName(),result);
      break;
    case database::select_dataset:
      if(query.selectConditions().empty())
        SelectRawDataSetFromContainer(query.databaseName(),query.containerName(),query.selectDataset(),result);
      else
        exit(1);
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
