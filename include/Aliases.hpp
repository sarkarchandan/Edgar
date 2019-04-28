#ifndef ALIASES_H
#define ALIASES_H
#include "ComparableString.hpp"
#include <map>
#include <vector>
#include <string>
#include "Paradigms.hpp"

namespace database 
{
  using impl_schema_type = std::map<std::string,database::DataType>;
  using impl_storage_type = std::vector<std::vector<database::ComparableString>>;
  using impl_dataset_type = std::map<std::string,std::vector<database::ComparableString>>;
  using impl_filter_type = std::map<std::string,std::vector<database::ComparableString>>;
  using impl_filtercompare_type = std::map<std::string,std::vector<database::ComparisonType>>;
  using impl_insert_update_type = std::map<std::string,database::ComparableString>;

  using api_dataset_type = std::map<std::string,std::vector<std::string>>;
  using api_schema_type = std::map<std::string,database::QueryDataType>;
  using api_insert_update_type = std::map<std::string,std::string>;
  using api_filter_type = std::map<std::string,std::vector<std::string>>;
  using api_filtercompare_type = std::map<std::string,std::vector<database::ComparisonType>>;
}

#endif //ALIASES_H