const Nth = new nth();
var area = document.getElementsByTagName('nth-area')[0]
var help = document.getElementsByTagName('nth-help')[0]
var editor = document.getElementById('editortemplate')
var helpbutton = document.getElementsByTagName('help-button')[0]
var textarea = document.getElementById("input");
function focusInput()
{
  textarea.focus()
  textarea.click()
}
textarea.oninput = inputFallback;
var header = document.getElementsByTagName('nth-header')[0]
helpbutton.addEventListener('click', (e)=> {help.style.display = 'block'; e.stopPropagation()})
var closehelp = document.getElementsByTagName('close-help')[0]
closehelp.addEventListener('click', (e)=>help.style.display = 'none')

header.onclick = () => {setTheme(Theme = ((Theme + 1) % 4));}
const shortcutreplace = [
  [/\\\\arrow/g, '→'],
  [/\\\\product/g, '⨯'],
  [/\\\\quotient/g, '÷'],
  [/\\\\and/g, '∧'],
  [/\\\\or/g, '∨'],
  [/\\\\not/g, '¬'],
  [/\\\\neq/g, '≠'],
  [/\\\\leq/g, '≤'],
  [/\\\\geq/g, '≥'],
  [/\\\\quote/g, '▶'],
  [/\\\\unquote/g, '▷'],
  [/\\\\requote/g, '▽'],
  [/\\\\inquote/g, '△']
]
var active = document.getElementById("editorwindow")
active['caret'] = active.lastChild
active.setAttribute('active', 'true')
focusInput()
document['clipboard'] = null;
area.onclick = (e) => 
{
  switch(e.target.tagName)
  {
    case 'NTH-SYMBOL':
      highlightSymbol(e);
      break;
  }
  focusInput()
}
function createSnippet() 
{
  var snippet = editor.cloneNode(true)
  for (var x = 0; x < area.childElementCount; x++)
    area.children[x].removeAttribute('active')
  snippet.setAttribute('active', 'true')
  snippet.id = null
  snippet.style.display = 'grid'
  //active = snippet
  
  snippet['caret'] = snippet.lastChild
  snippet.caret.symbol = null
  snippet.children[1].children[0].children[0].onclick = Run
  //got rid of rewrite button, there's other ways to do this that are just as easy
  //snippet.children[1].children[0].children[1].onclick = Rewrite
  snippet.children[1].children[0].children[1].children[0].children[0].onclick = Copy
  snippet.children[1].children[0].children[1].children[0].children[1].onclick = Cut
  snippet.children[1].children[0].children[1].children[0].children[2].onclick = Paste
  snippet.children[1].children[0].children[2].children[0].children[0].onclick = saveCode
  snippet.children[1].children[0].children[2].children[0].children[1].onclick = loadCode
  snippet.children[1].children[0].children[3].onclick = Close
  snippet.onpointerdown = dragCode
  focusInput()
  return snippet
}
function Close(e)
{
  area.removeChild(active)
}

function setActive(e)
{
  var snippet = e.currentTarget
  for (var x = 0; x < area.childElementCount; x++)
    area.children[x].removeAttribute('active')
  snippet.setAttribute('active', 'true');
  focusInput()
}

function dragCode(e)
{
  setActive(e)
  if (!mediaMobile.matches)
    return;
  e.preventDefault()
  area['clicklock'] = true
  setActive(e)
  var snippet = e.currentTarget
  if (snippet.offsetTop < 0) snippet.style.top = '0px';
  if (snippet.offsetLeft < 0) snippet.style.left = '0px';
  const offsetY = snippet.offsetTop - e.clientY
  const offsetX = snippet.offsetLeft - e.clientX
  const move = function(e)
  {
    e.preventDefault()
    var x = e.clientX + offsetX
    var y = e.clientY + offsetY
    if ((x > 0) && (x + snippet.offsetWidth < window.innerWidth))
      snippet.style.left = String(e.clientX + offsetX) + 'px'
    if ((y > 0) && (y + snippet.offsetHeight < window.innerHeight))
      snippet.style.top = String(e.clientY + offsetY) + 'px'
  }
  const stopmove = function(e)
  {
    e.preventDefault()
    document.onpointermove = null
    document.onpointerup = null
  }
  document.onpointermove = move
  document.onpointerup = stopmove
}

