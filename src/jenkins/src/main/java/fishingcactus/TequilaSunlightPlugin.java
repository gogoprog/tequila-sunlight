package fishingcactus;


import hudson.Extension;
import hudson.Plugin;
import org.kohsuke.stapler.DataBoundConstructor;
import org.kohsuke.stapler.StaplerRequest;
import net.sf.json.JSONObject;
import java.io.IOException;


@Extension
public class TequilaSunlightPlugin extends Plugin
{
    public static TequilaSunlightPlugin
        instance = null;
    public String
        port;

    @Override
    public void start() throws Exception
    {
        load();
        super.start();
    }

    @DataBoundConstructor
    public TequilaSunlightPlugin() throws IOException
    {
        System.out.println( "TequilaSunlight plugin constructed" );
        
        if( instance != null )
        {
            System.out.println( "Warning: Multiple instance are present! " );
        }

        instance = this;
    }

    @Override
    public void configure( StaplerRequest req, JSONObject formData ) throws IOException
    {        
        port = formData.optString( "port" );  
        save();
    }
}
