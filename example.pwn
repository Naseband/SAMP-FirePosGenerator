// Will generate coords when loading the script (as FS).

#include <a_samp>
#include <sscanf2>

#define MAX_FIRE_POS        5000
#include <randomfires>

public OnFilterScriptInit()
{
    if(GenerateFirePositions())
	{
		SaveFirePositionsToFile("randomfires.txt");
		
		new Float:x, Float:y, Float:z;

		for(new i = 0; i < MAX_FIRE_POS; i ++)
		{
            GetFirePosition(i, x, y, z);

            // Test:

			CreateDynamicObject(18690, x, y, z - 1.5, 0.0, 0.0, 0.0);
			CreateDynamicMapIcon(x, y, z, 0, 0xFFFFFFFF);
		}
	}
}
