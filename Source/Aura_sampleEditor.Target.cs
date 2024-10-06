// Copyright Luke

using UnrealBuildTool;
using System.Collections.Generic;

public class Aura_sampleEditorTarget : TargetRules
{
	public Aura_sampleEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V5;

		ExtraModuleNames.AddRange( new string[] { "Aura_sample" } );
	}
}
