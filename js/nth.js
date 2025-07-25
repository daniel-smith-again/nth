/*******************************************************************************
--------------------------------------------------------------------------------
                        The nth Programming Environment                         
--------------------------------------------------------------------------------
                                       by                                       
                                  Daniel Smith                                  

--------------------------------------------------------------------------------
                                     About                                      
--------------------------------------------------------------------------------

                  This is a fully conforming implementation of                  
                  the Nth Programming Language as described in                  
                    "The nth Programming Language HyperDoc".                    


--------------------------------------------------------------------------------
                                  Terms of Use                                  
--------------------------------------------------------------------------------

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


--------------------------------------------------------------------------------
                             STATEMENT OF WARRANTY                              
--------------------------------------------------------------------------------

        THIS SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,        
                EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO                
      THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE,      
                              AND NONINFRINGEMENT.                              

        IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES,        
   OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT, OR OTHERWISE,   
     ARISING FROM, OUT OF, OR IN CONNECTION WITH THE SOFTWARE, OR THE USE,     
                       OR OTHER DEALINGS IN THE SOFTWARE.                       


*******************************************************************************/

class nth 
{
  
  #buffer = ""
  #output = ""
  #ast = undefined
  #top_level = {}
  #lookup = (symbol) => //find a binding in the top level and return the data 
  {

  }
  #parse = () =>
  {
    this.#buffer = this.#buffer.replaceAll(/\s\s*/g, ' ')
    var str = this.#buffer
    var p = 0
    function next()
    {
      return (p < str.length ? str[p] : null)
    }
    function get(t)
    {
      var c = str[p]
      if (p < str.length)
      {
        p++;
        return c
      }
      else 
      {
        if (t)
          throw '"Unexpected End of Input"'
        else
          return null
      }
    }
    function checkdelim(c)
    {
      switch(c)
      {
        case '(':
        case ')':
        case ' ':
        case '"':
        case null:
          return true;
        default: 
          return false;
      }
    }
    function descend()
    {
      switch(next())
      {
        case '(':
          get(true)
          var expression = []
          while(next() != ')')
            expression.push(descend())
          get()
          return expression
        case ')':
          throw "\"Unexpected Closing Brace\""
        case '"':
          var string = ""
          get(true)
          while (next() != '"')
          {
            if (next() == '\\')
            {
              get(true)
              if (next() == '\\' || next() == '"')
                string = string + get(true)
              else if (next().match(/[a-fA-F0-9]/) != null)
              {
                var pair = "" + get(true)
                if (next().match(/[a-fA-F0-9]/) != null)
                {
                  pair = pair + get(true)
                  string = string + String.fromCharCode(parseInt(pair, 16))
                }
                else throw "\"Malformed Escape Sequence\""
              }
              else throw "\"Malformed Escape Sequence\""
            }
            else
            {
              string = string + get(true)
            }
          }
          get(true)
          return string
        default:
          var symbol = "" + get()
          while (!checkdelim(next()))
          {
            try { symbol = symbol + get() }
            catch(e) {break;}
          }
          if (next() == ' ')
            get(true)
          return symbol
      }
    }
    try 
    {
      this.#ast = descend()
    }
    catch(e) {this.#output = e; return}
    this.#expand()
  }
  #expand = () =>
  {
    console.log(this.#ast)
    this.#check()
  }
  #check = () =>
  {
    this.#execute()
  }
  #execute = () =>
  {
    this.#print()
  }
  #print = () => 
  {
    this.#output = "()"
  }
  constructor ()
  {
    this.eval = (str) => 
    {
      this.#buffer = str
      this.#parse()
      return this.#output
    }
  }
}

class othernth {
  constructor()
  {
   this.eval = (program) => 
    {

    } 
    this.isKeyword = (sym) => {}
    this.annotateSymbol = (sym) =>
    {
      return (this['\0toplevel']['\0keywords'].includes(sym) ||
              this['\0toplevel']['\0types'].includes(sym))
    }
  }
  #default =
  {
    ['\0toplevel']: {
      //keywords array is the symbols that should be highlighted in macros
      '\0keywords': [
        'define',
        'function',
        'let',
        'do',
        'with',
        '?',
        'module',
        'data',
        'syntax',
        'import',
        'export'
      ],
      //types array is the symbols that should be highlighted that bind types
      '\0types': [
        'Boolean',
        'Function',
        'Symbol',
        'Expression',
        'Program',
        'List',
        'Items',
        'Array',
        'Number',
        'Interval',
        'Natural',
        'Integer',
        'Rational',
        'View'
      ],
      '\0syntax': []
    },
    ['\0primitives']:
    {
      moduleBind: (symbol, data) => {},
      moduleCreate: (symbol, data) => {},
      moduleEnter: (symbol) => {},
      moduleLeave: (symbol) => {},
      function: (args, data) => {},
      list: (data) => {},
      items: (data) => {},
      
    },
    ['define']: () => {},
    ['function']: () => {},
    ['let']: () => {},
    ['do']: () => {},
    ['with']: () => {},
    ['?']: () => {},
    ['module']: () => {},
    ['data']: () => {},
    ['syntax']: () => {},
    ['import']: () => {},
    ['export']: () => {},
  }
  
}

class oldothernth {
  constructor(){this.#init(); this.eval = (program) => {this.#setbuffer(program); return this.#eval(); }}
  #string = class { constructor(){}; type = "String"; contents = ""; }
  #number = class { constructor(){}; type = "Number"; contents = ""; }
  #list = class { constructor(){}; type = "List"; contents = []; }
  #items = class { constructor(){}; type = "Items"; contents = []; }
  #array = class { constructor(){}; type = "Array"; contents = []; }
  #boolean = class { constructor(){}; type = "Boolean"; contents = true; }
  #type = class { constructor() {}; type = "Type"; contents = []; }
  #defaults = 
  {
    //special forms
    'define': (ast) => {},
    'let': (ast) => {},
    'with': (ast) => {},
    'function': (ast) => {},
    '?': (ast) => {},
    //constructors
    'function': (ast) => {},
    'quote': (ast) => {},
    'list': (ast) => {},
    'items': (ast) => {},
    'array': (ast) => {},
    'symbol': (ast) => {},
    'image': (ast) => {},
    'view': (ast) => {},
    //manipulation
    'get': (ast) => {},
    'set': (ast) => {},
    //arithmetic
    '+': (ast) => {},
    '-': (ast) => {},
    '⨯': (ast) => {},
    '÷': (ast) => {},
    '∧': (ast) => {},
    '∨': (ast) => {},
    '¬': (ast) => {},
    'and': (ast) => {},
    'or': (ast) => {},
    'not': (ast) => {},
    //comparison
    '=': (ast) => {},
    '≠': (ast) => {},
    '<': (ast) => {},
    '≤': (ast) => {},
    '>': (ast) => {},
    '≥': (ast) => {},
    //builtin functions
    'apply': (ast) => {},
    //reflection
    '#': (ast) => {},
    'size': (ast) => {},
    'signature': (ast) => {},
    'return': (ast) => {},


    'define': (ast) => {},
    'function': (ast) => {},
    'let': (ast) => {},
    'with': (ast) => {},
    'eval': (ast) => {},
    'simplify': (ast) => {},
    'read': (ast) => {},
    'print': (ast) => {},
    'format': (ast) => {},
    'symbol': (ast) => {},
    '+': (ast) => {},
    '-': (ast) => {},
    '⨯': (ast) => {},
    '÷': (ast) => {},
    'simplify': (ast) => {},
    'exp': (ast) => {},
    'log': (ast) => {},
    'root': (ast) => {},
    'and': (ast) => {},
    'or': (ast) => {},
    'not': (ast) => {},
    'items': (ast) => {},
    'list': (ast) => {},
    'array': (ast) => {},
    '?': (ast) => {},
    '#': (ast) => {},
    'the': (ast) => {},
    'apply': (ast) => {},
  }
  #toplevel = {}
  #init = () => 
  {

  }
  
  #next = (peek) => 
  { 
    var r = this.#buffer(peek).next(); 
    if (r.done) throw "Error: unexpected end of input."; 
    else return r.value; 
  }
  #setbuffer = (program) => {
    var i = 0;
    var buf = program;
    this.#buffer = function* (peek) {
      var c = null;
      if (i < buf.length) 
      { 
        c = buf[i]; 
        if (!peek) i = i + 1;
        yield c;
      }
    }
  }
  #buffer = null
  #eval = () => this.#interpret(this.#parse(null))
  #parse = (ast) =>
  {
    if (this.#next(true))
    {
      var n = {parent: ast, type: undefined, contents: undefined}
      switch(this.#next(true))
      {
        case '(':
          this.#next()
          n.type = 'Expression';
          n.contents = []
          while (this.#next(true) != ')')
          {
            var r = this.#parse(n);
            if (r)
            { n.contents.push(r); }
          }
          this.#next()
          return n;
          break;
        case ')':
          throw "Error: unexpected closing brace.";
          break;
        case ' ':
          this.#next();
          return null;
          break;
        case '"':
          n.type = 'String';
          n.contents = "";
          this.#next();
          strloop: while(1)
          {
            if (this.#next(true) == '\\') 
            { this.#next(); }
            if (this.#next(true) == '"')
            {
              this.#next();
              break strloop;
            }
            n.contents += this.#next()
          }
          return n;
          break;
        default:
          n.type = 'Symbol'
          n.contents = "" + this.#next()
          try 
          {
            symloop: while (1)
            {
              switch(this.#next(true))
              {
                case '(':
                case ')':
                case '"':
                case ' ':
                  break symloop;
                default:
                  n.contents += this.#next();
              }
            }
          }
          catch(e) 
          { undefined; }
          var test = n.contents.match(/[+-]?[1-9][0-9]*/);
          if (test)
          { 
            if (test[0].length == n.contents.length)
            { n.type = 'Number'; }
          }
          if (n.contents == "true" || n.contents == "false")
          { n.type = "Boolean"; }
          return n;
      }
    }
    else 
    {
      if (ast.parent)
      { throw "Error: unexpected end of input."}
    }
  }
  #expand = (ast) => 
  {
    
    return ast;
  }
  #check = (ast) =>
  {
    //symbols must be bound
    //function calls must conform to signatures
  }
  #interpret = (ast) => 
  {
    var astp = this.#expand(ast);
    this.#check(astp);
    return (this.#print(astp))
  }
  #print = (ast) =>
  {
    var s = ""
    switch(ast.type)
    {
      case "Symbol":
      case "Number":
      case "Boolean":
        s += ast.contents
      break;
      case "String":
        s += '"'
        s += ast.contents.replaceAll("\"", "\\\"").replaceAll("\\", "\\\\");
        s += '"'
      break;
      case "Expression":
        s += '(';
        for (var n in ast.contents)
        {
          s += this.#print(ast.contents[n])
          if (n < ast.contents.length - 1)
          { s += ' '; }
        }
        s += ')';
      break;
    }
    return s;
  }
}
