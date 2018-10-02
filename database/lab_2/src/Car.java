
public class Car {
	private int id;
	private String model;
	private String color;
	
	Car() {		
	}
	
	Car(int _id, String _model, String _color) {
		this.id = _id;
		this.model = _model;
		this.color = _color;	
	}
	
	public void setID(int _id) {
		this.id = _id;
	}
	
	public void setModel(String _model) {
		this.model = _model;
	}
	
	public void setColor(String _color) {
		this.color = _color;
	}
	
	public int getID() {
		return this.id;
	}
	
	public String getModel() {
		return this.model;
	}
	
	public String getColor() {
		return this.color;
	}
}
