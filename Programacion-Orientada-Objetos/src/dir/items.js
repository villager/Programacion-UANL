"use strict";

let Database = require('../database').Database;
let items = Object.create(null);
const Auth = require('./general').Auth;

let database = new Database('items');

App.Items = database;

class Item {
    constructor(options) {
        this.id = 0;
        this.name = '';
        this.price = 0;
        this.owner = 0; // ID
        this.des = '';
        Object.assign(this, options);
    }
}
function isLogged(req, res, callback) {
    if (Auth.isLogged(req)) {
        callback();
      } else {
        res.render('errors/403');
      }    
}
function isOperator(req, res, callback) {
    if (Auth.isOperator(req)) {
      callback();
    } else {
      res.render('errors/403');
    }
}
function createItem(req, res) {
    isOperator(req, res, () => {
        res.render('items/new', {
            name: App.users[req.ip].fullname(),
            id: App.users[req.ip].id,
            rank: App.users[req.ip].rank
        });
    });
}
function creatingItem(req, res) {
    isOperator(req, res, async () => {
        let name = req.body.name;
        let desc = req.body.desc;
        let id = App.users[req.ip].id;
        try {
            let create = await database.creeate(name, 0, id, desc);
            if (create) {
                res.redirect('/home');
                database.loadAll(result => {
                    items[result.id] = new Item(result);
                });
            }
        } catch(e) {
            throw e;
        }   
    });
}
function getItemsByOwner(id) {
    let list = [];
    for (let i in items) {
        if (items[i].owner === id) {
            list.push(items[i]);
        }
    }
    return list;
}
function getItemsByUser(id) {
    let list = [];
    if(!App.users[id]) return [];
    if(!App.users[id].courses) return [];
    for (const item of App.users[id].courses) {
        list.push(items[item]);
    }
    return list;
}
function addingItem(req, res) {
    isLogged(req, res, async () => {
        let course = req.query.id;
        let user = App.users[req.ip];
        let options = Object.create(null);
        try {
            let result = await App.Db.getting(user.id, "courses");
            if (!result.rows[0].courses) {
                options.courses = [];
                options.courses.push(course);
                if (!user.courses){
                    user.courses = options.courses;
                }
                App.Db.updateById(user.id, options);
            } else {
                options.courses = Array.from(result.rows[0].courses);
                let c = options.courses;
                if (c.indexOf(parseInt(course, 10)) === -1) {
                    c.push(course);
                   options.courses = c;            
                    App.Db.updateById(user.id, options);
                    user.courses = c;
                }
            }
            res.redirect('/home');
        } catch(e) {
            throw e;
        }   
    });
}
function ownItems(req, res) {
    isLogged(req, res, async () => {
        let user = App.users[req.ip];
        res.render('items/list', {
            name: user.fullname(),
            id: user.id,
            rank: user.rank,
            items: getItemsByOwner(user.id)
        });
    });
}
function userbag(req,res) {
    let user = App.users[req.ip];
    res.render('items/list', {
        name: user.fullname(),
        id: user.id,
        rank: user.rank,
        items: getItemsByUser(req.ip)
    });
}
function listItems(req, res) {
    isOperator(req, res, async () => {
        try {
            let user = App.users[req.ip];
            let result = await database.get();
            res.render('items/list', {
                name: user.fullname(),
                id: user.id,
                rank: user.rank,
                items: result.rows
            });
        } catch(e) {
            throw e;
        }
    });
}
exports.init = function(app) {
    database.loadAll(result => {
        items[result.id] = new Item(result);
    });
    app.get('/items/new', createItem);
    app.post('/items/creating', creatingItem);
    app.get('/items/ownlist',ownItems);
    app.get('/items/list', listItems);
    app.post('/items/adding', addingItem);
    app.get('/items/userbag', userbag);
};