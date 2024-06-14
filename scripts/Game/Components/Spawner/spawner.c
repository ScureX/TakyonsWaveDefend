[ComponentEditorProps(category: "Takyon/Test", description: "")]
class TKY_SpawnerComponentClass : ScriptComponentClass
{
}

modded enum ENotification
{
	TKY_BASE_CONTESTED = 69100,
	TKY_BASE_UNCONTESTED = 69200,
	TKY_BASE_LOST = 69300,
	TKY_WAVE_STARTED = 69400,
	TKY_WAVE_STOPPED = 69500,
	TKY_SPAWNER_LOCATIONS_UPDATED = 69600
}

enum GameState {
	BREAK,
	WAVE,
	LOST,
	WON
}

class Enemy
{
	ResourceName resourceName;
	int cost; // per round spawner system hast budget, higher cost for better units
	
	void Enemy(ResourceName rN, int c)
	{
		this.resourceName = rN;
		this.cost = c;
	}
}

/*
	enum for 
		normal enemy
		special enemy config

	give each a budget for wave
*/

/*
	make objects spawnable and activateable in gm mode
*/

class TKY_SpawnerComponent : ScriptComponent 
{
	[Attribute("", UIWidgets.EditBox, "List of SpawnerLocation Object names found in the level")]
	ref array<string> m_aSpawnerLocations;

	[Attribute(defvalue: "1.0", UIWidgets.EditBox, "refresh rate in seconds")]
	protected float CHECK_PERIOD;
	
	[Attribute(defvalue: "300.0", UIWidgets.EditBox, "seconds between waves")]
	float breakTimer; // seconds

	[Attribute(defvalue: "30.0", UIWidgets.EditBox, "enemy budget for spawning troops, higher = more/better enemies")]
	float enemyBudgetStep; 
	
	[Attribute(defvalue: "30.0", UIWidgets.EditBox, "minimum delay for spawning subwaves")]
	float minimumSubwaveDelay;
	
	[Attribute(defvalue: "60.0", UIWidgets.EditBox, "maximum delay for spawning subwaves")]
	float maximumSubwaveDelay;
	
	[Attribute(defvalue: "200.0", UIWidgets.EditBox, "how many supplies you get after a wave minimum")]
	float waveSupplyAmount;
	
	[Attribute(defvalue: "50.0", UIWidgets.EditBox, "how many supplies you get additionally for each wave (if 50: wave1: 250, wave2: 300: wave4: 350...)")]
	float waveSupplyMultiplier;
	
	[Attribute(defvalue: "1000.0", UIWidgets.EditBox, "how far this spawner should search for spawn locations")]
	float spawnLocationSearchRadius;
	
	bool m_isActive = false;
	protected int waveCount = 1;
	
	protected float timer = 0;
	protected float m_fCheckDelay;
	
	GameState gameState;
	protected SCR_ResourceComponent rc; // component with base resources that get added
	
	protected ref array<TKY_SpawnerLocation> m_spawnerLocations;
	protected ref array<ref Enemy> enemyPrefabs;
	protected ref array<ref ResourceName> waypointPrefabs;
	
