import java.util.Iterator;
import java.util.Set;

public class Main {
	static final String JDBC_DRIVER = "org.h2.Driver";
	static final String DB_URL_PERS = "jdbc:h2:file:/home/rem/direct/pm/database/lab_2/persons.db";
	static final String DB_URL_CARS = "jdbc:h2:file:/home/rem/direct/pm/database/lab_2/cars.db";

	public static void main(String[] args) {
		DaoPerson table_pers = new DaoPerson();
		// table_pers.createTable(JDBC_DRIVER, DB_URL_PERS);

		Set<Person> persons = table_pers.selectByID(1211);

		// Person p = new Person();
		// table_pers.insert(p);
		// Set<Person> persons = table_pers.selectByLastName("Smith");
		Iterator<Person> it = persons.iterator();

//		while (it.hasNext()) {
//			Person pers = it.next();
//			System.out.println(pers.getFirstName());
//		}

		DaoCar table_cars = new DaoCar();
		// table_cars.createTable(JDBC_DRIVER, DB_URL_CARS);		
//		Set<Car> cars_f = table_cars.selectByID(1211);//table_cars.selectAll();
//		
//		Iterator<Car> it_cars = cars_f.iterator();
////		System.out.println(it_cars.next());
//		Car del_car = table_cars.delete(it_cars.next());
//		System.out.println(del_car.getModel());
		table_cars.insert(new Car(1211, "VAZ-2107", "white"));
		
		Set<Car> cars = table_cars.selectAll();
		Iterator<Car> it_car = cars.iterator();
		while (it_car.hasNext()) {
			Car cars_tmp = it_car.next();
			System.out.println(cars_tmp.getModel());
		}

	}
}
