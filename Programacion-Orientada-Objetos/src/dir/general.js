"use strict";

const User = require('../users').User;
const crypto = require("bcrypt");
let users =  App.users = Object.create(null);
const Database = require('../database').Database;
const Db =  App.Db = new Database("users");

class Auth {
  static isLogged(req) {
    if (req.ip && users[req.ip]) return true;
    return false;
  }
  static hasPermission(req, rank) {
    if(Auth.isLogged(req) && users[req.ip].rank === rank) return true;
    return false;
  }  
  static isAdmin(req) {
    if(Auth.hasPermission(req, 'admin')) return true;
    return false;
  }
  static isOperator(req) {
    if (Auth.isAdmin(req)) return true;
    if (Auth.hasPermission(req, 'operator')) return true;
    return false;
  }
}
exports.Auth = Auth;

async function home(req, res) {
    if (! users[req.ip]) { 
        res.render('general/login', {
          title: 'Inicio',
          warning: '',
        });
    } else {
        res.redirect('/home');
    }   
}
async function login(req, res) {
    res.render('general/login', {
        title: 'Inicio',
        warning: '',
    });
}
async function logout(req, res) {
    if (users[req.ip]) delete  users[req.ip];
    res.redirect('/login');
}
async function auth(request, response) {
    let username = request.body.email, password = request.body.password;
    try {
      let result = await Db.look4Email(username);
      if (result && result.rows[0]) {
        crypto.compare(password, result.rows[0].password, (err, r) => {
          if(err) throw err;
          if(r) {
            if(!users[request.ip]) {
              users[request.ip] = new User(result.rows[0]);
              if(result.rows[0].courses) users[request.ip].courses = Array.from(result.rows[0].courses);
            }
            response.redirect('/home');
          } else {
            response.render('general/login', {
              title: 'Inicio',
              warning: 'Contraseña incorrecta'
            });
          }
        });  
      } else {
        response.render('general/login', {
          title: 'Inicio',
          warning: 'Usuario o contraseña incorrecto o inexistente'
        });
      }
    } catch(e) {
      throw e;
    }
}
async function creating (req, res) {
    let user = req.body.email,
    pass = req.body.password,
    name = req.body.name,
    lastname = req.body.lastname;
    if (user && pass) {
      try {
        let isRegistered = await Db.look4Email(user);
        if (isRegistered && isRegistered.rows[0]) {
          res.render('general/login', {
            title: 'Inicio',
            warning: 'Correo ya registrado, por favor inicie sesion'
          });        
        } else {
          let salt = await crypto.genSalt(10);
          let hash = await crypto.hash(pass, salt);
          let date = Date.now();
          let result = await Db.createUser(user, hash, name, lastname, date, 'user');
          if (result) {
            users[req.ip] = new User({
              name: name,
              lastname: lastname,
              date: date,
              password: pass,
              email: user,
            });
            res.redirect('/home');
          }
        }
      } catch(e) {
        throw e;
      }
    }
  }

async function realHome(req, res) {
    if (users[req.ip]) {
      try {
        let options = Object.create(null);
        options.name = users[req.ip].fullname();
        options.id = users[req.ip].id;
        options.rank = users[req.ip].rank;
        let result = await App.Items.get();
        options.items = result.rows;
        if (users[req.ip].rank === 'admin') {
          options.usercount = await Db.length();
          options.itemcount = await App.Items.length();;          
        }
        res.render('general/home', options);
      } catch(e) {
        throw e;
      }
    } else {
      res.redirect('/login');
    }
  }
  function create(req, res) {
    res.render('general/create',{
        title: "Creacion de usuario",
      });    
  }
  function doOnlyLogged(req, res, callback) {
    if (Auth.isLogged(req)) {
      callback();
    } else {
      res.render('errors/403');
    }
  }
  
  function selfUpdate(req, res) {
   // let id = req.query.id;
    try {
      doOnlyLogged(req, res, async () => {
        let result = await App.Db.look4Id(users[req.ip].id);
        res.render('user/update', {
          name:  users[req.ip].fullname(),
          rank:  users[req.ip].rank,
          id: users[req.ip].id,
          data: result.rows[0]
        });
      });
    }catch(e) {
      throw e;
    }
  }

  function deleteAccount(req, res) {
    let id = req.query.id;
    if(!id) id = users[req.ip].id;
    try {
      Db.deleteUser(id);
      res.redirect('/home');
    } catch(e) {
      throw e;
    }
  }
  
exports.init = function(app) {
    app.get('/', home);
    app.get('/login', login);
    app.get('/logout', logout);
    app.get('/home', realHome);
    app.get('/create', create);
    app.get('/update', selfUpdate);
    app.post('/delete', deleteAccount);
    app.post('/auth', auth);
    app.post('/creating',creating);
};