	ref array<float> waypointWeights; // Total should sum up to 1.0

	
	/*void ConvertSpawnerLocations()
	{
		Print("TKY: ConvertSpawnerLocations");
		for (int i = 0, length = m_aSpawnerLocations.Count(); i < length; i++)
		{
#ifdef WORKBENCH
			WorldEditorAPI m_API = GetOwner()._WB_GetEditorAPI();
			if (m_API)
			{
				m_spawnerLocations.Insert(TKY_SpawnerLocation.Cast(m_API.GetWorld().FindEntityByName(m_aSpawnerLocations[i])));
			}
			else
			{
				m_spawnerLocations.Insert(TKY_SpawnerLocation.Cast(GetGame().GetWorld().FindEntityByName(m_aSpawnerLocations[i])));
			}
#else
			m_spawnerLocations.Insert(TKY_SpawnerLocation.Cast(GetGame().GetWorld().FindEntityByName(m_aSpawnerLocations[i])));
#endif
		}
	}*/
	
	
	int GetWeightedRandomIndex(int maxIndex, array<float> weights)
    {
        if (weights.Count() != maxIndex + 1)
        {
            Print("Error: The weights array length does not match maxIndex + 1");
            return -1;
        }

        float totalWeight = 0;
        for (int i = 0; i < weights.Count(); i++)
        {
            totalWeight += weights[i];
        }

        float randomValue = Math.RandomFloat(0, totalWeight);
        float cumulativeWeight = 0.0;

        for (int i = 0; i < weights.Count(); i++)
        {
            cumulativeWeight += weights[i];
            if (randomValue < cumulativeWeight)
            {
                return i;
            }
        }

        return maxIndex;
    }
	
	
	// QueryEntitiesCallback type
	protected bool QueryEntitiesCallbackMethod(IEntity e)
	{
		GenericEntity e2 = GenericEntity.Cast(e);
		if (!e || !TKY_SpawnerLocation.Cast(e2))
			return true;

		m_spawnerLocations.Insert(TKY_SpawnerLocation.Cast(e));
		return true;
	}

	
	void InitializeWave()
    {
        enemyPrefabs = new array<ref Enemy>();
        enemyPrefabs = 
        {
            new Enemy("{0ACD74AD27EEEE7D}Prefabs/Groups/OPFOR/RHS_AFRF/MSV/VKPO_Demiseason/Group_RHS_RF_MSV_VKPO_DS_FireGroup.et", 50),
            new Enemy("{DFF74E2181355AFD}Prefabs/Groups/OPFOR/RHS_AFRF/MSV/VKPO_Demiseason/Group_RHS_RF_MSV_VKPO_DS_Team_GL.et", 40),
            new Enemy("{F40CDA3D5A7B1CDF}Prefabs/Groups/OPFOR/RHS_AFRF/MSV/VKPO_Demiseason/Group_RHS_RF_MSV_VKPO_DS_LightFireTeam.et", 25),
            new Enemy("{60E2D587BE5A9B43}Prefabs/Groups/OPFOR/RHS_AFRF/MSV/VKPO_Demiseason/Group_RHS_RF_MSV_VKPO_DS_MachineGunTeam.et", 20)
        };

        waypointPrefabs = new array<ref ResourceName>();
        waypointPrefabs =
        {
            "{D9C14ECEC9772CC6}PrefabsEditable/Auto/AI/Waypoints/E_AIWaypoint_Defend.et",
            "{FFF9518F73279473}PrefabsEditable/Auto/AI/Waypoints/E_AIWaypoint_Move.et",
            "{3C790BB71C5CD960}PrefabsEditable/Auto/AI/Waypoints/E_AIWaypoint_ForcedMove.et"
        };

        waypointWeights = new array<float>();
        waypointWeights = { 0.4, 0.4, 0.2 };
    }
	
	
	int CalculateBudget(int _waveCount)
    {
        return 100 + (_waveCount * enemyBudgetStep); // start with 100 base budget so its not as boring in the beginning
    }
	
	
	ref array<Enemy> SelectEnemies(int budget)
	{
	    ref array<Enemy> enemiesToSpawn = new array<Enemy>;
	    ref array<Enemy> affordableEnemies = new array<Enemy>;
	
	    while (budget > 0)
	    {
	        affordableEnemies.Clear();
	
	        // Find all enemies that can be afforded within the current budget
	        foreach (Enemy enemy : enemyPrefabs)
	        {
	            if (enemy.cost <= budget)
	            {
	                affordableEnemies.Insert(enemy);
	            }
	        }
	
	        // If no affordable enemies are found, break the loop
	        if (affordableEnemies.Count() == 0)
	        {
	            break;
	        }
	
	        // Pick a random affordable enemy
	        int randomIndex = Math.RandomInt(0, affordableEnemies.Count());
	        Enemy selectedEnemy = affordableEnemies[randomIndex];
	
	        enemiesToSpawn.Insert(selectedEnemy);
	        budget -= selectedEnemy.cost;
	    }
	
	    return enemiesToSpawn;
	}

	
	array<ref array<ref Enemy>> SplitIntoWaves(array<Enemy> enemiesToSpawn)
	{
	    array<ref array<ref Enemy>> spawnWaveList = new array<ref array<ref Enemy>>();
	    int spawnWaveAmount = enemiesToSpawn.GetRandomIndex() + 1;
		Print("TKY: " + spawnWaveAmount);
	
	    // Initialize the waves
	    for (int i = 0; i < spawnWaveAmount; i++)
	    {
	        spawnWaveList.Insert(new array<ref Enemy>());
	    }
	
	    // Distribute one enemy to each wave to ensure each wave has at least one enemy
	    int enemyIndex = 0;
	    while (enemyIndex < spawnWaveAmount && enemyIndex < enemiesToSpawn.Count())
	    {
	        spawnWaveList[enemyIndex].Insert(enemiesToSpawn[enemyIndex]);
	        enemyIndex++;
	    }
	
	    // Distribute the remaining enemies randomly
	    for (; enemyIndex < enemiesToSpawn.Count(); enemyIndex++)
	    {
	        int randomWaveIndex = Math.RandomInt(0, spawnWaveAmount);
	        spawnWaveList[randomWaveIndex].Insert(enemiesToSpawn[enemyIndex]);
	    }
	
	    return spawnWaveList;
	}

	
	void SpawnEnemies(array<ref array<ref Enemy>> spawnWaveList)
    {
        float cumulativeDelay = 0;

        for (int subWaveIndex = 0; subWaveIndex < spawnWaveList.Count(); subWaveIndex++)
        {
            array<ref Enemy> wave = spawnWaveList[subWaveIndex];

            if (subWaveIndex > 0)
            {
                cumulativeDelay += Math.RandomFloat(minimumSubwaveDelay * 1000, maximumSubwaveDelay * 1000);
            }

            foreach (Enemy enemyToSpawn : wave)
            {
                TKY_SpawnerLocation spawnerLocation = m_spawnerLocations.GetRandomElement();
                int waypointIndex = GetWeightedRandomIndex(waypointPrefabs.Count() - 1, waypointWeights);

                spawnerLocation.ScheduleSpawnEntity(
                    cumulativeDelay, 
                    enemyToSpawn.resourceName, 
                    GetOwner().GetOrigin(), 
                    waypointPrefabs.Get(waypointIndex)
                );
            }
        }
    }
	
	
	void StopStartSiren(GenericEntity e)
	{
		delete e;
	}
	
