#ifndef TRANSACTION_FACTORY_H
#define TRANSACTION_FACTORY_H
#include "Container.hpp"

namespace database
{
  class TransactionFactory
  {
    #pragma mark Public initializers
    public:
    TransactionFactory(){}
    ~TransactionFactory(){}

    #pragma mark Public factory methods
    public:
    //Container factory methods
    static database::Container ContainerWith_Name_Schema(const std::string& container_name, const std::map<std::string,std::string>& container_schema);
  };

  database::Container TransactionFactory::ContainerWith_Name_Schema(const std::string& container_name, const std::map<std::string,std::string>& container_schema)
  {
    database::Container container = {container_name,container_schema};
    return container;
  }
  
}

#endif //TRANSACTION_FACTORY_H