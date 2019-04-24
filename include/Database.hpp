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
    std::unordered_map<std::string,database::Container> m_containers;
    friend class DatabaseEngine;

    #pragma mark Private initializers and operators
    private:
    Database(const std::string name):m_database_name(name){}

    public:
    Database(){}
    ~Database(){}
  };
}

#endif //DATABASE_H