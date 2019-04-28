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
  {
    if(m_schema.find(pair.first) == m_schema.end())
      return false;
  }
  return true;
}

bool database::Container::_IsValidFilterCriteriaForAggregateSelection(const database::impl_filter_type& filter_criteria) const
{
  exit(EXIT_FAILURE);
}

bool _IsValidFilteringInformation(const database::impl_filter_type& filter_criteria, const database::impl_filtercompare_type& filter_comparison_params)
{
  return (filter_criteria.size() == filter_comparison_params.size()) && std::equal(filter_criteria.begin(),filter_criteria.end(),filter_comparison_params.begin(),[&](auto lhs_pair,auto rhs_pair){
    return (lhs_pair.first == rhs_pair.first) && (lhs_pair.second.size() == rhs_pair.second.size());
  });
}

bool database::Container::_IsValidDataSetRequested(const std::vector<std::string>& dataset) const
{
  if(dataset.empty()) return true;
  for(auto column_name: dataset)
  {
    if(m_schema.find(column_name) == m_schema.end()) return false;
  }
  return true;
}

void _PopulateValueIfNotExisting(std::vector<std::size_t>& vector,const std::size_t& value)
{
  if(std::find(vector.begin(),vector.end(),value) == vector.end())
    vector.emplace_back(value);
  else return;
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

void _for_each_criteria(const database::impl_filter_type& filter_criteria,const database::impl_filtercompare_type& filter_comparison_params,const std::function<void(const std::pair<std::string,std::vector<database::ComparableString>>&,const std::pair<std::string,std::vector<database::ComparisonType>>&)>& lambda)
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

void database::Container::_SelectRawDataSetWithCriteria(const database::impl_filter_type& filter_criteria,const database::impl_filtercompare_type& filter_comparison_params,const std::vector<std::string>& dataset,const std::function<void(const database::impl_dataset_type&)>& lambda) const
{
  /*Check validity of the provided key-value criteria against defined schema*/
  if(!database::Container::_IsValidFilterCriteriaForRawSelection(filter_criteria))
    throw std::runtime_error("Select attempted with invalid column identifiers");

  /*
  filter_criteria gives a column name as a key and the corresponding values on which basis data must be filtered.
  filter_comparison_params is a similar that gives what kind of comparison it should be e.g.,==,!=,<,<=,>,>= etc..
  Therefore filter_criteria and filter_comparison_types must be compatible to each other
  */
  if(!_IsValidFilteringInformation(filter_criteria,filter_comparison_params))
    throw std::runtime_error("Criteria and parameters for filtering are not compatible");

  /*Check if the requested dataset is valid*/
  if(!_IsValidDataSetRequested(dataset))
    throw std::runtime_error("One or more of requested columns are not present in schema");

  // Will have the final filtered indices for the values that match the criteria.
  std::vector<std::size_t> index_buffer;

  //Iterating over each key-value pair in the filter_criteria and filter_comparison_type
  _for_each_criteria(filter_criteria,filter_comparison_params,[&](auto criteria_pair,auto comparison_pair){

    //This is the index of the column on which filtering must be applied
    std::size_t filter_column_index = std::distance(m_schema.begin(),m_schema.find(criteria_pair.first));

    //Iterating over each pair of corresponding value respective compare_type provided in the given criteria
    _for_each_comparison(criteria_pair.second,comparison_pair.second,[&](auto value,auto compare) {
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
    std::transform(index_buffer.begin(),index_buffer.end(),std::back_inserter(data_buffer),[&](auto index) {
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

void database::Container::_Update(const database::impl_filter_type& filter_criteria,const database::impl_filtercompare_type& filter_comparison_params,const database::impl_insert_update_type& new_value)
{
  /*Check if criteria is provided for the update at all*/
  if(filter_criteria.empty() || filter_comparison_params.empty())
    throw std::runtime_error("Update attempted with inappropriate criteria");

  /*Check validity of the provided key-value criteria against defined schema*/
  if(!database::Container::_IsValidFilterCriteriaForRawSelection(filter_criteria))
    throw std::runtime_error("Update attempted with invalid column identifiers");

  /*
  filter_criteria gives a column name as a key and the corresponding values on which basis data must be filtered.
  filter_comparison_params is a similar that gives what kind of comparison it should be e.g.,==,!=,<,<=,>,>= etc..
  Therefore filter_criteria and filter_comparison_types must be compatible to each other
  */
  if(!_IsValidFilteringInformation(filter_criteria,filter_comparison_params))
    throw std::runtime_error("Criteria and parameters for filtering are not compatible");

  //Iterating over each key-value pair in the filter_criteria and filter_comparison_type
  _for_each_criteria(filter_criteria,filter_comparison_params,[&](auto criteria_pair,auto comparison_pair){

    //This is the index of the column on which filtering must be applied
    std::size_t filter_column_index = std::distance(m_schema.begin(),m_schema.find(criteria_pair.first));

    //Iterating over each pair of corresponding value respective compare_type provided in the given criteria
    _for_each_comparison(criteria_pair.second,comparison_pair.second,[&](auto value,auto compare) {

      auto column = m_data -> operator[](filter_column_index);

      //Iterating over each value in the given column to check if the filter criterion is satisfied O(n^3)
      for(std::size_t i = 0; i < column.size(); i += 1)
      {
        switch (compare)
        {
          case equal_to: if(column[i] == value) _UpdateValueForIndex(i,new_value); break;
          case not_eqaul_to: if(column[i] != value) _UpdateValueForIndex(i,new_value); break;
          case greater_or_equal_to: if(column[i] >= value) _UpdateValueForIndex(i,new_value); break;
          case lesser_or_equal_to: if(column[i] <= value) _UpdateValueForIndex(i,new_value); break;
          case greater_than: if(column[i] > value) _UpdateValueForIndex(i,new_value); break;
          case lesser_than: if(column[i] < value) _UpdateValueForIndex(i,new_value); break;
        }
      }
    });
  });
}

void database::Container::_DeleteFrom(const database::impl_filter_type& filter_criteria,const database::impl_filtercompare_type& filter_comparison_params)
{
  //TODO
}

void database::Container::_Truncate()
{
  //TODO
}