document.onkeydown = (e) => {
  if (document.activeElement.tagName == 'NTH-STRING') 
  {
    if (e.key == 'Escape') 
    {
      document.activeElement.blur()
    }
    return
  }
  if (e.ctrlKey)
  {
    switch(e.key)
    {
      case 'c': Copy(e); break
      case 'x': Cut(e); break
      case 'v': Paste(e); break
      default: return
    }
    return
  }
  if (e.key.length == 1) {
    e.preventDefault()
    switch(e.key) {
      case '(': insertExpression(); break
      case ')': outwards(); break
      case ' ': insertSymbol(); break
      case '"': insertString(); break
      default: insertCharacter(e.key)
    }
  }
  else {
    e.preventDefault();
    switch(e.key) {
      case 'Enter':
        if (e.shiftKey) Run()
        else insertBreak()
      break
      case 'Tab': growTab(); break
      case 'Backspace': backspace(); break
      case 'Delete': removeExpression(); break
      case 'ArrowRight': forwards(); break
      case 'ArrowLeft': backwards(); break
      case 'ArrowUp': outwards(); break
      case 'ArrowDown': inwards(); break
      /*case 'Escape': if (active != null) 
                        active.removeAttribute('active'),active.children[0].blur()
      */
     case 'Escape': break
    }
  }
}
function inputFallback(e)
{
  e.target.value = '#';
  e.target.selectionEnd = -1;
  e.target.selectionStart = -1;
  switch(e.inputType)
  {
    case "insertText" :
      switch (e.data)
      {
        case "(": insertExpression(); break;
        case ")": outwards(); break;
        case ' ': insertSymbol(); break;
        case '"': insertString(); break;
        default: insertCharacter(e.data);
      }
      break;
    case "deleteContentBackward":
      backspace(); break;
    case "insertLineBreak":
      insertBreak(); break;
    default: undefined; break;
  }

}
function insertCharacter(c) 
{
  var caret = active.caret;
  if (caret.symbol == null || 
      caret.symbol.tagName == 'NTH-LINEBREAK' || 
      caret.symbol.tagName == 'NTH-EXPRESSION' || 
      caret.symbol.tagName == 'NTH-STRING' || 
      caret.symbol.getAttribute('tab') != null)
  {
    insertSymbol()
  }
  if (caret.symbol.tagName != 'NTH-SYMBOL') return
  caret.symbol.textContent = caret.symbol.textContent + c
  checkQuote(caret.symbol)
  reduceShortcuts(caret.symbol)
}
function insertSymbol(i) 
{
  var caret = active.caret;
  if (caret.tagName == 'NTH-CODE' && caret.childElementCount > 0) return
  var s = document.createElement('nth-symbol')
  s.appendChild(document.createTextNode(''))
  s.setAttribute('active', 'true')
  s.onpointerdown = highlightSymbol
  
  if (caret.symbol) caret.symbol.removeAttribute('active');
  if (caret.childElementCount == 0) 
  {
    caret.appendChild(s)
    caret.symbol = caret.firstChild
  }
  else if (caret.symbol.previousSibling && 
           caret.symbol.tagName == 'NTH-SYMBOL' &&
           caret.symbol.textContent == '') 
  {
    caret.insertBefore(caret.symbol, caret.symbol.previousSibling)
    caret.symbol.setAttribute('active', 'true')
  }
  else if (caret.symbol.nextSibling)
  {
    caret.insertBefore(s, caret.symbol.nextSibling)
    caret.symbol = caret.symbol.nextSibling
  }
  else
  {
    caret.appendChild(s)
    caret.symbol = caret.lastChild
  } 
  //caret.symbol.scrollIntoView();
}
function insertExpression() 
{
  var caret = active.caret
  if (caret.tagName == 'NTH-CODE' && caret.childElementCount > 0) return
  var e = document.createElement('nth-expression')
  e['symbol'] = null;
  e.setAttribute('active', 'true')
  caret.removeAttribute('active');
  if (caret.symbol != null) caret.symbol.removeAttribute('active')
  if (caret.childElementCount == 0)
  {
    caret.appendChild(e)
    active.caret = caret.firstChild;
  }
  else if (caret.symbol.nextSibling)
  {
    caret.insertBefore(e, caret.symbol.nextSibling)
    if (e.previousSibling && 
        e.previousSibling.tagName == 'NTH-SYMBOL' &&
        e.previousSibling.textContent == ''
    )
    {
      caret.removeChild(e.previousSibling);
      caret.symbol = e;
      active.caret = caret.symbol;
    }
    else 
    {
      active.caret = caret.symbol.nextSibling
    }
  }
  else
  {
    caret.appendChild(e)
    if (e.previousSibling.tagName == 'NTH-SYMBOL' && e.previousSibling.textContent == '')
    {
      caret.removeChild(e.previousSibling);
    }
    active.caret = caret.lastChild
  }
  //caret.scrollIntoView()
}
function insertBreak() 
{
  var caret = active.caret
  if (caret.tagName == 'NTH-CODE') return;
  var b = document.createElement('nth-linebreak')
  b.appendChild(document.createTextNode('↩'))
  b.appendChild(document.createElement('br'));
  if (caret.childElementCount == 0) return;
  else
  {
    caret.symbol.removeAttribute('active')
    if (caret.symbol.nextSibling)
    {
      caret.insertBefore(b, caret.symbol.nextSibling)
    }
    else 
    {
      caret.appendChild(b)
    }
    active.caret.symbol = caret.symbol.nextSibling
  }
}
function growTab() 
{
  var caret = active.caret
  if (caret.symbol && caret.symbol.tagName == 'NTH-SYMBOL')
  {
    if (caret.symbol.textContent == '')
    {
      caret.symbol.textContent = caret.symbol.textContent + ' '
      caret.symbol.setAttribute('tab', 'true')
    }
    else if (caret.symbol.getAttribute('tab') != null)
      caret.symbol.textContent = caret.symbol.textContent + ' '

  }
  else
  {
    insertSymbol()
  }
}
function backspace() 
{
  var caret = active.caret
  if (caret.childElementCount > 0)
  {
    if (caret.symbol.tagName !== 'NTH-EXPRESSION' && caret.symbol.tagName !== 'NTH-LINEBREAK' && caret.symbol.textContent.length > 0 && caret.symbol.tagName != 'NTH-STRING')
    {
      caret.symbol.textContent = caret.symbol.textContent.slice(0, -1)
      checkQuote(caret.symbol)
      if (caret.symbol.textContent.length == 0 && caret.symbol.getAttribute('tab') != null)
        caret.symbol.removeAttribute('tab')
    }
    else 
    {
      if (caret.symbol.previousSibling)
      {
        var temp = caret.symbol
        active.caret.symbol = caret.symbol.previousSibling
        active.caret.symbol.setAttribute('active', 'true')
        caret.removeChild(temp)
      }
      else
      {
        if (caret.symbol.nextSibling)
        {
          var temp = caret.symbol
          active.caret.symbol = caret.symbol.nextSibling
          active.caret.symbol.setAttribute('active', 'true')
          caret.removeChild(temp)
        }
        else
        {
          caret.replaceChildren()
          active.caret.symbol = null;
        }
      }
    }
  }
  else removeExpression()
}
function removeExpression() 
{
  var caret = active.caret
  var temp = caret
  if (caret.parentElement.tagName == 'NTH-EXPRESSION' || caret.parentElement.tagName == 'NTH-CODE')
    active.caret = caret.parentElement
  else 
  {
    active.caret.replaceChildren()
    active.caret.symbol = null
    return
  }
  if (temp.previousSibling) active.caret.symbol = temp.previousSibling
  else if (temp.nextSibling) active.caret.symbol = temp.nextSibling
  else active.caret.symbol = null
  temp.parentElement.removeChild(temp)
  active.caret.setAttribute('active', 'true')
  if (active.caret.symbol) active.caret.symbol.setAttribute('active', 'true');
}
function forwards()
{
  var caret = active.caret
  if (caret.childElementCount == 0) return
  if (caret.symbol)
  {
    if (caret.symbol.nextSibling)
    {
    caret.symbol.removeAttribute('active')
    caret.symbol = caret.symbol.nextSibling
    caret.symbol.setAttribute('active', 'true')
    }
    else
    {
    caret.symbol.removeAttribute('active')
    caret.symbol = caret.firstChild
    caret.symbol.setAttribute('active', 'true')
    }
  }
  //caret.symbol.scrollIntoView()
}
function backwards()
{
  var caret = active.caret
  if (caret.childElementCount == 0) return
  if (caret.symbol)
  {
    if (caret.symbol.previousSibling)
    {
      caret.symbol.removeAttribute('active')
      caret.symbol = caret.symbol.previousSibling
      caret.symbol.setAttribute('active', 'true')
    }
    else
    {
      caret.symbol.removeAttribute('active')
      caret.symbol = caret.lastChild
      caret.symbol.setAttribute('active', 'true')
    }
  }
  //caret.symbol.scrollIntoView()
}
function inwards() 
{
 var caret = active.caret
 if (caret.symbol == null) return
 if (caret.symbol.tagName == 'NTH-EXPRESSION')
 {
  caret.removeAttribute('active')
  active.caret = caret.symbol
  active.caret.setAttribute('active', 'true')
  if (active.caret.symbol != null)
    active.caret.symbol.setAttribute('active', 'true')
  else if (active.caret.childElementCount > 0)
  {
    active.caret.symbol = active.caret.children[0]
    active.caret.symbol.setAttribute('active', 'true')
  }
 }
 else if (caret.symbol.tagName == 'NTH-STRING')
 {
  caret.symbol.click()
 }
 //caret.symbol.scrollIntoView()
 
}
function outwards() 
{
  var caret = active.caret
  var temp = caret
  if (caret.parentElement && (caret.parentElement.tagName == 'NTH-EXPRESSION' || caret.parentElement.tagName == 'NTH-CODE'))
  {
    caret.removeAttribute('active')
    if (caret.symbol != null) caret.symbol.removeAttribute('active')
    active.caret = caret.parentElement
    active.caret.setAttribute('active', 'true')
    active.caret.symbol = temp
    active.caret.symbol.setAttribute('active', 'true')
  }
  for (s of active.caret.children)
  {
    
    
  }
  //caret.symbol.scrollIntoView()
}
function highlightSymbol(e) 
{
  e.preventDefault()
  console.log(e)
  var p = e.target
  while (p.parentElement.tagName != 'NTH-EDITOR')
    p = p.parentElement
  p = p.parentElement
  if (active != null && active != p) active.caret.removeAttribute('active')
  {
    focusInput()
  }
  if (active.caret != null) active.caret.removeAttribute('active')
  if (active.caret.symbol) active.caret.symbol.removeAttribute('active')
  active.caret = e.target.parentElement;
  active.caret.setAttribute('active', 'true')
  active.caret.symbol = e.target
  active.caret.symbol.setAttribute('active', 'true')
}
function checkQuote(s) 
{
  return;
}

