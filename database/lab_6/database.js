var http = require('http')
var mongo = require('mongodb').MongoClient;

var url = "mongodb://localhost:27017/";
const mongoClient = new mongo(url, { useNewUrlParser: true });

function createDb() {
	mongoClient.connect(function(err, client) {
		if (err) {
			console.log(err);
			throw err;
		}

		var db = client.db('personsdb');
		var collection = db.collection('Persons');
		collection.insertMany([
			{Score: "10", FirstName: "Erofei", LastName: "Pavlov", Age: "23"},
			{Score: "25", FirstName: "Erofei", LastName: "Ivanov", Age: "19"},
			{Score: "17", FirstName: "Ivan", LastName: "Emin", Age: "20"},
			{Score: "14", FirstName: "Jhon", LastName: "Smirnov", Age: "21"},
			{Score: "28", FirstName: "Semyon", LastName: "Pavlov", Age: "20"},
			{Score: "30", FirstName: "Andrey", LastName: "Novov", Age: "19"},
			{Score: "17", FirstName: "Alex", LastName: "Emin", Age: "18"},
			{Score: "25", FirstName: "Genry", LastName: "Smirnov", Age: "21"},
			{Score: "25", FirstName: "Genry", LastName: "Pavlov", Age: "20"},
			{Score: "13", FirstName: "Anton", LastName: "No", Age: "19"},
			{Score: "28", FirstName: "Robbert", LastName: "Bobbert", Age: "18"},
			{Score: "13", FirstName: "Gorge", LastName: "South", Age: "25"},
			{Score: "12", FirstName: "Agus", LastName: "Hendro", Age: "24"},
			{Score: "11", FirstName: "Helly", LastName: "Ball", Age: "19"}
		], function(err, res) {
			if (err) {
				console.log(err);
				throw err;
			}
		});
		client.close();
	});
}

function mongo_client() {
	return new Promise(function(res, rej) {
		mongoClient.connect(function(err, client) {
			if (err) {
				console.log(err);
				rej(err);
			}
			console.log("CONNECT!");
			console.log(client);
			res(client);
		});
	});

}

function findByFirstName(client, pattern) {
	return new Promise(function(res, rej) {
			var db = client.db('personsdb');
			db.collection('Persons').find({FirstName: new RegExp(pattern)}).toArray((err, rec) => {
				if (err) throw rej(err);
				console.log(pattern);
				res(rec);
			});
		});
}

function findByLastName(client, pattern) {
	return new Promise(function(res, rej) {
			var db = client.db('personsdb');
			db.collection('Persons').find({LastName: new RegExp(pattern)}).toArray((err, rec) => {
				if (err) throw rej(err);
				res(rec);
			});
		});
}

function findByAge(client, pattern) {
	return new Promise(function(res, rej) {
			var db = client.db('personsdb');
			db.collection('Persons').find({Age: new RegExp(pattern)}).toArray((err, rec) => {
				if (err) throw rej(err);
				res(rec);
			});
		});
	}

function findByScore(client, pattern) {
	return new Promise(function(res, rej) {
			var db = client.db('personsdb');
			db.collection('Persons').find({Score: new RegExp(pattern)}).toArray((err, rec) => {
				if (err) throw rej(err);
				res(rec);
			});
		});
}

module.exports.findByFirstName = findByFirstName;
module.exports.findByLastName = findByLastName;
module.exports.findByAge = findByAge;
module.exports.findByScore = findByScore;
module.exports.mongo_client = mongo_client;

// createDb();