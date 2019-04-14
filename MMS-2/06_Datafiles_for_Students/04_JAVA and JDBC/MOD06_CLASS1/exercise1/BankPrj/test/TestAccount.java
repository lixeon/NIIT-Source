package com.mybank.test;
import com.mybank.domain.Account;

public class TestAccount {
    
    /** Creates a new instance of TestAccount */
    public TestAccount() {
    }
    
    public static void main(String[] args) {
        Account acct = new Account(100);
        acct.deposit(50);
        acct.withdraw(147);
        System.out.println("Final Account Balance is: " + acct.getBalance());
    }
    
}
