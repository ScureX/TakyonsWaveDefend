/**
Respawn attribute to enable/disable respawn and respawn time, for getting and setting varriables in Editor Attribute window
*/
// Script File 
class TKY_SpawnerAttribute {
	static TKY_SpawnerComponent IsValidEntity(Managed item) {
		SCR_EditableEntityComponent editableEntity = SCR_EditableEntityComponent.Cast(item);
		if (!editableEntity) {
			return null;
		};
		
		TKY_SpawnerComponent owner = TKY_SpawnerComponent.Cast(editableEntity.GetOwner().FindComponent(TKY_SpawnerComponent));
		if (!owner) {
			return null;
		};

		return TKY_SpawnerComponent.Cast(owner);
	};
};


[BaseContainerProps(), SCR_BaseEditorAttributeCustomTitle()]
class TKY_BreakTimerAttribute : SCR_BaseValueListEditorAttribute
{
	override SCR_BaseEditorAttributeVar ReadVariable(Managed item, SCR_AttributesManagerEditorComponent manager)
	{
		TKY_SpawnerComponent spawner = TKY_SpawnerAttribute.IsValidEntity(item);
		if (!spawner) {
			return null;
		};

		//if (spawner.EditorAttributeGetActive()) {
		//	return null;
		//};

		return SCR_BaseEditorAttributeVar.CreateInt(spawner.breakTimer);
	}
	
	override void WriteVariable(Managed item, SCR_BaseEditorAttributeVar var, SCR_AttributesManagerEditorComponent manager, int playerID)
	{
		if (!var) return;
		
		TKY_SpawnerComponent spawner = TKY_SpawnerAttribute.IsValidEntity(item);
		if (!spawner) return;

		//if (spawner.EditorAttributeGetActive()) return;
		
		spawner.breakTimer = var.GetFloat();
	}
};


[BaseContainerProps(), SCR_BaseEditorAttributeCustomTitle()]
class TKY_EnemyBudgetStepAttribute : SCR_BaseValueListEditorAttribute
{
	override SCR_BaseEditorAttributeVar ReadVariable(Managed item, SCR_AttributesManagerEditorComponent manager)
	{
		TKY_SpawnerComponent spawner = TKY_SpawnerAttribute.IsValidEntity(item);
		if (!spawner) {
			return null;
		};

		//if (spawner.EditorAttributeGetActive()) {
		//	return null;
		//};

		return SCR_BaseEditorAttributeVar.CreateInt(spawner.enemyBudgetStep);
	}
	
	override void WriteVariable(Managed item, SCR_BaseEditorAttributeVar var, SCR_AttributesManagerEditorComponent manager, int playerID)
	{
		if (!var) return;
		
		TKY_SpawnerComponent spawner = TKY_SpawnerAttribute.IsValidEntity(item);
		if (!spawner) return;

		//if (spawner.EditorAttributeGetActive()) return;
		
		spawner.enemyBudgetStep = var.GetFloat();
	}
};


[BaseContainerProps(), SCR_BaseEditorAttributeCustomTitle()]
class TKY_MinimumSubwaveDelayAttribute : SCR_BaseValueListEditorAttribute
{
	override SCR_BaseEditorAttributeVar ReadVariable(Managed item, SCR_AttributesManagerEditorComponent manager)
	{
		TKY_SpawnerComponent spawner = TKY_SpawnerAttribute.IsValidEntity(item);
		if (!spawner) {
			return null;
		};

		//if (spawner.EditorAttributeGetActive()) {
		//	return null;
		//};

		return SCR_BaseEditorAttributeVar.CreateInt(spawner.minimumSubwaveDelay);
	}
	
	override void WriteVariable(Managed item, SCR_BaseEditorAttributeVar var, SCR_AttributesManagerEditorComponent manager, int playerID)
	{
		if (!var) return;
		
		TKY_SpawnerComponent spawner = TKY_SpawnerAttribute.IsValidEntity(item);
		if (!spawner) return;

		//if (spawner.EditorAttributeGetActive()) return;
		
		spawner.minimumSubwaveDelay = var.GetFloat();
	}
};


[BaseContainerProps(), SCR_BaseEditorAttributeCustomTitle()]
class TKY_MaximumSubwaveDelayAttribute : SCR_BaseValueListEditorAttribute
{
	override SCR_BaseEditorAttributeVar ReadVariable(Managed item, SCR_AttributesManagerEditorComponent manager)
	{
		TKY_SpawnerComponent spawner = TKY_SpawnerAttribute.IsValidEntity(item);
		if (!spawner) {
			return null;
		};

		//if (spawner.EditorAttributeGetActive()) {
		//	return null;
		//};

		return SCR_BaseEditorAttributeVar.CreateInt(spawner.maximumSubwaveDelay);
	}
	
