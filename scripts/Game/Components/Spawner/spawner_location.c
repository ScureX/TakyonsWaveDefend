enum TestEnum
{
	TKY_SPAWNER_LOCATION
}

[ComponentEditorProps(category: "Takyon/Test", description: "")]
class TKY_SpawnerLocationClass : GenericEntityClass
{
}

class TKY_SpawnerLocation : GenericEntity 
{
	[Attribute(defvalue: "10.0", UIWidgets.EditBox, "completion radius for the waypoints")]
	protected float m_completionRadius;
	
	// doing this statically fucking SUCKS
	ref array<SCR_AIGroup> spawnedEnemies = {};
	int scheduledSpawns = 0;
	
	protected IEntity CreatePrefab(ResourceName prefab, BaseWorld myWorld, EntitySpawnParams params)
	{
		Resource res = Resource.Load(prefab);
		return GetGame().SpawnEntityPrefab(res, myWorld, params);
	}
	
	protected bool SpawnEntity(ResourceName prefab, ResourceName waypointResource, vector waypointLocation)
	{
		BaseWorld myWorld = GetGame().GetWorld();
		
		if (!myWorld)
			return false;
		
		EntitySpawnParams params();

		vector mat[4];
		GetWorldTransform(mat);
		params.Transform = mat;
		
		// IEntity SpawnEntityPrefab(notnull Resource templateResource, BaseWorld world = null, EntitySpawnParams params = null);
		SCR_AIGroup newEnt = SCR_AIGroup.Cast(CreatePrefab(prefab, myWorld, params));

		if (!newEnt)
			return false;
		
		newEnt.SetFlags(EntityFlags.VISIBLE, true);

		AIWaypoint newWP = AIWaypoint.Cast(CreatePrefab(waypointResource, myWorld, params));
 		
		
		newWP.SetOrigin(waypointLocation);
		newWP.SetCompletionRadius(m_completionRadius);
		
		newEnt.AddWaypoint(newWP);
		
		spawnedEnemies.Insert(newEnt);
		
		scheduledSpawns--;
		return true;
	}
	
	void ScheduleSpawnEntity(float delay, ResourceName resourceName, vector waypointLocation, ResourceName waypointResource)
	{
		scheduledSpawns++;
		//proto void CallLater(func fn, int delay = 0, bool repeat = false, void param1 = NULL,..);	
		GetGame().GetCallqueue().CallLater(
			this.SpawnEntity,
			delay,
			false,
			resourceName, 
			waypointResource,
			waypointLocation
		);
	}
}