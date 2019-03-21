#ifndef TRANSACTION_FACTORY_H
#define TRANSACTION_FACTORY_H
#include "Table.hpp"

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
    static database::Table Create(const std::string& table_name, const std::string* columns, const std::size_t& number_of_columns);
    static void Insert(database::Table& table,const std::unordered_map<std::string,std::string>& values);
    static std::unordered_map<std::string,std::vector<std::string>> Read(const database::Table& table, const std::unordered_map<std::string,std::string>& filter);
  };

  database::Table TransactionFactory::Create(const std::string& table_name, const std::string* columns, const std::size_t& number_of_columns)
  {
    database::Table table = { table_name, number_of_columns};
    for(std::size_t i = 0; i < number_of_columns; i += 1)
      *(table.m_column_names + i) = *(columns + i);
    std::cout << "Here" << "\n";
    return table;
  }

  void TransactionFactory::Insert(database::Table& table,const std::unordered_map<std::string,std::string>& values)
  {
    table._Insert(values);
  }
  
  std::unordered_map<std::string,std::vector<std::string>> TransactionFactory::Read(const database::Table& table, const std::unordered_map<std::string,std::string>& filter)
  {
    return table._Read(filter);
  }
}

#endif //TRANSACTION_FACTORY_H