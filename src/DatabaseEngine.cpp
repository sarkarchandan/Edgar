#include "DatabaseEngine.hpp"
#include "TransactionFactory.hpp"

#pragma mark Implementation for data definition
void database::DatabaseEngine::CreateDatabase(const std::string& database_name,const std::function<void(bool)>& completion)
{
  if(m_databases -> find(database_name) != m_databases -> end()) completion(false);
  else
  {
    database::Database database = database_name;
    m_databases -> operator[](database_name) = database;
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
  if(m_databases -> find(database_name) != m_databases -> end())
  {
    database::Database database = m_databases -> operator[](database_name);
    if(database.m_containers -> find(container_name) != database.m_containers -> end()) completion(false);
    else
    {
      auto schema = PrepareSchema(container_schema);
      database::Container container = database::TransactionFactory::ContainerWith_Name_Schema(container_name,schema);
      m_databases -> operator[](database_name).m_containers -> operator[](container_name) = container;
      completion(true);
    }
  }
  else
  {
    CreateDatabase(database_name,[&](auto database_created){
      if(database_created)
      {
        auto schema = PrepareSchema(container_schema);
        database::Container container = database::TransactionFactory::ContainerWith_Name_Schema(container_name,schema);
        m_databases -> operator[](database_name).m_containers -> operator[](container_name) = container;
        completion(true);
      }
      else completion(false);
    });
  }
}

void database::DatabaseEngine::InsertIntoContainer(const std::string& database_name, const std::string& container_name, const std::map<std::string,std::string>& values,const std::function<void(bool)>& completion)
{
  if(m_databases -> find(database_name) != m_databases -> end())
  {
    database::Database database = m_databases -> operator[](database_name);
    if(database.m_containers -> find(container_name) != database.m_containers -> end())
    {
      std::map<std::string,database::ComparableString> comparable_keyvalues;
      std::for_each(values.begin(),values.end(),[&](auto pair){ comparable_keyvalues[pair.first] = pair.second; });
      database::TransactionFactory::InsertInto(database.m_containers -> operator[](container_name),comparable_keyvalues);
      completion(true);
    }
    else completion(false);
  }
  else completion(false);
}

void database::DatabaseEngine::SelectAllFromContainer(const std::string& database_name,const std::string& container_name,const std::function<void(const std::map<std::string,std::vector<std::string>>&)>& result)
{
  if(m_databases -> find(database_name) != m_databases -> end())
  {
    database::Database database = m_databases -> operator[](database_name);
    if(database.m_containers -> find(container_name) != database.m_containers -> end())
    {
      std::map<std::string,std::vector<std::string>> query_result;
      database::TransactionFactory::SelectAllFrom(database.m_containers -> operator[](container_name),[&](auto result){
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

// #pragma mark Implementation for data manipulation
// void database::DatabaseEngine::ExecuteForDataDefintion(const database::Query& query,const std::function<void(bool)>& completion)
// {
//   switch (query.transactionType())
//   {
//     case database::create_database:
//       break;
//     case database::create_container:
//       break;
//     case database::
  
//     default:
//       break;
//   }
// }

// void database::DatabaseEngine::ExecuteForDataManipulation(const database::Query& query,const std::function<void(const std::map<std::string,std::vector<std::string>>&)>& result)
// {

// }