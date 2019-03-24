#ifndef PARADIGMS_H
#define PARADIGMS_H

namespace database
{
  enum ValueComparisonType
  {
    equal_to,
    not_eqaul_to,
    greater_or_equal_to,
    lesser_or_equal_to,
    greater_than,
    lesser_than
  };

  enum ValuePresentationType
  {
    group_by,
    order_by
  };

  // enum ValueAggregationType
  // {
  //   count,
  //   average,
  //   min,
  //   max,
  //   sum
  // };
}

#endif //PARADIGMS_H
