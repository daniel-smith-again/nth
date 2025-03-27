@font-face{ font-family:customfont; src:url('./OverpassMonoVariable.ttf'); }
@font-face{ font-family:noto; src:url('./NotoSansMonoVariable.ttf');}
html{ font-size:calc((12px + 1vw + 1vh) / 2); color:#777777ff; background-color:white; height:100%; width:100%; background-image: url("page-background.jpg"); background-repeat: repeat; }
* {font-variant-ligatures:none;}
body{ width:fit-content; padding: 7ch; margin:auto; min-height:100%; display:grid; grid-template-columns:100%; grid-template-rows: auto 1fr; background-image: url("background.jpg"); background-repeat: repeat;}
body > div{min-height:calc(100vh - 5lh);}
pre{ font-family:customfont, noto; line-height:2ch; }
footer{ font-family:customfont, noto; margin:auto; width:80ch;}
a{ text-decoration:none; color:#888888ff; background-color:inherit;} 
a:hover{text-decoration:underline;}
math{ font-size:calc((16px + 1vw + 1vh) / 2); display:block; width:fit-content; color:#222222; margin:auto; }
code{ color:maroon; font-family:noto;}
