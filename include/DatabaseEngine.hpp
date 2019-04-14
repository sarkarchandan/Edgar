#ifndef DATABASE_ENGINE_H
#define DATABASE_ENGINE_H
#include "Database.hpp"
#include "Query.hpp"
#include <functional>

namespace database
{
  class DatabaseEngine
  {
    #pragma mark Private member properties
    private:
    std::unique_ptr<std::unordered_map<std::string,database::Database>> m_databases;

    #pragma mark Public initializers
    public:
    DatabaseEngine(){ m_databases = std::make_unique<std::unordered_map<std::string,database::Database>>(); std::cout << "Engine Init" << "\n"; }
    ~DatabaseEngine(){ m_databases.release(); std::cout << "Engine DeInit" << "\n"; }
    DatabaseEngine(const DatabaseEngine& engine)
    {
      if(m_databases != nullptr) m_databases.release();
      m_databases = std::make_unique<std::unordered_map<std::string,database::Database>>();
      std::for_each(engine.m_databases -> begin(), engine.m_databases -> end(),[&](auto pair){
        m_databases -> insert(pair);
      });
    }
    DatabaseEngine& operator =(const DatabaseEngine& engine)
    {
      if(m_databases != nullptr) m_databases.release();
      m_databases = std::make_unique<std::unordered_map<std::string,database::Database>>();
      std::for_each(engine.m_databases -> begin(),engine.m_databases -> end(),[&](auto pair){
        m_databases -> insert(pair);
      });
      return *this;
    }

    #pragma mark Private implementation layer
    private:
    void CreateDatabase(const std::string& database_name,const std::function<void(bool)>& completion);
    void CreateContainer(const std::string& database_name, const std::string& container_name, const std::map<std::string,database::QueryDataType>& container_schema,const std::function<void(bool)>& completion);
    void InsertIntoContainer(const std::string& database_name, const std::string& container_name, const std::map<std::string,std::string>& values,const std::function<void(const std::map<std::string,std::vector<std::string>>&)>& result);
    void SelectAllFromContainer(const std::string& database_name,const std::string& container_name,const std::function<void(const std::map<std::string,std::vector<std::string>>&)>& result);

    #pragma mark Public api layer
    public:
    void ExecuteForDataDefintion(const database::Query& query,const std::function<void(bool)>& completion);
    void ExecuteForDataManipulation(const database::Query& query,const std::function<void(const std::map<std::string,std::vector<std::string>>&)>& result);
  };
}

#endif //DATABASE_ENGINE_H