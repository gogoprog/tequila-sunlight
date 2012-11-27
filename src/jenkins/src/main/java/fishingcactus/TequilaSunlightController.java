package fishingcactus;

import java.io.*;
import java.util.*;
import gnu.io.*;



public class TequilaSunlightController 
{
    // -- PUBLIC METHODS
    
    public void initialize()
    {
        log( "TequilaSunlightController Initializing..." );
        portEnumeration = CommPortIdentifier.getPortIdentifiers();
    }
    
    // ~~
    
    public void listPorts()
    {
        log( "Comm ports list : ");
        while ( portEnumeration.hasMoreElements() ) 
        {
            CommPortIdentifier portIdentifier = portEnumeration.nextElement();
            log( portIdentifier.getName() );
        }
    }   
    
    // ~~
    
    public boolean selectPort( String port_name ) throws Exception
    {
        while ( portEnumeration.hasMoreElements() ) 
        {
            CommPortIdentifier portIdentifier = portEnumeration.nextElement();
            if( portIdentifier.getName().equals( port_name ) )
            {
                log( "Trying to open..." );
                CommPort commPort = portIdentifier.open( "TequilaPort" , 100 );
                    
                if ( commPort instanceof SerialPort )
                {
                    serialPort = (SerialPort) commPort;
                    serialPort.setSerialPortParams( 9600 ,SerialPort.DATABITS_8, SerialPort.STOPBITS_1, SerialPort.PARITY_NONE );
                    inStream = serialPort.getInputStream();                        
                    outStream = serialPort.getOutputStream();
                    
                    //(new Thread(new SerialReader(inStream))).start();

                    portIsOpen = true;
                    log( "TequilaSunlightController port successfuly opened!" );
                    return true;
                }
            }
        }

        portIsOpen = false;
        return false;
    }
    
    // ~~
    
    public void log( String message )
    {
        System.out.println( message );
    }   
    
    // ~~
    
    public void sleep( int ms ) throws Exception
    {
        Thread.sleep( ms ); 
    }  
    
    // ~~
    
    public void startBlink( int lamp_index, int blink_period ) throws Exception
    {    
        newCommand();
        selectLamp( lamp_index );
        sendData( 98 );
        sendData( 48 + blink_period );
        log( "Blink started." );
    }
    
    // ~~
    
    public void stopBlink( int lamp_index ) throws Exception
    {  
        newCommand();
        selectLamp( lamp_index );
        sendData( 110 ); 
        log( "Blink stopped." );
    }

    // ~~

    public void setLamp( int lamp_index, boolean state ) throws Exception
    {
        newCommand();
        selectLamp( lamp_index );

        if( state )
        {
            sendData( 111 );
            log( "Lamp on." );
        }
        else
        {
            sendData( 112 );
            log( "Lamp off." );
        }
    }
    
    // ~~
    
    public void startSequence( int lamp_index, int sequence ) throws Exception
    {    
        newCommand();
        selectLamp( lamp_index );
        sendData( 48 + sequence );
        log( "Sequence started." );
    }  
    
    // ~~
    
    public void ping() throws Exception
    {
       sendData( 166 );
    }

    // ~~

    public boolean isPortOpen()
    {
        return portIsOpen;
    }
    
    // -- PRIVATE METHODS
    
    private void sendData( int data ) throws Exception
    {
        outStream.write( data );
        Thread.sleep( 1000 );    
    }
    
    // ~~
    
    private void newCommand() throws Exception
    {
        sendData( 13 );
    }
    
    // ~~
    
    public void selectLamp( int lamp ) throws Exception
    {
        sendData( '1' + lamp );
    }
    
    // -- ATTRIBUTES
    
    Enumeration<CommPortIdentifier> 
        portEnumeration;
    SerialPort 
        serialPort;
    boolean
        portIsOpen = false;
    InputStream
        inStream;
    OutputStream
        outStream;
        
    // -- CLASS
    
    public static class SerialReader implements Runnable 
    {
        InputStream in;
        
        public SerialReader ( InputStream in )
        {
            this.in = in;
        }
        
        public void run ()
        {
            byte[] buffer = new byte[16];
            int len = -1;
            try
            {
                while ( ( len = this.in.read(buffer)) > -1 )
                {
                    if( len > 0 )
                    {
                        System.out.println( "Received : <" + new String( buffer,0,len ) + ">" );
                    }
                }
            }
            catch ( IOException e )
            {
                //e.printStackTrace();
            }            
        }
    }
        
}

