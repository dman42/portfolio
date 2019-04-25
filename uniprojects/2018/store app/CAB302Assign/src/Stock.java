import java.util.HashMap;

/**
 * Class that stores the inventory of their items and their respective quantities.
 * 
 * @author John Tang
 */
public class Stock {
	private static HashMap<Item, Integer> inventory = new HashMap<Item, Integer>();

	/**
	 * Constructor for the Stock object. Does nothing except put all relevant items
	 * into the inventory and set the items quantity to 0 by default.
	 *
	 * @param itemArray The array of items to be added to the inventory.
	 */
	public Stock(Item[] itemArray) {
		for (int i = 0; i < itemArray.length; i++) {
			inventory.put(itemArray[i], 0);
		}
	}

	/**
	 * Returns the inventory.
	 *
	 * @return The inventory.
	 */
	public HashMap<Item, Integer> getInventory() {
		return inventory;
	}
	
	/**
	 * Adds a new type of item to the inventory.
	 *
	 * @param item Item to be added to the inventory.
	 * @param amount Amount of the item added to the inventory.
	 */
	public void addItem(Item item, int amount) {
		inventory.put(item, amount);
	}
	
	/**
	 * Increases the quantity of a particular item. 
	 *
	 * @param item   The item to increase in quantity.
	 * @param amount The amount for the item to increase by.
	 */
	public void increaseQuantity(Item item, int amount) {
		inventory.put(item, inventory.get(item) + Math.abs(amount));
	}

	/**
	 * Decreases the quantity of a particular item. Calls a StockException if there is 
	 * not enough of the item in the inventory to deduct by given amount.
	 *
	 * @param item   The item to decrease in quantity.
	 * @param amount The amount for the item to decrease by.
	 */
	public void decreaseQuantity(Item item, int amount) throws StockException {
		if (inventory.get(item) < amount) {
			throw new StockException(); 
		} 
				
		inventory.put(item, inventory.get(item) - amount);
		
	}

	/**
	 * Produces a sale log, by finding how much of an item is reduced by, then applying those changes 
	 * to the store's personal inventory and increasing the capital.
	 *
	 * @param sales Refers to the sales. We will refer to the item quantites here to alter the capital and inventory
	 * @param store Used to refer to the store object
	 */
	public void salesLog(Store store, HashMap<Item, Integer> sales) throws StockException {
		// For each item sold, the quantity is decreased, and the store capital is increased
		for (HashMap.Entry<Item, Integer> sale : sales.entrySet()) {
			decreaseQuantity(sale.getKey(), sale.getValue());
			store.setCapital((sale.getKey().getSalesCost()) * sale.getValue());
		}
	}
}