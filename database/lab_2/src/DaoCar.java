import java.util.HashSet;
import java.util.Set;
import java.sql.*;

public class DaoCar implements DaoInterface<Car> {
	private Connection connection;
	private Statement statement;

	DaoCar() {
		connection = ConnectionFactory.getConnection();
		try {
			statement = connection.createStatement();
		} catch (SQLException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}

	private Car extractData(ResultSet rs) {
		Car car = new Car();
		try {
			car.setModel(rs.getString("MODEL"));
			car.setColor(rs.getString("COLOR"));
			car.setID(rs.getInt("ID"));
			return car;
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
			stmt.execute("CREATE TABLE CARS (ID INTEGER, MODEL VARCHAR(255), COLOR VARCHAR(255))");

			stmt.execute("INSERT INTO CARS VALUES (1111, 'Ferrari', 'white')");
			stmt.execute("INSERT INTO CARS VALUES (1211, 'VAZ-2107', 'black')");
			stmt.execute("INSERT INTO CARS VALUES (1141, 'Reno Logan', 'grey')");
			stmt.execute("INSERT INTO CARS VALUES (1131, 'Mercedes-Benz', 'black')");
			stmt.execute("INSERT INTO CARS VALUES (2111, 'BMW-X6', 'red')");
			stmt.execute("INSERT INTO CARS VALUES (4111, 'Honda-Civic', 'white')");
			// ResultSet rs =
			// stmt.executeQuery("SELECT * FROM CARS WHERE FIRST_NAME='Anton' or FIRST_NAME='Jhon'");
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

	public Set<Car> selectAll() {
		try {
			ResultSet rs = statement.executeQuery("SELECT * FROM CARS");
			Set<Car> cars = new HashSet<Car>();
			while (rs.next()) {
				cars.add(extractData(rs));
			}
			return cars;
		} catch (SQLException ex) {
			throw new RuntimeException("Error connecting to the database", ex);
		}
	}

	public void insert(Car car) {
		try {
			statement.execute("INSERT INTO CARS VALUES (" + car.getID() + ", '"
					+ car.getModel() + "', '" + car.getColor() + "')");
		} catch (SQLException ex) {
			throw new RuntimeException("Error connecting to the database", ex);
		}
	}

	public Set<Car> selectByModel(String model) {
		try {
			ResultSet rs = statement
					.executeQuery("SELECT * FROM CARS WHERE MODEL='" + model
							+ "'");
			Set<Car> cars = new HashSet<Car>();
			while (rs.next()) {
				cars.add(extractData(rs));
			}
			return cars;
		} catch (SQLException ex) {
			throw new RuntimeException("Error connecting to the database", ex);
		}
	}

	public Set<Car> selectByColor(String color) {
		try {
			ResultSet rs = statement
					.executeQuery("SELECT * FROM CARS WHERE COLOR='" + color
							+ "'");
			Set<Car> cars = new HashSet<Car>();
			while (rs.next()) {
				cars.add(extractData(rs));
			}
			return cars;
		} catch (SQLException ex) {
			throw new RuntimeException("Error connecting to the database", ex);
		}
	}

	public Set<Car> selectByID(int id) {
		try {
			ResultSet rs = statement
					.executeQuery("SELECT * FROM CARS WHERE ID='" + id + "'");
			Set<Car> cars = new HashSet<Car>();
			while (rs.next()) {
				cars.add(extractData(rs));
			}
			return cars;
		} catch (SQLException ex) {
			throw new RuntimeException("Error connecting to the database", ex);
		}
	}

	public Car delete(Car car) {
		try {
			Set<Car> deleted_car = selectByID(car.getID());
			boolean rs = statement.execute("DELETE FROM CARS WHERE ID='"
					+ car.getID() + "' AND MODEL='" + car.getModel()
					+ "' AND COLOR='" + car.getColor() + "'");
			if (!rs) {
				return deleted_car.toArray(new Car[1])[0];
			} else {
				throw new RuntimeException("Deleted error:" + rs);
			}
		} catch (SQLException ex) {
			throw new RuntimeException("Error connecting to the database", ex);
		}
	}
}
