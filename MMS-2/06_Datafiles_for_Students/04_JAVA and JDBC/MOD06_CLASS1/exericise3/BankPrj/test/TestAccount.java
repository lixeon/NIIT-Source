package com.mybank.test;
import com.mybank.domain.Account;
/*
 * TestAccount.java
 *
 * Created on November 8, 2005, 1:33 AM
 *
 * To change this template, choose Tools | Template Manager
 * and open the template in the editor.
 */

/**
 *
 * @author student
 */
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
