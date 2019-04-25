/**
 * Class that stores the information of an item at the store, including the name of the item, 
 * cost of manufacturing, price of selling, the amount left that justifies reordering the item and
 * how much to order, and the maximum temperature it must be stored at if applicable.
 *
 * @author David McClarty
 */
public class Item {
	private String itemName; // Name of the item.
	private double itemManufacturerCost; // Cost of manufacturing.
	private double itemSalesCost; // The price of the item at the store.
	private int itemReorderAmount; // The amount that must be ordered when the reorder point is reached.
	private int itemReorderPoint; // The maximum quantity of the item before reordering more.
	private Double itemMaxTemp; // Minimum temperature the item can be safely stored.
								// Stored as a 'Double' so it can be compared to 'null'.
	
	/**
	 * Constructor for the Item object. Does nothing except initalize all 
	 * related aspects of the item.
	 * 
	 * @param name The name of the item.
	 * @param manufacturerCost The cost to manufacture the item.
	 * @param salesCost The price the item is sold at.
	 * @param reorderAmount The amount to be reordered when the reorder point is reached.
	 * @param reorderPoint The maximum quantity of the item that justifies reordering
	 * more of the item's stock.
	 * @param maxTemp The maximum temperature the item can be safely stored at. If the item can
	 * be stored at any temperature, this should be set to <strong>null</strong>.
	 * 
	 */
	public Item(String name, double manufacturerCost, double salesCost, 
	int reorderAmount, int reorderPoint, Double maxTemp) {
		itemName = name;
		itemManufacturerCost = manufacturerCost;
		itemSalesCost = salesCost;
		itemReorderAmount = reorderAmount;
		itemReorderPoint = reorderPoint;
		if (maxTemp != null) { // Maximum temperature is only set if the item has one.
			itemMaxTemp = maxTemp;
		}
	}
	
	/**
	 * Returns the name of the item.
	 * 
	 * @return Name of the item.
	 */
	public String getName() {
		return itemName;
	}
	
	/**
	 * Returns the item's manufacturing cost.
	 * 
	 * @return Item's manufacturing cost.
	 */
	public double getManufacturerCost() {
		return itemManufacturerCost;
	}
	
	/**
	 * Returns the price of the item.
	 * 
	 * @return Price of the item.
	 */
	public double getSalesCost() {
		return itemSalesCost;
	}
	
	/**
	 * Returns the amount to reorder when the reorder point has been reached.
	 * 
	 * @return Quantity of stock to be reordered.
	 */
	public int getReorderAmount() {
		return itemReorderAmount;
	}
	
	/**
	 * Returns the maximum quantity of an item to justify reordering the item's stock.
	 * 
	 * @return Maximum quantity of an item to justify reordering the item's stock.
	 */
	public int getReorderPoint() {
		return itemReorderPoint;
	}
	
	/**
	 * Returns the maximum temperature the item can be stored at, or <strong>null</strong> if 
	 * if the item can be stored at any temperature.
	 * 
	 * @return Maximum temperature the item can be safely stored at.
	 */
	public Double getMaxTemp() {
		return itemMaxTemp;
	}
	
}
