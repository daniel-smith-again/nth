/******************************************************************************
-------------------------------------------------------------------------------
                        The nth Programming Environment                         
-------------------------------------------------------------------------------
                                       by                                      
                                  Daniel Smith                                  


-------------------------------------------------------------------------------
                                     About                                     
-------------------------------------------------------------------------------

     This is a fully working implementation of the nth Programming Language    
     as described in "The nth Programming Language HyperDoc". It is a fully    
          conforming implementation as described in the "Conformance"          
                      section of the chapter, "Foreword".                      


-------------------------------------------------------------------------------
                                  Terms of Use                                 
-------------------------------------------------------------------------------

                  The author(s) grants the holder of this work                 
             full right of access, without limitation, irrevocably,            
                 in perpetuity, for any purpose, by any means.                 

           Authorship of this work is reserved only for contributions          
       to this work which are made under the terms given in this section.      
             Contribution to this work under terms other than those            
                given in this section constitutes authorship of                
                    a distinct and separate work, for which                    
                the status of authorship of this particular work               
                                is not granted.                                

            In plain terms, if you, the rights holder of this work,            
           create any derivation, modification, or copy of this work,          
           you cannot claim that it is a form, version, or variation           
              of the work titled "The nth Programming Environment"             
         without providing it under the terms set out in this section.         

               The purpose of these terms is to provide the most               
               permissive rights and allowances while preventing               
                any individual or party from claiming authorship               
                          with less permissive rights                          
                and thus tainting the availability of the work.                


-------------------------------------------------------------------------------
                             STATEMENT OF WARRANTY                             
-------------------------------------------------------------------------------

        THIS SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       
                EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO               
      THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE,     
                              AND NONINFRINGEMENT.                             

        IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES,        
   OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT, OR OTHERWISE,   
     ARISING FROM, OUT OF, OR IN CONNECTION WITH THE SOFTWARE, OR THE USE,     
                       OR OTHER DEALINGS IN THE SOFTWARE.                      


*******************************************************************************/


#include <stdio.h>
#include <stdlib.h>

typedef unsigned int Int;
typedef enum 
        { 
          symbol, 
          number, 
          string,
          stringsegment,
          expression,
          collection,
          type,
          quote, 
          unquote, 
          requote 
        } Kind;
typedef struct nth__syntax
{
  Kind kind;
  Int size;
  union { Int content; struct nth__syntax **nodes; };
} Syntax;
Int InsideQuote;

struct
{
  char *content;
  Int size;
  Int i;
  int e;
  Syntax *top;
  Syntax **nodes;
  Int count;
} input;

char __nth_consume_char();
char __nth_peek_char();
Syntax *__nth_syntax_create_node(Kind k);
void __nth_syntax_append_node(Syntax *a, Syntax *b);
void __nth_clean_buffer();
void __nth_wipe_buffer();
Syntax *__nth_syntax_parse_buffer();
Syntax *__nth_syntax_parse_expression();
Syntax *__nth_syntax_parse_collection();
Syntax *__nth_syntax_parse_type();
Syntax *__nth_syntax_parse_symbol();
Syntax *__nth_syntax_parse_string();
void __nth_print_syntax(Syntax *s);

int main ()
{
  __nth_clean_buffer();
  for(;;)
  {
    input.top = __nth_syntax_parse_buffer();
    if (input.top)
    {
      __nth_print_syntax(input.top);
      putchar('\n');
    }
    else if (input.e > -1)
    {
      printf("\x1b[30;107m?\x1b[0m\n");
      __nth_wipe_buffer();
    }
    else __nth_clean_buffer();
  }
  return 0;
}

char __nth_consume_char()
{
  if (input.i >= input.size)
  {
    for ( char c = 0;;
          c = getchar(),
          input.size++,
          input.content = realloc(input.content, sizeof(char) * input.size),
          input.content[input.size-1] = c)
      {if (c == '\n') break;}
  }
  char c = input.content[input.i];
  input.i++;
  return c;
}

char __nth_peek_char()
{
  char c = __nth_consume_char();
  input.i--;
  return c;
}

Syntax *__nth_syntax_create_node(Kind k)
{
  Syntax *n = malloc(sizeof(Syntax));
  n->kind = k;
  n->size = 0;
  n->content = 0;
  n->nodes = 0;
  input.count++;
  input.nodes = realloc(input.nodes, sizeof(Syntax*) * input.count);
  input.nodes[input.count-1] = n;
  return n;
}

void __nth_syntax_append_node(Syntax *a, Syntax *b)
{
  switch(a->kind)
  {
    case quote:
    case unquote:
    case requote:
      if (a->size == 1) break;
    case string:
    case expression:
      a->size++;
      a->nodes = realloc(a->nodes, sizeof(Syntax*) * a->size);
      a->nodes[a->size-1] = b;
  }
}

void __nth_clean_buffer()
{
  if (input.nodes) 
    for (Int i = 0; i < input.count; i++)
    {
        if (  input.nodes[i]->kind != symbol && 
              input.nodes[i]->kind != stringsegment &&
              input.nodes[i])
        {
          free(input.nodes[i]->nodes);
        }
        free(input.nodes[i]);
    }
  if (input.i < input.size)
  {
    char *t = input.content;
    input.content = malloc(sizeof(char) * input.size - input.i);
    for (Int i = 0; i < input.size - input.i; i++)
      input.content[i] = t[input.i + i];
    free(t);
    input.size -= input.i;
    input.i = 0;
  }
  else 
  {
    if (input.content) free(input.content);
    input.content = malloc(0);
    input.size = 0;
  }
  input.nodes = malloc(0);
  input.count = 0;
  input.e = -1;
  InsideQuote = 0;
}

