import java.util.ArrayList;
import java.util.Collection;
import java.util.List;
import java.sql.*;

import com.j256.ormlite.dao.BaseDaoImpl;
import com.j256.ormlite.support.ConnectionSource;
import com.j256.ormlite.table.TableUtils;

public class DaoPerson extends BaseDaoImpl<Person, Integer> {
	DaoPerson(ConnectionSource connectionSource) throws SQLException {
	        super(connectionSource, Person.class);
	}

	public void insertPerson(Person person) {
		try {
			this.update(person);
		} catch (SQLException ex) {
			throw new RuntimeException("Error connecting to the database", ex);
		}
	}
	
	public void createTable(List<Person> PersonsList) throws SQLException {
		TableUtils.createTableIfNotExists(connectionSource, Person.class);
		for(int i = 0; i < PersonsList.size(); ++i) {
			this.createIfNotExists(PersonsList.get(i));
		}
	}

	public List<Person> selectAll() throws SQLException {
		return this.queryForAll();
	}
	
	public List<Person> selectByFirstName(String firstName) {
		try {
			return super.queryForEq("first_name", firstName);
		} catch (SQLException ex) {
			throw new RuntimeException("Error connecting to the database", ex);
		}
	}

	public List<Person> selectByLastName(String lastName) {
		try {
			return super.queryForEq("last_name", lastName);
		} catch (SQLException ex) {
			throw new RuntimeException("Error connecting to the database", ex);
		}
	}

	public List<Person> selectByAge(int age) {
		try {
			return super.queryForEq("age", age);
		} catch (SQLException ex) {
			throw new RuntimeException("Error connecting to the database", ex);
		}
	}

	public List<Person> selectByID(int id) {
		try {
			return super.queryForEq("id", id);
		} catch (SQLException ex) {
			throw new RuntimeException("Error connecting to the database", ex);
		}
	}

	public Person deletePerson(Person person) {
		try {
			this.delete(person);
		} catch (SQLException ex) {
			throw new RuntimeException("Error connecting to the database", ex);
		}
		return person;
	}
}
