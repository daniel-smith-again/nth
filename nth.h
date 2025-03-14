#ifndef NTH_LIB
#define NTH_LIB

#define Int unsigned int
#define Byte unsigned char
#define Ref void*

typedef struct { Int length; Ref contents; } nth_String;
typedef struct { Int length; Ref numerator; Ref denominator; } nth_Number;
typedef struct { Int length; Ref contents; } nth_List;
typedef struct 
{ enum {string, number, list} type; 
  union { nth_String string; nth_Number number; nth_List list; };
} nth_Data;
typedef struct {} nth_Pool;

nth_Pool* nth_create_pool ();

#endif