function reduceShortcuts(s)
{
  if (s.textContent.includes('\\\\'))
  {
    for (var n of shortcutreplace)
    {
      s.textContent = s.textContent.replace(n[0], n[1])
    }
  }
}
function Cut(e)
{
  e.stopImmediatePropagation();
  if (active.caret == null) return;
  if (active.caret.symbol == null) return;
  document.clipboard = null;
  document.clipboard = active.caret.symbol
  document.clipboard.removeAttribute('active')
  if (active.caret.childElementCount == 1) 
    active.caret.symbol = null
  else if (document.clipboard.previousSibling) 
    active.caret.symbol = active.caret.previousSibling
  else if (document.clipboard.nextSibling)
    active.caret.symbol = active.caret.nextSibling
  else
    active.caret.symbol = null
  active.caret.removeChild(document.clipboard);
}
function Copy(e)
{
  e.stopImmediatePropagation();
  if (active.caret == null) return;
  if (active.caret.symbol == null) return;
  document.clipboard = null;
  document.clipboard = active.caret.symbol.cloneNode(true)
  const reinstateHandlers = function(exp)
  {
    if (exp.tagName == 'NTH-EXPRESSION')
    {
      exp.childNodes.forEach(reinstateHandlers)
    }
    else if (exp.tagName == 'NTH-SYMBOL')
    {
      exp.onpointerdown = highlightSymbol
    }
  }
  reinstateHandlers(document.clipboard)
}
function Paste(e)
{
  e.stopImmediatePropagation();
  if (document.clipboard == null) return;
  if (active.caret.tagName == 'NTH-CODE' && active.caret.childElementCount == 1) return;
  if (active.caret == null)
  {
    active.lastChild.appendChild(document.clipboard)
  }
  else 
  {
    if (active.caret.symbol && active.caret.symbol.getAttribute('active'))
      active.caret.symbol.removeAttribute('active');
    if (active.caret.symbol && active.caret.symbol.nextSibling)
    {
      active.caret.insertBefore(document.clipboard, active.caret.symbol.nextSibling)
      active.caret.symbol = active.caret.symbol.nextSibling
      active.caret.symbol.setAttribute('active', 'true')
    }
    else
    {
      active.caret.appendChild(document.clipboard)
      active.caret.symbol = active.caret.lastChild
      active.caret.symbol.setAttribute('active', 'true')
    }
    document.clipboard = null;
  }
}
function insertString()
{
  var caret = active.caret;
  if (caret.tagName == 'NTH-CODE' && caret.childElementCount > 0) return
  var s = document.createElement('nth-string')
  s.contentEditable = true
  s.autocapitalize = false
  s.spellcheck = false
  s.onclick = (e) =>
    {
      highlightSymbol(e)
      focusInput()
    }
  s.ontouchstart = (e) => 
    {
      highlightSymbol(e)
      focusInput()
    }
  s.setAttribute('active', 'true')
  if (caret.symbol) caret.symbol.removeAttribute('active')
  if (caret.childElementCount == 0) 
  {
    caret.appendChild(s)
    caret.symbol = caret.firstChild
  }
  else if (caret.symbol.nextSibling)
  {
    caret.insertBefore(s, caret.symbol.nextSibling)
    caret.symbol = caret.symbol.nextSibling
  }
  else
  {
    caret.appendChild(s)
    caret.symbol = caret.lastChild
  }
}
function saveCode(e)
{
  var p = structureToProgram(active.lastChild.lastChild)
  var download = document.createElement('a')
  download.setAttribute('href', 'data:text/plain;charset=utf8,' + encodeURIComponent(p))
  download.setAttribute('download', 'nth-source.txt')
  download.style.display = 'none'
  document.body.appendChild(download)
  download.click()
  document.body.removeChild(download)
}
function loadCode(e)
{
  var input = document.createElement('input')
  input.type = 'file';
  input.style.display = 'none'
  input.onchange = async (e) => 
  {
    var file = input.files[0]
    var program = await file.text()
    var e = programToStructure(program)
    active.lastChild.replaceChildren(e)
    active.caret = active.lastChild
    active.caret.setAttribute('active', 'true')
    if (active.caret.childElementCount > 0)
    {
      active.caret.symbol = active.caret.children[0]
      active.caret.symbol.setAttribute('active', 'true')
    }
  }
  document.body.appendChild(input)
  input.click()
}
function Run(e)
{
  if (active.lastChild.childElementCount == 0) return
  var result = Eval(active.lastChild.lastChild)
  var r = document.createElement('nth-symbol')
  r.appendChild(document.createTextNode('_'))
  //save this for when interpreter is done
  //active.lastChild.replaceChildren(result)
  active.lastChild.replaceChildren(r)
  active.caret = active.lastChild
  active.caret.symbol = r
  active.caret.symbol.setAttribute('active', 'true')
}
function Rewrite(e) 
{
  
}
function structureToList()
{
  var top = active.lastChild.lastChild
  const walk = (item) => {
    switch(item.tagName)
    {
      case 'NTH-SYMBOL':
        return item.textContent;
        break;
      case 'NTH-EXPRESSION':
        var result = []
        for (x of item.children)
        {
          result.push(walk(x))
        }
        return result;
    }
  }
  var list = walk(top)
  console.log(list)
}
function listToStructure(l)
{

}
function structureToProgram(s) 
{
  switch(s.tagName) {
    case 'NTH-EXPRESSION':
      var p = '(' 
      for (var x = 0; x < s.childElementCount; x++)
      {
        p += structureToProgram(s.children[x])
        if (x < s.childElementCount - 1) 
        {
          if (s.children[x].tagName != 'NTH-TAB' &&
              s.children[x].getAttribute('quote') == null &&
              s.children[x].getAttribute('tab') == null &&
              s.children[x].tagName != 'NTH-LINEBREAK' &&
              s.children[x + 1].tagName != 'NTH-LINEBREAK')
          {
            p += ' '
          }
        }
      }
      p += ')'
      return p
    break
    case 'NTH-SYMBOL': 
      return s.innerText
    break
    case 'NTH-STRING': 
      return '"' + s.innerText + '" '
    break
    case 'NTH-LINEBREAK': 
      return '\n'
    break
  }
}
function programToStructure(p)
{
  var i = 0;
  var getChar = () => {var char = (i >= p.length ? null : p[i]); i++; return char;}
  var peekChar = () => {return i >= p.length ? null : p[i]; }
  function parse()  
  {
    var expression
    var c = peekChar()
    switch(c)
    {
      case '(':
        expression = document.createElement('nth-expression')
        c = getChar()
        while (c = peekChar(), c != ')')
        {
          var temp = parse()
          if (temp) expression.appendChild(temp)
        }
        getChar()
        return expression
      break;
      case '"':
        c = getChar()
        expression = document.createElement('nth-string')
        expression.contentEditable = true
        expression.autocapitalize = false
        expression.spellcheck = false
        expression.onclick = (e) =>
          {
            highlightSymbol(e)
            focusInput()
          }
        expression.ontouchstart = (e) => 
          {
            highlightSymbol(e)
            focusInput()
          }
        var stringcontent = ''
        while (c = peekChar(), c != '"')
        {
          if (c == '\t' || c == '\r') continue
          if (c == '\\')
            stringcontent += c, getChar(), c = peekChar()
          if (c == '\n')
          {
            expression.appendChild(document.createTextNode(stringcontent))
            expression.appendChild(document.createElement('br'))
            stringcontent = ''
          }
          stringcontent += c, getChar()
        }
        getChar()
        expression.appendChild(document.createTextNode(stringcontent))
        return expression
      break;
      case '\n':
        c = getChar()
        expression = document.createElement('nth-linebreak')
        expression.appendChild(document.createTextNode('↩'))
        expression.appendChild(document.createElement('br'))
        return expression
      break;
      case ' ':
      case ' ':
        c = getChar()
        expression = document.createElement('nth-symbol')
        expression.onpointerdown = highlightSymbol
        expression.setAttribute('tab', 'true')
        expression.textContent = ' ';
        while (c = peekChar(), c == ' ' || c == ' ')
          expression.textContent += ' ', getChar()
        if (expression.textContent.length > 1) return expression
        else return null
      break;
      case ')':
        throw "Unexpected closing brace!"
      break;
      default:
        c = getChar()
        expression = document.createElement('nth-symbol')
        expression.onpointerdown = highlightSymbol
        expression.textContent = c
        while (c = peekChar(), c != '(' && c != ')' && c != ' ' && c != '\n' && c != null)
          expression.textContent += getChar()
        var t = expression.textContent
        checkQuote(expression)
        return expression
    }
  }
  var top
  try {
    top = parse ()
  }
  catch(e) {top = document.createElement('nth-symbol').appendChild(document.createTextNode('_'))}
  return top
}
function Eval()
{
  var result = Nth.eval(structureToList())
  return ''
}

