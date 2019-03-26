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
    
    /**
     * Creates a database container
     * @param name of the container
     * @param schema for the container
    */
    static database::Container ContainerWith_Name_Schema(const std::string& container_name, const std::map<std::string,DataType>& container_schema);

    /**
     * Inserts data into the container
     * @param container
     * @param values to be inserted (can be compared as the set of key-value mapping between the column name and the data)
    */
    static void InsertInto(database::Container& container, const std::map<std::string,database::ComparableString>& values);

    /**
     * Selects all data from the container
     * @param container
     * @param lambda function where the result is expected
    */
    static void SelectAllFrom(const database::Container& container,const std::function<void(const std::map<std::string,std::vector<database::ComparableString>>&)>& lambda);
    /**
     * Selects data from the container based based on some criteria
     * @param container
     * @param key-values which should be consideried for filtering of data
     * @param key_values which should be regarded for comparison of data for filtering
     * @param columns for which data should be selected
     * @param lambda function where the result is expected
    */
    static void SelectRawWithCriteriaFrom(const database::Container& container,const std::map<std::string,std::vector<database::ComparableString>>& filter_criteria,const std::map<std::string,std::vector<database::ComparisonType>>& filter_comparison_params,const std::vector<std::string>& dataset,const std::function<void(const std::map<std::string,std::vector<database::ComparableString>>&)>& lambda);

  };
}

#endif //TRANSACTION_FACTORY_H
