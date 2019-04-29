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

void _Process_For_Regex(const std::string& expression,const std::regex& regex,const database::ComparisonType& type,database::api_filter_type& filter)
{
  std::sregex_iterator pos = {expression.cbegin(),expression.cend(),regex};
  std::sregex_iterator end;
  for(;pos != end; ++pos)
  {
    std::pair<std::string,database::ComparisonType> pair = {pos -> str(2),type};
    if(filter.find(pos -> str(1)) != filter.end())
      filter[pos -> str(1)].emplace_back(pair);
    else
      filter[pos -> str(1)] = {pair};  
  }
}

void _FilterParser(const std::string& expression,const std::function<void(const database::api_filter_type&)>& lambda)
{
  //e.g. employee_status = fulltime and employee_joiningdatae >= 20170823 
  std::regex regex_equal_to("([[:w:]]+) = ([[:w:]]+)",std::regex_constants::icase);
  std::regex regex_not_equal_to("([[:w:]]+) <> ([[:w:]]+)",std::regex_constants::icase);
  std::regex regex_greater_or_equal_to("([[:w:]]+) >= ([[:w:]]+)",std::regex_constants::icase);
  std::regex regex_lesser_or_equal_to("([[:w:]]+) <= ([[:w:]]+)",std::regex_constants::icase);
  std::regex regex_greater_than("([[:w:]]+) > ([[:w:]]+)",std::regex_constants::icase);
  std::regex regex_lesser_than("([[:w:]]+) < ([[:w:]]+)",std::regex_constants::icase);

  // using api_compare_type = std::vector<std::pair<std::string,database::ComparisonType>>;
  // using api_filter_type = std::map<std::string,database::api_compare_type>;

  database::api_filter_type filter;
  if(std::regex_search(expression,regex_equal_to))
    _Process_For_Regex(expression,regex_equal_to,database::equal_to,filter);
  
  if(std::regex_search(expression,regex_not_equal_to))
    _Process_For_Regex(expression,regex_not_equal_to,database::not_equal_to,filter);

  if(std::regex_search(expression,regex_greater_or_equal_to))
    _Process_For_Regex(expression,regex_greater_or_equal_to,database::greater_or_equal_to,filter);

  if(std::regex_search(expression,regex_lesser_or_equal_to))
    _Process_For_Regex(expression,regex_lesser_or_equal_to,database::lesser_or_equal_to,filter);

  if(std::regex_search(expression,regex_greater_than))
    _Process_For_Regex(expression,regex_greater_than,database::greater_than,filter);

  if(std::regex_search(expression,regex_lesser_than))
    _Process_For_Regex(expression,regex_lesser_than,database::lesser_than,filter);

  lambda(filter);
}

void _ParseCreateContainerQuery(const std::string& expression,const std::function<void(const std::string& databaseName,const std::string& containerName, const database::api_schema_type& schema)>& lambda)
{
  //e.g. company.employee(employee_id int,employee_name string,employee_status string)
  std::regex root_regex("^([[:w:]]+).([[:w:]]+)+",std::regex_constants::icase);
  std::smatch root_smatch;
  if(!std::regex_search(expression,root_smatch,root_regex))
    throw std::runtime_error("Invalid expression provided for extracting transaction parameters");
  std::string databaseName = root_smatch[1].str();
  std::string containerName = root_smatch[2].str();
  std::string schemaDetail = root_smatch.suffix().str();
  
  //e.g. (employee_id int,employee_name string,employee_status string)
  std::regex schema_regex("([[:w:]]+) ([[:w:]]+)",std::regex_constants::icase);
  std::smatch schema_smatch;
  if(!std::regex_search(expression,schema_smatch,schema_regex))
    throw std::runtime_error("Invalid expression provided for extracting transaction parameters");
  std::sregex_iterator pos = {expression.cbegin(),expression.cend(),schema_regex};
  std::sregex_iterator end;
  std::map<std::string,std::string> containerSchema;
  for(;pos != end; ++pos)
    containerSchema[pos -> str(1)] = pos -> str(2);

  database::api_schema_type refined_schema;
  std::for_each(containerSchema.begin(),containerSchema.end(),[&](auto pair){
    if(pair.second == "integer") refined_schema[pair.first] = database::QueryDataType::integer;
    else if(pair.second == "string") refined_schema[pair.first] = database::QueryDataType::string;
    else if(pair.second == "boolean") refined_schema[pair.first] = database::QueryDataType::boolean;
    else throw std::runtime_error("Undefined datatypes is used for schema");
  });
  lambda(databaseName,containerName,refined_schema);
}

