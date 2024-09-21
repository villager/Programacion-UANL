'use strict';
/**
 * Admin Routes
 */
const Auth = require('./general').Auth;

function isAdmin(req, res, callback) {
    if (Auth.isAdmin(req)) {
      callback();
    } else {
      res.render('errors/403');
    }
  }
  
function adminHome(req, res)  {
    isAdmin(req, res, () => {
      res.redirect('/home');
    });
}
function userList(req, res) {
    isAdmin(req, res, async () => {
        try {
          let result = await App.Db.get();
          res.render('admin/list', {
            name: App.users[req.ip].fullname(),
            rank: App.users[req.ip].rank,
            userlist: result.rows,
            id: App.users[req.ip].id,
          });
        } catch(e) {
          throw e;
        }
    });
}
function toId(text) {
  if (typeof text !== 'string' && typeof text !== 'number') return '';
  return ('' + text).toLowerCase().replace(/[^a-z0-9]+/g, '');
}
const USER_ALIAS = new Set(['usuario', 'user', 'cliente', 'clientes', 'usuarios', 'users']);
const OPERATOR_ALIAS = new Set(['operator', 'operador', 'creador', 'maestro', 'teacher', 'operators',  'operadores']);
const ADMIN_ALIAS = new Set(['admin', 'admins', 'administrador', 'administradores', 'jefe', 'jefes']);

async function realList(searchBy, lookingFor) {
  try {
    let results = await App.Db.get();
    let list = [];
    for (const result of results.rows) {
      switch (searchBy) {
        case 'id':
          if (result.id === parseInt(lookingFor, 10)) {
            list.push(result);
          }
        break;
        case 'rank':
          if (USER_ALIAS.has(toId(lookingFor)) && result.rank === 'user') {
            list.push(result);
          }
          if (OPERATOR_ALIAS.has(toId(lookingFor)) && result.rank === 'operator') {
            list.push(result);
          }            
          if (ADMIN_ALIAS.has(toId(lookingFor)) && result.rank === 'admin') {
            list.push(result);
          }
        break;
        case 'email':
          if(result[searchBy].indexOf(lookingFor) > -1) {
            list.push(result);
          }
        break;
        case 'name':
          if (toId(result.name).indexOf(lookingFor) > -1 || toId(result.lastname).indexOf(lookingFor) > -1) {
            list.push(result);
          }
        break;
      }
    }  
    return list;
  } catch(e) {
    throw e;
  }
}
function userListRedir(req, res) {
    isAdmin(req, res, () => {
        res.redirect('/admin/users');
    });
}
function update(req, res) {
    isAdmin(req, res, async () => {
        let id = req.query.id;
        if (!id) throw "Uncaught ID";
        try {
          let result = await App.Db.look4Id(id);
          res.render('admin/update', {
            name: App.users[req.ip].fullname(),
            data: result.rows[0],
            rank: App.users[req.ip].rank,
            id: App.users[req.ip].id,
          });
        } catch(e) {
          throw e;
        }
    });
}
async function updating(req, res) {
    let id = req.query.id;
    let options = Object.create(null);
    if (req.body.name) options.name = req.body.name;
    if (req.body.lastname) options.lastname = req.body.lastname;
    if (req.body.rank) options.rank = req.body.rank;
    if (req.body.email) options.email = req.body.email;

    if (req.body.password) {
      options.password = req.body.password;
    }
    try {
      App.Db.updateById(id, options);
      if(!req.body.self && Auth.isAdmin(req)) {
        res.redirect('/admin/users');
      } else {
        res.redirect('/home');
      }
    } catch(e) {
      throw e;
    }
}
async function userSearch(req, res) {
  isAdmin(req, res, async () => {
      try {
        let result = await App.Db.get();
        let searchBy = req.query.searchBy;
        let lookingFor = req.query.lookingFor;
        if (!searchBy || !lookingFor) {
          res.render('admin/search', {
            name: App.users[req.ip].fullname(),
            rank: App.users[req.ip].rank,
            userlist: result.rows,
            id: App.users[req.ip].id,
          }); 
        } else {
          let result = await realList(searchBy, lookingFor);
          res.render('admin/search-result', {
            name: App.users[req.ip].fullname(),
            rank: App.users[req.ip].rank,
            userlist: result,
            id: App.users[req.ip].id,
            lookingFor: lookingFor,
            searchBy: searchBy,
          });           
        }
      } catch(e) {
        throw e;
      }
  });
}
exports.init = function(app) {
    app.get('/admin', adminHome);
    app.get('/admin/users', userList);
    app.get('/admin/users/list', userListRedir);
    app.get('/admin/users/update', update);
    app.post('/updating', updating);
    app.get('/admin/users/search', userSearch);
}