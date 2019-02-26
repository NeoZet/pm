var http = require('http')
var neo4j = require('neo4j-driver').v1;
var client;

var driver = neo4j.driver('bolt://localhost');

function __loadFromCSV(PersonCSV, carsCSV) {
	var session = driver.session();
	Promise.all([
		session.run(
			"LOAD CSV WITH HEADERS FROM 'file:///" + PersonCSV + "' AS pers " 
			+ "MERGE (:Person { Score:toInt(pers.Score), FirstName:pers.FirstName, LastName:pers.LastName, Age:toInt(pers.Age)})"
		),
		session.run(
				"LOAD CSV WITH HEADERS FROM 'file:///" + carsCSV + "' AS cars " 
				+ "MERGE (:Car { Ticket:toInt(cars.Ticket), Car:cars.Car, Color:cars.Color})"
		)
	])
	.then(function() {
		session.close();
	})
	.catch(err => {
		console.log(err);
		session.close();
	});
}

function __createRelationships() {
	var session = driver.session();
	Promise.all([
		session.run(
			"MATCH (pers:Person) WHERE pers.Score=30 " +
			"MATCH (car:Car) WHERE car.Ticket=1111 " +
			"MERGE (pers)-[:IS_WON]->(car)"
		),
		session.run(
			"MATCH (pers:Person) WHERE pers.Score<30 AND pers.Score>25 " +
			"MATCH (car:Car) WHERE car.Ticket=1131 " +
			"MERGE (pers)-[:IS_WON]->(car)"		
		),
		session.run(
			"MATCH (pers:Person) WHERE pers.Score<=25 AND pers.Score>20 " +
			"MATCH (car:Car) WHERE car.Ticket=2111 " +
			"MERGE (pers)-[:IS_WON]->(car)"
		),
		
		session.run(
			"MATCH (pers:Person) WHERE pers.Score<=20 AND pers.Score>15 " +
			"MATCH (car:Car) WHERE car.Ticket=4111 " +
			"MERGE (pers)-[:IS_WON]->(car)"
		),
		session.run(
			"MATCH (pers:Person) WHERE pers.Score<=15 AND pers.Score>12 " +
			"MATCH (car:Car) WHERE car.Ticket=1141 " +
			"MERGE (pers)-[:IS_WON]->(car)"
		),
		session.run(
			"MATCH (pers:Person) WHERE pers.Score<=12 AND pers.Score>9 " +
			"MATCH (car:Car) WHERE car.Ticket=1211 " +
			"MERGE (pers)-[:IS_WON]->(car)"
		)
	])
	.then(function() {
			session.close();
	})
	.catch(err => {
		console.log(err);
		session.close();
	});
}

function createDb() {
	__loadFromCSV('persons.csv', 'cars.csv');
	__createRelationships();
} 

function findPerson(request, pattern) {
	var session = driver.session();
	return session.run(request, {substring: pattern})
	.then(res => {
		session.close();
		return res.records.map(record => {
				var persInfo = record.get('pers').properties;
				persInfo['Age'] = persInfo['Age'].toNumber();	
				persInfo['Score'] = persInfo['Score'].toNumber();
				persInfo['Car'] = record.get('car').properties['Car'];
				return persInfo;
		});
	})
	.catch(err => {
		console.log(err);
		session.close();
		throw err;
	});
}

function findByFirstName(pattern) {
	var request = 'MATCH (pers:Person)-[:IS_WON]-(car) WHERE pers.FirstName CONTAINS {substring} RETURN pers,car';
	return findPerson(request, pattern);
}

function findByLastName(pattern) {
	var request = 'MATCH (pers:Person)-[:IS_WON]-(car) WHERE pers.LastName CONTAINS {substring} RETURN pers,car';
	return findPerson(request, pattern);
}

function findByAge(pattern) {
	var request = 'MATCH (pers:Person)-[:IS_WON]-(car) WHERE toString(pers.Age) CONTAINS {substring} RETURN pers,car';
	return findPerson(request, pattern);
}

function findByScore(pattern) {
	var request = 'MATCH (pers:Person)-[:IS_WON]-(car) WHERE toString(pers.Score) CONTAINS {substring} RETURN pers,car';
	return findPerson(request, pattern);
}

module.exports.create = createDb;
module.exports.findByFirstName = findByFirstName;
module.exports.findByLastName = findByLastName;
module.exports.findByAge = findByAge;
module.exports.findByScore = findByScore;