void _ParseInsertIntoContainerQuery(const std::string& expression,const std::function<void(const std::string& databaseName,const std::string& containerName, const database::api_insert_update_type& values)>& lambda)
{
  //company.employee values(employee_id:1,employee_name:chandan,employee_status:full_time)
  std::regex root_regex("^([[:w:]]+).([[:w:]]+) values+",std::regex_constants::icase);
  std::smatch root_smatch;
  if(!std::regex_search(expression,root_smatch,root_regex))
    throw std::runtime_error("Invalid expression provided for extracting transaction parameters");
  std::string databaseName = root_smatch[1].str();
  std::string containerName = root_smatch[2].str();
  std::string values_dataset = root_smatch.suffix().str();
  
  //e.g. (employee_id:1,employee_name:chandan,employee_status:full_time)
  std::regex values_regex("([[:w:]]+):([[:w:]]+)",std::regex_constants::icase);
  std::smatch values_smatch;
  if(!std::regex_search(expression,values_smatch,values_regex))
    throw std::runtime_error("Invalid expression provided for extracting transaction parameters");
  std::sregex_iterator pos = {expression.cbegin(),expression.cend(),values_regex};
  std::sregex_iterator end;
  database::api_insert_update_type values;
  for(;pos != end; ++pos)
    values[pos -> str(1)] = pos -> str(2);
  lambda(databaseName,containerName,values);
}

void _ParseSelectAllFromContainerQuery(const std::string& expression,const std::function<void(const std::string& databaseName,const std::string& containerName)>& lambda)
{
  //from companye.employee
  std::regex root_regex("from ([[:w:]]+).([[:w:]]+)",std::regex_constants::icase);
  std::smatch root_smatch;
  if(!std::regex_search(expression,root_smatch,root_regex))
    throw std::runtime_error("Invalid regular expression provided for extracting transaction parameters");
  std::string databaseName = root_smatch[1].str();
  std::string containerName = root_smatch[2].str();
  lambda(databaseName,containerName);
}

void _ParseSelectDataSetFromContainerQuery_ForData(const std::string& expression,const std::function<void(const std::vector<std::string>&)>& lambda)
{
  //e.g. employee_id,employee_name
  std::regex regex("([[:w:]]+)",std::regex_constants::icase);
  std::sregex_iterator pos = {expression.cbegin(),expression.cend(),regex};
  std::sregex_iterator end;
  std::vector<std::string> dataset;
  for(;pos != end; ++pos)
    dataset.emplace_back(pos -> str(1));
  lambda(dataset);
}

void _ParseSelectDataSetFromContainerQuery(const std::string& expression,const std::function<void(const std::string& databaseName,const std::string& containerName, const std::vector<std::string>& dataset,const database::api_filter_type& filter)>& lambda)
{
  //employee_id,employee_name from company.employee where employee_id = 1
  std::regex regex_for_condition("from ([[:w:]]+).([[:w:]]+) where ",std::regex_constants::icase);
  std::regex regex("from ([[:w:]]+).([[:w:]]+)",std::regex_constants::icase);
  std::smatch root_smatch;
  if(std::regex_search(expression,root_smatch,regex_for_condition))
  {
    std::string dataset_string = root_smatch.prefix().str();
    std::string databaseName = root_smatch[1].str();
    std::string containerName = root_smatch[2].str();
    std::string filter_string = root_smatch.suffix().str();
    std::vector<std::string> dataset;
    _ParseSelectDataSetFromContainerQuery_ForData(dataset_string,[&](auto _dataset) {
      dataset = _dataset;
    });
    database::api_filter_type filter;
    _FilterParser(filter_string,[&](auto _filter) {
      filter = _filter;
    });
    if(filter.empty())
      throw std::runtime_error("Invalid expression provided for extracting transaction parameters");
    lambda(databaseName,containerName,dataset,filter);
  }else if(std::regex_search(expression,root_smatch,regex))
  {
    std::string dataset_string = root_smatch.prefix().str();
    std::string databaseName = root_smatch[1].str();
    std::string containerName = root_smatch[2].str();
    std::vector<std::string> dataset;
    _ParseSelectDataSetFromContainerQuery_ForData(dataset_string,[&](auto _dataset){
      dataset = _dataset;
    });
    lambda(databaseName,containerName,dataset,{});
  }else
    throw std::runtime_error("Invalid expression provided for extracting transaction parameters");
}

