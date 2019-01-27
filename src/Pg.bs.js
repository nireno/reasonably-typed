// Generated by BUCKLESCRIPT VERSION 4.0.18, PLEASE EDIT WITH CARE
'use strict';

var Pg = require("pg");

function makePool(database, user, password, $staropt$star, $staropt$star$1, param) {
  var host = $staropt$star !== undefined ? $staropt$star : "localhost";
  var port = $staropt$star$1 !== undefined ? $staropt$star$1 : 5432;
  return new Pg.Pool({
              database: database,
              user: user,
              password: password,
              host: host,
              port: port
            });
}

var query = ( function(pool, sql) {
    return pool.query(sql);
  });

var query2 = ( function(pool, sql, options) {
    return pool.query(sql, options);
  });

var endPool = (function(pool){ pool.end();});

exports.makePool = makePool;
exports.query = query;
exports.query2 = query2;
exports.endPool = endPool;
/* query Not a pure module */