const {Client} = require('pg');

const database = new Client({
	host     : 'localhost',
	user     : 'postgres',
	password : 'password',
	database : 'users'
});
exports.init = function() {
  database.connect(err => {
    if (err) {
      console.error('Fallo al conectar con la base de datos', err.stack);
      throw err;
    } else {
      console.log('Conexion establecida con la base de datos');
    }
  });
}
class Base {
  constructor(name) {
    this.name = name;
  }
  async loadAll(callback) {
    try {
      let results = await this.get();
      if(results && results.rows) {
        for (const result of results.rows) {
          callback(result);
        }        
      }
    } catch(e) {
      throw e;
    }
  }
  get(options ,...args) {
    if(!options) options = '';
    return new Promise ((resolve, reject) => {
      database.query(`SELECT * from ${this.name} ${options}`, args, (err, result) => {
        if(err) reject(err);
        resolve(result);
      });
    })
  }
  put(options, ...args) {
    return new Promise((resolve, reject) => {
      database.query(`INSERT INTO ${this.name} ${options}`, args, (err, result) => {
        if (err) reject(err);
        resolve(result);
      });
    });
  }
  length() {
    return new Promise((resolve, reject) =>{
      database.query(`SELECT count(*) from ${this.name}`,[], (err, result) => {
        if(err) reject(err);
        if(result && result.rows[0]) {
          resolve(result.rows[0].count);
        }
      });
    });
  }
  getting(id, column) {
    return new Promise((resolve, reject) =>{
      database.query(`SELECT ${column} from ${this.name} WHERE id=${id}`, [], (err, result) => {
        if(err) reject(err);
        if (result) resolve(result);
      });
    })
  }
  update(by, arg, option) {
    return new Promise((resolve, reject) => {
      database.query(`UPDATE ${this.name} SET ${arg} = ($1) WHERE ${by}`, [option], (err, result) => {
          if(err) reject(err);
          resolve(result);
      });
    });
  }
  delete(by, lookingFor) {
    return new Promise((resolve, reject) => {
      database.query(`DELETE from ${this.name} WHERE ${by}=${lookingFor}`);
    });
  }
}
class Database extends Base {
  constructor(name) {
    super(name);
  }
  async creeate(...args) {
    let msg = '("name", "price", "owner", "des")';
    msg += 'VALUES ($1, $2, $3, $4)';
    return this.put(msg, ...args);
  }
  async createUser(...args) {
    let msg = '("email", "password", "name", "lastname", "date", "rank") ';
    msg += 'VALUES ($1, $2, $3, $4, $5, $6)';
    return this.put(msg, ...args);
  }
  async look4Email(email) {
    return this.get("WHERE email = $1", email);
  }
  async look4Id(id) {
    return this.get("WHERE id =$1", id);
  }
  async updateById(id, options) {
    return this.update(`id = ${id}`, options);
  }
  async deleteUser(id) {
    return super.delete('id', id);
  }
  async update(by, args) {
    for (let i in args) {
      try {
        super.update(by, i, args[i]);        
      } catch(e) {
        throw e;
      }
    }
  }
}
exports.Database = Database;
exports.Base = Base;