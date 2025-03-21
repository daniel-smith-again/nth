#ifndef NTH_LIB
#define NTH_LIB

typedef void* Ref;
typedef void* Nat; //predictable size of max-cpu-register-width.
typedef unsigned char Byte;
typedef struct {Nat size; Byte* contents;} Interval;
typedef struct {Nat size; Byte* contents;} Natural;
typedef struct {Natural numerator; Natural denominator;} Integer;
typedef struct {Nat size; Ref *items;} List;
typedef struct {Ref *type; Nat length; Byte* contents;} Array;
typedef struct {Nat length; Byte* contents;} Symbol;
/*
 * add number
 * subtract number
 * multiply number
 * divide number
 * raise number
 * log number
 * root number
 * add interval
 * subtract interval
 * multiply interval
 * divide interval
 * raise interval
 * log interval
 * root interval 
 * apply function 
 * apply over list
 * get argument type
 * get return type
 * list concatenate
 * string concatenate
 * define
 *
 */

#endif
