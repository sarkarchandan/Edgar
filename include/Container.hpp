#ifndef CONTAINER_H
#define CONTAINER_H
#include <exception>
#include <memory>
#include <stdexcept>
#include <functional>
#include "Aliases.hpp"

namespace database
{
  struct Container
  {
    #pragma mark Private types and member properties
    private:
    std::size_t m_id;
    std::string m_name;
    database::impl_schema_type m_schema;
    std::unique_ptr<database::impl_storage_type> m_data;

    friend class TransactionFactory;

    #pragma mark Private initializer
    private:
    Container(const std::string& name,const database::impl_schema_type& schema)
    :m_name(name),m_schema(schema) { _PrepareContainer(); }
    #pragma mark Public deallocator,copy initializers and accessors
    public:
    Container()
    { 
      m_id = 0;
      m_name = "";
      m_schema = {};
      m_data = std::make_unique<database::impl_storage_type>();
    }
    ~Container() { m_data.release(); }
    Container(const database::Container& container)
    {
      this -> m_id = container.m_id;
      this -> m_name = container.m_name;
      this -> m_schema = container.m_schema;

      this -> m_data = std::make_unique<database::impl_storage_type>();
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

      this -> m_data = std::make_unique<database::impl_storage_type>();
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
    database::impl_schema_type schema() const;

    #pragma mark Private implementation layer
    private:
    void _PrepareContainer();

    bool _HaveSameKeysFor(
      const database::impl_insert_update_type& lhs, 
      const database::impl_schema_type& rhs
    ) const;

    void _InsertInto(
      const impl_insert_update_type& values
    );

    bool _IsValidFilterCriteriaForRawSelection(
      const database::impl_filter_type& filter_criteria) const;

    bool _IsValidFilterCriteriaForAggregateSelection(
      const database::impl_filter_type& filter_criteria) const;

    bool _IsValidDataSetRequested(
      const std::vector<std::string>& dataset) const;

    void _SelectAll(
      const std::function<void(const database::impl_dataset_type&)>& lambda) const;

    void _SelectRawDataSet(
      const std::vector<std::string>& dataset,
      const std::function<void(const database::impl_dataset_type&)>& lambda) const;

    void _SelectRawDataSetWithCriteria(
      const database::impl_filter_type& filter_criteria,
      const std::vector<std::string>& dataset,
      const std::function<void(const database::impl_dataset_type&)>& lambda) const;

    //Update data in the container with given filter criteria
    void _UpdateValueForIndex(
      const std::size_t& index,
      const impl_insert_update_type& new_value);

    void _Update(
      const database::impl_filter_type& filter_criteria,
      const database::impl_insert_update_type& new_value);

    
    void _DeleteFrom(
      const database::impl_filter_type& filter_criteria);

    
    void _Truncate();
  };
}

#endif //CONTAINER_H
