var http = require('http')
var fs = require('fs')
var db = require('./database.js');

console.log("Start server");

http.createServer(function(req, res) {	
	request = req.url.split('&');
	res.writeHead(200, {'Content-Type':'text/html'});
	var database = db.run();		
	var rules_callbacks = {
		'/fname' : findInFirst,
		'/lname' : findInLast,
		'/age' : findInAge,
		'/id' : findInID
	};	
	if (request[0].localeCompare('/') == 0) {
		fs.readFile("./ajax.html", function(err, cont) {
				res.end(cont);
		});
	}
	else if (request[0].localeCompare('/fname') == 0 || 
			 request[0].localeCompare('/lname') == 0 ||
			 request[0].localeCompare('/age') == 0 ||
			 request[0].localeCompare('/id') == 0) 
	{
		findInDB(database, request[1], rules_callbacks[request[0]])
			.then(resolve => {
				res.write(JSON.stringify(resolve));
				res.end();
			})
	}
}).listen(3000);


function findInDB(database, pattern, rules_callback) {
	return new Promise((resolve, reject) => {
		database.all("SELECT * FROM PERSONS", function (err, rows) {
			if(err) {
				var error = new Error("ERROR");
				error.code = -1;				
				reject(error);
 			}
 			data = [];
			for(var i = 0; i < rows.length; i++) {			
				data.push(rows[i]);			
			}		
			result_list = rules_callback(data, pattern);
			resolve(result_list);	
		});	
	});
}

var findInFirst = function (rows, pattern) {
	result_list = [];
	for(var i = 0; i < rows.length; i++) {	
	    if(rows[i].FIRST_NAME.indexOf(pattern) > -1) {
			result_list.push(rows[i]);   	
	    }	    
	}
	return result_list;
}


var findInLast = function (rows, pattern) {
	result_list = [];
	for(var i = 0; i < rows.length; i++) {
	    if(rows[i].LAST_NAME.indexOf(pattern) > -1) {
			result_list.push(rows[i]);   	
	    }
	}
	return result_list;
}

var findInAge = function (rows, pattern) {
	result_list = [];
	for(var i = 0; i < rows.length; i++) {
	    if(rows[i].AGE.toString().indexOf(pattern) > -1) {
			result_list.push(rows[i]);   	
	    }	    
	}
	return result_list;
}

var findInID = function (rows, pattern) {
	result_list = [];
	for(var i = 0; i < rows.length; i++) {
	    if(rows[i].ID.toString().indexOf(pattern) > -1) {
			result_list.push(rows[i]);   	
	    }	    
	}
	return result_list;
}