let Keyboard = {
    KeyMap: [
        ['`~!@#%^&|/,. 1234567890 +-×÷√<≤>≥=≠', '←↑↓→∖∏∑∐ ∧∨¬∀∃∄⊢∈∋ {}[]∴∵∎∘∙'],
        ['\'tbjdkyhr* aofglmncie \\spqxzuvw→','"TBJDKYHR; AOFGLMNCIE ?SPQXZUVW:'],
        ['αβγδεζηθ ικλμνξοπ ρστυφχψω', 'ΑΒΓΔΕΖΗΘ ΙΚΛΜΝΞΟΠ ΡΣΤΥΦΧΨΩ'],
    ],
    Map: 1,
    Visible: true,
    ShiftLock: 0,
    Symb: false,
    Toggle: document.getElementsByTagName('keyboard-toggle')[0],
    Area: document.getElementsByTagName('nth-keyboard-area')[0],
    Display: document.getElementsByTagName('nth-keyboard')[0]
}
//toggleKeyboard()
//showKeyboard()

function keyPressColor(e) {
e.stopPropagation()
    e.target.style.backgroundColor = 'var(--bgdd)';
    e.target.addEventListener('pointerup', keyPressColorReset)
    e.target.addEventListener('pointerup', () => e.target.style.backgroundColor = 'var(--bg)')
}

