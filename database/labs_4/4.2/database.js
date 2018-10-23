var http = require('http')
var sqlite3 = require('sqlite3').verbose();
var db;

function createDb() {
    db = new sqlite3.Database('persons.db', createTable);
}


function createTable() {
    db.run("CREATE TABLE IF NOT EXISTS PERSONS (ID INTEGER, FIRST_NAME TEXT, LAST_NAME TEXT, AGE INTEGER)", setRows);
}

function setRows() {
    var stmt = db.prepare("INSERT INTO PERSONS VALUES (?, ?, ?, ?)");

    stmt.run(1111, "Erofei", "Pavlov", 23);
    stmt.run(1112, "Erofei", "Ivanov", 19);
    stmt.run(1121, "Ivan", "Emin", 20);
    stmt.run(1211, "Jhon", "Smirnov", 21);
    stmt.run(2111, "Semyon", "Pavlov", 20);
    stmt.run(1311, "Andrey", "Novov", 19);
    stmt.run(4111, "Anton", "Emin", 18);
    stmt.run(1113, "Genry", "Smirnov", 21);
    stmt.run(1141, "Genry", "Pavlov", 20);
    stmt.run(1411, "Anton", "No", 19);
    stmt.run(4111, "Anton", "Emin", 18);    
}

var html_print = function (err, rows) {
    if(err) {
	console.log("ERROR");
    }
    http.createServer(function(request, response) {
	response.writeHead(200, {'Content-Type':'text/html'});
	response.write('<!DOCTYPE html>\n' +
		   '<html>\n'+
		   '<head>\n' +
		   '<meta charset=\'utf-8\'>\n' +
		   '</head>\n'+
		   '<body>\n');
	response.write('<table border="1px" cellspacing="0px" width="30%" height="10%">\n');
	response.write('<tr>\n'+
		       '<td style="color: black">\nФамилия Имя</td>\n'+
		       '<td id="str" align="center">\nВозрст</td>\n'+
		       '<td id="numb1" align="center">\nID</td>\n'+
		       '</tr>\n');
	for(var i = 0; i < rows.length; i++) {
	    response.write('<tr>\n'+
			   '<td style="color: black">\n'+ rows[i].LAST_NAME + ' ' + rows[i].FIRST_NAME + '</td>\n'+
			   '<td id="str" align="center">\n' + rows[i].AGE + '</td>\n'+
			   '<td id="numb1" align="center">\n' + rows[i].ID + '</td>\n'+
			   '</tr>\n');
	}
	response.end('</table>\n'+
		     '</body>\n'+
		     '</html>\n');
    }).listen(3000);
}

function readAllRows() {
    db.all("SELECT * FROM PERSONS", html_print);
}

function closeDb() {
    db.close();
}

function run() {
    createDb();
    return db;
}	

module.exports.run = run;