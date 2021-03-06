import java.util.HashMap;

/**
 * Abstract class that hold the capacity for each kind of truck, 
 * and the neccesary functions for each kind of truck.
 * 
 * @author David McClarty
 */
public abstract class Truck {
    static int coldTruckCap = 800; // Capacity for refrigerated truck
    static int ordTruckCap = 1000; // Capacity for ordinary truck
    
    protected double cost; 
    protected HashMap<Item, Integer> cargo = new HashMap<>(); 
    protected Double temperature; 

    abstract void setCost(); // Setter for the cost of each truck
    abstract void addCargo(Item item, int quantity) throws DeliveryException; // Adding items and a quantity to a truck
    
    /**
     * Returns the temperature of the truck.
     * 
     * @return The temperature of the truck.
     */
    public Double getTemperature() {
        return temperature;
    }
    
    /**
     * Returns the truck's cost.
     * 
     * @return The truck's cost.
     */
    public double getCost() {
        return cost;
    }
    
    /**
     * Returns the truck's cargo.
     * 
     * @return The truck's cargo
     */
    public HashMap<Item, Integer> getCargo() {
        return cargo;
    }
    
    /**
     * Returns the amount of items in the truck's cargo.
     * 
     * @return The amount of items in the truck's cargo.
     */
    public int getCargoCount() {
    	int totalQuantity = 0;
    	for (HashMap.Entry<Item, Integer> item : cargo.entrySet()) {
    		totalQuantity += item.getValue();
    	}
        return totalQuantity;
    }
    
    
}
