#include "Container.hpp"
#include "Utility.hpp"

std::string database::Container::name() const { return m_name; }
std::size_t database::Container::id() const { return m_id; }
database::impl_schema_type database::Container::schema() const { return m_schema; }

void database::Container::_PrepareContainer()
{
  std::hash<std::string> s_hash;
  m_id = s_hash(m_name);
  m_data = std::make_unique<database::impl_storage_type>();
  for(std::size_t i = 0; i < m_schema.size(); i += 1)
  {
    std::vector<database::ComparableString> vector;
    m_data -> emplace_back(vector);
  }
}

bool database::Container::_HaveSameKeysFor(const database::impl_insert_update_type& lhs, const database::impl_schema_type& rhs) const
{
  return (lhs.size() == rhs.size()) && std::equal(lhs.begin(),lhs.end(),rhs.begin(),[&](auto lhs_pair,auto rhs_pair){ return lhs_pair.first == rhs_pair.first; });
}

void database::Container::_InsertInto(const database::impl_insert_update_type& values)
{
  /*Check validity of the provided key-values against defined schema*/
  if(!_HaveSameKeysFor(values,m_schema))
    throw std::runtime_error("Insert attempted with invalid columns");

  std::for_each(values.begin(),values.end(),[&](auto pair){
    std::size_t index = std::distance(m_schema.begin(),m_schema.find(pair.first));
    m_data -> operator[](index).emplace_back(pair.second);
  });
}

bool database::Container::_IsValidFilterCriteriaForRawSelection(const database::impl_filter_type& filter_criteria) const
{
  for(auto pair: filter_criteria)
    if(m_schema.find(pair.first) == m_schema.end())
      return false;
  return true;
}

bool database::Container::_IsValidFilterCriteriaForAggregateSelection(const database::impl_filter_type& filter_criteria) const
{
  exit(EXIT_FAILURE);
}

bool database::Container::_IsValidDataSetRequested(const std::vector<std::string>& dataset) const
{
  if(dataset.empty()) return true;
  for(auto column_name: dataset)
    if(m_schema.find(column_name) == m_schema.end()) 
      return false;
  return true;
}

std::vector<std::size_t> _CustomIntersect(const std::vector<std::size_t>& lhs,const std::vector<std::size_t>& rhs)
{
  if(lhs.empty() && rhs.empty()) return {};
  else if(lhs.empty()) return rhs;
  else if(rhs.empty()) return lhs;
  else 
  {
    std::vector<std::size_t> intersection_buffer;
    std::set_intersection(lhs.begin(),lhs.end(),rhs.begin(),rhs.end(),std::back_inserter(intersection_buffer));
    if(!intersection_buffer.empty())
      return intersection_buffer;
    else
    {
      std::vector<std::size_t> union_buffer;
      std::set_union(lhs.begin(),lhs.end(),rhs.begin(),rhs.end(),std::back_inserter(union_buffer));
      return union_buffer;
    }
  }
}

void database::Container::_SelectAll(const std::function<void(const database::impl_dataset_type&)>& lambda) const
{
  database::impl_dataset_type result;
  std::for_each(m_schema.begin(),m_schema.end(),[&](auto pair){
    std::size_t index = std::distance(m_schema.begin(),m_schema.find(pair.first));
    result[pair.first] = m_data -> operator[](index);
  });
  lambda(result);
}

void database::Container::_SelectRawDataSet(const std::vector<std::string>& dataset,const std::function<void(const database::impl_dataset_type&)>& lambda) const
{
  if(dataset.empty())
  {
    _SelectAll(lambda);
    return;
  }

  if(!_IsValidDataSetRequested(dataset))
    throw std::runtime_error("One or more of requested columns are not present in schema");

  database::impl_dataset_type result;
  std::for_each(dataset.begin(),dataset.end(),[&](auto column_name) {
    auto column_index = std::distance(m_schema.begin(),m_schema.find(column_name));
    result[column_name] = m_data -> operator[](column_index);
  });
  lambda(result);
}

void _for_each_criteria(const database::impl_filter_type& filter_criteria,const std::function<void(const std::string& filter_column,const database::impl_compare_type)>& lambda)
{
  auto criteria_iterator = filter_criteria.begin();
  while(criteria_iterator != filter_criteria.end())
  {
    lambda(criteria_iterator -> first,criteria_iterator -> second);
    ++criteria_iterator;
  }
}

void _for_each_comparison(const database::impl_compare_type values,const std::function<void(const database::ComparableString&,const database::ComparisonType&)>& lambda)
{
  auto value_iterator = values.begin();
  while(value_iterator != values.end())
  {
    lambda(value_iterator -> first,value_iterator -> second);
    ++value_iterator;
  }
}

