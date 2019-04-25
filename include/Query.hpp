#ifndef QUERY_H
#define QUERY_H
#include <stdexcept>
#include <vector>
#include <map>
#include <regex>
#include "ComparableString.hpp"
#include "Paradigms.hpp"

namespace database
{
  struct Query
  {
    #pragma mark Private member properties
    private:
    std::string m_query_string;
    database::TransactionMetaType m_transaction_metatype;
    database::TransactionType m_transaction_type;
    std::string m_database_name;
    std::string m_container_name;
    std::map<std::string,database::QueryDataType> m_container_schema;
    std::map<std::string,std::string> m_insert_dataset;
    std::vector<std::string> m_select_dataset;
    std::map<std::string,std::string> m_select_conditions;
    std::map<std::string,std::string> m_update_data;
    std::map<std::string,std::string> m_update_conditions;
    std::map<std::string,std::string> m_delete_conditions;

    #pragma mark Public accessors
    public:
    std::string string_value() const { return m_query_string; }
    database::TransactionMetaType transactionMetaType() const { return m_transaction_metatype; }
    database::TransactionType transactionType() const { return m_transaction_type; }
    std::string databaseName() const { return m_database_name; }
    std::string containerName() const { return m_container_name; }
    std::map<std::string,database::QueryDataType> containerSchema() const { return m_container_schema; }
    std::map<std::string,std::string> insertDataset() const { return m_insert_dataset; }
    std::vector<std::string> selectDataset() const { return m_select_dataset; }
    std::map<std::string,std::string> selectConditions() const { return m_select_conditions; }
    std::map<std::string,std::string> updateDataset() const { return m_update_data; }
    std::map<std::string,std::string> updateConditions() const { return m_update_conditions; }
    std::map<std::string,std::string> deleteConditions() const { return m_delete_conditions; }

    #pragma mark Public initializers and accessors
    public:
    Query(const char* query_string)
    :m_query_string((std::string)query_string) { _ParseQueryString(); }
    ~Query() {}

    #pragma mark Private implementation layer
    private:
    void _ParseQueryString();
  };
}

#endif //QUERY_H
