import java.util.HashSet;
import java.util.Set;
import java.sql.*;

public class DaoPerson implements DaoInterface<Person> {
	private Connection connection;
	private Statement statement;

	DaoPerson() {
		connection = ConnectionFactory.getConnection();
		try {
			statement = connection.createStatement();
		} catch (SQLException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}

	private Person extractData(ResultSet rs) {
		Person person = new Person();
		try {
			person.setFirstName(rs.getString("FIRST_NAME"));
			person.setLastName(rs.getString("LAST_NAME"));
			person.setAge(rs.getInt("AGE"));
			// person.setID(rs.getInt("ID"));
			return person;
		} catch (SQLException e) {
			throw new RuntimeException("Error connecting to the database", e);
		}
	}

	public void createTable(String JDBC_DRIVER, String DB_URL) {
		Connection conn = null;
		Statement stmt = null;
		try {
			Class.forName(JDBC_DRIVER).newInstance();
			conn = connection;
			stmt = statement;
			stmt.execute("CREATE TABLE PERSONS (FIRST_NAME VARCHAR(255), LAST_NAME VARCHAR(255), AGE INTEGER)");

			stmt.execute("INSERT INTO PERSONS VALUES ('Erofei', 'Pavlov', 23)");
			stmt.execute("INSERT INTO PERSONS VALUES ('Erofei', 'Ivanov', 19)");
			stmt.execute("INSERT INTO PERSONS VALUES ('Ivan', 'Emin', 20)");
			stmt.execute("INSERT INTO PERSONS VALUES ('Jhon', 'Smirnov', 21)");
			stmt.execute("INSERT INTO PERSONS VALUES ('Semyon', 'Pavlov', 20)");
			stmt.execute("INSERT INTO PERSONS VALUES ('Andrey', 'Novov', 19)");
			stmt.execute("INSERT INTO PERSONS VALUES ('Anton', 'Emin', 18)");
			stmt.execute("INSERT INTO PERSONS VALUES ('Genry', 'Smirnov', 21)");
			stmt.execute("INSERT INTO PERSONS VALUES ('Genry', 'Pavlov', 20)");
			stmt.execute("INSERT INTO PERSONS VALUES ('Anton', 'No', 19)");
			stmt.execute("INSERT INTO PERSONS VALUES ('Anton', 'Emin', 18)");
			ResultSet rs = stmt.executeQuery("SELECT * FROM PERSONS WHERE FIRST_NAME='Anton' or FIRST_NAME='Jhon'");
			stmt.close();
			conn.close();
		} catch (SQLException se) {
			se.printStackTrace();
		} catch (Exception e) {
			e.printStackTrace();
		} finally {
			try {
				if (stmt != null)
					stmt.close();
			} catch (SQLException se2) {
			}
			try {
				if (conn != null)
					conn.close();
			} catch (SQLException se) {
				se.printStackTrace();
			}
		}
	}

	public Set<Person> selectAll() {
		try {
			ResultSet rs = statement.executeQuery("SELECT * FROM PERSONS");
			Set<Person> persons = new HashSet<Person>();
			while (rs.next()) {
				persons.add(extractData(rs));
			}
			return persons;
		} catch (SQLException ex) {
			throw new RuntimeException("Error connecting to the database", ex);
		}
	}

	public void insert(Person t) {
		try {
			statement.execute("INSERT INTO PERSONS VALUES ('Jerremy', 'Smith', 25)");
		} catch (SQLException ex) {
			throw new RuntimeException("Error connecting to the database", ex);
		}
	}

	public Set<Person> selectByFirstName(String firstName) {
		try {
			ResultSet rs = statement.executeQuery("SELECT * FROM PERSONS WHERE FIRST_NAME='" + firstName + "'");
			Set<Person> persons = new HashSet<Person>();
			while (rs.next()) {
				persons.add(extractData(rs));
			}
			return persons;
		} catch (SQLException ex) {
			throw new RuntimeException("Error connecting to the database", ex);
		}
	}

	public Set selectByLastName(String lastName) {
		try {
			ResultSet rs = statement.executeQuery("SELECT * FROM PERSONS WHERE LAST_NAME='" + lastName + "'");
			Set<Person> persons = new HashSet<Person>();
			while (rs.next()) {
				persons.add(extractData(rs));
			}
			return persons;
		} catch (SQLException ex) {
			throw new RuntimeException("Error connecting to the database", ex);
		}
	}

	public Set selectByAge(int age) {
		return null;

	}

	public Set selectByID(int id) {
		return null;

	}
}
