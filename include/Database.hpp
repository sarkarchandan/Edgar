#ifndef DATABASE_H
#define DATABASE_H
#include "Container.hpp"
#include <unordered_map>
#include <string>

namespace database
{
  struct Database
  {
    #pragma mark Private member types
    private:
    std::string m_database_name;
    std::unique_ptr<std::unordered_map<std::string,database::Container>> m_containers;
    friend class DatabaseEngine;

    #pragma mark Private initializers and operators
    private:
    Database(const std::string name):m_database_name(name){}
    
    public:
    Database(){}
    ~Database(){ m_containers.release(); }
    Database(const database::Database& database)
    {
      if(m_containers != nullptr) m_containers.release();
      this -> m_database_name = database.m_database_name;
      std::for_each(database.m_containers -> begin(), database.m_containers -> end(),[&](auto pair) {
        m_containers -> insert(pair);
      });
    }
    Database& operator =(const Database& database)
    {
      if(m_containers != nullptr) m_containers.release();
      m_database_name = database.m_database_name;
      std::for_each(database.m_containers -> begin(),database.m_containers -> end(),[&](auto pair){
        m_containers -> insert(pair);
      });
      return *this;
    }
  };
}

#endif //DATABASE_H