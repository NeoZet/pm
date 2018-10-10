import java.sql.SQLException;
import java.util.ArrayList;
import java.util.Collection;
import java.util.List;

import com.j256.ormlite.jdbc.JdbcPooledConnectionSource;
import com.j256.ormlite.logger.LocalLog;


public class Main {

	public static void main(String[] args) throws SQLException {
		System.setProperty(LocalLog.LOCAL_LOG_LEVEL_PROPERTY, "ERROR");
		ConnectionFactory connection = new ConnectionFactory();
		List<Person> PersList = new ArrayList<Person>();
		PersList.add(new Person(1111, "Erofei", "Pavlov", 23));
		PersList.add(new Person(1112, "Erofei", "Ivanov", 19));
		PersList.add(new Person(1121, "Ivan", "Emin", 20));
		PersList.add(new Person(1211, "Jhon", "Smirnov", 21));
		PersList.add(new Person(2111, "Semyon", "Pavlov", 20));
		PersList.add(new Person(1311, "Andrey", "Novov", 19));
		PersList.add(new Person(4111, "Anton", "Emin", 18));
		PersList.add(new Person(1113, "Genry", "Smirnov", 21));
		PersList.add(new Person(1141, "Genry", "Pavlov", 20));
		PersList.add(new Person(1411, "Anton", "No", 19));
		PersList.add(new Person(4111, "Anton", "Emin", 18));
		
		DaoPerson tabel_pers = new DaoPerson(ConnectionFactory.getConnection());
		tabel_pers.createTable(PersList);
		List<Person> all_pers = tabel_pers.selectAll();
		for (int i = 0; i < all_pers.size(); i++) {
			   System.out.println(all_pers.get(i).getFirstName());
		}
		
		List<Car> CarsList = new ArrayList<Car>();
		CarsList.add(new Car(1111, "Ferrari", "white"));
		CarsList.add(new Car(1211, "VAZ-2107", "black"));
		CarsList.add(new Car(1141, "Reno Logan", "grey"));
		CarsList.add(new Car(1131, "Mercedes-Benz", "black"));
		CarsList.add(new Car(2111, "BMW-X6", "red"));
		CarsList.add(new Car(4111, "Honda-Civic", "white"));
	
		
		DaoCar tabel_cars = new DaoCar(ConnectionFactory.getConnection());
		tabel_cars.createTable(CarsList);

		List<Car> all_cars = tabel_cars.selectAll();
		for (int i = 0; i < all_cars.size(); i++) {
			   System.out.println(all_cars.get(i).getModel());
		}
		System.out.println("--------------------------------------");
		List<Car> cars_id = tabel_cars.selectByID(1131);
		for (int i = 0; i < cars_id.size(); i++) {
			   System.out.println(cars_id.get(i).getModel());
		}
		
	}

}
