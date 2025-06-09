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

namespace nth {
    namespace nth_internal {
        class reference {
            public:
                *reference;
        }
        class stack {

        }
        class ffi {

        }
        class native {

        }
    }
    class number {
        public:
        virtual number operator +(number a, number b);
        virtual number operator -(number a, number b);
        virtual number operator *(number a, number b);
        virtual number operator /(number a, number b);
        virtual number operator %(number a, number b);
        virtual number modulo(number a, number b);
        virtual number abs(number a);
        virtual number floor(number a);
        virtual number ceiling(number a);
        virtual 
    }
    class natural : public number 
    {
        public:
            natural();
            ~natural();
            natural operator +(natural a, natural b);
            natural operator -(natural a, natural b);
            natural operator *(natural a, natural b);
            natural operator /(natural a, natural b);
            natural modulo(natural a, natural b);
            natural abs(number a);
        private:
            unsigned char length;
            unsigned char contents[];
    }
    class integer : public number 
    {
        public:
            integer();
            ~integer();
            integer operator +(integer a, integer b);
            integer operator -(integer a, integer b);
            integer operator *(integer a, integer b);
            integer operator /(integer a, integer b);
            integer modulo(integer a, integer b);
            natural abs(number a);
    }
    class rational : public number {
        public:
            rational();
            ~rational();
            rational operator +(rational a, rational b);
            rational operator -(rational a, rational b);
            rational operator *(rational a, rational b);
            rational operator /(rational a, rational b);
            rational modulo(integer a, integer b);
            rational abs(number a);

    }
    class interval : public number {}
}

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
