#ifndef TABLE_H
#define TABLE_H

#include <cstdlib>
#include <string>
#include <unordered_map>
#include <vector>
#include <exception>

namespace database
{
  struct Table
  {
    #pragma mark Private types and member properties
    private:
    std::size_t m_table_id;
    std::string m_table_name;
    std::size_t m_number_of_columns;
    std::size_t m_row_capacity;
    std::size_t m_number_of_rows;
    std::string *m_column_names;
    std::string **m_columns;

    friend class TransactionFactory;

    #pragma mark Private initializer
    private:
    Table(const std::string& table_name,const std::size_t& number_of_columns)
    :m_table_name(table_name),m_number_of_columns(number_of_columns),m_number_of_rows(0),m_row_capacity(10)
    {
      std::hash<std::string> string_hash;
      m_table_id = string_hash(m_table_name);
      m_column_names = new std::string[m_number_of_columns];

      m_columns = new std::string*[m_number_of_columns * sizeof(std::string*)];
      for(std::size_t i = 0; i < m_number_of_columns; i += 1)
        *(m_columns + i) = new std::string[m_row_capacity];
    }

    #pragma mark Public deallocator
    public:
    ~Table()
    {
      delete[] m_column_names;
      for(std::size_t i = 0; i < m_number_of_columns; i += 1)
        delete[] *(m_columns + i);
      delete[] m_columns;
    }

    #pragma mark Public copy initializer
    Table(const database::Table& table)
    {
      this -> m_table_id = table.m_table_id;
      this -> m_table_name = table.m_table_name;
      this -> m_number_of_columns = table.m_number_of_columns;
      this -> m_row_capacity = table.m_row_capacity;
      this -> m_number_of_rows = table.m_number_of_rows;
      
      this -> m_column_names = new std::string[m_number_of_columns];
      for(std::size_t i = 0; i < m_number_of_columns; i += 1)
        *(m_column_names + i) = *(table.m_column_names + i);
        
      this -> m_columns = new std::string*[m_number_of_columns * sizeof(std::string*)];
      for(std::size_t i = 0; i < m_number_of_columns; i += 1)
        *(m_columns + i) = new std::string[m_row_capacity];
      
      for(std::size_t i = 0; i < m_number_of_columns; i += 1)
      {
        for(std::size_t j = 0; j < m_number_of_rows; j += 1)
          this -> m_columns[i][j] = table.m_columns[i][j];
      }
    }
    
    #pragma mark Private implementation layer
    private:
    void _AdjustRowsIfNeeded()
    {
      if(m_number_of_rows < m_row_capacity)
        return;
      else
      {
        for(std::size_t i = 0; i < m_number_of_columns; i += 1)
        {
          std::string* temp = new std::string[m_number_of_rows];
          for(std::size_t j = 0; j < m_number_of_rows; j += 1)
            *(temp + j) = m_columns[i][j];
          
          delete[] *(m_columns + i);

          *(m_columns + i) = new std::string[2 * m_row_capacity];
          for(std::size_t k = 0; k < m_number_of_rows; k += 1)
            m_columns[i][k] = *(temp + k);

          delete[] temp;
        }
        m_row_capacity *= 2;
      }
    }

    void _Insert(const std::unordered_map<std::string,std::string>& values)
    {
      _AdjustRowsIfNeeded();
      for(std::size_t i = 0; i < m_number_of_columns; i += 1)
      { 
        if(values.find(*(m_column_names + i)) != values.end())
          m_columns[i][m_number_of_rows] = values.at(*(m_column_names + i));
        else
          m_columns[i][m_number_of_rows] = "NULL";
      }
      m_number_of_rows += 1;
    }

    std::size_t _FindIndexForFilterColumn(const std::string& key) const
    {
      for(std::size_t i = 0; i < m_number_of_columns; i += 1)
      {
        if(*(m_column_names + i) == key)
          return i;
      }
      return -1;
    }

    std::unordered_map<std::string,std::vector<std::string>> _Read(const std::unordered_map<std::string,std::string>& filter)const
    {
      if(filter.size() > 1)
        throw std::runtime_error("More than one filter is not implemented yet");
      const std::size_t column_index = _FindIndexForFilterColumn(filter.begin() -> first);
      if(column_index == -1)
        throw std::invalid_argument("Invalid filter clause provided");
      
      std::unordered_map<std::string,std::vector<std::string>> result;

      std::vector<std::size_t> buffer;
      for(std::size_t i = 0; i < m_number_of_rows; i += 1)
      {
        if(m_columns[column_index][i] == filter.begin() -> second)
          buffer.emplace_back(i);
      }

      for(std::size_t i = 0; i < m_number_of_columns; i += 1)
      {
        for(std::size_t j: buffer)
        {
          if(result.find(m_column_names[i]) == result.end())
            result[m_column_names[i]] = {m_columns[i][j]};
          else
            result[m_column_names[i]].emplace_back(m_columns[i][j]);
        }   
      }
      return result;
    }
    
  };
}

#endif //TABLE_H