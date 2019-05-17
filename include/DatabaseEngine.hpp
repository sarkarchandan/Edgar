#ifndef DATABASE_ENGINE_H
#define DATABASE_ENGINE_H
#include "Database.hpp"
#include "Query.hpp"
#include <functional>
#include "Aliases.hpp"

namespace database
{
  class DatabaseEngine
  {
    #pragma mark Private member properties
    private:
    std::unordered_map<std::string,database::Database> m_databases;

    #pragma mark Public initializers
    public:
    DatabaseEngine(){}
    ~DatabaseEngine(){}

    #pragma mark Private implementation layer
    private:
    /*
     ! Creates a database with a name
     * @param name of the database
     * @param lambda function indicating whether the operation is successful
    */
    void CreateDatabase(
      const std::string& database_name,
      const std::function<void(bool)>& completion);

    /*
     ! Creates a container with name in a specified database
     * @param name of the database
     * @param name of the container
     * @param schema for the container
     * @param lambda function indicating whether the operation is successful 
    */
    void CreateContainer(
      const std::string& database_name, 
      const std::string& container_name, 
      const database::api_schema_type& container_schema,
      const std::function<void(bool)>& completion);

    /*
     ! Inserts data into container
     * @param name of the database
     * @param name of the container
     * @param values to be inserted
     * @param lambda function where result of the operation is expected
    */
    void InsertIntoContainer(
      const std::string& database_name, 
      const std::string& container_name, 
      const database::api_insert_update_type& values,
      const std::function<void(const database::api_dataset_type&)>& result);

    /*
     ! Selects all from a given container
     * @param name of the database
     * @param name of the container
     * @param lambda function where result of the operation is expected
    */
    void SelectAllFromContainer(
      const std::string& database_name,
      const std::string& container_name,
      const std::function<void(const database::api_dataset_type&)>& result);

    /*
     ! Selects specified dataset from the container
     * @param name of the database
     * @param name of the container
     * @param column names for which datset is required
     * @param lambda function where result of the operation is expected
    */
    void SelectRawDataSetFromContainer(
      const std::string& database_name,
      const std::string& container_name,
      const std::vector<std::string>& data_set,
      const std::function<void(const database::api_dataset_type&)>& result);

    /*
     ! Selects specified dataset from the container based on given criteria
     * @param name of the database
     * @param name of the container
     * @param key_values which should be regarded for comparison of data for filtering
     * @param column names for which datset is required
     * @param lambda function where result of the operation is expected
    */
    void SelectRawDataSetFromContainerWithCriteria(
      const std::string& database_name,
      const std::string& container_name,
      const database::api_filter_type& filter_criteria,
      const std::vector<std::string>& data_set,
      const std::function<void(const database::api_dataset_type&)>& lambda);

    /*
    ! Updates specified dataset in the container based on given criteria
    * @param name of the database
    * @param name of the container
    * @param key_values which should be regarded for comparison of data for filtering
    * @param key-values representing the new values which should be updated
    */
    void UpdateDataSetInContainerWithCriteria(
      const std::string& database_name,
      const std::string& container_name,
      const database::api_filter_type& filter_criteria,
      const database::api_insert_update_type& new_value);

    #pragma mark Public api layer
    public:
    /*
     ! Executes a data definition query
     * @param a data definition query
     * @param lambda function indicating whether the operation is successful
    */
    void ExecuteForDataDefinition(
      const database::Query& query,
      const std::function<void(bool)>& completion);
    
    /*
     ! Executes a data manipulation query
     * @param a data manipulation query
     * @param lambda function where result of the operation is expected
    */
    void ExecuteForDataManipulation(
      const database::Query& query,
      const std::function<void(const database::api_dataset_type&)>& result);
  };
}

#endif //DATABASE_ENGINE_H
