#ifndef _nth_header_lib_
#define _nth_header_lib_

#define Byte unsigned char
#define Int unsigned int

typedef struct { Byte data; } nth_boolean;
typedef struct { Int data; } nth_character;
typedef struct { Int data; } nth_number;
typedef struct { Int length; void* data; } nth_array;
typedef struct { Int length; void** data; } nth_tuple;
typedef struct { Int length; void** data; } nth_type;
typedef struct { Int size; void* data; } nth_data;



#endif