void __nth_wipe_buffer()
{
  __nth_clean_buffer();
  free(input.content);
  input.content = malloc(0);
  input.size = 0;
  input.i = 0;
}

Syntax *__nth_syntax_parse_buffer()
{
  Int alreadyquoted = 0;
  char c = 0;
  Syntax *t, *S;
  Kind k;
  switch(__nth_peek_char())
  {
    case '{':
      __nth_consume_char();
      for ( S = __nth_syntax_create_node(collection);
            __nth_peek_char() != '}';)
      {
        t = __nth_syntax_parse_buffer();
        if (input.e > -1) goto parse_error;
        if (t) __nth_syntax_append_node(S, t);
      }
      __nth_consume_char();
      return S;
      break;
    case '[':
      __nth_consume_char();
      for ( S = __nth_syntax_create_node(type);
            __nth_peek_char() != ']';)
      {
        t = __nth_syntax_parse_buffer();
        if (input.e > -1) goto parse_error;
        if (t) __nth_syntax_append_node(S, t);
      }
      __nth_consume_char();
      return S;
      break;
    case '(':
      __nth_consume_char();
      for ( S = __nth_syntax_create_node(expression); 
            __nth_peek_char() != ')';)
      {
        t = __nth_syntax_parse_buffer();
        if (input.e > -1) goto parse_error;
        if (t) __nth_syntax_append_node(S, t);
      }
      __nth_consume_char();
      return S;
      break;
    case ' ':
    case '\n':
      while (__nth_peek_char() <= ' ') __nth_consume_char();
      return 0;
      break;
    case ')': 
      __nth_consume_char(); 
      goto parse_error;
    case '"':
      S = __nth_syntax_create_node(string);
      __nth_consume_char();
      for(;__nth_peek_char() != '"';) 
      {
        switch(__nth_peek_char())
        {
          case '\\': 
            __nth_consume_char();
            if (__nth_peek_char() == '(')
            {
              t = __nth_syntax_parse_buffer();
              if (input.e > -1) goto parse_error;
              if (t) __nth_syntax_append_node(S, t);
              continue;
            } 
            __nth_syntax_append_node(S, __nth_syntax_create_node(stringsegment));
            S->nodes[S->size - 1]->content = input.i;
          default:
            if (!S->nodes || S->nodes[S->size - 1]->kind != stringsegment)
              __nth_syntax_append_node(S, __nth_syntax_create_node(stringsegment)),
              S->nodes[S->size - 1]->content = input.i;
            S->nodes[S->size - 1]->size++;
            __nth_consume_char();
        }
      }
      __nth_consume_char();
      return S;
      break;
    case '\'':
      __nth_consume_char();
      S = __nth_syntax_create_node(quote);
      if (__nth_peek_char() == '\'') { __nth_consume_char(); S->kind = unquote; }
      if (__nth_peek_char() == '\'') { __nth_consume_char(); S->kind = requote; }
      if (S->kind != quote && !InsideQuote) goto parse_error;
      if (InsideQuote) alreadyquoted = 1;
      else InsideQuote = 1;
      t = __nth_syntax_parse_buffer();
      if (input.e > -1) goto parse_error;
      if (t) __nth_syntax_append_node(S, t);
      if (!alreadyquoted) InsideQuote = 0;
      return S;
    default:
      for ( S = __nth_syntax_create_node(symbol), 
            S->content = input.i, 
            S->size = 1, 
            __nth_consume_char();;
            S->size++, 
            __nth_consume_char())
      { 
        if (c = __nth_peek_char(), 
            ( c <= ' ' || 
              c == '(' || c == ')' || 
              c == '[' || c == ']' || 
              c == '{' || c == '}')) 
        return S; 
      }
      break;
  }
  parse_error:
  if (input.e == -1) input.e = input.i;
  return 0;
}

void __nth_print_syntax(Syntax *s)
{
  switch(s->kind)
  {
    case string:
      putchar('"');
      for (Int i = 0; i < s->size; i++)
        __nth_print_syntax(s->nodes[i]);
      putchar('"');
      break;
    case expression:
      putchar('(');
      for (Int i = 0; i < s->size; i++) 
        __nth_print_syntax(s->nodes[i]), i < s->size - 1 ? putchar(' ') : 0;
      putchar(')');
      break;
    case collection:
      putchar('{');
      for (Int i = 0; i < s->size; i++)
        __nth_print_syntax(s->nodes[i]), i < s->size - 1 ? putchar(' ') : 0;
      putchar('}');
      break;
    case type:
      putchar('[');
      for (Int i = 0; i < s->size; i++)
        __nth_print_syntax(s->nodes[i]), i < s->size - 1 ? putchar(' ') : 0;
      putchar(']');
      break;
    case quote: printf("'"); goto __nth_print_syntaxQuote; break;
    case unquote: printf("''"); goto __nth_print_syntaxQuote; break;
    case requote: printf("'''"); goto __nth_print_syntaxQuote; break;
      __nth_print_syntaxQuote: 
      __nth_print_syntax(s->nodes[0]);
      break;
    case stringsegment:
    case symbol:
      for (Int i = 0; i < s->size; i++)
        putchar(input.content[s->content+i]);
      break;
  }
}
  