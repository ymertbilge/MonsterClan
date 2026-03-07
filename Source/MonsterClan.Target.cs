// Copyright by Neon Fiction Studios

using UnrealBuildTool;
using System.Collections.Generic;

public class MonsterClanTarget : TargetRules
{
	public MonsterClanTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V6;

		ExtraModuleNames.AddRange( new string[] { "MonsterClan" } );
	}
}
