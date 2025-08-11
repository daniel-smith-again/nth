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

typedef struct {unsigned int value : 8;} Byte;
typedef struct {signed int value : 32;} Chunk;
typedef void* Reference;
typedef Byte* String;
typedef String Symbol;
typedef struct {Chunk length; Reference *items;} List;
typedef Byte* Natural;
typedef struct {Byte sign; Natural digits;} Integer;
typedef struct __nth_internal_page Page;
struct {Reference prev; Reference next; Reference size; Byte* contents;} __nth_internal_page;
typedef struct {List symbols; List data;} Module;
typedef struct 
{
    Module *module;
    Page *pages;
    Reference(*page)(void);
    void(*drop)(Reference);
    void(*print)(String);
    String(*read)(void);
} Session;

Module* NthInitModule(Module *module)
{
    module->symbols.length.value = 0;
    module->data.length.value = 0;
}

Session* NthInitSession(Session* s, Module* module, Reference(*page)(void), void(*drop)(Reference), void(*print)(String), String(*read)(void))
{
    s->module = module;
    s->pages = 0;
    s->page = page;
    s->drop = drop;
    s->print = print;
    s->read = read;
}

typedef struct 
{
    struct {Reference PC; Reference FP; Reference SP;} 
} NthContext;

void __nth_bytecode_

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
