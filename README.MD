# performance comparison
 Benchmark | Times |
| :--- | :--- |
|DecimalBoost_Addition                   |     5094 ns |
|DecimalBoost_Subtraction                |     5180 ns |
|DecimalBoost_Multiplication             |     5742 ns |
|DecimalBoost_Division                   |    10834 ns |
|DecimalBoost_CompoundAddition           |     4331 ns |
|DecimalBoost_CompoundSubtraction        |     4404 ns |
|DecimalBoost_CompoundMultiplication     |     4981 ns |
|DecimalBoost_CompoundDivision           |    10550 ns |
|DecimalBoost_Comparison                 |     4851 ns |
|DecimalCNL_Addition                     |      127 ns |
|DecimalCNL_Subtraction                  |      126 ns |
|DecimalCNL_Multiplication               |      184 ns |
|DecimalCNL_Division                     |      190 ns |
|DecimalCNL_CompoundAddition             |      129 ns |
|DecimalCNL_CompoundSubtraction          |      131 ns |
|DecimalCNL_CompoundMultiplication       |      192 ns |
|DecimalCNL_CompoundDivision             |      194 ns |
|DecimalCNL_Comparison                   |      230 ns |
|DecimalCustomer_Addition                |     45.4 ns |
|DecimalCustomer_Subtraction             |     46.7 ns |
|DecimalCustomer_Multiplication          |     68.5 ns |
|DecimalCustomer_Division                |     63.9 ns |
|DecimalCustomer_CompoundAddition        |     41.8 ns |
|DecimalCustomer_CompoundSubtraction     |     41.8 ns |
|DecimalCustomer_CompoundMultiplication  |     58.8 ns |
|DecimalCustomer_CompoundDivision        |     57.7 ns |
|DecimalCustomer_Comparison              |     51.7 ns |
|DecimalFix16_Addition                   |     29.7 ns |
|DecimalFix16_Subtraction                |     29.0 ns |
|DecimalFix16_Multiplication             |     29.0 ns |
|DecimalFix16_Division                   |     49.6 ns |
|DecimalFix16_CompoundAddition           |     22.8 ns |
|DecimalFix16_CompoundSubtraction        |     22.4 ns |
|DecimalFix16_CompoundMultiplication     |     23.1 ns |
|DecimalFix16_CompoundDivision           |     45.4 ns |
|DecimalFix16_Comparison                 |     26.9 ns |

## DecimalBoost
It uses boost::cpp_dec_float_50 as the underlying data type. It has no precision limitations, but its performance is poor.
## DecimalCNL
It uses int64_t as the underlying data type. The decimal precision is adjustable, and it supports various arithmetic operations.
## DecimalCustomer
It uses int64_t as the underlying data type. The decimal precision is adjustable. Currently, only basic arithmetic operations have been developed.
## DecimalFix16
It uses int32_t as the underlying data type. It offers the best performance and supports various arithmetic operations, but it cannot represent large numbers.
## Summary
In summary, based on DecimalCustomer, the implementation of DecimalFix16 should be introduced to support more arithmetic operations.