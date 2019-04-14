package example.MidletSuite;


import javax.microedition.lcdui.*;
import javax.microedition.lcdui.Form;
import javax.microedition.lcdui.Command;
import javax.microedition.lcdui.CommandListener;

class MoviesUpdate extends Form implements CommandListener{

  Command CMDOk;

  
  /** Constructor */
  public MoviesUpdate() throws Exception{
    super("**Movies Update**");
    try{
   
    Ticker tick = new Ticker("BLACK is running successfully at the box office all over India");
    this.setTicker(tick);
    
    CMDOk = new Command("ok",Command.SCREEN,1);
    
    this.addCommand(CMDOk);
    this.setCommandListener(this);

   
   
    Display.getDisplay(MoviesUpdateMidlet.instance).setCurrent(this);
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
        MoviesUpdateMidlet.quitApp();
     }
	}catch(Exception e){
		
	}
  }	
}