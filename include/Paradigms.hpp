#ifndef PARADIGMS_H
#define PARADIGMS_H

namespace database
{
  enum TransactionMetaType
  {
    ddl,
    dml
  };

  enum TransactionType
  {
    create_database,//ddl
    create_container,//ddl
    insert_into,//dml
    select_all,//dml
    select_dataset,//dml
    update,//dml
    truncate,//dml
    // alter,//ddl
    delete_from,//dml
    drop_container,//ddl
    drop_database//ddl
  };

  enum ComparisonType
  {
    equal_to,
    not_eqaul_to,
    greater_or_equal_to,
    lesser_or_equal_to,
    greater_than,
    lesser_than
  };

  // enum AssociationType
  // {
  //   logical_and,
  //   logical_or,
  //   logical_not,
  //   none
  // };

  enum PresentationType
  {
    group_by,
    order_by
  };

  enum QueryDataType
  {
    integer,
    string,
    boolean
  };

  enum DataType
  {
    quantifiable,
    non_quantifiable,
    other
  };

  enum AggregationType
  {
    count,
    average,
    min,
    max,
    sum
  };
}

#endif //PARADIGMS_H
