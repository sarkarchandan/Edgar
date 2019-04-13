#ifndef DATABASE_ENGINE_H
#define DATABASE_ENGINE_H
#include "Database.hpp"
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
    DatabaseEngine(){}
    ~DatabaseEngine(){ m_databases.release(); }
    DatabaseEngine(const DatabaseEngine& engine)
    {
      if(m_databases != nullptr) m_databases.release();
      std::for_each(engine.m_databases -> begin(), engine.m_databases -> end(),[&](auto pair){
        m_databases -> insert(pair);
      });
    }
    DatabaseEngine& operator =(const DatabaseEngine& engine)
    {
      if(m_databases != nullptr) m_databases.release();
      std::for_each(engine.m_databases -> begin(),engine.m_databases -> end(),[&](auto pair){
        m_databases -> insert(pair);
      });
    }

    #pragma mark Public api layer
    public:
    void CreateDatabase(const std::string& database_name,const std::function<void(bool)>& completion);
    void CreateContainer(const std::string& database_name, const std::string& container_name, const std::map<std::string,database::QueryDataType>& container_schema,const std::function<void(bool)>& completion);
    void InsertIntoContainer(const std::string& database_name, const std::string& container_name, const std::map<std::string,std::string>& values,const std::function<void(bool)>& completion);
  };
}

#endif //DATABASE_ENGINE_H