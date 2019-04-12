#include "Query.hpp"
#include <regex>

database::TransactionType _Transaction_Filter(const std::string& expression)
{
  std::regex tran_create_database("^CREATE DATABASE.",std::regex_constants::icase);
  std::regex tran_create_container("^CREATE CONTAINER.",std::regex_constants::icase);
  std::regex tran_insert_into("^INSERT INTO.",std::regex_constants::icase);
  std::regex tran_select("^SELECT.",std::regex_constants::icase);
  std::regex tran_update("^UPDATE.",std::regex_constants::icase);
  std::regex tran_truncate("^TRUNCATE CONTAINER.",std::regex_constants::icase);
  std::regex tran_alter("^ALTER CONTAINER.",std::regex_constants::icase);
  std::regex tran_delete("^DELETE FROM.",std::regex_constants::icase);
  std::regex tran_drop_container("^DROP CONTAINER.",std::regex_constants::icase);
  std::regex tran_drop_database("^DROP DATABASE.",std::regex_constants::icase);

  if(std::regex_search(expression,tran_create_database)) return database::TransactionType::create_database;
  else if(std::regex_search(expression,tran_create_container)) return database::TransactionType::create_container;
  else if(std::regex_search(expression,tran_insert_into)) return database::TransactionType::insert_into;
  else if(std::regex_search(expression,tran_select)) return database::TransactionType::select;
  else if(std::regex_search(expression,tran_update)) return database::TransactionType::update;
  else if(std::regex_search(expression,tran_truncate)) return database::TransactionType::truncate;
  else if(std::regex_search(expression,tran_alter)) return database::TransactionType::alter;
  else if(std::regex_search(expression,tran_delete)) return database::TransactionType::delete_from;
  else if(std::regex_search(expression,tran_drop_container)) return database::TransactionType::drop_container;
  else if(std::regex_search(expression,tran_drop_database)) return database::TransactionType::drop_database;
  else
    throw std::runtime_error("Undefined transaction attempted");
}

void database::Query::_ParseQueryString()
{
  m_transaction_type = _Transaction_Filter(m_query_string);
}
