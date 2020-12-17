// The main MemoryManagement program, created by Alexander Reeder, 2000 Nov 19

import java.io.*;

public class MemoryManagement 
{
  public static void main(String[] args) 
  {
    ControlPanel controlPanel;
    Kernel kernel;
    String commands = "src/main/resources/commands";
    String config = "src/main/resources/memory.conf";

    File f = new File(commands);
    if ( ! ( f.exists() ) ) 
    {
      System.out.println( "MemoryM: error, file '" + f.getName() + "' does not exist." );
      System.exit( -1 );
    }
    if ( ! ( f.canRead() ) ) 
    {
      System.out.println( "MemoryM: error, read of " + f.getName() + " failed." );
      System.exit( -1 );
    }

    f = new File(config);
    if ( ! ( f.exists() ) )
    {
      System.out.println( "MemoryM: error, file '" + f.getName() + "' does not exist." );
      System.exit( -1 );
    }
    if ( ! ( f.canRead() ) )
    {
      System.out.println( "MemoryM: error, read of " + f.getName() + " failed." );
      System.exit( -1 );
    }

    kernel = new Kernel();
    controlPanel = new ControlPanel( "Memory Management" );
    controlPanel.init( kernel , commands , config );
  }
}
