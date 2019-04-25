import static org.junit.Assert.assertEquals;

import java.util.HashMap;
import java.util.Random;
import org.junit.Before;
import org.junit.FixMethodOrder;
import org.junit.Test;
import org.junit.runners.MethodSorters;

/**
 * JUnit tests for the testable classes authored by John Tang, which are:
 * Store.java, and Stock.java.
 *
 * @author David McClarty
 */

@FixMethodOrder(MethodSorters.NAME_ASCENDING)
public class DavidsJUnitTests {

	private static Random random = new Random(); // Random number generator.

	// Tests for Store.java

	Store store;
	Stock stock;

	@Before
	public void test00setUpAll() {
		store = null;
		Item[] itemArray = new Item[] {};
		stock = new Stock(itemArray);
		
	}

	// Tests Store.java.
	@Test
	public void test01StoreName() {
		store = new Store("test");
		assertEquals(store.getName(), "test");
	}

	@Test
	public void test02InitialCapital() {
		store = new Store("test");
		double expectedCapital = 100000.0;
		assertEquals(store.getCapital(), expectedCapital, 0.0);
	}

	@Test
	public void test03AlteredCapital() {
		store = new Store("test");
		double randomNumber = random.nextDouble() * 10000;
		store.setCapital(randomNumber);
		double expectedCapital = 100000.0 + randomNumber;
		assertEquals(store.getCapital(), expectedCapital, 0.0);
	}

	// Tests Stock.java.
	
	Item test1 = new Item("test1", 2, 4, 8, 6, null);

	@Test
	public void test04InitialStock() {
		
		Item test2 = new Item("test2", 2, 4, 8, 6, null);
		Item test3 = new Item("test3", 2, 4, 8, 6, null);
		Item[] itemArray = new Item[]{test1, test2, test3};
		stock = new Stock(itemArray);

		assertEquals(stock.getInventory().get(test1), 0, 0.0);
		assertEquals(stock.getInventory().get(test2), 0, 0.0);
		assertEquals(stock.getInventory().get(test3), 0, 0.0);
		assertEquals(stock.getInventory().size(), 3);
	}

	@Test
	public void test05AddedItem() {
		
		int amount = random.nextInt(10);
		Item test4 = new Item("test4", 2, 4, 8, 6, null);
		stock.addItem(test4, amount);

		for(HashMap.Entry<Item, Integer> item : stock.getInventory().entrySet()) {
			if (item.getKey().getName().equals(test4.getName())) {
				assertEquals(item.getValue(), amount, 0.0);
			}
			else {
				assertEquals(item.getValue(), 0, 0.0);
			}
		}
		assertEquals(stock.getInventory().size(), 4);
	}

	@Test
	public void test06IncreasedItem() {
		int amount = random.nextInt(10);
		Item test5 = new Item("test5", 2, 4, 8, 6, null);
		stock.addItem(test5, 0);
		stock.increaseQuantity(test5, amount);

		assertEquals(stock.getInventory().get(test5), amount, 0.0);
	}

	@Test
	public void test07DecreasedItem() throws StockException {

		int amount = random.nextInt(10) + 1;
		int amount2 = random.nextInt(amount);
		
		Item test6 = new Item("test6", 2, 4, 8, 6, null);
		stock.addItem(test6, amount);
		stock.decreaseQuantity(test6, amount2);

		assertEquals(stock.getInventory().get(test6), amount - amount2, 0.0);
	}

	// Tests both stock and store classes (Using salesLog).

	@Test
	public void test08SalesLog() throws StockException {

		store = new Store("test");

		HashMap<Item, Integer> expectedHashMap = new HashMap<Item, Integer>();
		HashMap<Item, Integer> sales = new HashMap<Item, Integer>();
		double expectedCapital = 100000.0;

		for (HashMap.Entry<Item, Integer> item : stock.getInventory().entrySet()) {
			int amount = random.nextInt(10) + 1;
			stock.increaseQuantity(item.getKey(), amount);
			int amount2 = random.nextInt(amount) + 1;
			sales.put(item.getKey(), amount2);
			expectedCapital += (item.getKey().getSalesCost() * amount2);

			expectedHashMap.put(item.getKey(), item.getValue() - amount2);
		}
		stock.salesLog(store, sales);

		for(HashMap.Entry<Item, Integer> actualItem : stock.getInventory().entrySet()) {
			for (HashMap.Entry<Item, Integer> expectedItem : expectedHashMap.entrySet()) {
				if (expectedItem.getKey().getName().equals(actualItem.getKey().getName())) {
					assertEquals(expectedItem.getValue(), actualItem.getValue());
				}
			}
		}
		assertEquals(store.getCapital(), expectedCapital, 0.0);
	}


}
