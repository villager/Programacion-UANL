"use strict";

const express = require('express'),
path = require('path'),
src = require('./src');

let App = global.App = {};
let app = App.app = express();
app.set('views', path.join(__dirname, 'views'));
app.set('view engine', 'ejs');
app.use(express.json());
app.use(express.urlencoded({ extended: false }));
app.use(express.static(path.join(__dirname, 'public')));
src.initDatabase();
src.init(app);
app.listen(3000, ()=> {
  console.log('Servidor en linea');
})
/**
 * Dev Routes
 */
// Error 404
app.use(function(req, res, next) {
  res.render('errors/404');
});

// Otros Errores
app.use(function(err, req, res) {
  res.locals.message = err.message;
  res.locals.error = req.app.get('env') === 'development' ? err : {};
  console.log(err);
  res.render('errors/general',{
    e: err,
    code: err.status || 500,
  });
});