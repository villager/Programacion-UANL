const cpass = document.getElementById("cpassword");
const email = document.getElementById("email");
const pass = document.getElementById("password");
const form = document.getElementById("form");
const parrafo = document.getElementById("warnings");

form.addEventListener("submit", e=>{
    let warnings = "";
    let entrar = false;
    let regexEmail = /^\w+([\.-]?\w+)*@\w+([\.-]?\w+)*(\.\w{2,4})+$/
    parrafo.innerHTML = ""
    let isPassed = false;
    if(!regexEmail.test(email.value)){
        warnings += `El email no es valido <br>`
        entrar = true
    }
    if(pass.value.length < 10 || pass.value.length > 20){
        warnings += `La contraseña no es valida <br>`
        entrar = true
        isPassed = true;
    }
    if(!isPassed && (cpass.value.length < 10 || cpass.value.length > 20)){
        warnings += `La confirmacion de contraseña no es valida <br>`
        entrar = true
        isPassed = true;
    }
    if(!isPassed && (pass.value !== cpass.value)) {
        warnings += "La contraseña y la confirmacion no son iguales <br>"
        entrar = true
        isPassed = true;
    }
    if(entrar){
        e.preventDefault();
        parrafo.innerHTML = warnings;
    }
});