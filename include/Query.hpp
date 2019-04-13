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

    #pragma mark Public member properties
    public:
    database::TransactionType m_transaction_type;
    std::string m_database_name;
    std::string m_container_name;
    std::map<std::string,std::string> m_container_schema;
    std::map<std::string,std::string> m_insert_dataset;

    #pragma mark Public initializers and accessors
    public:
    Query(const char* query_string)
    :m_query_string((std::string)query_string) { _ParseQueryString(); }
    ~Query() {}

    #pragma mark Private implementation layer
    private:
    void _ParseQueryString();

    #pragma mark Public api layer
    public:
  };
}

#endif //QUERY_H
