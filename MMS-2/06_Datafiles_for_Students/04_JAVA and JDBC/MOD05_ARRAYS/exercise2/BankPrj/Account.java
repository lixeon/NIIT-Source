package com.mybank.domain;

public class Account {
    
    private double balance;
    
    public Account(double initBalance) {
        balance = initBalance;
    }
    
    public double getBalance() {
        return balance;
    }
    
    public boolean deposit(double amt) {
        balance = balance + amt;
        return true;
    }
    
    public boolean withdraw(double amt) {
        boolean result = false;  // assume operation failure
        if ( amt <= balance ) {
            balance = balance - amt;
            result = true;  // operation succeeds
        }
        return result;
    }
}