function keyPressColorReset(e) {
    e.target.style.backgroundColor = 'var(--bg)'
    e.target.removeEventListener('pointerup', keyPressColorReset)
}

function toggleKeyboard(e) {
  if (e) { e.stopPropagation(); }
  if (Keyboard.Visible) {
      Keyboard.Area.style.position = 'absolute';
      Keyboard.Area.style.left = '-500ch'
      Keyboard.Area.style.top = '0px'
      Keyboard.Visible = false
      Keyboard.Toggle.style.position = 'fixed'
      Keyboard.Toggle.style.left = '1ch'
      Keyboard.Toggle.style.bottom = '1ch'
      document.getElementById('nth-area').style.height = 'calc(90vh - 16ch)'
  }
  else {
      Keyboard.Area.style.position = 'static'
      Keyboard.Toggle.style.position = 'static'
      Keyboard.Visible = true;
      document.getElementById('nth-area').style.height = 'calc(90vh - 32ch)'
  }
}

function nextLayout(e) {
    Keyboard.Map < Keyboard.KeyMap.length - 1 ? Keyboard.Map ++ : Keyboard.Map = 1
    e.target.style.backgroundColor = 'var(--bgdd)';
    e.target.addEventListener('pointerup', () => e.target.style.backgroundColor = 'var(--bg)')
    showKeyboard()
}
function prevLayout(e){
    Keyboard.Map > 1 ? Keyboard.Map -- : Keyboard.Map = Keyboard.KeyMap.length - 1
    e.target.style.backgroundColor = 'var(--bgdd)';
    e.target.addEventListener('pointerup', () => e.target.style.backgroundColor = 'var(--bg)')
    showKeyboard()
}

