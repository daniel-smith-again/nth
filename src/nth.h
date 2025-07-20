/***************************************************************************** /

--------------------------------------------------------------------------------
|==============================================================================|
|                    The nth Programming Language Reference                    |
|==============================================================================|
|                                      by                                      |
|                                Daniel Smith                                  |
|                         daniel.smith.again@gmail.com                         |
|                                                                              |
|                                                                              |
|.................................... About ...................................|
|                                                                              |
|                 This is a fully conforming implementation of                 |
|                 the Nth Programming Language as described in                 |
|          "The nth Programming Language Interactive Specification".           |
|                                                                              |
|         Licensing information is included at the bottom of this file.        |
|                                                                              |
|                The interpreter is designed as a "header-only"                |
|                  library written in C. It is intended to be                  |
|                 incorporated into a "host" program which will                |
|                    supply some host-dependent capabilities                   |
|                          to the interpreter runtime.                         |
|                                                                              |
|                                                                              |
|.....................................API......................................|
|                                                                              |
|               Exported things are prefixed with "Nth", and begin             | 
|              with a capitol letter. Functions which are internal             |
|                      are prefixed with "_nth_internal".                      |
|______________________________________________________________________________|

/ *****************************************************************************/

#ifndef ___NTH___
#define ___NTH___

#include <cstdint>
#include <stddef.h>

typedef uint8_t Byte;
typedef intptr_t Value;
typedef Byte* Segment;
typedef struct { Value Instruction; Value Frame; Value Top; } NthContext;
typedef Byte (_nth_internal_operation)(NthContext)
void _nth_internal_next_op(NthContext *n)
{

}
void _nth_internal_execute(NthContext *n)
{
    for (x = n.Code[0];_nth_internal_next_op(n);)
    {
        switch(x)
        {
            case 0: goto done;
            case 1: break;
            case 2: break;
            case 3: break;
            case 4: break;
            case 5: break;
            case 6: break;
            case 7: break;
            case 8: break;
            case 9: break;
            case 10: break;
            case 11: break;
            case 12: break;
            case 13: break;
        }
    }
    done: return;
}

typedef struct _internal_nth_context NthContext;
typedef struct _internal_nth_data NthData;

void _nth_next_op(NthContext *n);
void _nth_execute_program(NthContext *n);
void _nth_stack_reference(NthContext *n);

typedef struct
{
    Segment Code;
    Value Instruction;
    Value Frame;
    Value Top;
} _internal_nth_context;

typedef struct 
{
    Segment Prev;
    Value PageFlag;
    Value Locals;
    Segment Data;
} _internal_nth_stack_frame;

void _nth_execute(Segment Code)
{
    for()
}