	override void WriteVariable(Managed item, SCR_BaseEditorAttributeVar var, SCR_AttributesManagerEditorComponent manager, int playerID)
	{
		if (!var) return;
		
		TKY_SpawnerComponent spawner = TKY_SpawnerAttribute.IsValidEntity(item);
		if (!spawner) return;

		//if (spawner.EditorAttributeGetActive()) return;
		
		spawner.maximumSubwaveDelay = var.GetFloat();
	}
};


[BaseContainerProps(), SCR_BaseEditorAttributeCustomTitle()]
class TKY_WaveSupplyAmountAttribute : SCR_BaseValueListEditorAttribute
{
	override SCR_BaseEditorAttributeVar ReadVariable(Managed item, SCR_AttributesManagerEditorComponent manager)
	{
		TKY_SpawnerComponent spawner = TKY_SpawnerAttribute.IsValidEntity(item);
		if (!spawner) {
			return null;
		};

		//if (spawner.EditorAttributeGetActive()) {
		//	return null;
		//};

		return SCR_BaseEditorAttributeVar.CreateInt(spawner.waveSupplyAmount);
	}
	
	override void WriteVariable(Managed item, SCR_BaseEditorAttributeVar var, SCR_AttributesManagerEditorComponent manager, int playerID)
	{
		if (!var) return;
		
		TKY_SpawnerComponent spawner = TKY_SpawnerAttribute.IsValidEntity(item);
		if (!spawner) return;

		//if (spawner.EditorAttributeGetActive()) return;
		
		spawner.waveSupplyAmount = var.GetFloat();
	}
};


[BaseContainerProps(), SCR_BaseEditorAttributeCustomTitle()]
class TKY_WaveSupplyMultiplierAttribute : SCR_BaseValueListEditorAttribute
{
	override SCR_BaseEditorAttributeVar ReadVariable(Managed item, SCR_AttributesManagerEditorComponent manager)
	{
		TKY_SpawnerComponent spawner = TKY_SpawnerAttribute.IsValidEntity(item);
		if (!spawner) {
			return null;
		};

		//if (spawner.EditorAttributeGetActive()) {
		//	return null;
		//};

		return SCR_BaseEditorAttributeVar.CreateInt(spawner.waveSupplyMultiplier);
	}
	
	override void WriteVariable(Managed item, SCR_BaseEditorAttributeVar var, SCR_AttributesManagerEditorComponent manager, int playerID)
	{
		if (!var) return;
		
		TKY_SpawnerComponent spawner = TKY_SpawnerAttribute.IsValidEntity(item);
		if (!spawner) return;

		//if (spawner.EditorAttributeGetActive()) return;
		
		spawner.waveSupplyMultiplier = var.GetFloat();
	}
};


[BaseContainerProps(), SCR_BaseEditorAttributeCustomTitle()]
class TKY_SpawnerLocationSearchRadiusAttribute : SCR_BaseValueListEditorAttribute
{
	override SCR_BaseEditorAttributeVar ReadVariable(Managed item, SCR_AttributesManagerEditorComponent manager)
	{
		TKY_SpawnerComponent spawner = TKY_SpawnerAttribute.IsValidEntity(item);
		if (!spawner) {
			return null;
		};

		//if (spawner.EditorAttributeGetActive()) {
		//	return null;
		//};

		return SCR_BaseEditorAttributeVar.CreateInt(spawner.spawnLocationSearchRadius);
	}
	
	override void WriteVariable(Managed item, SCR_BaseEditorAttributeVar var, SCR_AttributesManagerEditorComponent manager, int playerID)
	{
		if (!var) return;
		
		TKY_SpawnerComponent spawner = TKY_SpawnerAttribute.IsValidEntity(item);
		if (!spawner) return;

		//if (spawner.EditorAttributeGetActive()) return;
		
		spawner.EditorSetSpawerLocationSearchRadius(var.GetFloat());
	}
};


// enable 
[BaseContainerProps(), SCR_BaseEditorAttributeCustomTitle()]
class TKY_SpawnerActivateAttribute : SCR_BaseValueListEditorAttribute
{
	override SCR_BaseEditorAttributeVar ReadVariable(Managed item, SCR_AttributesManagerEditorComponent manager)
	{
		TKY_SpawnerComponent spawner = TKY_SpawnerAttribute.IsValidEntity(item);
		if (!spawner) return null;

		if (spawner.m_isActive) return null;

		return SCR_BaseEditorAttributeVar.CreateBool(spawner.m_isActive);
	}
	
	override void WriteVariable(Managed item, SCR_BaseEditorAttributeVar var, SCR_AttributesManagerEditorComponent manager, int playerID)
	{
		if (!var) return;
		
		TKY_SpawnerComponent spawner = TKY_SpawnerAttribute.IsValidEntity(item);
		if (!spawner) return;

		if (spawner.m_isActive) return;
		
		spawner.EditorSetSpawerLocationSearchRadius(spawner.spawnLocationSearchRadius);
		spawner.m_isActive = var.GetBool();
	}
};
