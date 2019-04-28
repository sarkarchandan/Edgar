#ifndef QUERY_H
#define QUERY_H
#include <stdexcept>
#include <vector>
#include <regex>
#include "Aliases.hpp"

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
    database::api_schema_type m_container_schema;

    database::api_insert_update_type m_values;
    std::vector<std::string> m_dataset;
    database::api_filter_type m_filter;

    #pragma mark Public accessors
    public:
    std::string string_value() const { return m_query_string; }
    database::TransactionMetaType transactionMetaType() const { return m_transaction_metatype; }
    database::TransactionType transactionType() const { return m_transaction_type; }
    std::string databaseName() const { return m_database_name; }
    std::string containerName() const { return m_container_name; }
    std::map<std::string,database::QueryDataType> schema() const { return m_container_schema; }
    database::api_insert_update_type values() const { return m_values; }
    std::vector<std::string> dataset() const { return m_dataset; }
    database::api_filter_type filter() const { return m_filter; }

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
