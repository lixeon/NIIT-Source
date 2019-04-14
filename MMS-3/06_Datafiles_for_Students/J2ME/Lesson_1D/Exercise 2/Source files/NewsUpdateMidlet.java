package example.MidletSuite;

import javax.microedition.midlet.*;
import javax.microedition.lcdui.*;
import javax.microedition.lcdui.Display;
import javax.microedition.lcdui.Displayable;



public class NewsUpdateMidlet extends MIDlet {
  public static NewsUpdateMidlet instance;
  NewsUpdate displayable;
  public static String SERVER_URL = null;

  /** Constructor */
  public NewsUpdateMidlet() {
    instance = this; //Read From JAD File if there are any parameters
    SERVER_URL = getAppProperty("URL");
  }

  /** Main method */
  public void startApp() {
    try{
      displayable = new NewsUpdate();
//      Display.getDisplay(this).setCurrent(displayable);
    }catch(Exception e){
      e.printStackTrace();
    }
  }

  /** Handle pausing the MIDlet */
  public void pauseApp() {

  }

  /** Handle destroying the MIDlet */
  public void destroyApp(boolean unconditional) {
  }

  /** Quit the MIDlet */
  public static void quitApp() {
    instance.destroyApp(true);
    instance.notifyDestroyed();
    instance = null;
  }

}
