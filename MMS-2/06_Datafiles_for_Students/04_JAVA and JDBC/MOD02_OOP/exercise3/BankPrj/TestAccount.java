/*
 * TestAccount.java
*/
public class TestAccount {
    
    /** Creates a new instance of TestAccount */
    public TestAccount() {
    }
    
    public static void main(String[] args) {
        Account acct = new Account(100);
        acct.deposit(47);
        acct.withdraw(150);
        System.out.println("Final Account Balance is: " + acct.getBalance());
    }
    
}
