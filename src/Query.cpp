#include "Query.hpp"
#include <regex>
#include <functional>

std::string _Get_Transaction_Detail(const std::string& expression,const std::regex& regex)
{
  std::smatch smatch;
  if(!std::regex_search(expression,smatch,regex))
    throw std::runtime_error("Invalid regular expression provided for extracting transaction parameters");
  std::string transactionParameters = smatch.suffix().str();
  return transactionParameters;
}

void _Tran_CreateContainer_Root_Filter(const std::string& expression,const std::function<void(const std::string& databaseName,const std::string& containerName, const std::string& schemaDetail)>& lambda)
{
  //e.g. company.employee(employee_id int,employee_name string,employee_status string)
  std::regex regex("^([[:w:]]+).([[:w:]]+)+",std::regex_constants::icase);
  std::smatch smatch;
  if(!std::regex_search(expression,smatch,regex))
    throw std::runtime_error("Invalid regular expression provided for extracting transaction parameters");
  std::string databaseName = smatch[1].str();
  std::string containerName = smatch[2].str();
  std::string schemaDetail = smatch.suffix().str();
  lambda(databaseName,containerName,schemaDetail);
}

void _Tran_CreateContainer_Schema_Filter(const std::string& expression,const std::function<void(const std::map<std::string,std::string>&)>& lambda)
{
  //e.g. (employee_id int,employee_name string,employee_status string)
  std::regex regex("([[:w:]]+) ([[:w:]]+)",std::regex_constants::icase);
  std::smatch smatch;
  if(!std::regex_search(expression,smatch,regex))
    throw std::runtime_error("Invalid regular expression provided for extracting transaction parameters");
  std::sregex_iterator pos = {expression.cbegin(),expression.cend(),regex};
  std::sregex_iterator end;
  std::map<std::string,std::string> containerSchema;
  for(;pos != end; ++pos)
    containerSchema[pos -> str(1)] = pos -> str(2);
  lambda(containerSchema);
}

void _Tran_InsertInto_Root_Filter(const std::string& expression,const std::function<void(const std::string& databaseName,const std::string& containerName, const std::string& dataset)>& lambda)
{
  //company.employee values(employee_id:1,employee_name:chandan,employee_status:full_time)
  std::regex regex("^([[:w:]]+).([[:w:]]+) values+",std::regex_constants::icase);
  std::smatch smatch;
  if(!std::regex_search(expression,smatch,regex))
    throw std::runtime_error("Invalid regular expression provided for extracting transaction parameters");
  std::string databaseName = smatch[1].str();
  std::string containerName = smatch[2].str();
  std::string dataset = smatch.suffix().str();
  lambda(databaseName,containerName,dataset);
}

void _Tran_InsertInto_Data_Filter(const std::string& expression,const std::function<void(const std::map<std::string,std::string>&)>& lambda)
{
  //e.g. (employee_id:1,employee_name:chandan,employee_status:full_time)
  std::regex regex("([[:w:]]+):([[:w:]]+)",std::regex_constants::icase);
  std::smatch smatch;
  if(!std::regex_search(expression,smatch,regex))
    throw std::runtime_error("Invalid regular expression provided for extracting transaction parameters");
  std::sregex_iterator pos = {expression.cbegin(),expression.cend(),regex};
  std::sregex_iterator end;
  std::map<std::string,std::string> containerSchema;
  for(;pos != end; ++pos)
    containerSchema[pos -> str(1)] = pos -> str(2);
  lambda(containerSchema);
}

std::pair<database::TransactionType,std::string> _Transaction_Filter(const std::string& expression)
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
  std::pair<database::TransactionType,std::string> first_order_filter_result = _Transaction_Filter(m_query_string);
  m_transaction_type = first_order_filter_result.first;

  //Create Database
  if(m_transaction_type == database::TransactionType::create_database)
    m_database_name = first_order_filter_result.second;
  //Create Container
  else if(m_transaction_type == database::TransactionType::create_container)
  {
    std::string databaseName;
    std::string containerName;
    std::string schemaDetail;
    _Tran_CreateContainer_Root_Filter(first_order_filter_result.second,[&](auto _databaseName, auto _containerName, auto _schemaDetail){
      databaseName = _databaseName;
      containerName = _containerName;
      schemaDetail = _schemaDetail;
    });
    m_database_name = databaseName;
    m_container_name = containerName;
    std::map<std::string,std::string> containerSchema;
    _Tran_CreateContainer_Schema_Filter(schemaDetail,[&](auto schema){
      containerSchema = schema;
    });
    m_container_schema = containerSchema;
  }
  else if(m_transaction_type == database::TransactionType::insert_into)
  {
    std::string databaseName;
    std::string containerName;
    std::string dataset;
    _Tran_InsertInto_Root_Filter(first_order_filter_result.second,[&](auto _databaseName, auto _containerName, auto _dataset){
      databaseName = _databaseName;
      containerName = _containerName;
      dataset = _dataset;
    });
    m_database_name = databaseName;
    m_container_name = containerName;
    std::map<std::string,std::string> insertDataSet;
    _Tran_InsertInto_Data_Filter(dataset,[&](auto _dataset){
      insertDataSet = _dataset;
    });
    m_insert_dataset = insertDataSet;
  }
}
