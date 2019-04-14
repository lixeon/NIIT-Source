package example.MidletSuite;




import javax.microedition.lcdui.*;
import javax.microedition.lcdui.Form;

import javax.microedition.lcdui.Command;
import javax.microedition.lcdui.CommandListener;


class NewsUpdate extends Form implements CommandListener{

  Command CMDOk;

  
  /** Constructor */
  public NewsUpdate() throws Exception{
    super("**Latest News**");
    
    Ticker tick = new Ticker("Elections in IRAQ held under dark shedow");
    this.setTicker(tick);
    
    CMDOk = new Command("ok",Command.SCREEN,1);
    this.addCommand(CMDOk);
    this.setCommandListener(this);
    Display.getDisplay(NewsUpdateMidlet.instance).setCurrent(this);

		
  }

  
  /**Handle command events*/
  public void commandAction(Command c, Displayable disp) {
    int cmdType = c.getCommandType();

    try{

     /** @todo Add command handling code */
     if (c.equals(CMDOk)) {
        NewsUpdateMidlet.quitApp();
     }
	}catch(Exception e){
		
	}
  }	
}