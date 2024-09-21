const email = document.getElementById("email");
const pass = document.getElementById("password");
const form = document.getElementById("form");
const creating = document.getElementById("creating");
const parrafo = document.getElementById("warnings");

form.addEventListener("submit", e=>{
    let warnings = ""
    let entrar = false
    let regexEmail = /^\w+([\.-]?\w+)*@\w+([\.-]?\w+)*(\.\w{2,4})+$/
    parrafo.innerHTML = ""
    if(!regexEmail.test(email.value)){
        warnings += `El email no es valido <br>`
        entrar = true
    }
    if(pass.value.length < 5){
        warnings += `La contraseña no es valida <br>`
        entrar = true
    }
    if(entrar){
        e.preventDefault();
        parrafo.innerHTML = warnings
    }
});