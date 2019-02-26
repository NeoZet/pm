var http = require('http')
var fs = require('fs')
var db = require('./database.js');

console.log("Start server");

db.create();

http.createServer(function(req, res) {
	request = req.url.split('&');
	res.writeHead(200, {
		'Content-Type': 'text/html'
	});
	var rules_callbacks = {
		'/fname': db.findByFirstName,
		'/lname': db.findByLastName,
		'/age'  : db.findByAge,
		'/score': db.findByScore
	};
	if (request[0].localeCompare('/') == 0) {
		fs.readFile("./ajax.html", function(err, cont) {
			res.end(cont);
		});
	}else if(request[0].localeCompare('/fname') == 0 ||
			 request[0].localeCompare('/lname') == 0 ||
			 request[0].localeCompare('/age') == 0   ||
			 request[0].localeCompare('/score') == 0) 
	{
		findInDB(request[1], rules_callbacks[request[0]])
			.then(resolve => {		
				res.write(JSON.stringify(resolve));
				res.end();
			});
	}
}).listen(3000);


function findInDB(pattern, rules_callback) {
	return new Promise((resolve, reject) => {
		rules_callback(pattern)
			.then(function(res) {
				resolve(res);
			});
	});
}