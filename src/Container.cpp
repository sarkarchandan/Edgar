#include "Container.hpp"

std::string database::Container::name() const { return m_name; }
std::size_t database::Container::id() const { return m_id; }
std::map<std::string,std::string> database::Container::schema() const { return m_schema; }

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

bool database::Container::_HaveSameKeysFor(const std::map<std::string,database::ComparableString>& lhs, const std::map<std::string,std::string>& rhs) const
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

bool database::Container::_IsValidFilterCriteria(const std::map<std::string,database::ComparableString>& filter_criteria) const
{
  for(auto pair: filter_criteria)
  {
    if(m_schema.find(pair.first) == m_schema.end())
      return false;
  }
  return true;
}
    
void database::Container::_PopulateValueIfNotExisting(std::vector<std::size_t>& vector,const std::size_t& value) const
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

std::map<std::string,std::vector<database::ComparableString>> database::Container::_SelectWithCriteria(const std::map<std::string,database::ComparableString>& filter_criteria,const std::vector<database::ValueComparisonType>& filter_comparison_types) const
{
  if(!_IsValidFilterCriteria(filter_criteria)) //Check validity of the provided key-value against defined schema
    throw std::runtime_error("Select attempted with invalid columns");

  if(filter_criteria.size() != filter_comparison_types.size())
    throw std::runtime_error("Number of filter-criterion and filter_comparison_types must match");
  /*
  filter_criteria gives a column name as a key and the corresponding value on which basis data must be filtered.
  filter_comparison_types if a vector that gives what kind of comparison it should be e.g.,==,!=,<,<=,>,>= etc..
  Therefore filter_criteria and filter_comparison_types must match in their numbers.
  */

  std::vector<std::size_t> index_buffer; // Will have the final filtered indices

  std::for_each(filter_criteria.begin(),filter_criteria.end(),[&](auto pair){

    database::ValueComparisonType comparison_type = filter_comparison_types[std::distance(filter_criteria.begin(),filter_criteria.find(pair.first))];

    std::size_t data_index = std::distance(m_schema.begin(),m_schema.find(pair.first));

    // std::unique_ptr<std::vector<std::vector<database::ComparableString>>> m_data;

    auto column_vector = m_data -> operator [](data_index); //Data underneath the column name specified in the criterion

    std::for_each(column_vector.begin(),column_vector.end(),[&](auto value){

      switch(comparison_type)
      {
        case equal_to:
        if(value == pair.second)
        {
          std::size_t index = std::distance(column_vector.begin(),std::find(column_vector.begin(),column_vector.end(),value));
          _PopulateValueIfNotExisting(index_buffer,index);
        }
        break;
        case not_eqaul_to:
        if(value != pair.second)
        {
          std::size_t index = std::distance(column_vector.begin(),std::find(column_vector.begin(),column_vector.end(),value));
          _PopulateValueIfNotExisting(index_buffer,index);
        }
        break;
        case greater_than:
        if(value > pair.second)
        {
          std::size_t index = std::distance(column_vector.begin(),std::find(column_vector.begin(),column_vector.end(),value));
          _PopulateValueIfNotExisting(index_buffer,index);
        }
        break;
        case lesser_than:
        if(value < pair.second)
        {
          std::size_t index = std::distance(column_vector.begin(),std::find(column_vector.begin(),column_vector.end(),value));
          _PopulateValueIfNotExisting(index_buffer,index);
        }
        break;
        case greater_or_equal_to:
        if(value >= pair.second)
        {
          std::size_t index = std::distance(column_vector.begin(),std::find(column_vector.begin(),column_vector.end(),value));
          _PopulateValueIfNotExisting(index_buffer,index);
        }
        break;
        case lesser_or_equal_to:
        if(value <= pair.second)
        {
          std::size_t index = std::distance(column_vector.begin(),std::find(column_vector.begin(),column_vector.end(),value));
          _PopulateValueIfNotExisting(index_buffer,index);
        }
        break;
      }
    }); //Iterating over the specific criterion column

  });//Iterating over the criterion (assuming just one for now)

  std::map<std::string,std::vector<database::ComparableString>> result; //Extract data with the filtered indices

  std::for_each(m_schema.begin(),m_schema.end(),[&](auto pair) {

    std::size_t column_index = std::distance(m_schema.begin(),m_schema.find(pair.first));
    auto column = m_data -> operator[](column_index);
    std::vector<database::ComparableString> data_buffer;

    std::for_each(index_buffer.begin(),index_buffer.end(),[&](std::size_t index) {
      data_buffer.emplace_back(column[index]);
    }); //Iterating over the filtered indices.

    result[pair.first] = data_buffer;
  }); //Iterating over the schema
  return result;
}