void _nth_internal_fetch_op(NthContext *n);
void _nth_internal_init_frame(NthContext *n, void(*c)(NthContext*));
void _nth_intenral_function_call(NthContext *n, void(*c)(NthContext*));
void _nth_internal_push(NthContext *n, void(*c)(NthContext*));
void _nth_internal_function_return(NthContext *n, void(*c)(NthContext*));
void _nth_internal_handler_call(NthContext *n, void(*c)(NthContext*));
void _nth_internal_arithmetic_add(NthContext *n, void(*c)(NthContext*));
void _nth_internal_arithmetic_subtract(NthContext *n, void(*c)(NthContext*));
void _nth_internal_arithmetic_multiply(NthContext *n, void(*c)(NthContext*));
void _nth_internal_arithmetic_quotient(NthContext *n, void(*c)(NthContext*));
void _nth_internal_arithmetic_remainder(NthContext *n, void(*c)(NthContext*));
void _nth_internal_arithmetic_modulus(NthContext *n, void(*c)(NthContext*));
void _nth_internal_arithmetic_exponent(NthContext *n, void(*c)(NthContext*));
void _nth_internal_arithmetic_root(NthContext *n, void(*c)(NthContext*));
void _nth_internal_arithmetic_logarithm(NthContext *n, void(*c)(NthContext*));
void _nth_internal_boolean_conjunction(NthContext *n, void(*c)(NthContext*));
void _nth_internal_boolean_disjunction(NthContext *n, void(*c)(NthContext*));
void _nth_internal_boolean_negation(NthContext *n, void(*c)(NthContext*));
void _nth_internal_list_concatenate(NthContext *n, void(*c)(NthContext*));
void _nth_internal_list_subscript(NthContext *n, void(*c)(NthContext*));
void _nth_internal_constructor_function(NthContext *n, void(*c)(NthContext*));
void _nth_internal_constructor_list(NthContext *n, void(*c)(NthContext*));
void _nth_internal_constructor_number(NthContext *n, void(*c)(NthContext*));
void _nth_internal_constructor_boolean(NthContext *n, void(*c)(NthContext*));
void _nth_internal_constructor_type(NthContext *n, void(*c)(NthContext*));
void _nth_internal_constructor_array(NthContext *n, void(*c)(NthContext*));
void _nth_internal_constructor_interval(NthContext *n, void(*c)(NthContext*));
void _nth_internal_constructor_natural(NthContext *n, void(*c)(NthContext*));
void _nth_internal_constructor_integer(NthContext *n, void(*c)(NthContext*));
void _nth_internal_constructor_fraction(NthContext *n, void(*c)(NthContext*));
void _nth_internal_define_binding(NthContext *n, void(*c)(NthContext*));
void _nth_internal_define_function(NthContext *n, void(*c)(NthContext*));
void _nth_internal_define_data(NthContext *n, void(*c)(NthContext*));
void _nth_internal_define_syntax(NthContext *n, void(*c)(NthContext*));
void _nth_internal_load_module(NthContext *n, void(*c)(NthContext*));
void _nth_internal_export_module(NthContext *n, void(*c)(NthContext*));

struct _internal_nth_data
{
    Value length;
    Segment contents;
};

struct _internal_nth_context
{
    NthContext* self;
    Value(*allocate)(Value);
    Value(*deallocate)(Value);
    Segment frame;
    Segment top;
    Value frameMax;
};

NthContext*NthInitializeContext(NthContext *c, Value(*a)(Value), Value(*d)(Value))
{
    return c->self = c, c->allocate = a, c->deallocate = d, c;
}

#endif //___NTH___


/***************************************************************************** /
________________________________________________________________________________
|==============================================================================|
|                                    License                                   |
|==============================================================================|
|                                                                              |
|                                                                              |
|...................................Preamble...................................|
|                                                                              |
|                 The author(s) grants the holder of this work                 |
|            full right of access, without limitation, irrevocably,            |
|                 in perpetuity, for any purpose, by any means.                |
|                                                                              |
|          Authorship of this work is reserved only for contributions          |
|      to this work which are made under the terms given in this section.      |
|            Contribution to this work under terms other than those            |
|                given in this section constitutes authorship of               |
|                    a distinct and separate work, for which                   |
|               the status of authorship of this particular work               |
|                                is not granted.                               |
|                                                                              |
|            In plain terms, if you, the rights holder of this work,           |
|          create any derivation, modification, or copy of this work,          |
|           you cannot claim that it is a form, version, or variation          |
|                              of the work titled                              |
|             "The nth Programming Language Reference Interpreter"             |
|         without providing it under the terms set out in this section.        |
|                                                                              |
|               The purpose of these terms is to provide the most              |
|               permissive rights and allowances while preventing              |
|               any individual or party from claiming authorship               |
|                          with less permissive rights                         |
|                and thus tainting the availability of the work.               |
|                                                                              |
|                                                                              |
|.............................STATEMENT OF WARRANTY............................|
|                                                                              |
|       THIS SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       |
|               EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO               |
|     THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE,     |
|                             AND NONINFRINGEMENT.                             |
|                                                                              |
|        IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES,       |
|     OR OTHER LIABILITY, WHETHER IN ACTION OF CONTRACT, TORT, OR OTHERWISE    |
|     ARISING FROM, OUT OF, OR IN CONNECTION WITH THE SOFTWARE, OR THE USE     |
|                      OR OTHER DEALINGS IN THE SOFTWARE.                      |
|______________________________________________________________________________|

/ *****************************************************************************/