#ifndef QUERY_H
#define QUERY_H

#include <cstdlib>
#include <stdexcept>
#include <iostream>
#include <unordered_map>
#include <regex>

namespace database
{
  struct Query
  {
    #pragma mark Private typed and member properties
    private:
    std::string m_query_string;
    enum TransactionType { CREATE,INSERT,READ,UPDATE,TRUNCATE,ALTER,DELETE };
    TransactionType m_transaction_type;
    std::string m_table_name;
    std::string* m_columns;
    std::unordered_map<std::string,std::string> m_filters;
    
    #pragma mark Public initializers
    public:
    Query(const std::string& query_string)
    :m_query_string(query_string)
    {
      _ParseQueryString(m_query_string);
    }
    ~Query()
    {
      //TODO
    }
    
    #pragma mark Private implementation layer
    private:
    void _ParseQueryString(const std::string& query_string);//Uses regex
    
    #pragma mark Public api layer
    public:
  };
}

#endif //QUERY_H