#if defined __VEH_NODES_INC__
#endinput
#endif
#define __VEH_NODES_INC__

#define MAX_VEH_NODES 				32000

static enum E_VEH_NODE
{
	Used,
	Float:X,
	Float:Y,
	Float:Z
};
static
	gVehNodes[MAX_VEH_NODES][E_VEH_NODE],
	bool:gVehNodesLoaded = false;

stock LoadVehNodes()
{
	if(gVehNodesLoaded) return 0;

	for(new i = 0; i < MAX_VEH_NODES; i ++)
	{
		gVehNodes[i][Used] = 0;
	}

	new File:FIn = fopen("GPS.dat", io_read);

	if(!FIn) return 0;

	new text[100], count;

	while(fread(FIn, text))
	{
		new Float:x, Float:y, Float:z, id;

		if(!sscanf(text, "'0'fff{i}i", x, y, z, id))
		{
			if(id >= 0 && id < MAX_VEH_NODES)
			{
				gVehNodes[id][Used] = 1;
				gVehNodes[id][X] = x;
				gVehNodes[id][Y] = y;
				gVehNodes[id][Z] = z;

				count ++;
			}
		}
	}

	fclose(FIn);

	printf("Loaded Veh Nodes (%d)", count);

	if(count) gVehNodesLoaded = true;

	return 1;
}

stock VehNodesLoaded()
{
	return _:gVehNodesLoaded;
}

stock GetClosestVehNodeFromPoint(Float:x, Float:y, Float:z, Float:maxdist = 10000.0, except = -1)
{
	new id = -1, Float:tmp_dist, Float:dist = maxdist;

	for(new i = 0; i < MAX_VEH_NODES; i ++)
	{
		if(gVehNodes[i][Used] && (tmp_dist = VectorSize(x - gVehNodes[i][X], y - gVehNodes[i][Y], z - gVehNodes[i][Z])) <= dist)
		{
			dist = tmpdist;
			id = i;
		}
	}

	return id;
}

stock IsAnyVehNodeInRangeOfPoint(Float:x, Float:y, Float:z, Float:range)
{
	for(new i = 0; i < MAX_VEH_NODES; i ++)
	{
		if(gVehNodes[i][Used] && VectorSize(x - gVehNodes[i][X], y - gVehNodes[i][Y], z - gVehNodes[i][Z]) <= range)
		{
			return 1;
		}
	}

	return 0;
}

stock GetRandomVehNode()
{
	static 
		list[MAX_VEH_NODES];
	
	new 
		count;

	for(new i = 0; i < MAX_VEH_NODES; i ++) if(gVehNodes[i][Used])
	{
		list[count ++] = i;
	}

	if(!count) return -1;

	return list[random(count)];
}

stock GetRandomVehNodePos(&Float:x, &Float:y, &Float:z)
{
	new id = GetRandomVehNode();

	if(id == -1) return 0;

	x = gVehNodes[id][X];
	y = gVehNodes[id][Y];
	z = gVehNodes[id][Z];

	return 1;
}