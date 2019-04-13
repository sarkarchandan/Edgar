#ifndef CONTAINER_H
#define CONTAINER_H
#include "ComparableString.hpp"
#include <vector>
#include <exception>
#include <map>
#include <memory>
#include <stdexcept>
#include "Paradigms.hpp"
#include <functional>

namespace database
{
  struct Container
  {
    #pragma mark Private types and member properties
    private:
    std::size_t m_id;
    std::string m_name;
    std::map<std::string,database::DataType> m_schema;
    std::unique_ptr<std::vector<std::vector<database::ComparableString>>> m_data;

    friend class TransactionFactory;

    #pragma mark Private initializer
    private:
    Container(const std::string& name,const std::map<std::string,database::DataType>& schema)
    :m_name(name),m_schema(schema) { _PrepareContainer(); }
    #pragma mark Public deallocator,copy initializers and accessors
    public:
    Container(){}
    ~Container() { m_data.release(); }
    Container(const database::Container& container)
    {
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

    Container& operator =(const database::Container& container)
    {
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
      return *this;
    }

    std::string name() const;
    std::size_t id() const;
    std::map<std::string,database::DataType> schema() const;

    #pragma mark Private implementation layer
    private:
    void _PrepareContainer();

    //Insert data into Container
    bool _HaveSameKeysFor(const std::map<std::string,database::ComparableString>& lhs, const std::map<std::string,database::DataType>& rhs) const;
    void _InsertInto(const std::map<std::string,database::ComparableString> values);

    //Select from container with given filter criteria
    bool _IsValidFilterCriteriaForRawSelection(const std::map<std::string,std::vector<database::ComparableString>>& filter_criteria) const;
    bool _IsValidFilterCriteriaForAggregateSelection(const std::map<std::string,std::vector<database::ComparableString>>& filter_criteria) const;
    bool _IsValidDataSetRequested(const std::vector<std::string>& dataset) const;
    void _SelectAll(const std::function<void(const std::map<std::string,std::vector<database::ComparableString>>&)>& lambda) const;
    void _SelectDataSet(const std::vector<std::string>& dataset,const std::function<void(const std::map<std::string,std::vector<database::ComparableString>>&)>& lambda) const;
    void _SelectRawWithCriteria(const std::map<std::string,std::vector<database::ComparableString>>& filter_criteria,const std::map<std::string,std::vector<database::ComparisonType>>& filter_comparison_params,const std::vector<std::string>& dataset,const std::function<void(const std::map<std::string,std::vector<database::ComparableString>>&)>& lambda) const;

    //Update data in the container with given filter criteria
    void _UpdateValueForIndex(const std::size_t& index,const std::map<std::string,database::ComparableString>& new_value);
    void _Update(const std::map<std::string,std::vector<database::ComparableString>>& filter_criteria,const std::map<std::string,std::vector<database::ComparisonType>>& filter_comparison_params,const std::map<std::string,database::ComparableString>& new_value);

    //Delete data from container with given filter criteria
    void _DeleteFrom(const std::map<std::string,std::vector<database::ComparableString>>& filter_criteria,const std::map<std::string,std::vector<database::ComparisonType>>& filter_comparison_params);

    //Delete all data from the container keeping the schema intact
    void _Truncate();
  };
}

#endif //CONTAINER_H
