/**
 * Class that stores the store's capital and its name.
 * 
 * @author John Tang
 */
public class Store {
    private static double capital; // Store's total assets
    private static String name; // Store name

    /**
     * Constructor for the Store object. Sets the capital to $100,000 and sets the store name.
     * 
     * @param name The store's name.
     */
    public Store(String name) {
    	capital = 100000;
    	Store.name = name;
    }

    /**
	 * Returns the store's capital.
	 * 
	 * @return Store capital.
	 */
    public double getCapital() {
    	return capital;
    }
    
    /**
	 * Returns the store's name.
	 * 
	 * @return Store name.
	 */
    public String getName() {
    	return name;
    }

    /**
	 * Adds to the store's capital.
	 * 
	 * @param amount The amount to be added/subtracted to the capital (to deduct from capital).
	 */
    public void setCapital(double amount) {
    	capital += amount;
    }
}
