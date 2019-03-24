#ifndef CONTAINER_H
#define CONTAINER_H
#include <cstdlib>
#include <string>
#include <vector>
#include <exception>
#include <map>
#include <algorithm>
#include <memory>
#include <stdexcept>
#include "Paradigms.hpp"
#include "ComparableString.hpp"

namespace database
{
  struct Container
  {
    #pragma mark Private types and member properties
    private:
    std::size_t m_id;
    std::string m_name;
    std::map<std::string,std::string> m_schema;
    std::unique_ptr<std::vector<std::vector<database::ComparableString>>> m_data;

    friend class TransactionFactory;

    #pragma mark Private initializer
    private:
    Container(const std::string& name,const std::map<std::string,std::string>& schema)
    :m_name(name),m_schema(schema) { _PrepareContainer(); }
    Container() = delete;
    #pragma mark Public deallocator,copy initializers and accessors
    public:
    ~Container() {}
    Container(const database::Container& container)
    {
      std::cout << "Copy contructor called..." << "\n";
      this -> m_id = container.m_id;
      this -> m_name = container.m_name;
      this -> m_schema = container.m_schema;

      this -> m_data = std::make_unique<std::vector<std::vector<database::ComparableString>>>();
      this -> m_data -> reserve(container.m_data -> size());
      std::for_each(container.m_data -> begin(),container.m_data -> end(),[&](auto column){
        std::vector<database::ComparableString> buffer;
        buffer.reserve(column.size());
        std::for_each(column.begin(),column.end(),[&](auto data) {
          buffer.emplace_back(data);
        });
        this -> m_data -> emplace_back(buffer);
      });
    }
    std::string name() const { return m_name; }
    std::size_t id() const { return m_id; }
    std::map<std::string,std::string> schema() const { return m_schema; }

    #pragma mark Private implementation layer
    private:
    void _PrepareContainer()
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
    
    //Insert Into Container
    bool _HaveSameKeysFor(const std::map<std::string,database::ComparableString>& lhs, const std::map<std::string,std::string>& rhs) const
    {
      return (lhs.size() == rhs.size()) && std::equal(lhs.begin(),lhs.end(),rhs.begin(),[&](auto lhs_pair,auto rhs_pair){
        return lhs_pair.first == rhs_pair.first;
      });
    }
    void _InsertInto(const std::map<std::string,database::ComparableString> values)
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

    //Select all from container with one selection criterion (for now)
    bool _IsValidFilterCriterion(const std::map<std::string,database::ComparableString>& filter_criteria) const 
    {
      for(auto pair: filter_criteria)
      {
        if(m_schema.find(pair.first) == m_schema.end()) 
          return false;
      }
      return true;
    }
    void _PopulateValueIfNotExisting(std::vector<std::size_t>& vector,const std::size_t& value) const
    {
      if(std::find(vector.begin(),vector.end(),value) == vector.end())
        vector.emplace_back(value);
      else return;
    }

    std::map<std::string,std::vector<database::ComparableString>> _SelectAll() const
    {
      std::map<std::string,std::vector<database::ComparableString>> result;
      std::for_each(m_schema.begin(),m_schema.end(),[&](auto pair){
        std::size_t index = std::distance(m_schema.begin(),m_schema.find(pair.first));
        result[pair.first] = m_data -> operator[](index);
      }); 
      return result;
    }

    // std::map<std::string,std::vector<database::ComparableString>> _SelectAllWithCriteria(const std::map<std::string,database::ComparableString>& filter_criteria,const std::vector<database::ValueComparisonType>& filter_comparison_types) const
    // {
    //   if(!_IsValidFilterCriterion(filter_criteria)) //Check validity of the provided key-value against defined schema
    //     throw std::runtime_error("Select attempted with invalid columns");
      
    //   if(filter_criteria.size() != filter_comparison_types.size())
    //     throw std::runtime_error("Number of filter-criterion and filter_comparison_types must match");
    //   /*
    //   filter_criteria gives a column name as a key and the corresponding value on which basis data must be filtered.
    //   filter_comparison_types if a vector that gives what kind of comparison it should be e.g.,==,!=,<,<=,>,>= etc..
    //   Therefore filter_criteria and filter_comparison_types must match in their numbers.
    //   */
      
    //   std::vector<std::size_t> index_buffer; // Will have the final filtered indices

    //   std::for_each(filter_criteria.begin(),filter_criteria.end(),[&](auto pair){

    //     database::ValueComparisonType comparison_type = filter_comparison_types[std::distance(filter_criteria.begin(),filter_criteria.find(pair.first))];
        
    //     std::size_t data_index = std::distance(m_schema.begin(),m_schema.find(pair.first));
        
    //     // std::unique_ptr<std::vector<std::vector<database::ComparableString>>> m_data;
        
    //     auto column_vector = m_data -> operator [](data_index); //Data underneath the column name specified in the criterion
        
    //     std::for_each(column_vector.begin(),column_vector.end(),[&](auto value){
          
    //       switch(comparison_type)
    //       {
    //         case equal_to:
    //         if(value == pair.second)
    //         {
    //           std::size_t index = std::distance(column_vector.begin(),std::find(column_vector.begin(),column_vector.end(),value));
    //           _PopulateValueIfNotExisting(index_buffer,index);
    //         }
    //         break;
    //         case not_eqaul_to:
    //         if(value != pair.second)
    //         {
    //           std::size_t index = std::distance(column_vector.begin(),std::find(column_vector.begin(),column_vector.end(),value));
    //           _PopulateValueIfNotExisting(index_buffer,index);
    //         }
    //         break;
    //         case greater_than:
    //         if(value > pair.second)
    //         {
    //           std::size_t index = std::distance(column_vector.begin(),std::find(column_vector.begin(),column_vector.end(),value));
    //           _PopulateValueIfNotExisting(index_buffer,index);
    //         }
    //         break;
    //         case lesser_than:
    //         if(value < pair.second)
    //         {
    //           std::size_t index = std::distance(column_vector.begin(),std::find(column_vector.begin(),column_vector.end(),value));
    //           _PopulateValueIfNotExisting(index_buffer,index);
    //         }
    //         break;
    //         case greater_or_equal_to:
    //         if(value >= pair.second)
    //         {
    //           std::size_t index = std::distance(column_vector.begin(),std::find(column_vector.begin(),column_vector.end(),value));
    //           _PopulateValueIfNotExisting(index_buffer,index);
    //         }
    //         break;
    //         case lesser_or_equal_to:
    //         if(value <= pair.second)
    //         {
    //           std::size_t index = std::distance(column_vector.begin(),std::find(column_vector.begin(),column_vector.end(),value));
    //           _PopulateValueIfNotExisting(index_buffer,index);
    //         }
    //         break;
    //       }
    //     }); //Iterating over the specific criterion column

    //   });//Iterating over the criterion (assuming just one for now)

    //   std::map<std::string,std::vector<database::ComparableString>> result; //Extract data with the filtered indices
      
    //   std::for_each(m_schema.begin(),m_schema.end(),[&](auto pair) {
        
    //     std::size_t column_index = std::distance(m_schema.begin(),m_schema.find(pair.first));
    //     auto column = m_data -> operator[](column_index);
    //     std::vector<database::ComparableString> data_buffer;

    //     std::for_each(index_buffer.begin(),index_buffer.end(),[&](std::size_t index) {
    //       data_buffer.emplace_back(column[index]);
    //     }); //Iterating over the filtered indices.

    //     result[pair.first] = data_buffer;
    //   }); //Iterating over the schema
    //   return result;
    // }
  };
}

#endif //CONTAINER_H
