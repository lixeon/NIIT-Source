package com.mybank.test;

import com.mybank.domain.*;

/*
 * This class creates the program to test the banking classes.
 * It creates a new Customer and Account (with an initial balance),
 * and performs a series of transactions with the Account object.
 */
public class TestBanking {

  public static void main(String[] args) {
    Customer customer;
    Account  account;

    // Create an account that can has a 500.00 balance and type savings.
    System.out.println("Creating the customer Jane Smith.");
    customer = new Customer("Jane", "Smith");
    System.out.println("Creating her account with a 500.00 balance.");
    customer.setAccount(new Account(500.00));

    account = customer.getAccount();
    System.out.println("Withdraw 150.00");
    account.withdraw(150.00);

    System.out.println("Deposit 22.50");
    account.deposit(22.50);

    System.out.println("Withdraw 47.62");
    account.withdraw(47.62);

    // Print out the final account balance
  
	//Do the coding here
  }
}