void database::Container::_SelectRawDataSetWithCriteria(const database::impl_filter_type& filter_criteria,const std::vector<std::string>& dataset,const std::function<void(const database::impl_dataset_type&)>& lambda) const
{
  /*Check validity of the provided key-value criteria against defined schema*/
  if(!database::Container::_IsValidFilterCriteriaForRawSelection(filter_criteria))
    throw std::runtime_error("Select attempted with invalid column identifiers");

  /*Check if the requested dataset is valid*/
  if(!_IsValidDataSetRequested(dataset))
    throw std::runtime_error("One or more of requested columns are not present in schema");

  // Will have the final filtered indices for the values that match the criteria.
  std::vector<std::size_t> primary_index_vector;
  //Iterating over each key-value pair in the filter_criteria and filter_comparison_type
  _for_each_criteria(filter_criteria,[&](auto filter_column,auto comparison_params){

    //This is the index of the column on which filtering must be applied
    std::size_t filter_column_index = std::distance(m_schema.begin(),m_schema.find(filter_column));
    
    std::vector<std::size_t> secondary_index_vector;
    //Iterating over each pair of corresponding value respective compare_type provided in the given criteria
    _for_each_comparison(comparison_params,[&](auto value,auto compare) {
      auto column = m_data -> operator[](filter_column_index);
      
      std::vector<std::size_t> tertiary_index_vector;
      //Iterating over each value in the given column to check if the filter criterion is satisfied O(n^3)
      for(std::size_t i = 0; i < column.size(); i += 1)
      {
        switch (compare)
        {
          case equal_to: if(column[i] == value) tertiary_index_vector.emplace_back(i); break;
          case not_equal_to: if(column[i] != value) tertiary_index_vector.emplace_back(i); break;
          case greater_or_equal_to: if(column[i] >= value) tertiary_index_vector.emplace_back(i); break;
          case lesser_or_equal_to: if(column[i] <= value) tertiary_index_vector.emplace_back(i); break;
          case greater_than: if(column[i] > value) tertiary_index_vector.emplace_back(i); break;
          case lesser_than: if(column[i] < value) tertiary_index_vector.emplace_back(i); break;
        }
      }
      secondary_index_vector = _CustomIntersect(secondary_index_vector,tertiary_index_vector);
    });
    primary_index_vector = _CustomIntersect(primary_index_vector,secondary_index_vector);
  });

  database::impl_dataset_type result; //Extract data with the filtered indices

  std::vector<std::string> required_columns;
  if(dataset.empty())
    std::transform(m_schema.begin(),m_schema.end(),std::back_inserter(required_columns),[&](auto pair){ return pair.first; });
  else
    required_columns = dataset;

  for_each(required_columns.begin(),required_columns.end(),[&](auto column_name) {
    std::size_t column_index = std::distance(m_schema.begin(),m_schema.find(column_name));
    auto column = m_data -> operator[](column_index);

    std::vector<database::ComparableString> data_buffer;
    std::transform(primary_index_vector.begin(),primary_index_vector.end(),std::back_inserter(data_buffer),[&](auto index) {
      return column[index];
    });
    result[column_name] = data_buffer;
  });
  lambda(result);
}

void database::Container::_UpdateValueForIndex(const std::size_t& index,const database::impl_insert_update_type& new_value)
{
  if(new_value.empty()) return;
  for(auto pair: new_value)
  {
    std::size_t column_index = std::distance(m_schema.begin(),m_schema.find(pair.first));
    m_data -> operator[](column_index)[index] = pair.second;
  }
}

void database::Container::_Update(const database::impl_filter_type& filter_criteria,const database::impl_insert_update_type& new_value)
{
  /*Check validity of the provided key-value criteria against defined schema*/
  if(!database::Container::_IsValidFilterCriteriaForRawSelection(filter_criteria))
    throw std::runtime_error("Update attempted with invalid column identifiers");

  //Iterating over each key-value pair in the filter_criteria and filter_comparison_type
  _for_each_criteria(filter_criteria,[&](auto filter_column,auto comparison_params){

    //This is the index of the column on which filtering must be applied
    std::size_t filter_column_index = std::distance(m_schema.begin(),m_schema.find(filter_column));

    //Iterating over each pair of corresponding value respective compare_type provided in the given criteria
    _for_each_comparison(comparison_params,[&](auto value,auto compare) {

      auto column = m_data -> operator[](filter_column_index);

      //Iterating over each value in the given column to check if the filter criterion is satisfied O(n^3)
      for(std::size_t i = 0; i < column.size(); i += 1)
      {
        switch (compare)
        {
          case equal_to: if(column[i] == value) _UpdateValueForIndex(i,new_value); break;
          case not_equal_to: if(column[i] != value) _UpdateValueForIndex(i,new_value); break;
          case greater_or_equal_to: if(column[i] >= value) _UpdateValueForIndex(i,new_value); break;
          case lesser_or_equal_to: if(column[i] <= value) _UpdateValueForIndex(i,new_value); break;
          case greater_than: if(column[i] > value) _UpdateValueForIndex(i,new_value); break;
          case lesser_than: if(column[i] < value) _UpdateValueForIndex(i,new_value); break;
        }
      }
    });
  });
}

void database::Container::_DeleteFrom(const database::impl_filter_type& filter_criteria)
{
  exit(EXIT_FAILURE);
}

void database::Container::_Truncate()
{
  exit(EXIT_FAILURE);
}