void _ParseUpdateContainerQuery_Separator(const std::string& expression,const std::function<void(const std::string& new_data_part, const std::string& filter_part)>& lambda)
{
  //e.g. employee_name = Tim,employee_status = fulltime where employee_id = 1
  std::regex regex(".where.",std::regex_constants::icase);
  std::smatch smatch;
  if(!std::regex_search(expression,smatch,regex))
    throw std::runtime_error("Invalid regular expression provided for extracting transaction parameters");
  std::string newData = smatch.prefix().str();
  std::string conditions = smatch.suffix().str();
  lambda(newData,conditions);
}

void _ParseUpdateContainerQuery_Data(const std::string& expression,const std::function<void(const database::api_insert_update_type&)>& lambda)
{
  //e.g. employee_name = Tim,employee_status = fulltime
  std::regex regex("([[:w:]]+) = ([[:w:]]+)",std::regex_constants::icase);
  std::sregex_iterator pos = {expression.cbegin(),expression.cend(),regex};
  std::sregex_iterator end;
  database::api_insert_update_type newData;
  for(;pos != end; ++pos)
    newData[pos -> str(1)] = pos -> str(2);
  lambda(newData);
}

void _ParseUpdateContainerQuery(const std::string& expression,const std::function<void(const std::string& databaseName,const std::string& containerName, const database::api_insert_update_type& values,const database::api_filter_type& filter)>& lambda)
{
  //e.g. company.employee set employee_name = Tim,employee_status = fulltime where employee_id = 1
  std::regex regex("([[:w:]]+).([[:w:]]+) set ",std::regex_constants::icase);
  std::smatch root_smatch;
  if(!std::regex_search(expression,root_smatch,regex))
    throw std::runtime_error("Invalid regular expression provided for extracting transaction parameters");
  std::string databaseName = root_smatch[1].str();
  std::string containerName = root_smatch[2].str();
  std::string remainder_part = root_smatch.suffix().str();
  
  database::api_insert_update_type values;
  database::api_filter_type filter;
  _ParseUpdateContainerQuery_Separator(remainder_part,[&](auto new_data_part,auto filter_part){
    _ParseUpdateContainerQuery_Data(new_data_part,[&](auto _values){
      values = _values;
    });
    _FilterParser(filter_part,[&](auto _filter) {
      filter = _filter;
    });
  });
  lambda(databaseName,containerName,values,filter);
}


void _ParseTruncateContainerQuery(const std::string& expression,const std::function<void(const std::string& databaseName,const std::string& containerName)>& lambda)
{
  //e.g. company.employee
  std::regex root_regex("([[:w:]]+).([[:w:]]+)",std::regex_constants::icase);
  std::smatch root_smatch;
  if(!std::regex_search(expression,root_smatch,root_regex))
    throw std::runtime_error("Invalid expression provided for extracting transaction parameters");
  std::string databaseName = root_smatch[1].str();
  std::string containerName = root_smatch[2].str();
  lambda(databaseName,containerName);
}

// void _Tran_DeleteFrom_PrefixSeparator_Filter(const std::string& expression,const std::function<void(const std::string& databaseName,const std::string& containerName)>& lambda)
// {
//   //e.g. company.employee
//   std::regex regex("([[:w:]]+).([[:w:]]+)",std::regex_constants::icase);
//   std::smatch smatch;
//   if(!std::regex_search(expression,smatch,regex))
//     throw std::runtime_error("Invalid regular expression provided for extracting transaction parameters");
//   std::string databaseName = smatch[1].str();
//   std::string containerName = smatch[2].str();
//   lambda(databaseName,containerName);
// }

// void _Tran_DeleteFrom_SuffixSeparator_Filter(const std::string& expression,const std::function<void(const std::map<std::string,std::string>&)>& lambda)
// {
//   //e.g. employee_id = 1
//   std::regex regex("([[:w:]]+) = ([[:w:]]+)",std::regex_constants::icase);
//   std::sregex_iterator pos = {expression.cbegin(),expression.cend(),regex};
//   std::sregex_iterator end;
//   std::map<std::string,std::string> conditions;
//   for(;pos != end; ++pos)
//     conditions[pos -> str(1)] = pos -> str(2);
//   lambda(conditions);
// }

