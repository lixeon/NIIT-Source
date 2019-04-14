public class TestIsSubString {
    
    private static boolean isSubString(String item, String line) 
	{
        int        i, j;
        boolean    found = false;
        
        for ( i = 0; (i < line.length()) && !found; i++ ) 
	{
            j = 0;
            found = false;
            while ( ! found ) 
		{
                if ( j >= item.length() ) 
		{
                    break;
                } else if ( (i + j) >= line.length() ) 
		{
                    break;
                } else if ( item.charAt(j) != line.charAt(i + j) ) 
		{
                    break;
                } else 
		{
                    j++;
                    if ( j == item.length() ) 
		    {
                        found = true;
                    }
                }
            }
        }
        
        return found;
    }
    
    