	void StartWave()
    {
        if (waveCount == 1)
        {
            InitializeWave();
        }

        SCR_NotificationsComponent.SendToEveryone(ENotification.TKY_WAVE_STARTED, waveCount);
		
		
		ResourceName prefab = "{93FC2145F05A1EEE}Prefabs/Systems/Sound/sound_airraidsiren.et";
		
		EntitySpawnParams params();
		vector mat[4];
		GetOwner().GetWorldTransform(mat);
		params.Transform = mat;
		
		GenericEntity newEnt = GenericEntity.Cast(GetGame().SpawnEntityPrefab(Resource.Load(prefab), GetGame().GetWorld(), params));
		newEnt.SetFlags(EntityFlags.VISIBLE, true);

		//proto void CallLater(func fn, int delay = 0, bool repeat = false, void param1 = NULL,..);	
		GetGame().GetCallqueue().CallLater(
			this.StopStartSiren,
			5000,
			false,
			newEnt
		);
		
		
        int budget = CalculateBudget(waveCount);
		Print("TKY: " + budget);
        ref array<Enemy> enemiesToSpawn = SelectEnemies(budget);

        array<ref array<ref Enemy>> spawnWaveList = SplitIntoWaves(enemiesToSpawn);

        SpawnEnemies(spawnWaveList);

        waveCount++;
    }
	
	
	//------------------------------------------------------------------------------------------------
	bool IsWaveOver()
	{
		int scheduledSpawns = 0;
		
		foreach (TKY_SpawnerLocation spawnLoc : m_spawnerLocations)
		{
			scheduledSpawns += spawnLoc.scheduledSpawns;
		}
		
		if (scheduledSpawns > 0)
		{
			//Print("WAVE NOT OVER: " + TKY_SpawnerLocation.scheduledSpawns + " scheduledSpawns remaining");
			return false;
		}
		
		
		int spawnedEnemies = 0;
		
		foreach (TKY_SpawnerLocation spawnLoc : m_spawnerLocations)
		{
			// shit doesnt get removed when group is killed or deleted or whatever idk this sucks
			for (int i = 0; i < spawnLoc.spawnedEnemies.Count(); i++)
			{
				if(!spawnLoc.spawnedEnemies.Get(i))
					spawnLoc.spawnedEnemies.Remove(i);
			}
		
			
			spawnedEnemies += spawnLoc.spawnedEnemies.Count();
		}
		
		if (spawnedEnemies > 0)
		{
			//Print("WAVE NOT OVER: " + TKY_SpawnerLocation.spawnedEnemies.Count() + " spawnedEnemies remaining");
			return false;
		}
		
		Print("WAVE OVER");
		SCR_NotificationsComponent.SendToEveryone(ENotification.TKY_WAVE_STOPPED);
		return true;
	}
	
	
	//------------------------------------------------------------------------------------------------
	override void EOnFrame(IEntity owner, float timeSlice)
	{
		super.EOnFrame(owner, timeSlice);
		
		if (!m_isActive) {
			return;
		};
		
		timer += timeSlice;
		
		// check if new wave should start after breakTimer
		if (timer >= breakTimer && gameState == GameState.BREAK)
		{
			Print("new wave");
			Print(waveCount);
			StartWave();
			gameState = GameState.WAVE;
		}
		
		// check if wave is over 
		m_fCheckDelay -= timeSlice;
		if (gameState == GameState.WAVE && m_fCheckDelay <= 0)
		{
			Print("Delay hit");
			m_fCheckDelay = CHECK_PERIOD;
			if (IsWaveOver())
			{
				gameState = GameState.BREAK;
				
				// reset timer
				timer = 0;
				
				// give supply
				rc.GetContainer(EResourceType.SUPPLIES).IncreaseResourceValue(waveSupplyAmount + (waveCount * waveSupplyMultiplier));
			}
		}
	}

	
	//------------------------------------------------------------------------------------------------
	protected override void OnPostInit(IEntity owner)
	{
		Print("TKY: OnPostInit");
		gameState = GameState.BREAK;
		SetEventMask(owner, EntityEvent.FRAME);
		
		m_spawnerLocations = new array<TKY_SpawnerLocation>;
	}
	
	
	//------------------------------------------------------------------------------------------------
	override void OnChildAdded(IEntity parent, IEntity child)
	{
		TKY_ResourceComponent customComp = TKY_ResourceComponent.Cast(child.FindComponent(TKY_ResourceComponent));
		if (customComp)
		{
			rc = SCR_ResourceComponent.Cast(child.FindComponent(SCR_ResourceComponent));;
		}
	}
	
	void EditorSetSpawerLocationSearchRadius(int radius)
	{
		spawnLocationSearchRadius = radius;
		
		array<IEntity> entities = {};
		
		//external void GetActiveEntities(notnull out array<IEntity> entities);
		GetOwner().GetWorld().GetActiveEntities(entities);
		
		foreach (IEntity e : entities)
		{
			if (!e || !TKY_SpawnerLocation.Cast(e))
				continue;

			m_spawnerLocations.Insert(TKY_SpawnerLocation.Cast(e));
		}
		
		m_spawnerLocations.Clear();
		//bool QueryEntitiesBySphere(vector center, float radius, QueryEntitiesCallback addEntity, QueryEntitiesCallback filterEntity = null, EQueryEntitiesFlags queryFlags = EQueryEntitiesFlags.ALL);
		GetOwner().GetWorld().QueryEntitiesBySphere(GetOwner().GetOrigin(), spawnLocationSearchRadius, QueryEntitiesCallbackMethod, null, EQueryEntitiesFlags.STATIC);
		Print(m_spawnerLocations.Count());
		SCR_NotificationsComponent.SendToEveryone(ENotification.TKY_SPAWNER_LOCATIONS_UPDATED, m_spawnerLocations.Count());
	}
} 