// void _Tran_DeleteFrom_RootSeparator_Filter(const std::string& expression,const std::function<void(const std::string&,const std::string&,const std::map<std::string,std::string>&)>& lambda)
// {
//   //e.g. company.employee where employee_id = 1
//   std::regex regex(".where.",std::regex_constants::icase);
//   std::smatch smatch;
//   if(!std::regex_search(expression,smatch,regex))
//     throw std::runtime_error("Invalid regular expression provided for extracting transaction parameters");
//   std::string prefixData = smatch.prefix().str();
//   std::string suffixData = smatch.suffix().str();
  
//   std::string databaseName;
//   std::string containerName;
//   _Tran_DeleteFrom_PrefixSeparator_Filter(prefixData,[&](auto _databaseName,auto _containerName) {
//     databaseName = _databaseName;
//     containerName = _containerName;
//   });
//   std::map<std::string,std::string> deleteConditions;
//   _Tran_DeleteFrom_SuffixSeparator_Filter(suffixData,[&](auto conditions) {
//     deleteConditions = conditions;
//   });
//   lambda(databaseName,containerName,deleteConditions);
// }

// void _Tran_DropContainer_PrefixSeparator_Filter(const std::string& expression,const std::function<void(const std::string& databaseName,const std::string& containerName)>& lambda)
// {
//   //e.g. company.employee
//   std::regex regex("([[:w:]]+).([[:w:]]+)",std::regex_constants::icase);
//   std::smatch smatch;
//   if(!std::regex_search(expression,smatch,regex))
//     throw std::runtime_error("Invalid regular expression provided for extracting transaction parameters");
//   std::string databaseName = smatch[1].str();
//   std::string containerName = smatch[2].str();
//   lambda(databaseName,containerName);
// }

std::pair<database::TransactionType,std::string> _Transaction_Filter(const std::string& expression)
{
  std::regex tran_create_database("^CREATE DATABASE.",std::regex_constants::icase);
  std::regex tran_create_container("^CREATE CONTAINER.",std::regex_constants::icase);
  std::regex tran_insert_into("^INSERT INTO.",std::regex_constants::icase);
  std::regex tran_select_all("^SELECT \\*.",std::regex_constants::icase);
  std::regex tran_select_dataset("^SELECT.",std::regex_constants::icase);
  std::regex tran_update("^UPDATE.",std::regex_constants::icase);
  std::regex tran_truncate("^TRUNCATE CONTAINER.",std::regex_constants::icase);
  // std::regex tran_alter("^ALTER CONTAINER.",std::regex_constants::icase);
  std::regex tran_delete("^DELETE FROM.",std::regex_constants::icase);
  std::regex tran_drop_container("^DROP CONTAINER.",std::regex_constants::icase);
  std::regex tran_drop_database("^DROP DATABASE.",std::regex_constants::icase);

  if(std::regex_search(expression,tran_create_database)) return std::make_pair<database::TransactionType,std::string>(database::create_database,_Get_Transaction_Detail(expression,tran_create_database));
  else if(std::regex_search(expression,tran_create_container)) return std::make_pair<database::TransactionType,std::string>(database::create_container,_Get_Transaction_Detail(expression,tran_create_container));
  else if(std::regex_search(expression,tran_insert_into)) return std::make_pair<database::TransactionType,std::string>(database::insert_into,_Get_Transaction_Detail(expression,tran_insert_into));
  else if(std::regex_search(expression,tran_select_all)) return std::make_pair<database::TransactionType,std::string>(database::select_all,_Get_Transaction_Detail(expression,tran_select_all));
  else if(std::regex_search(expression,tran_select_dataset)) return std::make_pair<database::TransactionType,std::string>(database::select_dataset,_Get_Transaction_Detail(expression,tran_select_dataset));
  else if(std::regex_search(expression,tran_update)) return std::make_pair<database::TransactionType,std::string>(database::update,_Get_Transaction_Detail(expression,tran_update));
  else if(std::regex_search(expression,tran_truncate)) return std::make_pair<database::TransactionType,std::string>(database::truncate,_Get_Transaction_Detail(expression,tran_truncate));
  // else if(std::regex_search(expression,tran_alter)) return std::make_pair<database::TransactionType,std::string>(database::alter,_Get_Transaction_Detail(expression,tran_alter));
  else if(std::regex_search(expression,tran_delete)) return std::make_pair<database::TransactionType,std::string>(database::delete_from,_Get_Transaction_Detail(expression,tran_delete));
  else if(std::regex_search(expression,tran_drop_container)) return std::make_pair<database::TransactionType,std::string>(database::drop_container,_Get_Transaction_Detail(expression,tran_drop_container));
  else if(std::regex_search(expression,tran_drop_database)) return std::make_pair<database::TransactionType,std::string>(database::drop_database,_Get_Transaction_Detail(expression,tran_drop_database));
  else
    throw std::runtime_error("Wrong syntax or undefined transaction attempted");
}

