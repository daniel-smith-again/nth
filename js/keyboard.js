let Keyboard = {
    KeyMap: [
        ['.~!@#%^&|; 1234567890 +-*×/÷√<>=', '≤≥≰≱≠∽≈≡≣∎ ∀∃∄⊢∈∋∖∏∑∐ ¬∧∨∴∵∘∙'],
        ['\'tbjdkyhr, aofglmncie (spqxzuvw)','"TBJDKYHR\\\ AOFGLMNCIE ?SPQXZUVW:'],
        ['αβγδεζηθ ικλμνξοπ ρστυφχψω', 'ΑΒΓΔΕΖΗΘ ΙΚΛΜΝΞΟΠ ΡΣΤΥΦΧΨΩ'],
        ['𝕥𝕓𝕛𝕕𝕜𝕪𝕙𝕣 𝕒𝕠𝕗𝕘𝕝𝕞𝕟𝕔𝕚𝕖 𝕤𝕡𝕢𝕩𝕫𝕦𝕧𝕨', '𝕋𝔹𝕁𝔻𝕂𝕐ℍℝ 𝔸𝕆𝔽𝔾𝕃𝕄ℕℂ𝕀𝔼 𝕊ℙℚ𝕏ℤ𝕌𝕍']
    ],
    Map: 1,
    Visible: true,
    ShiftLock: 0,
    Symb: false,
    Toggle: document.getElementsByTagName('nth-keyboard-toggle')[0],
    Area: document.getElementsByTagName('nth-keyboard-area')[0],
    Display: document.getElementsByTagName('nth-keyboard')[0]
}
showKeyboard()

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

function toggleKeyboard() {
    if (Keyboard.Visible) {
        Keyboard.Area.style.bottom = '-20ch';
        Keyboard.Visible = false;
        Keyboard.Toggle.style.position = 'fixed';
        Keyboard.Toggle.style.left = '1%'
    }
    else {
        Keyboard.Area.style.bottom = '0';
        Keyboard.Toggle.style.left = '50%';
        Keyboard.Visible = true;
        setTimeout(() => {Keyboard.Toggle.style.position = 'static'; Keyboard.Area.style.bottom = '0';}, 100)
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

Keyboard.Area.onpointerdown = function (e) {
    if (e.target.classList[0] == 'key') {
        e.target.style.backgroundColor = 'var(--bgdd)';
        window.onpointerup = () => e.target.style.backgroundColor = 'var(--bg)', window.pointerup = null;
    }
    if (e.target.classList.contains("modifier"))
    {

    }
    else if (e.target.classList.contains("key"))
    {
        console.log(e.target.textContent);
    }
}

function cursorRight(e){
   
}

function cursorLeft(e){
   
}

function openExpression(e){
    insertExpression()
}

function closeExpression(e){
    

}