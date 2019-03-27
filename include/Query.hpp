#ifndef QUERY_H
#define QUERY_H

#include <stdexcept>
#include <vector>
#include <map>
#include <regex>
#include "ComparableString.hpp"


namespace database
{
  struct Query
  {
    #pragma mark Private typed and member properties
    private:
    std::string m_query_string;
    database::TransactionType m_transaction_type;
    std::string m_database_name;
    std::string m_table_name;
    std::vector<std::string> m_column_names;
    std::map<std::string,std::vector<database::ComparableString>> m_filters;
    
    #pragma mark Public initializers
    public:
    Query(const std::string& query_string)
    :m_query_string(query_string) { _ParseQueryString(m_query_string); }
    ~Query() {}
    
    #pragma mark Private implementation layer
    private:
    void _ParseQueryString(const std::string& query_string);//Uses regex
    
    #pragma mark Public api layer
    public:
  };
}

#endif //QUERY_H