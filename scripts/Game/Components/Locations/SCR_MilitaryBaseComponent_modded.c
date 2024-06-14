modded class SCR_MilitaryBaseComponent
{
	override void OnFactionChanged(FactionAffiliationComponent owner, Faction previousFaction, Faction faction)
	{
		super.OnFactionChanged(owner, previousFaction, faction);
		
		SCR_NotificationsComponent.SendToEveryone(ENotification.TKY_BASE_LOST);
		
		Managed comp = GetOwner().FindComponent(TKY_SpawnerComponent);
		
		if(!comp)
		{
			return;
		}
		
		
		TKY_SpawnerComponent spawner = TKY_SpawnerComponent.Cast(comp);
		
		spawner.gameState = GameState.LOST;
		Print("\n\nOnFactionChanged LOST\n\n");
	}
	
	
	override void OnPointContested(notnull SCR_Faction faction, notnull SCR_SeizingComponent point)
	{
		super.OnPointContested(faction, point);

		SCR_NotificationsComponent.SendToEveryone(ENotification.TKY_BASE_CONTESTED);
		Print("\n\nOnPointContested CONTESTED\n\n");
	}
	
	
	override void OnPointSecured(notnull SCR_Faction faction, notnull SCR_SeizingComponent point)
	{
		super.OnPointSecured(faction, point);
		
		SCR_NotificationsComponent.SendToEveryone(ENotification.TKY_BASE_UNCONTESTED);
		Print("\n\nOnPointUNcontested CONTESTED\n\n");
	}
	
	
	override void RegisterLogicComponent(notnull SCR_MilitaryBaseLogicComponent component)
	{
		if (m_aSystems.Contains(component))
			return;

		m_aSystems.Insert(component);

		SCR_ServicePointComponent service = SCR_ServicePointComponent.Cast(component);

		if (service)
		{
			if (m_OnServiceRegistered)
				m_OnServiceRegistered.Invoke(this, service);

			OnServiceStateChanged(service.GetServiceState(), service);
			service.GetOnServiceStateChanged().Insert(OnServiceStateChanged);
		}

		SCR_MilitaryBaseSystem.GetInstance().OnLogicRegisteredInBase(component, this);

		SCR_FlagComponent flag = SCR_FlagComponent.Cast(component);

		if (flag && GetFaction())
			GetGame().GetCallqueue().CallLater(ChangeFlags, 500, false, GetFaction());	// Give the system time to properly register slots

		if (!IsProxy())
		{
			SCR_SeizingComponent seizingComponent = SCR_SeizingComponent.Cast(component);
			
			if (seizingComponent)
			{
				//seizingComponent.SetMax(50.0);
				//seizingComponent.SetMin(45.0);
				seizingComponent.SetMin(45.0);
				seizingComponent.SetMax(50.0);
				
				seizingComponent.GetOnCaptureFinish().Insert(OnPointCaptured);
				seizingComponent.GetOnCaptureStart().Insert(OnPointContested);
				seizingComponent.GetOnCaptureInterrupt().Insert(OnPointSecured);
			}
		}
	}
}