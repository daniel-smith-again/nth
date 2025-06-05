#ifndef ___NTH___
#define ___NTH___

/*
 * Nth Types:
 * Symbol
 * Number
 * List
 * Function
 * Type
 * Array
 * Tuple
 * Datatype
 * View
 * Boolean
 * Bits
*/

typedef struct { void *reference; } nth_internal_reference_type;
typedef struct { unsigned char length; unsigned char contents[]; } nth_natural_type;
typedef struct { unsigned char sign; unsigned char length; unsigned char contents[]; } nth_integer_type;
typedef struct { nth_integer_type min; nth_integer_type max; nth_natural_type value; } nth_interval_type;
typedef struct { nth_natural_type numerator; nth_integer_type denominator; } nth_rational_type;
typedef struct { unsigned char length; unsigned char contents[]; } nth_symbol_type;
typedef struct { nth_natural_type length; nth_internal_reference_type contents[]; } nth_list_type;
typedef struct { nth_list_type members; } nth_sum_type;
typedef struct { nth_list_type items; } nth_tuple_type;


#endif //___NTH___
