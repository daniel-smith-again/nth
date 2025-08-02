function SelectSymbol(e)
{
  switch(e.target.tagName)
  {
    case 'NTH-SESSION':
      e.target.focus()
      //e.target.firstChild.click()
      window['focused'] = e.target
      break;
    case 'NTH-SYMBOL':
      break;
    case 'NTH-EXPRESSION':
      break;
    case 'NTH-LINEBREAK':
      break;
    case 'NTH-INDENT':
      break;
  }
  console.log(e)
}

function SessionInput(e)
{
  console.log(e)
  console.log(window.focused)
}

window.onkeydown = SessionInput
var __SESSIONS__ = document.querySelectorAll("nth-session")
for (s of __SESSIONS__)
{
  console.log(s.firstChild)
  s.onpointerdown = SelectSymbol
  s.onkeydown = SessionInput
  s.oninput = SessionInput

  /*
   * sexpr representation:
   * [ {parent} {type} {children}*]
   */
  s['expr'] = [undefined]
  s['cursor'] = s['expr']
}