import java.util.List;
import java.sql.*;

import com.j256.ormlite.dao.BaseDaoImpl;
import com.j256.ormlite.support.ConnectionSource;
import com.j256.ormlite.table.TableUtils;

public class DaoCar extends BaseDaoImpl<Car, Integer> {
	DaoCar(ConnectionSource connectionSource) throws SQLException {
	        super(connectionSource, Car.class);
	}

	public void insertCar(Car car) {
		try {
			this.update(car);
		} catch (SQLException ex) {
			throw new RuntimeException("Error connecting to the database", ex);
		}
	}
	
	public void createTable(List<Car> CarsList) throws SQLException {
		TableUtils.createTableIfNotExists(connectionSource, Car.class);
		for(int i = 0; i < CarsList.size(); ++i) {
			this.createIfNotExists(CarsList.get(i));
		}
	}

	public List<Car> selectAll() throws SQLException {
		return this.queryForAll();
	}
	
	public List<Car> selectByModel(String model) {
		try {
			return super.queryForEq("model", model);
		} catch (SQLException ex) {
			throw new RuntimeException("Error connecting to the database", ex);
		}
	}

	public List<Car> selectByColor(String color) {
		try {
			return super.queryForEq("color", color);
		} catch (SQLException ex) {
			throw new RuntimeException("Error connecting to the database", ex);
		}
	}

	public List<Car> selectByID(int id) {
		try {
			return super.queryForEq("id", id);
		} catch (SQLException ex) {
			throw new RuntimeException("Error connecting to the database", ex);
		}
	}

	public Car deleteCar(Car car) {
		try {
			this.delete(car);
		} catch (SQLException ex) {
			throw new RuntimeException("Error connecting to the database", ex);
		}
		return car;
	}
}