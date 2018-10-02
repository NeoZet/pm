import java.sql.*;

public class ConnectionFactory {
	public static final String JDBC_DRIVER = "org.h2.Driver";
	public static final String DB_URL = "jdbc:h2:file:/home/rem/direct/pm/database/lab_2/persons.db";
//TODO singleton
	static{
		
	}
	public static Connection getConnection() {
		try {
			Class.forName(JDBC_DRIVER).newInstance();
		} catch (InstantiationException e) {
			e.printStackTrace();
		} catch (IllegalAccessException e) {
			e.printStackTrace();
		} catch (ClassNotFoundException e) {
			e.printStackTrace();
		}
		try {
			return DriverManager.getConnection(DB_URL);
		} catch (SQLException ex) {
			throw new RuntimeException("Error connecting to the database", ex);
		}
	}
}
