import java.util.HashSet;
import java.util.Set;
import java.sql.*;

public class DaoPerson implements DaoInterface<Person> {
	private Connection connection;
	private Statement statement;
	private PreparedStatement preparedStatement = null;

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
			person.setID(rs.getInt("ID"));
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
			stmt.execute("CREATE TABLE PERSONS (ID INTEGER, FIRST_NAME VARCHAR(255), LAST_NAME VARCHAR(255), AGE INTEGER)");

			stmt.execute("INSERT INTO PERSONS VALUES (1111, 'Erofei', 'Pavlov', 23)");
			stmt.execute("INSERT INTO PERSONS VALUES (1112, 'Erofei', 'Ivanov', 19)");
			stmt.execute("INSERT INTO PERSONS VALUES (1121, 'Ivan', 'Emin', 20)");
			stmt.execute("INSERT INTO PERSONS VALUES (1211, 'Jhon', 'Smirnov', 21)");
			stmt.execute("INSERT INTO PERSONS VALUES (2111, 'Semyon', 'Pavlov', 20)");
			stmt.execute("INSERT INTO PERSONS VALUES (1311, 'Andrey', 'Novov', 19)");
			stmt.execute("INSERT INTO PERSONS VALUES (4111, 'Anton', 'Emin', 18)");
			stmt.execute("INSERT INTO PERSONS VALUES (1113, 'Genry', 'Smirnov', 21)");
			stmt.execute("INSERT INTO PERSONS VALUES (1141, 'Genry', 'Pavlov', 20)");
			stmt.execute("INSERT INTO PERSONS VALUES (1411, 'Anton', 'No', 19)");
			stmt.execute("INSERT INTO PERSONS VALUES (4111, 'Anton', 'Emin', 18)");
			// ResultSet rs =
			// stmt.executeQuery("SELECT * FROM PERSONS WHERE FIRST_NAME='Anton' or FIRST_NAME='Jhon'");
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

	public void insert(Person person) {
		try {
			preparedStatement = connection
					.prepareStatement("INSERT INTO PERSONS VALUES (?, ?, ?, ?)");
			preparedStatement.setInt(1, person.getID());
			preparedStatement.setString(2, person.getFirstName());
			preparedStatement.setString(3, person.getLastName());
			preparedStatement.setInt(4, person.getAge());
			preparedStatement.execute();
		} catch (SQLException ex) {
			throw new RuntimeException("Error connecting to the database", ex);
		}
	}

	public Set<Person> selectByFirstName(String firstName) {
		try {
			preparedStatement = connection
					.prepareStatement("SELECT * FROM PERSONS WHERE FIRST_NAME=?");
			preparedStatement.setString(1, firstName);
			ResultSet rs = preparedStatement.executeQuery();
			Set<Person> persons = new HashSet<Person>();
			while (rs.next()) {
				persons.add(extractData(rs));
			}
			return persons;
		} catch (SQLException ex) {
			throw new RuntimeException("Error connecting to the database", ex);
		}
	}

	public Set<Person> selectByLastName(String lastName) {
		try {
			preparedStatement = connection
					.prepareStatement("SELECT * FROM PERSONS WHERE LAST_NAME=?");
			preparedStatement.setString(1, lastName);
			ResultSet rs = preparedStatement.executeQuery();
			Set<Person> persons = new HashSet<Person>();
			while (rs.next()) {
				persons.add(extractData(rs));
			}
			return persons;
		} catch (SQLException ex) {
			throw new RuntimeException("Error connecting to the database", ex);
		}
	}

	public Set<Person> selectByAge(int age) {
		try {
			preparedStatement = connection
					.prepareStatement("SELECT * FROM PERSONS WHERE AGE=?");
			preparedStatement.setInt(1, age);
			ResultSet rs = preparedStatement.executeQuery();
			Set<Person> persons = new HashSet<Person>();
			while (rs.next()) {
				persons.add(extractData(rs));
			}
			return persons;
		} catch (SQLException ex) {
			throw new RuntimeException("Error connecting to the database", ex);
		}
	}

	public Set<Person> selectByID(int id) {
		try {
			preparedStatement = connection
					.prepareStatement("SELECT * FROM PERSONS WHERE ID=?");
			preparedStatement.setInt(1, id);
			ResultSet rs = preparedStatement.executeQuery();
			Set<Person> persons = new HashSet<Person>();
			while (rs.next()) {
				persons.add(extractData(rs));
			}
			return persons;
		} catch (SQLException ex) {
			throw new RuntimeException("Error connecting to the database", ex);
		}
	}

	public Person delete(Person person) {
		try {
			preparedStatement = connection
					.prepareStatement("DELETE FROM PERSONS WHERE ID=? " +
							"AND FIRST_NAME=? " + 
							"AND LAST_NAME=? " +
							"AND AGE=?)");
			preparedStatement.setInt(1, person.getID());
			preparedStatement.setString(1, person.getFirstName());
			preparedStatement.setString(1, person.getLastName());
			preparedStatement.setInt(1, person.getAge());
			ResultSet rs = 	preparedStatement.executeQuery();
			return extractData(rs);
		} catch (SQLException ex) {
			throw new RuntimeException("Error connecting to the database", ex);
		}
	}
}
