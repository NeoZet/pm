
public class Person {
    public static final String TABLE_NAME = "Person";
    public static final String ID_COLUMN = "id";
    public static final String FIRST_NAME_COLUMN = "first_name";
    public static final String LAST_NAME_COLUMN = "last_name";
    public static final String AGE = "age";
	
	private int id;
	private String firstName;
	private String lastName;
	private int age;
	
	Person() {
	}
	
	Person(int _id, String _firstName, String _lastName, int _age) {
		this.id = _id;
		this.firstName = _firstName;
		this.lastName = _lastName;
		this.age = _age;
	}
	
	public void setFirstName(String _firstName) {
		firstName = _firstName;
	}
	public void setLastName(String _lastName) {
		lastName = _lastName;
	}
	public void setAge(int _age) {
		age = _age;
	}
	public void setID(int _id) {
		id = _id;
	}

	public String getFirstName() {
		return firstName;
	}
	public String getLastName() {
		return lastName;
	}
	public int getAge() {
		return age;
	}
	public int getID() {
		return id;
	} 
}









