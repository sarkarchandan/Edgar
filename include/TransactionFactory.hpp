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

    /*
     ! Creates a database container
     * @param name of the container
     * @param schema for the container
    */
    static database::Container ContainerWith_Name_Schema(
      const std::string& container_name, 
      const std::map<std::string,DataType>& container_schema);

    /*
     ! Inserts data into the container
     * @param container
     * @param values to be inserted (can be compared as the set of key-value mapping between the column name and the data)
    */
    static void InsertInto(
      database::Container& container, 
      const database::impl_insert_update_type& values);

    /*
     ! Selects all data from the container
     * @param container
     * @param lambda function where the result is expected
    */
    static void SelectAllFrom(
      const database::Container& container,
      const std::function<void(const database::impl_dataset_type&)>& lambda);

    /*
     ! Selects chosen dataset from the container
     * @param container
     * @param required columns
     * @param lambda function where the result is expected
    */
    static void SelectRawDataSetFrom(
      const database::Container& container, 
      const std::vector<std::string>& dataset,
      const std::function<void(const database::impl_dataset_type&)>& lambda);

    /*
     ! Selects data from the container based based on some criteria
     * @param container
     * @param key_values which should be regarded for comparison of data for filtering
     * @param columns for which data should be selected
     * @param lambda function where the result is expected
    */
    static void SelectRawDataSetWithCriteriaFrom(
      const database::Container& container,
      const database::impl_filter_type& filter_criteria,
      const std::vector<std::string>& dataset,
      const std::function<void(const database::impl_dataset_type&)>& lambda);
      
    /*
     ! Updates data in the container based on the passed in criteria
     * @param container
     * @param key_values which should be regarded for comparison of data for filtering
     * @param key-values representing the new values which should be updated
    */
    static void Update(
      database::Container& container,
      const database::impl_filter_type& filter_criteria,
      const database::impl_insert_update_type& new_value);

    /*
     ! Deletes data from the container based on the passed in criteria
     * @param container
     * @param key_values which should be regarded for comparison of data for filtering
    */
    static void DeleteFrom(
      database::Container& container,
      const database::impl_filter_type& filter_criteria);

    /*
     ! Deletes all data from the container keeping the schema intact
     * @param container
    */
    static void Truncate(database::Container& container);

  };
}

#endif //TRANSACTION_FACTORY_H
