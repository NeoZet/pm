var http = require('http')
var cassandra = require('cassandra-driver');
var client;

client = new cassandra.Client({
	contactPoints: ['127.0.0.1'],
	localDataCenter: 'datacenter1',
});

function createDb() {
	client.connect()
		.then(function() {
			const query = "CREATE KEYSPACE IF NOT EXISTS lab5 WITH replication =" +
				"{'class': 'SimpleStrategy', 'replication_factor': '1' }";

			return client.execute(query);
		})
		.then(function() {
			const query = "CREATE TABLE IF NOT EXISTS lab5.persons " +
				"(ID int, FIRST_NAME TEXT, LAST_NAME TEXT, AGE int, PRIMARY KEY (ID))";
			return client.execute(query);
		})
		.then(function() {
			var query = "INSERT INTO lab5.persons (ID, FIRST_NAME, LAST_NAME, AGE) VALUES (?, ?, ?, ?)";

			params = [1111, "Erofei", "Pavlov", 23];
			client.execute(query, params, {
				prepare: true
			});
			params = [1112, "Erofei", "Ivanov", 19];
			client.execute(query, params, {
				prepare: true
			});
			params = [1121, "Ivan", "Emin", 20];
			client.execute(query, params, {
				prepare: true
			});
			params = [1211, "Jhon", "Smirnov", 21];
			client.execute(query, params, {
				prepare: true
			});
			params = [2111, "Semyon", "Pavlov", 20];
			client.execute(query, params, {
				prepare: true
			});
			params = [1311, "Andrey", "Novov", 19];
			client.execute(query, params, {
				prepare: true
			});
			params = [4111, "Alex", "Emin", 18];
			client.execute(query, params, {
				prepare: true
			});
			params = [1113, "Genry", "Smirnov", 21];
			client.execute(query, params, {
				prepare: true
			});
			params = [1141, "Genry", "Pavlov", 20];
			client.execute(query, params, {
				prepare: true
			});
			params = [1411, "Anton", "No", 19];
			client.execute(query, params, {
				prepare: true
			});
		});
	return client;
}

function findByFirstName(clt, pattern) {
	query = "CREATE CUSTOM INDEX IF NOT EXISTS ON lab5.persons ( FIRST_NAME ) USING 'org.apache.cassandra.index.sasi.SASIIndex' WITH OPTIONS = {'analyzed' : 'true','analyzer_class' : 'org.apache.cassandra.index.sasi.analyzer.NonTokenizingAnalyzer','case_sensitive' : 'false', 'mode' : 'CONTAINS'}";
	client.execute(query);
	query = "SELECT * FROM lab5.persons where FIRST_NAME LIKE ?";
	return clt.execute(query, ['%' + pattern + '%'], {
		prepare: true
	});
}

function findByLastName(clt, pattern) {
	query = "CREATE CUSTOM INDEX IF NOT EXISTS last_idx ON lab5.persons ( LAST_NAME ) USING 'org.apache.cassandra.index.sasi.SASIIndex' WITH OPTIONS = {'analyzed' : 'true','analyzer_class' : 'org.apache.cassandra.index.sasi.analyzer.NonTokenizingAnalyzer','case_sensitive' : 'false', 'mode' : 'CONTAINS'}";
    client.execute(query);
	query = "SELECT * FROM lab5.persons where LAST_NAME LIKE ?";
	return clt.execute(query, ['%' + pattern + '%'], {
		prepare: true
	});
}

function findByAge(clt, pattern) {
	query = "CREATE INDEX IF NOT EXISTS ON lab5.persons ( AGE )";
	client.execute(query);
	query = "SELECT * FROM lab5.persons where AGE = ?";
	return clt.execute(query, [pattern], {
		prepare: true
	});
}

function findByID(clt, pattern) {
	// query = "CREATE CUSTOM INDEX IF NOT EXISTS ON lab5.persons ( ID )";
	// client.execute(query);
	query = "SELECT * FROM lab5.persons where ID = ?";
	return clt.execute(query, [pattern], {
		prepare: true
	});
}

function closeDb() {
	client.shutdown();
}

function run() {
	openDb();
	return db;
}

// var cl = createDb();
// findByFirstName(cl, "Genry")
// 	.then(function(res) {
// 		console.log(res);
// 	});

module.exports.create = createDb;
module.exports.findByFirstName = findByFirstName;
module.exports.findByLastName = findByLastName;
module.exports.findByAge = findByAge;
module.exports.findByID = findByID;