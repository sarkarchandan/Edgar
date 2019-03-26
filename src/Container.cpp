#include "Container.hpp"
#include "Utility.hpp"

std::string database::Container::name() const { return m_name; }
std::size_t database::Container::id() const { return m_id; }
std::map<std::string,database::DataType> database::Container::schema() const { return m_schema; }

void database::Container::_PrepareContainer()
{
  std::hash<std::string> s_hash;
  m_id = s_hash(m_name);
  m_data = std::make_unique<std::vector<std::vector<database::ComparableString>>>();
  for(std::size_t i = 0; i < m_schema.size(); i += 1)
  {
    std::vector<database::ComparableString> vector;
    m_data -> emplace_back(vector);
  }
}

bool database::Container::_HaveSameKeysFor(const std::map<std::string,database::ComparableString>& lhs, const std::map<std::string,database::DataType>& rhs) const
{
  return (lhs.size() == rhs.size()) && std::equal(lhs.begin(),lhs.end(),rhs.begin(),[&](auto lhs_pair,auto rhs_pair){ return lhs_pair.first == rhs_pair.first; });
}

void database::Container::_InsertInto(const std::map<std::string,database::ComparableString> values)
{
  if(!_HaveSameKeysFor(values,m_schema)) //Check validity of the provided key-values against defined schema
    throw std::runtime_error("Insert attempted with invalid columns");

  std::for_each(values.begin(),values.end(),[&](auto pair){
    std::size_t index = std::distance(m_schema.begin(),m_schema.find(pair.first));
    m_data -> operator[](index).emplace_back(pair.second);
  });

  // std::vector<database::ComparableString> value_buffer;
  // std::transform(values.begin(),values.end(),std::back_inserter(value_buffer),[&](auto pair){
  //   return pair.second;
  // });
  // m_data -> emplace_back(value_buffer);
}

bool database::Container::_IsValidFilterCriteriaForRawSelection(const std::map<std::string,std::vector<database::ComparableString>>& filter_criteria) const
{
  for(auto pair: filter_criteria)
  {
    if(m_schema.find(pair.first) == m_schema.end())
      return false;
  }
  return true;
}

bool database::Container::_IsValidFilterCriteriaForAggregateSelection(const std::map<std::string,std::vector<database::ComparableString>>& filter_criteria) const { return true; }

bool _IsValidFilteringInformation(const std::map<std::string,std::vector<database::ComparableString>>& filter_criteria, const std::map<std::string,std::vector<database::ComparisonType>>& filter_comparison_params) 
{
  return (filter_criteria.size() == filter_comparison_params.size()) && std::equal(filter_criteria.begin(),filter_criteria.end(),filter_comparison_params.begin(),[&](auto lhs_pair,auto rhs_pair){
    return (lhs_pair.first == rhs_pair.first) && (lhs_pair.second.size() == rhs_pair.second.size());
  });
}

void _PopulateValueIfNotExisting(std::vector<std::size_t>& vector,const std::size_t& value)
{
  if(std::find(vector.begin(),vector.end(),value) == vector.end())
    vector.emplace_back(value);
  else return;
}

void database::Container::_SelectAll(const std::function<void(const std::map<std::string,std::vector<database::ComparableString>>&)>& lambda) const
{
  std::map<std::string,std::vector<database::ComparableString>> result;
  std::for_each(m_schema.begin(),m_schema.end(),[&](auto pair){
    std::size_t index = std::distance(m_schema.begin(),m_schema.find(pair.first));
    result[pair.first] = m_data -> operator[](index);
  });
  lambda(result);
}

void _for_each_criteria(const std::map<std::string,std::vector<database::ComparableString>>& filter_criteria,const std::map<std::string,std::vector<database::ComparisonType>>& filter_comparison_params,const std::function<void(const std::pair<std::string,std::vector<database::ComparableString>>&,const std::pair<std::string,std::vector<database::ComparisonType>>&)>& lambda)
{
  if(filter_criteria.size() != filter_comparison_params.size())
    throw std::runtime_error("Iteration over unequal number of filtering criteria and comparison parameters is not defined");
  
  auto criteria_iterator = filter_criteria.begin();
  auto comparison_params_iterator = filter_comparison_params.begin();

  while(criteria_iterator != filter_criteria.end() && comparison_params_iterator != filter_comparison_params.end())
  {
    lambda(*criteria_iterator,*comparison_params_iterator);
    ++criteria_iterator;
    ++comparison_params_iterator;
  }
}

