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

void Boot();
void *Read();
void *Compute(void *p);
void Print(void *d);
typedef unsigned long int Int;
typedef struct {

} AST;

int main ()
{
  Boot();
  while(1) Print(Compute(Read()));
}

void* Env;
typedef struct {
  Int size;
  Int data; 
} Object;

void Boot() 
{

}

void *Read()
{

}

void *Compute(void *p)
{
  Object *P = (Object *)p;

}

void Print()
{

}