var http = require('http')
var fs = require('fs')
var db = require('./database.js');

console.log("Start server");

http.createServer(function(req, res) {
	request = req.url.split('&');
	res.writeHead(200, {
		'Content-Type': 'text/html'
	});
	var client = db.create();
	var rules_callbacks = {
		'/fname': db.findByFirstName,
		'/lname': db.findByLastName,
		'/age'  : db.findByAge,
		'/id'   : db.findByID
	};
	if (request[0].localeCompare('/') == 0) {
		fs.readFile("./ajax.html", function(err, cont) {
			res.end(cont);
		});
	} else if (request[0].localeCompare('/fname') == 0 ||
		request[0].localeCompare('/lname') == 0 ||
		request[0].localeCompare('/age') == 0 ||
		request[0].localeCompare('/id') == 0) {
		findInDB(client, request[1], rules_callbacks[request[0]])
			.then(resolve => {
				res.write(JSON.stringify(resolve.rows));
				res.end();
			})
	}
}).listen(3000);


function findInDB(client, pattern, rules_callback) {
	return new Promise((resolve, reject) => {
		rules_callback(client, pattern)
			.then(function(res) {
				resolve(res);
			});
	});
}