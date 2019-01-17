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
				let http_str = html_format(resolve.rows);
				return http_str;
			})
			.then(http_str => {
				res.write(http_str);
				res.end();
			});
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

// var findInFirst = function(rows, pattern) {
// 	result_list = [];
// 	for (var i = 0; i < rows.length; i++) {
// 		if (rows[i]['first_name'].indexOf(pattern) > -1) {
// 			result_list.push(rows[i]);
// 		}
// 	}
// 	return result_list;
// }


// var findInLast = function(rows, pattern) {
// 	result_list = [];
// 	for (var i = 0; i < rows.length; i++) {
// 		if (rows[i]['last_name'].indexOf(pattern) > -1) {
// 			result_list.push(rows[i]);
// 		}
// 	}
// 	return result_list;
// }

// var findInAge = function(rows, pattern) {
// 	result_list = [];
// 	for (var i = 0; i < rows.length; i++) {
// 		if (rows[i]['age'].indexOf(pattern) > -1) {
// 			result_list.push(rows[i]);
// 		}
// 	}
// 	return result_list;
// }

// var findInID = function(rows, pattern) {
// 	result_list = [];
// 	for (var i = 0; i < rows.length; i++) {
// 		if (rows[i]['id'].indexOf(pattern) > -1) {
// 			result_list.push(rows[i]);
// 		}
// 	}
// 	return result_list;
// }

function html_format(rows) {
	var str = '<!DOCTYPE html>\n' +
		'<html>\n' +
		'<head>\n' +
		'<meta charset=\'utf-8\'>\n' +
		'</head>\n' +
		'<body>\n' +
		'<table border="1px" cellspacing="0px" width="30%" height="10%">\n' +
		'<tr>\n' +
		'<td style="color: black">\nФамилия Имя</td>\n' +
		'<td id="str" align="center">\nВозрст</td>\n' +
		'<td id="numb1" align="center">\nID</td>\n' +
		'</tr>\n';
	for (var i = 0; i < rows.length; i++) {
		str += '<tr>\n' +
			'<td style="color: black">\n' + rows[i]['last_name'] + ' ' + rows[i]['first_name'] + '</td>\n' +
			'<td id="str" align="center">\n' + rows[i]['age'] + '</td>\n' +
			'<td id="numb1" align="center">\n' + rows[i]['id'] + '</td>\n' +
			'</tr>\n';
	}
	str += '</table>\n' +
		'</body>\n' +
		'</html>\n';
	return str;
}