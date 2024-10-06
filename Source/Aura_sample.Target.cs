// Copyright Luke

using UnrealBuildTool;
using System.Collections.Generic;

public class Aura_sampleTarget : TargetRules
{
	public Aura_sampleTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V5;

		ExtraModuleNames.AddRange( new string[] { "Aura_sample" } );
	}
}