function showKeyboard() {
    var chars = Array.from(Keyboard.KeyMap[Keyboard.Symb ? 0 : Keyboard.Map][Keyboard.ShiftLock])
    Keyboard.Display.replaceChildren();
    Keyboard.Display.appendChild(document.createElement('keyboard-row'))
    chars.map(c => {
        if (c != ' ') {
            Keyboard.Display.lastChild.appendChild(document.createElement('keyboard-key'))
            Keyboard.Display.lastChild.lastChild.classList.add('key')
            Keyboard.Display.lastChild.lastChild.appendChild(document.createTextNode(c))
            }
        else {
            Keyboard.Display.appendChild(document.createElement('keyboard-row'))
        }
    })
}

function toggleShift(e) {
    e.stopPropagation()
    Keyboard.ShiftLock = Keyboard.ShiftLock == 1 ? 0 : 1
    if (Keyboard.ShiftLock == 1)
        e.target.style.backgroundColor = 'var(--bgdd)'
    else 
        e.target.style.backgroundColor = 'unset'
    showKeyboard()
}

function toggleSymb(e) {
    e.stopPropagation()
    Keyboard.Symb = !Keyboard.Symb
    if (Keyboard.Symb)
        e.target.style.backgroundColor = 'var(--bgdd)'
    else
        e.target.style.backgroundColor = 'unset'
    showKeyboard()
}

/*
Keyboard.Area.onpointerdown = function (e) {
    if (e.target.classList[0] == 'key' && !e.target.classList.contains("toggle")) {
        e.target.style.backgroundColor = 'var(--bgdd)';
        if (navigator.vibrate)
          navigator.vibrate(70);
        window.onpointerup = () => e.target.style.backgroundColor = 'var(--bg)', window.pointerup = null;
    }
    if (e.target.classList.contains("modifier"))
    {

    }
    else if (e.target.classList.contains("key"))
    {
        insertCharacter(e.target.textContent);
    }
}
*/

