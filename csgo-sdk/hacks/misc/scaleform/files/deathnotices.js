R"(
var contextPanel = $.GetContextPanel();
for (var dnBackground of contextPanel.FindChildrenWithClassTraverse('DeathNoticeBGBorder')) {

	dnBackground.style.border = '0px solid #00000000';
	dnBackground.style.boxShadow = 'inset #00000000 0px 0px 0px 0px;';
	dnBackground.style.borderRadius = '3px';
	dnBackground.style.height = '100%';
	dnBackground.style.width = '100%';
	//dnBackground.style.height = '36px';
}

for (var dnBackgroundg of contextPanel.FindChildrenWithClassTraverse('DeathNoticeBG')) {
	//dnBackgroundg.style.backgroundImage = 'url("https://images2.imgbox.com/49/45/mrwub2K0_o.png")';
	dnBackgroundg.style.backgroundSize = '100% 100%';
	dnBackgroundg.style.height = '100%';
	dnBackgroundg.style.width = '100%';
	dnBackgroundg.style.backgroundColor = 'gradient( linear, 0% 0%, 0% 0%, from( #000000CC ), color-stop( 1, #00000000 ) , color-stop( 1, #00000000), color-stop( 1, #00000000 ), to( #00000000 ) )';
	dnBackgroundg.style.opacity = '0.65';
}

for (var dnBackgroundgradient of contextPanel.FindChildrenWithClassTraverse('DeathNoticeBGGradient')) {
	dnBackgroundgradient.style.height = '100%';
	dnBackgroundgradient.style.width = '95%';
	dnBackgroundgradient.opacity = '0.76';
	dnBackgroundgradient.verticalAlign = 'center';
}
//DeathNoticeContent 
for (var content of contextPanel.FindChildrenWithClassTraverse('DeathNoticeWipeContainer')) {
	content.padding = '6px 10px 3px 10px';
	content.S2MixBlendMode = 'SRGBadditive';
	content.fontSize = '40px';
}

for (var dnIcon of contextPanel.FindChildrenWithClassTraverse('DeathNoticeIcon')) {
	if (dnIcon.id === "Weapon") {
		dnIcon.style.verticalAlign = 'top';
		dnIcon.style.transform = 'translateY(5px)';
		dnIcon.style.margin = '-2px 4px 0px 5px';         
		dnIcon.style.uiScale = "51%";
	} else {
		dnIcon.style.height = '20px';
		dnIcon.style.verticalAlign = 'top';
		dnIcon.style.transform = 'translateY(0px) scaleY(.8) scaleX(.8)';
		dnIcon.style.uiScale = "100%";
	}
}

for (var dnT of contextPanel.FindChildrenWithClassTraverse('DeathNoticeTColor')) {
	dnT.style.color = '#d7c788';

}
for (var dnCt of contextPanel.FindChildrenWithClassTraverse('DeathNoticeCTColor')) {
	dnCt.style.color = '#93abd2';

}

for (var deathnotice of contextPanel.FindChildrenWithClassTraverse('DeathNotice')) {
	// ---------------- ICON REMOVALS ----------------
	deathnotice.FindChildTraverse('NoScopeIcon').style.visibility = 'collapse';
	deathnotice.FindChildTraverse('ThroughSmokeIcon').style.visibility = 'collapse';
	deathnotice.FindChildTraverse('AttackerBlindIcon').style.visibility = 'collapse';
	deathnotice.FindChildTraverse('Domination').style.visibility = 'collapse';
	// ---------------- REMOVALS ----------------
	deathnotice.style.margin = '0px';

	if (deathnotice.BHasClass('DeathNotice_Killer')) {
		for (var killer of contextPanel.FindChildTraverse('HudDeathNotice').FindChildrenWithClassTraverse('DeathNotice_Killer')) {
			for (var content of killer.FindChildrenWithClassTraverse('DeathNoticeBG')) {
				content.style.border = '2px solid #b3070d;';
				content.style.borderRadius = '3px';
				content.style.backgroundColor = 'black';
				content.style.opacity = '0.9';
			}
		}
	}
	if (deathnotice.BHasClass('DeathNotice_Victim')) {
		for (var victim of contextPanel.FindChildTraverse('HudDeathNotice').FindChildrenWithClassTraverse('DeathNotice_Victim')) {
			for (var content of victim.FindChildrenWithClassTraverse('DeathNoticeBG')) {

				content.style.backgroundColor = '#0e0e0eB3';
				content.style.borderRadius = '3px';
				content.style.boxShadow = 'inset #e10000e6 0px 0px 1px;';
				content.style.opacity = '1';
			}
		}
	}
}
)"