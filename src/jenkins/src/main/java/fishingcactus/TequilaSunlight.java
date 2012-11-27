package fishingcactus;

import hudson.Extension;
import hudson.model.*;
import java.util.ArrayList;
import java.util.List;

@Extension
public class TequilaSunlight extends PeriodicWork
{
    // -- PUBLIC

    public static TequilaSunlightController 
        tsc;
    
    // ~~

    public TequilaSunlight()
    {
        tsc = new TequilaSunlightController();
        System.out.println( "TequilaSunlight periodic work initialized." );
    }

    // ~~

    @Override
    public long getRecurrencePeriod()
    {
        return MIN / 2;
    }

    // ~~

    @Override
    public void doRun() throws Exception
    {
        System.out.println( "TequilaSunlightPeriodicTask" );

        if( !tsc.isPortOpen() )
        {
            if( !TequilaSunlightPlugin.instance.port.equals( "" ) )
            {
                TequilaSunlight.tsc.initialize();
                TequilaSunlight.tsc.selectPort( TequilaSunlightPlugin.instance.port );
            } 
            else 
            {
                 System.out.println( "COM port is not set!" );
            }

        }

        if( tsc.isPortOpen() )
        {
            System.out.println( "TequilaSunlightController : analyzing..." );

            resetCounts();
            List<AbstractProject> projects = getProjects();

            for( int i = 0; i < projects.size(); i++ )
            {
                //System.out.println( projects.get(i).getName() + " = " + getResult( projects.get(i) ) );
                analyzeResult( projects.get(i) );
            }

            if( failureCount > 0 )
            {
                tsc.setLamp( 0 , false );
                tsc.startSequence( 2, 1 + failureCount );
            }
            else
            {
                tsc.setLamp( 0 , true );
            }

            if( buildingCount > 0 )
            {
                tsc.startBlink( 1, 11 - buildingCount  );
            }
            else
            {
                tsc.stopBlink( 1 );
            }
        }
    }

    // -- PRIVATE

    private List<AbstractProject> getProjects()
    {
        List<AbstractProject> result = new ArrayList<AbstractProject>();
        List<TopLevelItem> topLevelItems = Hudson.getInstance().getItems();
        for (TopLevelItem topLevelItem : topLevelItems)
        {
            if (topLevelItem instanceof AbstractProject)
            {
                AbstractProject abstractProject = (AbstractProject) topLevelItem;
                //if (abstractProject.getPublishersList().get(MonitorPublisher.DESCRIPTOR) != null)
                {
                    result.add(abstractProject);
                }
            }
        }

        return result;
    }

    // ~~

    private String getResult(AbstractProject project)
    {
        String result;
        
        if ((project.getLastCompletedBuild() != null) && (project.getLastCompletedBuild().getResult() != null))
        {
            if (project.isDisabled())
            {
                result = "DISABLED";
            }
            else
            {
                result = project.getLastCompletedBuild().getResult().toString();
            }
        }
        else
        {
            result = "NOT_BUILD";
        }

        return result;
    }

    // ~~

    private void resetCounts()
    {
        successCount = 0;
        failureCount = 0;
        buildingCount = 0;
    }

    // ~~

    private void analyzeResult( AbstractProject project )
    {
        if( getResult( project ).equals( "SUCCESS" ) )
        {
            successCount++;
        }
        else if(  getResult( project ).equals( "FAILURE" ) )
        {
            failureCount++;
        }

        if( project.isBuilding() )
        {
            buildingCount++;
        }
    }

    // -- ATTRIBUTES

    int
        successCount,
        failureCount,
        buildingCount;
}

