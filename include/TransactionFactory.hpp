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
    static void InsertInto(database::Container& container, const std::map<std::string,database::ComparableString>& values);
    static std::map<std::string,std::vector<database::ComparableString>> SelectAllFrom(const database::Container& container);
    // static std::map<std::string,std::vector<database::ComparableString>> SelectAllWithCriteriaFrom(const database::Container& container,const std::map<std::string,database::ComparableString>& filter_criteria,const std::vector<database::ValueComparisonType>& filter_comparison_types);
    // void Update(database::Container& container,const std::map<std::string,std::string>& values,std::map<std::string,std::string>& where);
    // void DeleteFrom(database::Container& container,const std::map<std::string,std::string>& where);
  };
}

#endif //TRANSACTION_FACTORY_H
