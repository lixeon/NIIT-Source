package example.MidletSuite;

import java.util.Vector;
import java.util.Enumeration;

import javax.microedition.lcdui.*;
import javax.microedition.lcdui.Form;


import javax.microedition.lcdui.Command;
import javax.microedition.lcdui.CommandListener;



class Jokes extends Form implements CommandListener {

  Command CMDOk;

  
  /** Constructor */
  public Jokes() throws Exception{
    super("**Jokes**");
    try{
        
    Ticker tick = new Ticker("Santa is buying a TV. Santa: Do you have color TVs? Salesman:Sure. Santa:Give me a green one, please.");
    
    this.setTicker(tick);
    
    System.out.println ("11");
    
    CMDOk = new Command("ok",Command.SCREEN,1);
    this.addCommand(CMDOk);
    this.setCommandListener(this);
    System.out.println ("22");
   
    
    Display.getDisplay(JokesMidlet.instance).setCurrent(this);
	}catch(Exception e){
		System.out.println ("exception is "+e.getMessage());
	}	
  }

  
  /**Handle command events*/
  public void commandAction(Command c, Displayable disp) {
    int cmdType = c.getCommandType();

    try{

     /** @todo Add command handling code */
     if (c.equals(CMDOk)) {
        JokesMidlet.quitApp();
     }
	}catch(Exception e){
		
	}
  }	
}