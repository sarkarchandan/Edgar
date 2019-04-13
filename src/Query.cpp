#include "Query.hpp"
#include <regex>

std::string _Get_Transaction_Detail(const std::string& expression,const std::regex& regex)
{
  std::smatch smatch;
  if(!std::regex_search(expression,smatch,regex))
    throw std::runtime_error("Invalid regular expression provided for extracting transaction parameters");
  std::string transactionParameters = smatch.suffix().str();
  return transactionParameters;
}

std::pair<database::TransactionType,std::string> _Transaction_Filter(const std::string& expression)
{
  std::regex tran_create_database("^CREATE DATABASE .",std::regex_constants::icase);
  std::regex tran_create_container("^CREATE CONTAINER .",std::regex_constants::icase);
  std::regex tran_insert_into("^INSERT INTO .",std::regex_constants::icase);
  std::regex tran_select("^SELECT .",std::regex_constants::icase);
  std::regex tran_update("^UPDATE .",std::regex_constants::icase);
  std::regex tran_truncate("^TRUNCATE CONTAINER .",std::regex_constants::icase);
  std::regex tran_alter("^ALTER CONTAINER .",std::regex_constants::icase);
  std::regex tran_delete("^DELETE FROM .",std::regex_constants::icase);
  std::regex tran_drop_container("^DROP CONTAINER .",std::regex_constants::icase);
  std::regex tran_drop_database("^DROP DATABASE .",std::regex_constants::icase);

  if(std::regex_search(expression,tran_create_database)) return std::make_pair<database::TransactionType,std::string>(database::TransactionType::create_database,_Get_Transaction_Detail(expression,tran_create_database));
  else if(std::regex_search(expression,tran_create_container)) return std::make_pair<database::TransactionType,std::string>(database::TransactionType::create_container,_Get_Transaction_Detail(expression,tran_create_container));
  else if(std::regex_search(expression,tran_insert_into)) return std::make_pair<database::TransactionType,std::string>(database::TransactionType::insert_into,_Get_Transaction_Detail(expression,tran_insert_into));
  else if(std::regex_search(expression,tran_select)) return std::make_pair<database::TransactionType,std::string>(database::TransactionType::select,_Get_Transaction_Detail(expression,tran_select));
  else if(std::regex_search(expression,tran_update)) return std::make_pair<database::TransactionType,std::string>(database::TransactionType::update,_Get_Transaction_Detail(expression,tran_update));
  else if(std::regex_search(expression,tran_truncate)) return std::make_pair<database::TransactionType,std::string>(database::TransactionType::truncate,_Get_Transaction_Detail(expression,tran_truncate));
  else if(std::regex_search(expression,tran_alter)) return std::make_pair<database::TransactionType,std::string>(database::TransactionType::alter,_Get_Transaction_Detail(expression,tran_alter));
  else if(std::regex_search(expression,tran_delete)) return std::make_pair<database::TransactionType,std::string>(database::TransactionType::delete_from,_Get_Transaction_Detail(expression,tran_delete));
  else if(std::regex_search(expression,tran_drop_container)) return std::make_pair<database::TransactionType,std::string>(database::TransactionType::drop_container,_Get_Transaction_Detail(expression,tran_drop_container));
  else if(std::regex_search(expression,tran_drop_database)) return std::make_pair<database::TransactionType,std::string>(database::TransactionType::drop_database,_Get_Transaction_Detail(expression,tran_drop_database));
  else
    throw std::runtime_error("Undefined transaction attempted");
}

void database::Query::_ParseQueryString()
{
  m_transaction_type = _Transaction_Filter(m_query_string).first;
}
