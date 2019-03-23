#ifndef CONTAINER_H
#define CONTAINER_H
#include <cstdlib>
#include <string>
#include <vector>
#include <exception>
#include <map>
#include <algorithm>
#include <memory>

namespace database
{
  struct Container
  {
    #pragma mark Private types and member properties
    private:
    std::size_t m_id;
    std::string m_name;
    std::map<std::string,std::string> m_schema;
    std::unique_ptr<std::vector<std::vector<std::string>>> m_data;

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
      this -> m_id = container.m_id;
      this -> m_name = container.m_name;
      this -> m_schema = container.m_schema;
      
      this -> m_data = std::make_unique<std::vector<std::vector<std::string>>>();
      this -> m_data -> reserve(container.m_data -> size());
      std::for_each(container.m_data -> begin(),container.m_data -> end(),[&](const std::vector<std::string>& column){
        std::vector<std::string> buffer;
        buffer.reserve(column.size());
        std::for_each(column.begin(),column.end(),[&](const std::string& data) {
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
      m_data = std::make_unique<std::vector<std::vector<std::string>>>();
      m_data -> reserve(m_schema.size());
    }
    

    // void _AdjustRowsIfNeeded()
    // {
    //   if(m_number_of_rows < m_row_capacity)
    //     return;
    //   else
    //   {
    //     for(std::size_t i = 0; i < m_number_of_columns; i += 1)
    //     {
    //       std::string* temp = new std::string[m_number_of_rows];
    //       for(std::size_t j = 0; j < m_number_of_rows; j += 1)
    //         *(temp + j) = m_data[i][j];
          
    //       delete[] *(m_data + i);

    //       *(m_data + i) = new std::string[2 * m_row_capacity];
    //       for(std::size_t k = 0; k < m_number_of_rows; k += 1)
    //         m_data[i][k] = *(temp + k);

    //       delete[] temp;
    //     }
    //     m_row_capacity *= 2;
    //   }
    // }

    // void _Insert(const std::unordered_map<std::string,std::string>& values)
    // {
    //   _AdjustRowsIfNeeded();
    //   for(std::size_t i = 0; i < m_number_of_columns; i += 1)
    //   { 
    //     if(values.find(*(m_data_metadata + i)) != values.end())
    //       m_data[i][m_number_of_rows] = values.at(*(m_data_metadata + i));
    //     else
    //       m_data[i][m_number_of_rows] = "NULL";
    //   }
    //   m_number_of_rows += 1;
    // }

    // std::size_t _FindIndexForFilterColumn(const std::string& key) const
    // {
    //   for(std::size_t i = 0; i < m_number_of_columns; i += 1)
    //   {
    //     if(*(m_data_metadata + i) == key)
    //       return i;
    //   }
    //   return -1;
    // }

    // std::unordered_map<std::string,std::vector<std::string>> _Read(const std::unordered_map<std::string,std::string>& filter)const
    // {
    //   if(filter.size() > 1)
    //     throw std::runtime_error("More than one filter is not implemented yet");
    //   const std::size_t column_index = _FindIndexForFilterColumn(filter.begin() -> first);
    //   if(column_index == -1)
    //     throw std::invalid_argument("Invalid filter clause provided");
      
    //   std::unordered_map<std::string,std::vector<std::string>> result;

    //   std::vector<std::size_t> buffer;
    //   for(std::size_t i = 0; i < m_number_of_rows; i += 1)
    //   {
    //     if(m_data[column_index][i] == filter.begin() -> second)
    //       buffer.emplace_back(i);
    //   }

    //   for(std::size_t i = 0; i < m_number_of_columns; i += 1)
    //   {
    //     for(std::size_t j: buffer)
    //     {
    //       if(result.find(m_data_metadata[i]) == result.end())
    //         result[m_data_metadata[i]] = {m_data[i][j]};
    //       else
    //         result[m_data_metadata[i]].emplace_back(m_data[i][j]);
    //     }   
    //   }
    //   return result;
    // }
    
  };
}

#endif //CONTAINER_H