void database::Query::_ParseQueryString()
{
  std::pair<database::TransactionType,std::string> first_order_filter_result = _Transaction_Filter(m_query_string);
  m_transaction_type = first_order_filter_result.first;

  //Create Database
  if(m_transaction_type == database::create_database)
  {
    m_transaction_metatype = database::ddl;
    m_database_name = first_order_filter_result.second;
  }

  //Create Container
  else if(m_transaction_type == database::create_container)
  {
    m_transaction_metatype = database::ddl;
    _ParseCreateContainerQuery(first_order_filter_result.second,[&](auto databaseName, auto containerName, auto schema){
      m_database_name = databaseName;
      m_container_name = containerName;
      m_container_schema = schema;
    });
  }

  //Insert into Container
  else if(m_transaction_type == database::insert_into)
  {
    m_transaction_metatype = database::dml;
    _ParseInsertIntoContainerQuery(first_order_filter_result.second,[&](auto databaseName, auto containerName, auto values){
      m_database_name = databaseName;
      m_container_name = containerName;
      m_values = values;
    });
  }

  //Select All from Container
  else if(m_transaction_type == database::select_all)
  {
    m_transaction_metatype = database::dml;
    _ParseSelectAllFromContainerQuery(first_order_filter_result.second,[&](auto databaseName, auto containerName){
      m_database_name = databaseName;
      m_container_name = containerName;
    });
  }

  //Select Dataset from Container
  else if(m_transaction_type == database::select_dataset)
  {
    m_transaction_metatype = database::dml;
    _ParseSelectDataSetFromContainerQuery(first_order_filter_result.second,[&](auto databaseName, auto containerName, auto dataset, auto filter){
      m_database_name = databaseName;
      m_container_name = containerName;
      m_dataset = dataset;
      m_filter = filter;
    });
  }

  //Update Container
  else if(m_transaction_type == database::update)
  {
    m_transaction_metatype = database::dml;
    _ParseUpdateContainerQuery(first_order_filter_result.second,[&](auto databaseName, auto containerName, auto values,auto filter){
      m_database_name = databaseName;
      m_container_name = containerName;
      m_values = values;
      m_filter = filter;
    });
  }

  //Truncate Container
  else if(m_transaction_type == database::truncate)
  {
    m_transaction_metatype = database::dml;
    _ParseTruncateContainerQuery(first_order_filter_result.second,[&](auto databaseName,auto containerName){
      m_database_name = databaseName;
      m_container_name = containerName;
    });
  }

  //Alter Container - To Be Adjusted Later
  // else if(m_transaction_type == database::alter)
  // {
  //   m_transaction_metatype = database::ddl;
  // }

  
  //Delete from Container
  // else if(m_transaction_type == database::delete_from)
  // {
  //   m_transaction_metatype = database::dml;
  //   _Tran_DeleteFrom_RootSeparator_Filter(first_order_filter_result.second,[&](auto _databaseName,auto _containerName,auto _deleteConditions) {
  //     m_database_name = _databaseName;
  //     m_container_name = _containerName;
  //     m_delete_conditions = _deleteConditions;
  //   });
  // }
  // else if(m_transaction_type == database::drop_container)
  // {
  //   m_transaction_metatype = database::ddl;
  //   _Tran_DropContainer_PrefixSeparator_Filter(first_order_filter_result.second,[&](auto _databaseName, auto _containerName){
  //     m_database_name = _databaseName;
  //     m_container_name = _containerName;
  //   });
  // }
  // else if(m_transaction_type == database::drop_database)
  // {
  //   m_transaction_metatype = database::ddl;
  //   m_database_name = first_order_filter_result.second;
  // }
}