void _for_each_comparison(const std::vector<database::ComparableString>& values,const std::vector<database::ComparisonType>& types,const std::function<void(const database::ComparableString&,const database::ComparisonType&)>& lambda)
{
  if(values.size() != types.size())
    throw std::runtime_error("Iteration over unequal number of values and types is not defined");
  auto value_iterator = values.begin();
  auto type_iterator = types.begin();
  while(value_iterator != values.end() && type_iterator != types.end())
  {
    lambda(*value_iterator,*type_iterator);
    ++value_iterator;
    ++type_iterator;
  }
}

void database::Container::_SelectRawWithCriteria(const std::map<std::string,std::vector<database::ComparableString>>& filter_criteria,const std::map<std::string,std::vector<database::ComparisonType>>& filter_comparison_params,const std::vector<std::string>& dataset,const std::function<void(const std::map<std::string,std::vector<database::ComparableString>>&)>& lambda) const
{
  if(!database::Container::_IsValidFilterCriteriaForRawSelection(filter_criteria)) //Check validity of the provided key-value against defined schema
    throw std::runtime_error("Select attempted with invalid column identifiers");

  /*
  filter_criteria gives a column name as a key and the corresponding values on which basis data must be filtered.
  filter_comparison_params is a similar that gives what kind of comparison it should be e.g.,==,!=,<,<=,>,>= etc..
  Therefore filter_criteria and filter_comparison_types must be compatible to each other
  */
  if(!_IsValidFilteringInformation(filter_criteria,filter_comparison_params))
    throw std::runtime_error("Criteria and parameters for filtering are not compatible");
  
  // Will have the final filtered indices for the values that match the criteria.
  std::vector<std::size_t> index_buffer; 

  /*std::map<std::string,std::vector<database::ComparableString>> filter_criteria*/
  /*std::map<std::string,std::vector<database::ComparisonType>> filter_comparison_params*/

  //Iterating over each key-value pair in the filter_criteria and filter_comparison_type
  _for_each_criteria(filter_criteria,filter_comparison_params,[&](auto criteria_pair,auto comparison_pair){
    // criteria_pair: <std::string,std::vector<database::ComparableString>>
    // comparison_pair: <std::string,std::vector<database::ComparisonType>>

    //This is the index of the column on which filtering must be applied
    std::size_t filter_column_index = std::distance(m_schema.begin(),m_schema.find(criteria_pair.first));
    
    //Iterating over each pair of corresponding value respective compare_type provided in the given criteria
    _for_each_comparison(criteria_pair.second,comparison_pair.second,[&](auto value,auto compare) {
      std::cout << "Value: " << value << "\n";
      auto column = m_data -> operator[](filter_column_index);

      //Iterating over each value in the given column to check if the filter criterion is satisfied O(n^3)
      for(std::size_t i = 0; i < column.size(); i += 1)
      {
        switch (compare)
        {
          case equal_to: if(column[i] == value) _PopulateValueIfNotExisting(index_buffer,i); break;
          case not_eqaul_to: if(column[i] != value) _PopulateValueIfNotExisting(index_buffer,i); break;
          case greater_or_equal_to: if(column[i] >= value) _PopulateValueIfNotExisting(index_buffer,i); break;
          case lesser_or_equal_to: if(column[i] <= value) _PopulateValueIfNotExisting(index_buffer,i); break;
          case greater_than: if(column[i] > value) _PopulateValueIfNotExisting(index_buffer,i); break;
          case lesser_than: if(column[i] < value) _PopulateValueIfNotExisting(index_buffer,i); break;
        }
      }
    });
  });

  std::cout << "Indices accumulated: " << index_buffer << "\n";
  std::map<std::string,std::vector<database::ComparableString>> result; //Extract data with the filtered indices

  std::for_each(m_schema.begin(),m_schema.end(),[&](auto pair) {

    std::size_t column_index = std::distance(m_schema.begin(),m_schema.find(pair.first));
    auto column = m_data -> operator[](column_index);

    std::vector<database::ComparableString> data_buffer;
    std::transform(index_buffer.begin(),index_buffer.end(),std::back_inserter(data_buffer),[&](auto index) { 
      return column[index];
    });

    result[pair.first] = data_buffer;
  });
  lambda(result);
}




