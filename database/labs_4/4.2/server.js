var http = require('http')
var fs = require('fs')
var db = require('./database.js');

http.createServer(function(req, res) {
	switch(req.url) {
		case '/':
			fs.readFile("./ajax.html", function(err, cont) {
				res.writeHead(200, {'Content-Type':'text/html'});
				res.end(cont);
			});
			break;
		case '/INF':
			res.writeHead(200, {'Content-Type':'text/html'});
			
			var d = db.run();
			res.write(html_format());
			console.log(d);
			res.end('---call1----');			
			break;	
	}
}).listen(3000);


function selectAll(database) {
	database.all("SELECT * FROM PERSONS", function(err, rows) {
		rw = []
		for(var i = 0; i < rows.length; i++) {
			rw.push(rows[i]);
		}
	});		
}

var findInLast = function (err, rows, pattern) {
	index_list = [];
	for(var i = 0; i < rows.length; i++) {
	    if(str.indexOf(rows[i].LAST_NAME.indexOf(pattern) > -1)) {
			index_list.push(i);   	
	    }
	}
}

var findInFirst = function (err, rows, pattern) {
	index_list = [];
	for(var i = 0; i < rows.length; i++) {
	    if(str.indexOf(rows[i].FIRST_NAME.indexOf(pattern) > -1)) {
			index_list.push(i);   	
	    }	    
	}
}

var findInID = function (err, rows, pattern) {
	index_list = [];
	for(var i = 0; i < rows.length; i++) {
	    if(str.indexOf(rows[i].ID.indexOf(pattern) > -1)) {
			index_list.push(i);   	
	    }	    
	}
}

var findInAge = function (err, rows, pattern) {
	index_list = [];
	for(var i = 0; i < rows.length; i++) {
	    if(str.indexOf(rows[i].ID.indexOf(pattern) > -1)) {
			index_list.push(i);   	
	    }	    
	}
}


var html_print = function (err, rows) {
	console.log(rows);

	// if(err) {
	// 	console.log("ERROR");
 // 	}
 // 	else {
 // 		response.write(str);
 // 	}
}

function html_format(rows) {
	var str ='<!DOCTYPE html>\n' +
		   	'<html>\n'+
		   	'<head>\n' +
		   	'<meta charset=\'utf-8\'>\n' +
		   	'</head>\n'+
		   	'<body>\n' +
			'<table border="1px" cellspacing="0px" width="30%" height="10%">\n' +
			'<tr>\n'+
		    '<td style="color: black">\nФамилия Имя</td>\n'+
		    '<td id="str" align="center">\nВозрст</td>\n'+
		    '<td id="numb1" align="center">\nID</td>\n'+
		    '</tr>\n';
	for(var i = 0; i < rows.length; i++) {
	   str += '<tr>\n'+
	   '<td style="color: black">\n'+ rows[i].LAST_NAME + ' ' + rows[i].FIRST_NAME + '</td>\n'+
	   '<td id="str" align="center">\n' + rows[i].AGE + '</td>\n'+
	   '<td id="numb1" align="center">\n' + rows[i].ID + '</td>\n'+
	   '</tr>\n';
	}
	str += '</table>\n'+
		    '</body>\n'+
		    '</html>\n';
	return str;
}