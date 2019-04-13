#include "DatabaseEngine.hpp"
#include "TransactionFactory.hpp"

bool database::DatabaseEngine::CreateDatabase(const std::string& database_name)
{
  if(m_databases -> find(database_name) != m_databases -> end()) return false;
  else
  {
    database::Database database = database_name;
    m_databases -> operator[](database_name) = database;
    return true;
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

bool database::DatabaseEngine::CreateContainer(const std::string& database_name, const std::string& container_name, const std::map<std::string,database::QueryDataType>& container_schema)
{
  if(m_databases -> find(database_name) != m_databases -> end())
  {
    database::Database database = m_databases -> operator[](database_name);
    if(database.m_containers -> find(container_name) != database.m_containers -> end()) return false;
    else
    {
      auto schema = PrepareSchema(container_schema);
      database::Container container = database::TransactionFactory::ContainerWith_Name_Schema(container_name,schema);
      m_databases -> operator[](database_name).m_containers -> operator[](container_name) = container;
      return true;
    }
  }
  else
  {
    if(CreateDatabase(database_name))
    {
      auto schema = PrepareSchema(container_schema);
      database::Container container = database::TransactionFactory::ContainerWith_Name_Schema(container_name,schema);
      m_databases -> operator[](database_name).m_containers -> operator[](container_name) = container;
      return true;
    }
    else return false;
  }
  return true;
}

bool database::DatabaseEngine::InsertIntoContainer(const std::string& database_name, const std::string& container_name, const std::map<std::string,std::string>& values)
{
  return true;
}