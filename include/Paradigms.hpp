#ifndef PARADIGMS_H
#define PARADIGMS_H

namespace database
{
  enum TransactionType 
  { 
    create_database,
    create_container,
    insert_into,
    select,
    update,
    truncate,
    alter,
    delete_from,
    drop_container,
    drop_database
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
