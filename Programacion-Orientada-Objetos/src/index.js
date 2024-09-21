'use strict';
const fs = require('fs');
const path = require('path');


exports.init =  function (app) {
    for (let file of fs.readdirSync(path.resolve(__dirname, '../src/dir'))) {
        let e = require('./dir/' + file);
        if (typeof e.init === 'function') e.init(app);
    }
}

exports.User = require('./users').User;
exports.Base = require('./database').Base;
exports.Database = require('./database').Database;
exports.initDatabase = require('./database').init;
