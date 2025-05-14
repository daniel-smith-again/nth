#ifndef _NTH_
#define _NTH_

#include <stdint.h>
typedef uint64_t Nat;
typedef uint8_t Byte;
#define _nth_flag_eq    '='
#define _nth_flag_neq   '!'
#define _nth_flag_l     '<'
#define _nth_flag_le    'l'
#define _nth_flag_g     '>'
#define _nth_flag_ge    'g'

typedef struct _nth_context
{
  Byte[4] Flags;
  union
  {
    struct { void* PC; void* FP; void* SP; };
    Nat[256] RegFile;
  };
  Nat Handlers;
  void** HandlerList;
  void (*claim)();
  void (*drop)();
} nth_context;

int nth_init_context(nth_context *ctxt, void (*allocator)(), void (*deallocator)());
int nth_register_handlers(nth_context *ctxt, const void* handlers, int nhandlers);
int nth_execute(nth_context *ctxt, Byte* code, Byte* initial_page, Nat page_size)
{
  ctxt->Flags = 0;
  ctxt->PC = code[0];
  ctxt->FP = initial_page;
  ctxt->SP = initial_page + page_size;

  Byte[4] = fetch;
  while(1)
  {
    fetch[0] = *(ctxt->PC), fetch[1] = *(ctxt->PC + 1), fetch[2] = *(ctxt->PC + 2), fetch[3] = *(ctxt->PC + 3);
    ctxt->PC += 3;
    switch(fetch[0])
    {
      case 0:
        break;
      case 1:
        break;
      case 2:
        break;
      case 3:
        break;
      case 4:
        break;
      case 5:
        break;
      case 6:
        break;
      case 7:
        break;
      case 8:
        break;
      case 9:
        break;
      case 10:
        break;
      case 11:
        break;
      case 12:
        break;
      case 13:
        break;
      case 14:
        break;
      case 15:
        break;
      case 16:
        break;
      case 17:
        break;
      case 18:
        break;
      case 19:
        break;
      case 20:
        break;
      case 21:
        break;
      case 22:
        break;
      case 23:
        break;
      case 24:
        break;
      case 25:
        break;
      case 26:
        break;
      case 27:
        break;
      case 28:
        break;
      case 29:
        break;
      case 30:
        break;
      case 31:
        break;
      case 32:
        break;
      default:
        break;
    }
  }
}


#endif //_NTH_
