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

nth_array *nthUTF8toUTF32(nth_array *str)
  Int l = 0;
  for (Int i = 0; i < str->length; i++)
  {
    switch((Byte)str.data[i] & 0b11110000)
    {
      case 0b11000000 : l++; i++; break;
      case 0b11100000 : l++; i += 2; break;
      case 0b11110000 : l++; i += 3; break;
      default: l++;
    }
  }
  nth_array *outstr = malloc(sizeof(nth_array) + (sizeof(Int) * l));
  outstr->length = l;
  outstr->data = outstr + (sizeof(nth_array));
  for (Int i = 0, j = 0; i < outstr->length; i++, j++)
  {
    outstr->data[i] = 0;
    switch((Byte)str.data[i] & 0b11110000)
    {
      case 0b11000000 : 
        outstr->data[j] |= (Int)str.data[i] << 3;
        outstr->data[j] |= (Int)str.data[i+1] << 2;
        i++;
        break;
      case 0b11100000 :
        outstr->data[j] |= (Int)str.data[i] << 4;
        outstr->data[j] |= (Int)str.data[i+1] << 2;
        outstr->data[j] |= (Int)str.data[i+2] << 2;
        i += 2;
        break;
      case 0b11110000 :
        outstr->data[j] |= (Int)str.data[i] << 5;
        outstr->data[j] |= (Int)str.data[i+1] << 2;
        outstr->data[j] |= (Int)str.data[i+2] << 2;
        outstr->data[j] |= (Int)str.data[i+3] << 2;
        i += 3;
        break;
      default :
        outstr->data[j] = (Int)str.data[i];
        break;
    }
  }
}

#endif
