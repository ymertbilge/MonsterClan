// Copyright by Neon Fiction Studios

using UnrealBuildTool;
using System.Collections.Generic;

public class MonsterClanEditorTarget : TargetRules
{
	public MonsterClanEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V6;

		ExtraModuleNames.AddRange( new string[] { "MonsterClan" } );
	}
}
