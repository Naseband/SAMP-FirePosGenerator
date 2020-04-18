#if defined __RFIRES_INC__
#endinput
#endif
#define __RFIRES_INC__

#include <vehnodes>
#include <OC>

static enum VEC3D
{
	Float:X,
	Float:Y,
	Float:Z
};

static
	gFirePositions[MAX_FIRE_POS][VEC3D],
	gNumFirePositions = 0;


stock GenerateFirePositions(num = MAX_FIRE_POS)
{
	if(num > MAX_FIRE_POS) num = MAX_FIRE_POS;
	else if(num < 1) return 0;

	print("Generating Fire Positions");

	LoadVehNodes();

	new tries;

	for(new i = 0; i < num; i ++)
	{
		new Float:x, Float:y, Float:z, bool:fin = false,
			model, Float:cx, Float:cy, Float:cz,
			Float:a;

		do
		{
			if(!GetRandomVehNodePos(x, y, z)) return 0;

			for(new j = 0; j < 5 && !fin; j ++)
			{
				a = random(3600) / 10.0;
				cx = x + 50.0 * floatsin(a, degrees);
				cy = y + 50.0 * floatcos(a, degrees);

				if(0 < (model = CA_RayCastLine(x, y, z + 1.0 + (random(40) / 10.0), cx, cy, z + 1.0 + (random(40) / 10.0), cx, cy, cz)) < WATER_OBJECT && !IsAnyVehNodeInRangeOfPoint(cx, cy, cz, 20.0))
				{
					switch(GetModelCategoryID(model))
					{
						case OC_FACTORIES, OC_SHOPS, OC_GRAVEYARD, OC_HOUSES, OC_SKYSCRAPERS, OC_OTHER_BUILDINGS, OC_RESTAURANTS, OC_STADIUMS,
							OC_CLUBS, OC_GENERAL_INDUSTRIAL, OC_CONSTRUCTION, OC_CRANES:
						{
							gFirePositions[i][X] = cx;
							gFirePositions[i][Y] = cy;
							gFirePositions[i][Z] = cz;
							fin = true;
						}
					}
				}

				tries ++;
			}
		}
		while(!fin);

		if((i + 1) % 100 == 0 || i == num - 1)
		{
			printf("Found %d/"#MAX_FIRE_POS" after %d tries", i + 1, tries);
			tries = 0;
		}
	}

	print("Finished");

	gNumFirePositions = num;

	return 1;
}

stock GetRandomFirePosition(&Float:x, &Float:y, &Float:z)
{
	if(gNumFirePositions) return GetFirePosition(random(gNumFirePositions), x, y, z);

	return 0;
}

stock GetFirePosition(id, &Float:x, &Float:y, &Float:z)
{
	if(!gNumFirePositions || id < 0 || id >= gNumFirePositions) return 0;

	x = gFirePositions[id][X];
	y = gFirePositions[id][Y];
	z = gFirePositions[id][Z];

	return 1;
}

stock SaveFirePositionsToFile(const filename[] = "randomfires.txt")
{
	if(!gNumFirePositions) return 0;

	new File:FOut = fopen(filename, io_write);

	if(!FOut) return 0;

	new text[100];

	format(text, sizeof(text), "# Nr. of Fire positions: %d\r\n\r\n", gNumFirePositions);
	fwrite(FOut, text);

	for(new i = 0; i < gNumFirePositions; i ++)
	{
		format(text, sizeof(text), "%f %f %f\r\n", gFirePositions[i][X], gFirePositions[i][Y], gFirePositions[i][Z]);
		fwrite(FOut, text);
	}

	fclose(FOut);

	return 1;
}